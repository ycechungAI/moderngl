#pragma once
#include "mgl.hpp"
#include "internal/opengl/gl_methods.hpp"

PyObject * get_limits(const GLMethods & gl, int version_code);
