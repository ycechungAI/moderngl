#pragma once
#include "mgl.hpp"
#include "gl_context.hpp"
#include "gl_methods.hpp"

struct MGLContext {
	PyObject_HEAD
	PyObject * wrapper;
	GLContext gl_context;
	GLMethods gl;
	PyTypeObject * MGLBuffer_type;
};

struct MGLObject {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
};

struct MGLBuffer {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
	int buffer_obj;
	int flags;
	Py_ssize_t size;
};
