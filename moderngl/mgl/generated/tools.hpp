#pragma once
#include "../mgl.hpp"
#include "../internal/modules.hpp"

PyTypeObject * detect_class(PyObject * module, const char * name, int & slots_len);
int slot_offset(PyTypeObject * type, const char * name, int & counter);
void assert_slots_len(PyTypeObject * type, int slots_len);
