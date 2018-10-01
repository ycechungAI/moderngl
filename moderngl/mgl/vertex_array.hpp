#pragma once
#include "mgl.hpp"
#include "context.hpp"

struct MGLVertexArray {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    int vertex_array_obj;
    int max_vertices;
};
