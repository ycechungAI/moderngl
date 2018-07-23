#pragma once
#include "python.hpp"

PyTypeObject * define_python_class(const char * name, int size, PyType_Slot * slots);
