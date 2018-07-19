#pragma once
#include "python.hpp"

/* Duplicate an object in memory.
 * Providing arrays for PyType_FromSpec.
 */
template <typename T>
T * dup(const T & t) {
    T * res = (T *)malloc(sizeof(t));
    memcpy(res, &t, sizeof(t));
    return res;
}

PyTypeObject * detect_class(PyObject * module, const char * name, int & slots_len);
int slot_offset(PyTypeObject * type, const char * name, int & counter);
void assert_slots_len(PyTypeObject * type, int slots_len);
