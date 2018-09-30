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
    int enable_flags;

    PyTypeObject * MGLBuffer_class;
    PyTypeObject * MGLProgram_class;
    PyTypeObject * MGLTexture_class;
    PyTypeObject * MGLVertexArray_class;
};

PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs);

void MGLContext_define();

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

inline MGLObject * _MGLContext_new_object(MGLContext * self, PyTypeObject * type, PyTypeObject * cls, int slot) {
    MGLObject * res = new_object(MGLObject, type);
    res->wrapper = new_object(PyObject, cls);
    SLOT(res->wrapper, MGLObject, slot) = res;
    res->context = self;
    Py_INCREF(self);
    return res;
}

#define MGLContext_new_object(self, name) (MGL ## name *)_MGLContext_new_object(self, self->MGL ## name ## _class, name ## _class, name ## _class_mglo)

extern PyTypeObject * MGLContext_class;

/* Macros */

#ifdef MGL_DEBUG
#define ENSURE_CONTEXT(ctx, ret) if (!ctx->gl_context.active()) { PyErr_Format(PyExc_AssertionError, "the current context is not active"); return ret; }
#else
#define ENSURE_CONTEXT(ctx, ret)
#endif
