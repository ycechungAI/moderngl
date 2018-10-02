#pragma once
#include "mgl.hpp"
#include "internal/opengl/gl_context.hpp"
#include "internal/opengl/gl_methods.hpp"

struct MGLContext {
    PyObject_HEAD
    PyObject * wrapper;
    GLContext gl_context;
    GLMethods gl;

    int version_code;
    int default_texture_unit;
    int enable_only;

    PyTypeObject * MGLBuffer_class;
    PyTypeObject * MGLFramebuffer_class;
    PyTypeObject * MGLProgram_class;
    PyTypeObject * MGLQuery_class;
    PyTypeObject * MGLRenderbuffer_class;
    PyTypeObject * MGLSampler_class;
    PyTypeObject * MGLScope_class;
    PyTypeObject * MGLTexture_class;
    PyTypeObject * MGLVertexArray_class;

    PyObject * gc;
};

PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs);

/* Every objects derived from a Context must have the following HEAD */

struct MGLObject {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
};

/* Every internal object has its own wrapper class defined in the python layer.
 * An internal object holds a ref to the instance of its wrapper class.
 * The wrapper class holds a ref to the internal object.
 * Such a pair can be created using MGLContext_new_object.
 * The circular reference must be resolved when releasing objects.
 */

MGLObject * _MGLContext_new_object(MGLContext * self, PyTypeObject * type, PyTypeObject * cls, int slot);
MGLObject * _MGLObject_pop_mglo(PyObject * wrapper, int slot);
PyObject * _MGLObject_release(MGLObject * self);

#define MGLContext_new_object(self, name) (MGL ## name *)_MGLContext_new_object(self, self->MGL ## name ## _class, name ## _class, name ## _class_mglo)
#define MGLObject_pop_mglo(name, obj) (MGL ## name *)_MGLObject_pop_mglo(obj, name ## _class_mglo)
#define MGLObject_release(obj) _MGLObject_release((MGLObject *)obj);

extern PyTypeObject * MGLContext_class;

/* Macros */

#ifdef MGL_DEBUG
#define ENSURE_CONTEXT(ctx, ret) if (!ctx->gl_context.active()) { PyErr_Format(PyExc_AssertionError, "the current context is not active"); return ret; }
#else
#define ENSURE_CONTEXT(ctx, ret)
#endif
