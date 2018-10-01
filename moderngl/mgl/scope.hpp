#pragma once
#include "mgl.hpp"
#include "context.hpp"

struct MGLScope {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    // TODO:
};
