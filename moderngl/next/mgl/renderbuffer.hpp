#pragma once
#include "context.hpp"

struct MGLDataType;

struct MGLRenderbuffer : public MGLContextObject {
    MGLDataType * data_type;
    int renderbuffer_obj;
    int width;
    int height;
    int depth;
    int components;
    int levels;
    int samples;
};

extern PyType_Spec MGLRenderbuffer_spec;
extern PyTypeObject * MGLRenderbuffer_class;

PyObject * MGLContext_meth_renderbuffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
PyObject * MGLContext_meth_depth_renderbuffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
