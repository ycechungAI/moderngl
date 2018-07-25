#pragma once
#include "mgl.hpp"
#include "context.hpp"

struct MGLVertexArray {
	PyObject_HEAD
	PyObject * wrapper;
	MGLContext * context;
	int vertex_array_obj;
	int max_vertices;
};

PyObject * MGLContext_meth_vertex_array(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);

void MGLVertexArray_define(MGLContext * ctx);
