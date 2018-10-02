#pragma once
#include "mgl.hpp"
#include "context.hpp"

struct MGLSampler {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    int sampler_obj;
    // TODO:
};
