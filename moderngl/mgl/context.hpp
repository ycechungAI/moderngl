#pragma once
#include "mgl.hpp"
#include "internal/opengl/gl_context.hpp"
#include "internal/opengl/gl_methods.hpp"
#include "internal/tools.hpp"

struct MGLContext {
    PyObject_HEAD
    PyObject * wrapper;
	GLContext gl_context;
	GLMethods gl;

    PyTypeObject * MGLBuffer_class;
};

PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs);

void MGLContext_define();
