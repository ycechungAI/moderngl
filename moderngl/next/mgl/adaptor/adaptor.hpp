#include "internal/python.hpp"

PyObject * get_new_wrapper(PyObject * old_wrapper);

PyObject * meth_extend_context(PyObject * self, PyObject * args);
PyObject * meth_extend_buffer(PyObject * self, PyObject * args);
