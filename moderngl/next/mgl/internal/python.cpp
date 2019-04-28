#include "python.hpp"

void read_float(float *& ptr, PyObject * value) {
    *ptr++ = (float)PyFloat_AsDouble(value);
}

void read_int(int *& ptr, PyObject * value) {
    *ptr++ = PyLong_AsLong(value);
}

void read_unsigned(unsigned *& ptr, PyObject * value) {
    *ptr++ = PyLong_AsUnsignedLong(value);
}

void read_double(double *& ptr, PyObject * value) {
    *ptr++ = PyFloat_AsDouble(value);
}

void read_bool(int *& ptr, PyObject * value) {
    *ptr++ = PyObject_IsTrue(value);
}

void set_slot(PyObject * obj, const char * name, PyObject * value) {
    PyObject_SetAttrString(obj, name, value);
    if (PyErr_Occurred()) {
        printf("no slot %s.%s\n", Py_TYPE(obj)->tp_name, name);
        exit(0);
    }
    Py_DECREF(value);
}

PyObject * get_slot(PyObject * obj, const char * name) {
    PyObject * value = PyObject_GetAttrString(obj, name);
    if (!value) {
        printf("no slot %s.%s\n", Py_TYPE(obj)->tp_name, name);
        exit(0);
    }
    Py_XDECREF(value);
    return value;
}
