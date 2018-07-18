#pragma once
#include "mgl.hpp"
#include "context.hpp"
#include "internal/tools.hpp"

struct MGLBuffer {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
};

PyObject * MGLContext_meth_buffer(PyObject * self, PyObject * const * args, Py_ssize_t nargs);

void MGLBuffer_define(MGLContext * ctx);
