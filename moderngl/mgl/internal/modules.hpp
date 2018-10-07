#pragma once
#include "python.hpp"

/* Python modules */

extern PyObject * moderngl;
extern PyObject * moderngl_error;
extern PyObject * moderngl_compiler_error;
extern PyObject * moderngl_linker_error;
extern PyObject * moderngl_split_format;

extern PyObject * numpy;
extern PyObject * numpy_frombuffer;

extern PyObject * pillow;
extern PyObject * pillow_image;

extern PyObject * tobytes_str;
extern PyObject * size_str;
extern PyObject * mode_str;
extern PyObject * points_long;
extern PyObject * triangles_long;

bool load_modules();
