#pragma once
#include "types.hpp"

PyObject * MGLContext_meth_program(MGLContext * self, PyObject * args);

PyTypeObject * MGLProgram_define(int version_code);
