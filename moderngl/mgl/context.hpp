#pragma once
#include "mgl.hpp"
#include "types.hpp"

PyObject * meth_create_context(PyObject * self, PyObject * args);

PyTypeObject * MGLContext_define();
