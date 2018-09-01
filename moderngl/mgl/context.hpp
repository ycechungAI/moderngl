#pragma once
#include "mgl.hpp"
#include "internal/opengl/gl_context.hpp"

#ifdef MGL_DEBUG
#include "internal/opengl/gl_methods_with_traces.hpp"
#else
#include "internal/opengl/gl_methods.hpp"
#endif

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
	PyTypeObject * MGLVertexArray_class;
};

PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs);

void MGLContext_define();

/* Macros */

#ifdef MGL_DEBUG
#define ENSURE_CONTEXT(ctx, ret) if (!ctx->gl_context.active()) { PyErr_Format(PyExc_AssertionError, "the current context is not active"); return ret; }
#else
#define ENSURE_CONTEXT(ctx, ret)
#endif
