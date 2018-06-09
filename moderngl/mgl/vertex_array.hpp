#pragma once
#include "types.hpp"

PyObject * MGLContext_meth_vertex_array(MGLContext * self, PyObject * args);

PyTypeObject * MGLVertexArray_define(int version_code);
