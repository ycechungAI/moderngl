#pragma once
#include "types.hpp"

PyObject * MGLContext_meth_framebuffer(MGLContext * self, PyObject * args);
PyObject * MGLFramebuffer_meth_use(MGLFramebuffer * self);

PyTypeObject * MGLFramebuffer_define(int version_code);
