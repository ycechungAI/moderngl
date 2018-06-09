#pragma once
#include "mgl.hpp"

int slot_offset(PyTypeObject * type, const char * name, int & counter);
PyTypeObject * detect_class(PyObject * module, const char * name, int & slots_len);
void assert_slots_len(PyTypeObject * type, int slots_len);
void protect_slot(PyTypeObject * type, const char * name);
void remove_init(PyTypeObject * type);

template <typename T>
T * dup(const T & t) {
	T * res = (T *)malloc(sizeof(t));
	memcpy(res, &t, sizeof(t));
	return res;
}
