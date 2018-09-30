#pragma once
#include "mgl.hpp"
#include "context.hpp"

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

PyObject * MGLContext_meth_program(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);

void MGLTexture_define(MGLContext * ctx);
