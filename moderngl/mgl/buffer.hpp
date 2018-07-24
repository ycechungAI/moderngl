#pragma once
#include "mgl.hpp"
#include "context.hpp"

struct MGLBuffer {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
};

PyObject * MGLContext_meth_buffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);

void MGLBuffer_define(MGLContext * ctx);
