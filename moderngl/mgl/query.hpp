#pragma once
#include "mgl.hpp"
#include "context.hpp"

struct MGLQuery {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    // TODO:
};
