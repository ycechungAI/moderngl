#pragma once
#include "types.hpp"

PyObject * MGLContext_meth_texture(MGLContext * self, PyObject * args);

PyTypeObject * MGLTexture_define(int version_code);
