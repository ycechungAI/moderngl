#pragma once
#include "mgl.hpp"
#include "context.hpp"

struct MGLProgram {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    int program_obj;
    int shader_obj[5];
};

PyObject * MGLContext_meth_program(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);

void MGLProgram_define(MGLContext * ctx);
