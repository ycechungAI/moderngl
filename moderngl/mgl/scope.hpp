#pragma once
#include "mgl.hpp"
#include "context.hpp"

struct MGLFramebuffer;

struct MGLScope {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    MGLFramebuffer * framebuffer;
    MGLFramebuffer * old_framebuffer;
    int * samplers;
    int * buffers;
    int num_samplers;
    int num_buffers;
    int enable_only;
    int old_enable_only;
};
