#include "Types.hpp"

PyObject * MGLContext_renderbuffer(MGLContext * self, PyObject * args) {
	int width;
	int height;

	int components;

	int samples;

	const char * dtype;
	Py_ssize_t dtype_size;

	int args_ok = PyArg_ParseTuple(
		args,
		"(II)IIs#",
		&width,
		&height,
		&components,
		&samples,
		&dtype,
		&dtype_size
	);

	if (!args_ok) {
		return 0;
	}

	if (components < 1 || components > 4) {
		MGLError_Set("the components must be 1, 2, 3 or 4");
		return 0;
	}

	if ((samples & (samples - 1)) || samples > self->max_samples) {
		MGLError_Set("the number of samples is invalid");
		return 0;
	}

	MGLDataType * data_type = from_dtype(dtype, dtype_size);

	if (!data_type) {
		MGLError_Set("invalid dtype");
		return 0;
	}

	int format = data_type->internal_format[components];

	const GLMethods & gl = self->gl;

    MGLRenderbuffer * renderbuffer = PyObject_New(MGLRenderbuffer, MGLRenderbuffer_type);
    renderbuffer->released = false;

	renderbuffer->renderbuffer_obj = 0;
	gl.GenRenderbuffers(1, (GLuint *)&renderbuffer->renderbuffer_obj);

	if (!renderbuffer->renderbuffer_obj) {
		MGLError_Set("cannot create renderbuffer");
		Py_DECREF(renderbuffer);
		return 0;
	}

	gl.BindRenderbuffer(GL_RENDERBUFFER, renderbuffer->renderbuffer_obj);

	if (samples == 0) {
		gl.RenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	} else {
		gl.RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
	}

	renderbuffer->width = width;
	renderbuffer->height = height;
	renderbuffer->components = components;
	renderbuffer->samples = samples;
	renderbuffer->data_type = data_type;
	renderbuffer->depth = false;

	Py_INCREF(self);
	renderbuffer->context = self;

	Py_INCREF(renderbuffer);

	PyObject * result = PyTuple_New(2);
	PyTuple_SET_ITEM(result, 0, (PyObject *)renderbuffer);
	PyTuple_SET_ITEM(result, 1, PyLong_FromLong(renderbuffer->renderbuffer_obj));
	return result;
}

PyObject * MGLContext_depth_renderbuffer(MGLContext * self, PyObject * args) {
	int width;
	int height;

	int samples;

	int args_ok = PyArg_ParseTuple(
		args,
		"(II)I",
		&width,
		&height,
		&samples
	);

	if (!args_ok) {
		return 0;
	}

	if ((samples & (samples - 1)) || samples > self->max_samples) {
		MGLError_Set("the number of samples is invalid");
		return 0;
	}

	const GLMethods & gl = self->gl;

	MGLRenderbuffer * renderbuffer = PyObject_New(MGLRenderbuffer, MGLRenderbuffer_type);
    renderbuffer->released = false;

	renderbuffer->renderbuffer_obj = 0;
	gl.GenRenderbuffers(1, (GLuint *)&renderbuffer->renderbuffer_obj);

	if (!renderbuffer->renderbuffer_obj) {
		MGLError_Set("cannot create renderbuffer");
		Py_DECREF(renderbuffer);
		return 0;
	}

	gl.BindRenderbuffer(GL_RENDERBUFFER, renderbuffer->renderbuffer_obj);

	if (samples == 0) {
		gl.RenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	} else {
		gl.RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, width, height);
	}

	renderbuffer->width = width;
	renderbuffer->height = height;
	renderbuffer->components = 1;
	renderbuffer->samples = samples;
	renderbuffer->data_type = from_dtype("f4", 2);
	renderbuffer->depth = true;

	Py_INCREF(self);
	renderbuffer->context = self;

	Py_INCREF(renderbuffer);

	PyObject * result = PyTuple_New(2);
	PyTuple_SET_ITEM(result, 0, (PyObject *)renderbuffer);
	PyTuple_SET_ITEM(result, 1, PyLong_FromLong(renderbuffer->renderbuffer_obj));
	return result;
}

PyObject * MGLRenderbuffer_release(MGLRenderbuffer * self) {
	MGLRenderbuffer_Invalidate(self);
	Py_RETURN_NONE;
}

void MGLRenderbuffer_Invalidate(MGLRenderbuffer * renderbuffer) {
	if (renderbuffer->released) {
		return;
	}
	renderbuffer->released = true;

	// TODO: decref

	const GLMethods & gl = renderbuffer->context->gl;
	gl.DeleteRenderbuffers(1, (GLuint *)&renderbuffer->renderbuffer_obj);

	Py_DECREF(renderbuffer);
}
