#pragma once
#include "../mgl.hpp"

extern PyTypeObject * Uniform_class;
extern int Uniform_class_type;
extern int Uniform_class_location;
extern int Uniform_class_cols;
extern int Uniform_class_rows;
extern int Uniform_class_size;
extern int Uniform_class_shape;

void MGLUniform_init_wrapper();
