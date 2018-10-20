#include "query.hpp"
#include "context.hpp"

#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"

#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

/* MGLContext.query(time_elapsed, primitives_generated, samples_passed, any_samples_passed)
 */
PyObject * MGLContext_meth_query(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 4) {
        // TODO: error
        return 0;
    }

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

    MGLQuery * query = MGLContext_new_object(self, Query);

    const GLMethods & gl = self->gl;

    if (time_elapsed) {
        gl.GenQueries(1, (GLuint *)&query->query_obj[TIME_ELAPSED]);
        SLOT(query->wrapper, PyObject, Query_class_elapsed) = PyLong_FromLong(0);
    }

    if (primitives_generated) {
        gl.GenQueries(1, (GLuint *)&query->query_obj[PRIMITIVES_GENERATED]);
        SLOT(query->wrapper, PyObject, Query_class_primitives) = PyLong_FromLong(0);
    }

    if (samples_passed) {
        gl.GenQueries(1, (GLuint *)&query->query_obj[SAMPLES_PASSED]);
        SLOT(query->wrapper, PyObject, Query_class_samples) = PyLong_FromLong(0);
    }

    if (any_samples_passed) {
        gl.GenQueries(1, (GLuint *)&query->query_obj[ANY_SAMPLES_PASSED]);
    }

    return NEW_REF(query->wrapper);
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
        PyObject *& elapsed_slot = SLOT(self->wrapper, PyObject, Query_class_elapsed);
        Py_DECREF(elapsed_slot);
        elapsed_slot = PyLong_FromLong(elapsed);
    }

    if (self->query_obj[PRIMITIVES_GENERATED]) {
        int primitives = 0;
        gl.EndQuery(GL_PRIMITIVES_GENERATED);
        self->context->gl.GetQueryObjectiv(self->query_obj[PRIMITIVES_GENERATED], GL_QUERY_RESULT, &primitives);
        PyObject *& primitives_slot = SLOT(self->wrapper, PyObject, Query_class_primitives);
        Py_DECREF(primitives_slot);
        primitives_slot = PyLong_FromLong(primitives);
    }

    if (self->query_obj[SAMPLES_PASSED]) {
        int samples = 0;
        gl.EndQuery(GL_SAMPLES_PASSED);
        self->context->gl.GetQueryObjectiv(self->query_obj[SAMPLES_PASSED], GL_QUERY_RESULT, &samples);
        PyObject *& samples_slot = SLOT(self->wrapper, PyObject, Query_class_samples);
        Py_DECREF(samples_slot);
        samples_slot = PyLong_FromLong(samples);
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
