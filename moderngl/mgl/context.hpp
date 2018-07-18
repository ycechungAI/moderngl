#pragma once
#include "mgl.hpp"

struct MGLContext {
    PyObject_HEAD
    PyObject * wrapper;

    PyTypeObject * MGLBuffer_class;
};

PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs);
