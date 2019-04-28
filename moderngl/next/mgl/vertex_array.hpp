#pragma once
#include "context.hpp"

struct MGLBuffer;

struct MGLVertexArray : public MGLContextObject {
    MGLBuffer * index_buffer;
    int vertex_array_obj;
    int max_vertices;
};

extern PyType_Spec MGLVertexArray_spec;
extern PyTypeObject * MGLVertexArray_class;

PyObject * MGLContext_meth_vertex_array(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
