#pragma once
#include "types.hpp"

PyObject * MGLContext_meth_renderbuffer(MGLContext * self, PyObject * args);

PyTypeObject * MGLRenderbuffer_define(int version_code);
