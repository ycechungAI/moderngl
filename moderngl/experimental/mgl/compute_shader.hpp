#pragma once
#include "mgl.hpp"

struct MGLContext;

struct MGLComputeShader {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
    int program_obj;
    int shader_obj;
};

extern PyType_Spec MGLComputeShader_spec;
PyObject * MGLContext_meth_compute_shader(MGLContext * self, PyObject * source);
