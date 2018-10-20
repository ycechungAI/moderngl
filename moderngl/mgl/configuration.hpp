#pragma once
#include "mgl.hpp"

struct MGLContext;

PyObject * MGLContext_meth_configure(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
