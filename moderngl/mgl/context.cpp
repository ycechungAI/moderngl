#include "context.hpp"

#include "buffer.hpp"
#include "framebuffer.hpp"
#include "program.hpp"
#include "query.hpp"
#include "renderbuffer.hpp"
#include "scope.hpp"
#include "texture.hpp"
#include "texture_array.hpp"
#include "texture_cube.hpp"
#include "vertex_array.hpp"
#include "tools.hpp"

PyObject * meth_create_context(PyObject * self, PyObject * args) { TRACE_VARAGS
	int standalone;

	int args_ok = PyArg_ParseTuple(
		args,
		"p",
		&standalone
	);

	if (!args_ok) {
		return 0;
	}

	initialize_module();

	if (PyErr_Occurred()) {
		return 0;
	}

	Py_INCREF(MGLContext_type);
	MGLContext * context = PyObject_New(MGLContext, MGLContext_type);

	if (!context->gl_context.load(standalone)) {
		return 0;
	}

	if (!context->gl.load()) {
		return 0;
	}

	const GLMethods & gl = context->gl;

	int major = 0;
	int minor = 0;
	gl.GetIntegerv(GL_MAJOR_VERSION, &major);
	gl.GetIntegerv(GL_MINOR_VERSION, &minor);
	int version_code = major * 100 + minor * 10;

	int max_texture_units = 0;
	gl.GetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint *)&max_texture_units);
	context->default_texture_unit = max_texture_units - 1;
	context->version_code = version_code;
	context->enable_flags = 0;

	gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gl.Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	gl.Enable(GL_PRIMITIVE_RESTART);
	gl.PrimitiveRestartIndex(-1);

	context->MGLBuffer_type = MGLBuffer_define(version_code);
	context->MGLFramebuffer_type = MGLFramebuffer_define(version_code);
	context->MGLProgram_type = MGLProgram_define(version_code);
	context->MGLQuery_type = MGLQuery_define(version_code);
	context->MGLRenderbuffer_type = MGLRenderbuffer_define(version_code);
	context->MGLScope_type = MGLScope_define(version_code);
	context->MGLTextureArray_type = MGLTextureArray_define(version_code);
	context->MGLTextureCube_type = MGLTextureCube_define(version_code);
	context->MGLTexture_type = MGLTexture_define(version_code);
	context->MGLVertexArray_type = MGLVertexArray_define(version_code);

	PyObject * framebuffers = PyDict_New();

	context->wrapper = new_object(PyObject, Context_class);
	SLOT(context->wrapper, MGLContext, Context_class_mglo) = context;
	SLOT(context->wrapper, PyObject, Context_class_framebuffers) = framebuffers;
	SLOT(context->wrapper, PyObject, Context_class_screen) = PyObject_CallMethodObjArgs(context->wrapper, detect_framebuffer_str, zero_long, 0);
	SLOT(context->wrapper, PyObject, Context_class_fbo) = PyObject_CallMethodObjArgs(context->wrapper, detect_framebuffer_str, Py_None, 0);
	return NEW_REF(context->wrapper);
}

PyObject * MGLContext_meth_detect_framebuffer(MGLContext * self, PyObject * args) { TRACE_VARAGS
	PyObject * fbo;

	int args_ok = PyArg_ParseTuple(
		args,
		"O",
		&fbo
	);

	if (!args_ok) {
		return 0;
	}

	const GLMethods & gl = self->gl;

	int bound_framebuffer = 0;
	gl.GetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &bound_framebuffer);

	int framebuffer_obj = fbo == Py_None ? bound_framebuffer : PyLong_AsLong(fbo);

	if (PyErr_Occurred()) {
		return 0;
	}

	PyObject * key = PyLong_FromLong(framebuffer_obj);
	PyObject * framebuffers = SLOT(self->wrapper, PyObject, Context_class_framebuffers);
	PyObject * detected = PyDict_GetItem(framebuffers, key);
	if (detected) {
		return NEW_REF(detected);
	}

	MGLFramebuffer * framebuffer = MGLContext_new_object(self, Framebuffer);
	framebuffer->framebuffer_obj = framebuffer_obj;

	// TODO: detect attachments

	PyDict_SetItem(framebuffers, key, NEW_REF(framebuffer->wrapper));
	return NEW_REF(framebuffer->wrapper);
}

