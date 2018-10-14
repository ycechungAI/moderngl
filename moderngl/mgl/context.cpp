#include "context.hpp"

#include "buffer.hpp"
#include "framebuffer.hpp"
#include "limits.hpp"
#include "program.hpp"
#include "query.hpp"
#include "renderbuffer.hpp"
#include "sampler.hpp"
#include "scope.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"

#include "internal/enable.hpp"
#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"

/* moderngl.core.create_context(...)
 * Returns a Context object.
 */
PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 4) {
        // TODO: error
        return 0;
    }

    bool standalone = (bool)PyObject_IsTrue(args[0]);
    bool debug = (bool)PyObject_IsTrue(args[1]);
    PyObject * glhook = args[2];
    PyObject * gc = args[3];

    MGLContext * context = new_object(MGLContext, MGLContext_class);
    context->glsl_compiler_error = moderngl_compiler_error;
    context->glsl_linker_error = moderngl_linker_error;

    if (!context->gl_context.load(standalone)) {
        return 0;
    }

    if (!context->gl.load()) {
        return 0;
    }

    if (gc != Py_None) {
        context->gc = NEW_REF(gc);
    }

    const GLMethods & gl = context->gl;

    if (glhook != Py_None) {
        PyObject * dtype = PyUnicode_FromFormat("u%d", sizeof(void *));
        PyObject * glprocs = PyMemoryView_FromMemory((char *)&context->gl, sizeof(context->gl), PyBUF_WRITE);
        PyObject * result = call_function(glhook, glprocs, dtype);
        if (!result) {
            return 0;
        }
        Py_DECREF(dtype);
        Py_DECREF(glprocs);
        Py_DECREF(result);
    }

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

    context->enable_only = read_enable_only(gl);

    gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl.Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    gl.Enable(GL_PRIMITIVE_RESTART);
    gl.PrimitiveRestartIndex(-1);

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

/* _MGLContext_new_object(...)
 */
MGLObject * _MGLContext_new_object(MGLContext * self, PyTypeObject * type, PyTypeObject * cls, int slot) {
    MGLObject * res = new_object(MGLObject, type);
    res->wrapper = new_object(PyObject, cls);
    SLOT(res->wrapper, MGLObject, slot) = NEW_REF(res);
    res->context = NEW_REF(self);
    if (self->gc) {
        PyObject * track = PyObject_GetAttrString(self->gc, "append");
        if (track) {
            PyObject * result = call_function(track, res->wrapper);
            if (!result) {
                return 0;
            }
            Py_DECREF(result);
        }
    }
    return res;
}

/* _MGLObject_release(...)
 */
PyObject * _MGLObject_release(MGLObject * self) {
    if (self->context->gc) {
        PyObject * untrack = PyObject_GetAttrString(self->context->gc, "remove");
        if (untrack) {
            PyObject * result = call_function(untrack, self->wrapper);
            if (!result) {
                return 0;
            }
            Py_DECREF(result);
        }
    }
    Py_DECREF(self->wrapper);
    Py_DECREF(self->context);
    Py_DECREF(self);
    Py_RETURN_NONE;
}

/* _MGLObject_pop_mglo(...)
 */
MGLObject * _MGLObject_pop_mglo(PyObject * wrapper, int slot) {
    MGLObject * mglo = SLOT(wrapper, MGLObject, slot);
    SLOT(wrapper, MGLObject, slot) = 0;
    return mglo;
}

PyTypeObject * MGLContext_class;
