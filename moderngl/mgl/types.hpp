#pragma once
#include "mgl.hpp"
#include "gl_context.hpp"
#include "gl_methods.hpp"

struct MGLContext {
	PyObject_HEAD
	PyObject * wrapper;
	GLContext gl_context;
	GLMethods gl;
};
