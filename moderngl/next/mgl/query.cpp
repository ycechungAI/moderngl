#include "query.hpp"
#include "context.hpp"

#include "internal/wrapper.hpp"

#include "internal/tools.hpp"
#include "internal/glsl.hpp"

/* MGLContext.query(time_elapsed, primitives_generated, samples_passed, any_samples_passed)
 */
PyObject * MGLContext_meth_query(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(4);

    int time_elapsed = PyObject_IsTrue(args[0]);
    int primitives_generated = PyObject_IsTrue(args[1]);
    int samples_passed = PyObject_IsTrue(args[2]);
    int any_samples_passed = PyObject_IsTrue(args[3]);

    if (!time_elapsed && !primitives_generated && !samples_passed && !any_samples_passed) {
        time_elapsed = true;
        primitives_generated = true;
        samples_passed = true;
        any_samples_passed = false;
    }

    if (samples_passed && any_samples_passed) {
        any_samples_passed = false;
    }

    MGLQuery * query = PyObject_New(MGLQuery, MGLQuery_class);
    chain_objects(self, query);
    query->context = self;

    const GLMethods & gl = self->gl;

    if (time_elapsed) {
        gl.GenQueries(1, (GLuint *)&query->query_obj[TIME_ELAPSED]);
        // SLOT(query->wrapper, PyObject, Query_class_elapsed) = PyLong_FromLong(0);
    }

    if (primitives_generated) {
        gl.GenQueries(1, (GLuint *)&query->query_obj[PRIMITIVES_GENERATED]);
        // SLOT(query->wrapper, PyObject, Query_class_primitives) = PyLong_FromLong(0);
    }

    if (samples_passed) {
        gl.GenQueries(1, (GLuint *)&query->query_obj[SAMPLES_PASSED]);
        // SLOT(query->wrapper, PyObject, Query_class_samples) = PyLong_FromLong(0);
    }

    if (any_samples_passed) {
        gl.GenQueries(1, (GLuint *)&query->query_obj[ANY_SAMPLES_PASSED]);
    }

    query->wrapper = Query_New("O", query);
    return query->wrapper;
}

/* MGLQuery.begin()
 */
PyObject * MGLQuery_meth_begin(MGLQuery * self) {
    const GLMethods & gl = self->context->gl;

    if (self->query_obj[ANY_SAMPLES_PASSED]) {
        gl.BeginQuery(GL_ANY_SAMPLES_PASSED, self->query_obj[ANY_SAMPLES_PASSED]);
    }

    if (self->query_obj[SAMPLES_PASSED]) {
        gl.BeginQuery(GL_SAMPLES_PASSED, self->query_obj[SAMPLES_PASSED]);
    }

    if (self->query_obj[PRIMITIVES_GENERATED]) {
        gl.BeginQuery(GL_PRIMITIVES_GENERATED, self->query_obj[PRIMITIVES_GENERATED]);
    }

    if (self->query_obj[TIME_ELAPSED]) {
        gl.BeginQuery(GL_TIME_ELAPSED, self->query_obj[TIME_ELAPSED]);
    }

    Py_RETURN_NONE;
}

/* MGLQuery.end()
 */
PyObject * MGLQuery_meth_end(MGLQuery * self) {
    const GLMethods & gl = self->context->gl;

    if (self->query_obj[TIME_ELAPSED]) {
        int elapsed = 0;
        gl.EndQuery(GL_TIME_ELAPSED);
        self->context->gl.GetQueryObjectiv(self->query_obj[TIME_ELAPSED], GL_QUERY_RESULT, &elapsed);
        // PyObject *& elapsed_slot = SLOT(self->wrapper, PyObject, Query_class_elapsed);
        // Py_DECREF(elapsed_slot);
        // elapsed_slot = PyLong_FromLong(elapsed);
    }

    if (self->query_obj[PRIMITIVES_GENERATED]) {
        int primitives = 0;
        gl.EndQuery(GL_PRIMITIVES_GENERATED);
        self->context->gl.GetQueryObjectiv(self->query_obj[PRIMITIVES_GENERATED], GL_QUERY_RESULT, &primitives);
        // PyObject *& primitives_slot = SLOT(self->wrapper, PyObject, Query_class_primitives);
        // Py_DECREF(primitives_slot);
        // primitives_slot = PyLong_FromLong(primitives);
    }

    if (self->query_obj[SAMPLES_PASSED]) {
        int samples = 0;
        gl.EndQuery(GL_SAMPLES_PASSED);
        self->context->gl.GetQueryObjectiv(self->query_obj[SAMPLES_PASSED], GL_QUERY_RESULT, &samples);
        // PyObject *& samples_slot = SLOT(self->wrapper, PyObject, Query_class_samples);
        // Py_DECREF(samples_slot);
        // samples_slot = PyLong_FromLong(samples);
    }

    if (self->query_obj[ANY_SAMPLES_PASSED]) {
        gl.EndQuery(GL_ANY_SAMPLES_PASSED);
    }

    Py_RETURN_NONE;
}

/* MGLQuery.begin_render()
 */
PyObject * MGLQuery_meth_begin_render(MGLQuery * self) {
    if (self->query_obj[ANY_SAMPLES_PASSED]) {
        self->context->gl.BeginConditionalRender(self->query_obj[ANY_SAMPLES_PASSED], GL_QUERY_NO_WAIT);
    } else if (self->query_obj[SAMPLES_PASSED]) {
        self->context->gl.BeginConditionalRender(self->query_obj[SAMPLES_PASSED], GL_QUERY_NO_WAIT);
    } else {
        PyErr_Format(moderngl_error, "no samples");
        return 0;
    }
    Py_RETURN_NONE;
}

/* MGLQuery.end_render()
 */
PyObject * MGLQuery_meth_end_render(MGLQuery * self) {
    self->context->gl.EndConditionalRender();
    Py_RETURN_NONE;
}

void MGLQuery_dealloc(MGLQuery * self) {
    printf("MGLQuery_dealloc\n");
    Py_TYPE(self)->tp_free(self);
}

PyMethodDef MGLQuery_methods[] = {
    {"begin", (PyCFunction)MGLQuery_meth_begin, METH_NOARGS, 0},
    {"end", (PyCFunction)MGLQuery_meth_end, METH_NOARGS, 0},
    {"begin_render", (PyCFunction)MGLQuery_meth_begin_render, METH_NOARGS, 0},
    {"end_render", (PyCFunction)MGLQuery_meth_end_render, METH_NOARGS, 0},
    {0},
};

PyType_Slot MGLQuery_slots[] = {
    {Py_tp_methods, MGLQuery_methods},
    {Py_tp_dealloc, (void *)MGLQuery_dealloc},
    {0},
};

PyType_Spec MGLQuery_spec = {
    "moderngl.mgl.new.MGLQuery",
    sizeof(MGLQuery),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLQuery_slots,
};

PyTypeObject * MGLQuery_class;
