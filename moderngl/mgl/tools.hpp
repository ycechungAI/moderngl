#pragma once
#include "mgl.hpp"

int slot_offset(PyTypeObject * type, const char * name, int & counter);
PyTypeObject * detect_class(PyObject * module, const char * name, int & slots_len);
void assert_slots_len(PyTypeObject * type, int slots_len);
void protect_slot(PyTypeObject * type, const char * name);
void remove_init(PyTypeObject * type);

int prepare_buffer(PyObject * data, Py_buffer * view);

template <typename T>
T * dup(const T & t) {
	T * res = (T *)malloc(sizeof(t));
	memcpy(res, &t, sizeof(t));
	return res;
}

inline PyObject * _new_object(PyTypeObject * type) {
	PyObject * res = 0;
	Py_INCREF(type);
	if (type->tp_flags & Py_TPFLAGS_HAVE_GC) {
		res = PyObject_GC_New(PyObject, type);
	} else {
		res = PyObject_New(PyObject, type);
	}
	// PyObject_GC_Track(wrapper);
	memset((char *)res + sizeof(PyObject), 0, type->tp_basicsize - sizeof(PyObject));
	return res;
}

#define new_object(type, typeobj) (type *)_new_object(typeobj)

#define NEW_REF(obj) (Py_INCREF(obj), obj)
