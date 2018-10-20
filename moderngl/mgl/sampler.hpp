#pragma once
#include "mgl.hpp"

struct MGLContext;

struct MGLSampler {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    int sampler_obj;
    // TODO:
};
