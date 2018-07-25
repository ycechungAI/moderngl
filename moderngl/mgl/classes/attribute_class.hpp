#pragma once
#include "../mgl.hpp"

extern PyTypeObject * Attribute_class;
extern int Attribute_class_type;
extern int Attribute_class_location;
extern int Attribute_class_cols;
extern int Attribute_class_rows;
extern int Attribute_class_size;
extern int Attribute_class_shape;

void MGLAttribute_init_wrapper();
