#pragma once
#include "internal/python.hpp"

#ifdef MGL_DEBUG
#define mgl_name "moderngl.mgl_dbg"
#define PyInit_mgl PyInit_mgl_dbg
#else
#define mgl_name "moderngl.mgl"
#endif
