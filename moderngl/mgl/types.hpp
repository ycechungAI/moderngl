#pragma once
#include "mgl.hpp"
#include "gl_context.hpp"
#include "gl_methods.hpp"
#include "data_type.hpp"

struct MGLContext {
	PyObject_HEAD
	PyObject * wrapper;
	GLContext gl_context;
	GLMethods gl;

	int version_code;
	int default_texture_unit;
	int enable_flags;

	PyTypeObject * MGLBuffer_type;
	PyTypeObject * MGLFramebuffer_type;
	PyTypeObject * MGLProgram_type;
	PyTypeObject * MGLQuery_type;
	PyTypeObject * MGLRenderbuffer_type;
	PyTypeObject * MGLScope_type;
	PyTypeObject * MGLTextureArray_type;
	PyTypeObject * MGLTextureCube_type;
	PyTypeObject * MGLTexture_type;
	PyTypeObject * MGLVertexArray_type;
};

struct MGLObject {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
};

struct MGLBuffer {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
	int buffer_obj;
	int flags;
	Py_ssize_t size;
};

struct MGLProgram {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
	int program_obj;
	int shader_obj[5];
};

struct MGLVertexArray {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
	int vertex_array_obj;
	int max_vertices;
};

struct MGLTexture {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
	MGLDataType * data_type;
	int texture_obj;
	int width;
	int height;
	int components;
	int levels;
	int samples;
};

struct MGLTextureArray {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
	int texture_obj;
};

struct MGLTextureCube {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
	int texture_obj;
};

struct MGLFramebuffer {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
	int framebuffer_obj;
	int width;
	int height;
	int components;
	int samples;
	int viewport[4];
};

struct MGLRenderbuffer {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
	int renderbuffer_obj;
	int width;
	int height;
	int components;
	int samples;
};

struct MGLQuery {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
	int query_obj[4];
};

struct MGLScope {
	PyObject_HEAD
	MGLContext * context;
	PyObject * wrapper;
	MGLFramebuffer * framebuffer;
	MGLFramebuffer * old_framebuffer;
	int * textures;
	int * buffers;
	int num_textures;
	int num_buffers;
	int enable_flags;
	int old_enable_flags;
};
