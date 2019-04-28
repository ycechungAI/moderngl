#pragma once
#include "context.hpp"

struct MGLDataType;

struct MGLFramebuffer : public MGLContextObject {
    int framebuffer_obj;
    int width;
    int height;
    int depth;
    int components;
    int levels;
    int samples;
    int viewport[4];
    int attachments;
    char attachment_type[16];
    unsigned long long color_mask;
    bool depth_mask;
};

extern PyType_Spec MGLFramebuffer_spec;
extern PyTypeObject * MGLFramebuffer_class;

PyObject * MGLContext_meth_framebuffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
void MGLFramebuffer_use_core(MGLFramebuffer * self);
