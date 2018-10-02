#pragma once
#include "mgl.hpp"
#include "context.hpp"

struct MGLQuery {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    int query_obj[4];
};
