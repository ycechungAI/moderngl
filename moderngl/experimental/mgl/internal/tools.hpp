#pragma once
#include "python.hpp"

Py_ssize_t unpack_size(PyObject * size);
int prepare_buffer(PyObject * data, Py_buffer * view);
bool unpack_viewport(PyObject * viewport, int & x, int & y, int & width, int & height);
bool unpack_viewport(PyObject * viewport, int & x, int & y, int & z, int & width, int & height, int & depth);

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

PyTypeObject * detect_class(PyObject * module, const char * name, int & slots_len);
int slot_offset(PyTypeObject * type, const char * name, int & counter);
void assert_slots_len(PyTypeObject * type, int slots_len);
bool starts_with(const char * str, const char * prefix);
void clear_slots(PyObject * obj);
