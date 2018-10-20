#pragma once
#include "mgl.hpp"

struct MGLContext;
struct MGLFramebuffer;
struct MGLSampler;
struct MGLBuffer;

struct MGLScopeBinding {
    union {
        MGLSampler * sampler;
        MGLBuffer * buffer;
    };
    int binding;
};

struct MGLScope {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    MGLFramebuffer * framebuffer;
    MGLFramebuffer * old_framebuffer;
    MGLScopeBinding * bindings;
    int num_samplers;
    int num_uniform_buffers;
    int num_storage_buffers;
    int enable_only;
    int old_enable_only;
};
