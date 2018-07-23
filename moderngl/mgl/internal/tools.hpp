#pragma once
#include "python.hpp"

PyTypeObject * detect_class(PyObject * module, const char * name, int & slots_len);
int slot_offset(PyTypeObject * type, const char * name, int & counter);
void assert_slots_len(PyTypeObject * type, int slots_len);
Py_ssize_t unpack_size(PyObject * size);
