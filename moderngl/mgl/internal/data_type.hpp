#pragma once
#include "python.hpp"

struct MGLDataType {
    PyObject * numpy_dtype;
    int base_format[5];
    int internal_format[5];
    int gl_type;
    int size;
    char shape;
};

MGLDataType * from_dtype(PyObject * dtype);
