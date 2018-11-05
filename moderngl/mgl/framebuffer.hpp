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
