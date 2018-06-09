#pragma once
#include "types.hpp"

PyObject * MGLContext_meth_scope(MGLContext * self, PyObject * args);

PyTypeObject * MGLScope_define(int version_code);
