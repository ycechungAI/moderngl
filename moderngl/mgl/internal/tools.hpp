#pragma once
#include "python.hpp"

Py_ssize_t unpack_size(PyObject * size);
int prepare_buffer(PyObject * data, Py_buffer * view);
