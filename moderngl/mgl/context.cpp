#include "context.hpp"
#include "limits.hpp"
#include "buffer.hpp"
#include "program.hpp"
#include "vertex_array.hpp"
#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"

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

    context->MGLBuffer_class = (PyTypeObject *)PyType_FromSpec(&MGLBuffer_spec);
    context->MGLFramebuffer_class = (PyTypeObject *)PyType_FromSpec(&MGLFramebuffer_spec);
    context->MGLProgram_class = (PyTypeObject *)PyType_FromSpec(&MGLProgram_spec);
    context->MGLQuery_class = (PyTypeObject *)PyType_FromSpec(&MGLQuery_spec);
    context->MGLRenderbuffer_class = (PyTypeObject *)PyType_FromSpec(&MGLRenderbuffer_spec);
    context->MGLSampler_class = (PyTypeObject *)PyType_FromSpec(&MGLSampler_spec);
    context->MGLScope_class = (PyTypeObject *)PyType_FromSpec(&MGLScope_spec);
    context->MGLTexture_class = (PyTypeObject *)PyType_FromSpec(&MGLTexture_spec);
    context->MGLVertexArray_class = (PyTypeObject *)PyType_FromSpec(&MGLVertexArray_spec);

    context->wrapper = new_object(PyObject, Context_class);
    SLOT(context->wrapper, MGLContext, Context_class_mglo) = context;
    SLOT(context->wrapper, PyObject, Context_class_version_code) = PyLong_FromLong(version_code);
    SLOT(context->wrapper, PyObject, Context_class_limits) = get_limits(gl, version_code);
    return NEW_REF(context->wrapper);
}

PyTypeObject * MGLContext_class;
