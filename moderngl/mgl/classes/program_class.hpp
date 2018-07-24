#pragma once
#include "../mgl.hpp"

extern PyTypeObject * Program_class;
extern int Program_class_mglo;
extern int Program_class_attributes;
extern int Program_class_uniforms;
extern int Program_class_extra;

void MGLProgram_init_wrapper();
