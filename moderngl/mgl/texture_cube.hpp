#pragma once
#include "types.hpp"

PyObject * MGLContext_meth_texture_cube(MGLContext * self, PyObject * args);

PyTypeObject * MGLTextureCube_define(int version_code);
