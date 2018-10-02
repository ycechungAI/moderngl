#pragma once
#include "mgl.hpp"
#include "context.hpp"

struct MGLFramebuffer;

struct MGLScope {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
	MGLFramebuffer * framebuffer;
	MGLFramebuffer * old_framebuffer;
	int * textures;
	int * buffers;
	int num_textures;
	int num_buffers;
	int enable_only;
	int old_enable_only;
};