PyObject * MGLContext_meth_copy_buffer(MGLContext * self, PyObject * args) { TRACE_VARAGS
	PyObject * dst;
	PyObject * src;

	Py_ssize_t size;
	Py_ssize_t read_offset;
	Py_ssize_t write_offset;

	int args_ok = PyArg_ParseTuple(
		args,
		"OOnnn",
		&dst,
		&src,
		&size,
		&read_offset,
		&write_offset
	);

	if (!args_ok) {
		return 0;
	}

	if (dst->ob_type != Buffer_class) {
		PyErr_Format(module_error, "dst is not a buffer");
		return 0;
	}

	if (src->ob_type != Buffer_class) {
		PyErr_Format(module_error, "src is not a buffer");
		return 0;
	}

	MGLBuffer * buffer1 = SLOT(dst, MGLBuffer, Buffer_class_mglo);
	MGLBuffer * buffer2 = SLOT(src, MGLBuffer, Buffer_class_mglo);

	if (size < 0) {
		size = buffer2->size - read_offset;
	}

	if (read_offset < 0 || write_offset < 0) {
		PyErr_Format(module_error, "buffer underflow");
		return 0;
	}

	if (read_offset + size > buffer2->size || write_offset + size > buffer1->size) {
		PyErr_Format(module_error, "buffer overflow");
		return 0;
	}

	const GLMethods & gl = self->gl;

	gl.BindBuffer(GL_COPY_READ_BUFFER, buffer2->buffer_obj);
	gl.BindBuffer(GL_COPY_WRITE_BUFFER, buffer1->buffer_obj);
	gl.CopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, read_offset, write_offset, size);

	Py_RETURN_NONE;
}

PyObject * MGLContext_meth_enable(MGLContext * self, PyObject * args) { TRACE_VARAGS
	int flags;

	int args_ok = PyArg_ParseTuple(
		args,
		"I",
		&flags
	);

	if (!args_ok) {
		return 0;
	}

	enable_only(self->gl, flags);
	self->enable_flags = flags;
	Py_RETURN_NONE;
}

PyObject * MGLContext_meth_enter(MGLContext * self) { TRACE_NOARGS
	self->gl_context.enter();
	Py_RETURN_NONE;
}

PyObject * MGLContext_meth_exit(MGLContext * self) { TRACE_NOARGS
	self->gl_context.exit();
	Py_RETURN_NONE;
}

int MGLContext_set_line_width(MGLContext * self, PyObject * value) { TRACE_SETTER
	float line_width = (float)PyFloat_AsDouble(value);
	if (PyErr_Occurred()) {
		return -1;
	}
	self->gl.LineWidth(line_width);
	return 0;
}

int MGLContext_set_point_size(MGLContext * self, PyObject * value) { TRACE_SETTER
	float point_size = (float)PyFloat_AsDouble(value);
	if (PyErr_Occurred()) {
		return -1;
	}
	self->gl.LineWidth(point_size);
	return 0;
}

PyTypeObject * MGLContext_define() {
	PyMethodDef MGLContext_methods[] = {
		{"buffer", (PyCFunction)MGLContext_meth_buffer, METH_VARARGS, 0},
		{"framebuffer", (PyCFunction)MGLContext_meth_framebuffer, METH_VARARGS, 0},
		{"program", (PyCFunction)MGLContext_meth_program, METH_VARARGS, 0},
		{"query", (PyCFunction)MGLContext_meth_query, METH_VARARGS, 0},
		{"renderbuffer", (PyCFunction)MGLContext_meth_renderbuffer, METH_VARARGS, 0},
		{"scope", (PyCFunction)MGLContext_meth_scope, METH_VARARGS, 0},
		{"texture", (PyCFunction)MGLContext_meth_texture, METH_VARARGS, 0},
		{"texture_array", (PyCFunction)MGLContext_meth_texture_array, METH_VARARGS, 0},
		{"texture_cube", (PyCFunction)MGLContext_meth_texture_cube, METH_VARARGS, 0},
		{"vertex_array", (PyCFunction)MGLContext_meth_vertex_array, METH_VARARGS, 0},
		{"detect_framebuffer", (PyCFunction)MGLContext_meth_detect_framebuffer, METH_VARARGS, 0},
		{"copy_buffer", (PyCFunction)MGLContext_meth_copy_buffer, METH_VARARGS, 0},
		{"enable", (PyCFunction)MGLContext_meth_enable, METH_VARARGS, 0},
		{"enter", (PyCFunction)MGLContext_meth_enter, METH_NOARGS, 0},
		{"exit", (PyCFunction)MGLContext_meth_exit, METH_NOARGS, 0},
		{0},
	};

	PyGetSetDef MGLContext_getset[] = {
		{"line_width", 0, (setter)MGLContext_set_line_width, 0, 0},
		{"point_size", 0, (setter)MGLContext_set_point_size, 0, 0},
		{0},
	};

	PyType_Slot MGLContext_slots[] = {
		{Py_tp_methods, dup(MGLContext_methods)},
		{Py_tp_getset, dup(MGLContext_getset)},
		{0},
	};

	PyType_Spec MGLContext_spec = {"moderngl.mgl.Context", sizeof(MGLContext), 0, Py_TPFLAGS_DEFAULT, MGLContext_slots};
	return (PyTypeObject *)PyType_FromSpec(&MGLContext_spec);
}
