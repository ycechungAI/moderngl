#pragma once
#include "mgl.hpp"

struct MGLContext;
struct MGLDataType;

struct MGLFramebuffer {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    int framebuffer_obj;
    int width;
    int height;
    int depth;
    int components;
    int levels;
    int samples;
    int viewport[4];
    int attachments;
    char * attachment_type;
    unsigned long long color_mask;
    bool depth_mask;
};

extern PyType_Spec MGLFramebuffer_spec;
PyObject * MGLContext_meth_framebuffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
void MGLFramebuffer_use_core(MGLFramebuffer * self);
