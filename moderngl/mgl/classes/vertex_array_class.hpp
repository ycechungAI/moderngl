#pragma once
#include "../mgl.hpp"

extern PyTypeObject * VertexArray_class;
extern int VertexArray_class_mglo;
extern int VertexArray_class_ibo;
extern int VertexArray_class_program;
extern int VertexArray_class_mode;
extern int VertexArray_class_vertices;
extern int VertexArray_class_extra;

void MGLVertexArray_init_wrapper();
