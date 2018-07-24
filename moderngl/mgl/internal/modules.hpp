#pragma once
#include "python.hpp"

/* Python modules */

extern PyObject * moderngl;
extern PyObject * moderngl_error;
extern PyObject * moderngl_compiler_error;
extern PyObject * moderngl_linker_error;

bool load_modules();
