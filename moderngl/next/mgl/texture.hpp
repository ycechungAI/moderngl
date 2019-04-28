#pragma once
#include "context.hpp"

struct MGLDataType;

struct MGLTexture : public MGLContextObject {
    MGLDataType * data_type;
    int texture_obj;
    int texture_target;
    int width;
    int height;
    int depth;
    int components;
    int levels;
    int samples;
    int dimensions;
    int expected_size;
};

extern PyType_Spec MGLTexture_spec;
extern PyTypeObject * MGLTexture_class;

PyObject * MGLContext_meth_texture(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
