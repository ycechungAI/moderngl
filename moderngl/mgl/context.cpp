#include "context.hpp"
#include "limits.hpp"
#include "buffer.hpp"
#include "program.hpp"
#include "vertex_array.hpp"
#include "classes/classes.hpp"

/* MGLContext.create_context(...)
 * Returns a Context object.
 */
PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 1) {
        // TODO: error
        return 0;
    }

    bool standalone = PyObject_IsTrue(args[0]);

    Py_INCREF(MGLContext_class);
    MGLContext * context = PyObject_New(MGLContext, MGLContext_class);

    if (!context->gl_context.load(standalone)) {
        return 0;
    }

    if (!context->gl.load()) {
        return 0;
    }

    const GLMethods & gl = context->gl;

    int major = 0;
    int minor = 0;
    gl.GetIntegerv(GL_MAJOR_VERSION, &major);
    gl.GetIntegerv(GL_MINOR_VERSION, &minor);
    int version_code = major * 100 + minor * 10;

    if (version_code == 0) {
        gl.GetError();
        const char * ver = (const char *)gl.GetString(GL_VERSION);
        if (ver && '0' <= ver[0] && ver[0] <= '9' && ver[1] == '.' && '0' <= ver[2] && ver[2] <= '9') {
            version_code = (ver[0] - '0') * 100 + (ver[2] - '0') * 10;
        }
    }

    MGLBuffer_define(context);
    MGLProgram_define(context);
    MGLVertexArray_define(context);

    context->wrapper = new_object(PyObject, Context_class);
    SLOT(context->wrapper, MGLContext, Context_class_mglo) = context;
    SLOT(context->wrapper, PyObject, Context_class_version_code) = PyLong_FromLong(version_code);
    SLOT(context->wrapper, PyObject, Context_class_limits) = get_limits(gl, version_code);
    return NEW_REF(context->wrapper);
}

/* Backward compatible methods */

BC4(MGLContext, buffer);
BC4(MGLContext, program);
BC4(MGLContext, vertex_array);

/* Definition of MGLContext internal type */

void MGLContext_define() {
    PyMethodDef MGLContext_methods[] = {
        DEF4(MGLContext, buffer),
        DEF4(MGLContext, program),
        DEF4(MGLContext, vertex_array),
        {0},
    };

    PyType_Slot MGLContext_slots[] = {
        {Py_tp_methods, MGLContext_methods},
        {0},
    };

    MGLContext_class = define_python_class(mgl_name ".Context", sizeof(MGLContext), MGLContext_slots);
}
