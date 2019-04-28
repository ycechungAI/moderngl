#pragma once
#include "context.hpp"

struct MGLProgram : public MGLContextObject {
    int program_obj;
    int shader_obj[5];
};

extern PyType_Spec MGLProgram_spec;
extern PyTypeObject * MGLProgram_class;

PyObject * MGLContext_meth_program(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
