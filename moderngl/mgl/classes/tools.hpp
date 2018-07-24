#pragma once
#include "../mgl.hpp"
#include "../internal/modules.hpp"

PyTypeObject * define_python_class(const char * name, int size, PyType_Slot * slots);
PyTypeObject * detect_class(PyObject * module, const char * name, int & slots_len);
int slot_offset(PyTypeObject * type, const char * name, int & counter);
void assert_slots_len(PyTypeObject * type, int slots_len);
