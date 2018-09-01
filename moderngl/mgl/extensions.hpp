#pragma once
#include "mgl.hpp"
#include "context.hpp"

PyObject * meth_extensions(PyObject * self, PyObject * const * args, Py_ssize_t nargs);
PyObject * meth_hwinfo(PyObject * self, PyObject * const * args, Py_ssize_t nargs);
PyObject * meth_glprocs(PyObject * self, PyObject * const * args, Py_ssize_t nargs);
