#pragma once
#include "context.hpp"

struct MGLTexture;

struct MGLSampler : public MGLContextObject {
    MGLTexture * texture;
    int sampler_obj;

    struct {
        PyObject * anisotropy;
        PyObject * border;
        PyObject * compare_func;
        PyObject * filter;
        PyObject * lod_bias;
        PyObject * lod_range;
        PyObject * texture;
        PyObject * wrap;
    } slots;
};

extern PyType_Spec MGLSampler_spec;
extern PyTypeObject * MGLSampler_class;

PyObject * MGLContext_meth_sampler(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
