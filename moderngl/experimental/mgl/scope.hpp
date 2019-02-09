#pragma once
#include "mgl.hpp"

struct MGLContext;
struct MGLFramebuffer;
struct MGLSampler;
struct MGLBuffer;

struct MGLScopeBinding {
    union {
        PyObject * object;
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
    MGLScope * old_scope;
    int num_samplers;
    int num_uniform_buffers;
    int num_storage_buffers;
    int enable_only;
    int old_enable_only;
};

extern PyType_Spec MGLScope_spec;
PyObject * MGLContext_meth_scope(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
void MGLScope_begin_core(MGLScope * self);
void MGLScope_end_core(MGLScope * self);
