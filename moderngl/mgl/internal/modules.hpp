#pragma once
#include "python.hpp"

/* Python modules */

extern PyObject * moderngl;
extern PyObject * moderngl_error;

bool load_modules();
