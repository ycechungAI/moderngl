#pragma once
#include "types.hpp"

PyObject * MGLContext_meth_texture_array(MGLContext * self, PyObject * args);

PyTypeObject * MGLTextureArray_define(int version_code);
