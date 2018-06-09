#pragma once
#include "mgl.hpp"

struct MGLDataType {
	int base_format[5];
	int internal_format[5];
	int gl_type;
	int size;
};

MGLDataType * from_dtype(PyObject * dtype);
