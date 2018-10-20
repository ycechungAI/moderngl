#pragma once
#include "mgl.hpp"

struct MGLContext;

enum MGLQueryKeys {
    TIME_ELAPSED,
    PRIMITIVES_GENERATED,
    SAMPLES_PASSED,
    ANY_SAMPLES_PASSED,
};

struct MGLQuery {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    int query_obj[4];
};
