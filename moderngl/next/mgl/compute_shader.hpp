#pragma once
#include "context.hpp"

struct MGLComputeShader : public MGLContextObject {
    int program_obj;
    int shader_obj;
};

extern PyType_Spec MGLComputeShader_spec;
extern PyTypeObject * MGLComputeShader_class;

PyObject * MGLContext_meth_compute_shader(MGLContext * self, PyObject * source);
