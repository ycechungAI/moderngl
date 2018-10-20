#pragma once
#include "mgl.hpp"

struct MGLContext;
struct MGLDataType;

struct MGLTexture {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    MGLDataType * data_type;
    int texture_obj;
    int texture_target;
    int width;
    int height;
    int depth;
    int components;
    int levels;
    int samples;
};
