#pragma once
#include "mgl.hpp"
#include "gl_methods.hpp"

struct GLTypeInfo {
	int cols;
	int rows;
	int shape;
};

int slot_offset(PyTypeObject * type, const char * name, int & counter);
PyTypeObject * detect_class(PyObject * module, const char * name, int & slots_len);
void assert_slots_len(PyTypeObject * type, int slots_len);
void protect_slot(PyTypeObject * type, const char * name);
void protect_all_slots(PyTypeObject * type);
void remove_init(PyTypeObject * type);

int prepare_buffer(PyObject * data, Py_buffer * view);

void clean_glsl_name(char * name, int & name_len);
bool unpack_viewport(PyObject * viewport, int & x, int & y, int & width, int & height);
bool unpack_viewport(PyObject * viewport, int & x, int & y, int & z, int & width, int & height, int & depth);
Py_ssize_t unpack_size(PyObject * value);
void enable_only(const GLMethods & gl, int flags);

GLTypeInfo type_info(int type);

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

inline PyObject * int_tuple(int i0, int i1) {
	PyObject * res = PyTuple_New(2);
	PyTuple_SET_ITEM(res, 0, PyLong_FromLong(i0));
	PyTuple_SET_ITEM(res, 1, PyLong_FromLong(i1));
	return res;
}

inline PyObject * int_tuple(int i0, int i1, int i2) {
	PyObject * res = PyTuple_New(3);
	PyTuple_SET_ITEM(res, 0, PyLong_FromLong(i0));
	PyTuple_SET_ITEM(res, 1, PyLong_FromLong(i1));
	PyTuple_SET_ITEM(res, 2, PyLong_FromLong(i2));
	return res;
}

inline PyObject * int_tuple(int i0, int i1, int i2, int i3) {
	PyObject * res = PyTuple_New(4);
	PyTuple_SET_ITEM(res, 0, PyLong_FromLong(i0));
	PyTuple_SET_ITEM(res, 1, PyLong_FromLong(i1));
	PyTuple_SET_ITEM(res, 2, PyLong_FromLong(i2));
	PyTuple_SET_ITEM(res, 3, PyLong_FromLong(i3));
	return res;
}

void read_float(float *& ptr, PyObject * value);
void read_int(int *& ptr, PyObject * value);
void read_unsigned(unsigned *& ptr, PyObject * value);
void read_double(double *& ptr, PyObject * value);
void read_bool(int *& ptr, PyObject * value);

typedef void (* read_value)(void *& ptr, PyObject * value);
