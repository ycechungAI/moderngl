#pragma once
#include "mgl.hpp"

struct MGLContext;

struct MGLVertexArray {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    int vertex_array_obj;
    int max_vertices;
};
