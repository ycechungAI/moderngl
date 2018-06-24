#include "framebuffer.hpp"

#include "context.hpp"
#include "tools.hpp"

PyObject * MGLContext_meth_framebuffer(MGLContext * self, PyObject * args) { TRACE_VARAGS
	PyObject * attachments;

	int args_ok = PyArg_ParseTuple(
		args,
		"O",
		&attachments
	);

	attachments = PySequence_Fast(attachments, "attachments is not iterable");
	if (!attachments) {
		return 0;
	}

	const GLMethods & gl = self->gl;

	MGLFramebuffer * framebuffer = MGLContext_new_object(self, Framebuffer);
	gl.GenFramebuffers(1, (GLuint *)&framebuffer->framebuffer_obj);

	if (!framebuffer->framebuffer_obj) {
		PyErr_Format(module_error, "cannot create framebuffer");
		Py_DECREF(framebuffer);
		return 0;
	}

	gl.BindFramebuffer(GL_FRAMEBUFFER, framebuffer->framebuffer_obj);

	int width = 0;
	int height = 0;
	int samples = 0;
	int depth_attachments = 0;
	bool layered = false;

	int attachments_len = (int)PySequence_Fast_GET_SIZE(attachments);
	PyObject ** attachments_array = PySequence_Fast_ITEMS(attachments);

	char * attachment_type = new char[attachments_len];

	for (int i = 0; i < attachments_len; ++i) {
		PyObject * attachment = attachments_array[i];
		int attachment_width;
		int attachment_height;
		int attachment_samples;
		bool attachment_layered = false;
		if (attachment->ob_type == Texture_class) {
			MGLTexture * texture = SLOT(attachment, MGLTexture, Texture_class_mglo);
			int level = PyLong_AsLong(SLOT(attachment, PyObject, Texture_class_level));
			int layer = PyLong_AsLong(SLOT(attachment, PyObject, Texture_class_layer));
			attachment_width = texture->width >> level;
			attachment_height = texture->height >> level;
			attachment_width = attachment_width > 1 ? attachment_width : 1;
			attachment_height = attachment_height > 1 ? attachment_height : 1;
			attachment_samples = texture->samples;
			bool is3d = texture->texture_target == GL_TEXTURE_3D;
			if (texture->components > 0) {
				int idx = i - depth_attachments;
				if (is3d) {
					if (layer < 0) {
						gl.FramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, texture->texture_obj, level);
						attachment_layered = true;
					} else {
						gl.FramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, texture->texture_target, texture->texture_obj, level, layer);
					}
				} else {
					gl.FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, texture->texture_target, texture->texture_obj, level);
				}
				attachment_type[idx] = texture->data_type->shape;
			} else {
				if (is3d) {
					if (layer < 0) {
						gl.FramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->texture_obj, level);
						attachment_layered = true;
					} else {
						gl.FramebufferTexture3D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->texture_target, texture->texture_obj, level, layer);
					}
				} else {
					gl.FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->texture_target, texture->texture_obj, level);
				}
				depth_attachments += 1;
			}
		} else if (attachment->ob_type == Renderbuffer_class) {
			MGLRenderbuffer * renderbuffer = SLOT(attachment, MGLRenderbuffer, Renderbuffer_class_mglo);
			attachment_width = renderbuffer->width;
			attachment_height = renderbuffer->height;
			attachment_samples = renderbuffer->samples;
			if (renderbuffer->components > 0) {
				int idx = i - depth_attachments;
				gl.FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_RENDERBUFFER, renderbuffer->renderbuffer_obj);
				attachment_type[idx] = renderbuffer->data_type->shape;
			} else {
				gl.FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer->renderbuffer_obj);
				depth_attachments += 1;
			}
		} else {
			PyErr_Format(module_error, "wrong type");
			return 0;
		}

		if (width != attachment_width || height != attachment_height || samples != attachment_samples || layered != attachment_layered) {
			if (i) {
				PyErr_Format(module_error, "different attachments");
				return 0;
			}
			width = attachment_width;
			height = attachment_height;
			samples = attachment_samples;
			layered = attachment_layered;
		}
	}

	if (depth_attachments > 1) {
		PyErr_Format(module_error, "more then one depth attachment");
		return 0;
	}

	framebuffer->attachment_type = attachment_type;
	framebuffer->attachments = attachments_len - depth_attachments;

	int status = gl.CheckFramebufferStatus(GL_FRAMEBUFFER);
	PyObject * bound_framebuffer = SLOT(self->wrapper, PyObject, Context_class_fbo);
	gl.BindFramebuffer(GL_FRAMEBUFFER, SLOT(bound_framebuffer, MGLFramebuffer, Framebuffer_class_mglo)->framebuffer_obj);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		const char * message = "the framebuffer is not complete";

		switch (status) {
			case GL_FRAMEBUFFER_UNDEFINED: message = "the framebuffer is not complete (UNDEFINED)"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: message = "the framebuffer is not complete (INCOMPLETE_ATTACHMENT)"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: message = "the framebuffer is not complete (INCOMPLETE_MISSING_ATTACHMENT)"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: message = "the framebuffer is not complete (INCOMPLETE_DRAW_BUFFER)"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: message = "the framebuffer is not complete (INCOMPLETE_READ_BUFFER)"; break;
			case GL_FRAMEBUFFER_UNSUPPORTED: message = "the framebuffer is not complete (UNSUPPORTED)"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: message = "the framebuffer is not complete (INCOMPLETE_MULTISAMPLE)"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: message = "the framebuffer is not complete (INCOMPLETE_LAYER_TARGETS)"; break;
		}

		PyErr_Format(module_error, message);
		return 0;
	}

	framebuffer->width = width;
	framebuffer->height = height;
	framebuffer->samples = samples;
	framebuffer->viewport[2] = width;
	framebuffer->viewport[3] = height;

	SLOT(framebuffer->wrapper, PyObject, Framebuffer_class_viewport) = int_tuple(0, 0, width, height);
	SLOT(framebuffer->wrapper, PyObject, Framebuffer_class_size) = int_tuple(width, height);

	PyObject * framebuffers = SLOT(self->wrapper, PyObject, Context_class_framebuffers);
	PyObject * key = PyLong_FromLong(framebuffer->framebuffer_obj);
	PyDict_SetItem(framebuffers, key, NEW_REF(framebuffer->wrapper));

	return NEW_REF(framebuffer->wrapper);
}

PyObject * MGLFramebuffer_meth_clear(MGLFramebuffer * self, PyObject * args) { TRACE_VARAGS
	PyObject * value;
	int attachment;

	int args_ok = PyArg_ParseTuple(
		args,
		"Oi",
		&value,
		&attachment
	);

	if (!args_ok) {
		return 0;
	}

	const GLMethods & gl = self->context->gl;
	gl.BindFramebuffer(GL_FRAMEBUFFER, self->framebuffer_obj);
	char color[32] = {};

	bool scissor = false;
	if (self->viewport[0] || self->viewport[1] || self->viewport[2] != self->width || self->viewport[3] != self->height) {
		gl.Enable(GL_SCISSOR_TEST);
		gl.Scissor(self->viewport[0], self->viewport[1], self->viewport[2], self->viewport[3]);
		scissor = true;
	}

	if (attachment < 0) {
		float depth = (float)PyFloat_AsDouble(value);
		gl.ClearBufferfv(GL_DEPTH, 0, &depth);
	} else  if (attachment < self->attachments) {
		value = PySequence_Fast(value, "value is not iterable");
		if (!value) {
			return 0;
		}
		read_value func;
		void * ptr = color;
		const char shape = self->attachment_type[attachment];
		switch (shape) {
			case 'f': func = (read_value)read_float; break;
			case 'i': func = (read_value)read_int; break;
			case 'u': func = (read_value)read_unsigned; break;
		}
		int size = PySequence_Fast_GET_SIZE(value);
		for (int i = 0; i < size; ++i) {
			func(ptr, PySequence_Fast_GET_ITEM(value, i));
		}
		Py_DECREF(value);
		if (PyErr_Occurred()) {
			if (scissor) {
				gl.Disable(GL_SCISSOR_TEST);
			}
			return 0;
		}
		switch (shape) {
			case 'f': gl.ClearBufferfv(GL_COLOR, attachment, (float *)color); break;
			case 'i': gl.ClearBufferiv(GL_COLOR, attachment, (int *)color); break;
			case 'u': gl.ClearBufferuiv(GL_COLOR, attachment, (unsigned *)color); break;
		}
	} else {
		return 0;
	}

	if (scissor) {
		gl.Disable(GL_SCISSOR_TEST);
	}

	Py_RETURN_NONE;
}

PyObject * MGLFramebuffer_meth_use(MGLFramebuffer * self) { TRACE_NOARGS
	const GLMethods & gl = self->context->gl;
	gl.BindFramebuffer(GL_FRAMEBUFFER, self->framebuffer_obj);
	gl.Viewport(self->viewport[0], self->viewport[1], self->viewport[2], self->viewport[3]);
	PyObject *& slot = SLOT(self->context->wrapper, PyObject, Context_class_mglo);
	Py_INCREF(self->wrapper);
	Py_DECREF(slot);
	slot = self->wrapper;
	Py_RETURN_NONE;
}

int MGLFramebuffer_set_viewport(MGLFramebuffer * self, PyObject * value) { TRACE_SETTER
	int x = 0;
	int y = 0;
	int width = self->width;
	int height = self->height;

	if (!unpack_viewport(value, x, y, width, height)) {
		return -1;
	}

	self->viewport[0] = x;
	self->viewport[1] = y;
	self->viewport[2] = width;
	self->viewport[3] = height;

	SLOT(self->wrapper, PyObject, Framebuffer_class_viewport) = int_tuple(x, y, width, height);

	if (self->wrapper == SLOT(self->context->wrapper, PyObject, Context_class_fbo)) {
		self->context->gl.Viewport(x, y, width, height);
	}

	return 0;
}

PyObject * MGLFramebuffer_meth_read(MGLFramebuffer * self, PyObject * args) { TRACE_VARAGS
	PyObject * viewport;
	int components;
	int alignment;
	int attachment;
	PyObject * dtype;
	int np;

	int args_ok = PyArg_ParseTuple(
		args,
		"OIIIOp",
		&viewport,
		&components,
		&attachment,
		&alignment,
		&dtype,
		&np
	);

	if (!args_ok) {
		return 0;
	}

	if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
		PyErr_Format(module_error, "the alignment must be 1, 2, 4 or 8");
		return 0;
	}

	MGLDataType * data_type = from_dtype(dtype);

	if (!data_type) {
		PyErr_Format(module_error, "invalid dtype");
		return 0;
	}

	int x = 0;
	int y = 0;
	int width = self->width;
	int height = self->height;
	unpack_viewport(viewport, x, y, width, height);

	int expected_size = width * components * data_type->size;
	expected_size = (expected_size + alignment - 1) / alignment * alignment;
	expected_size = expected_size * height;

	int pixel_type = data_type->gl_type;
	int base_format = data_type->base_format[components];

	const GLMethods & gl = self->context->gl;
	gl.BindFramebuffer(GL_FRAMEBUFFER, self->framebuffer_obj);
	gl.ReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
	gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
	gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	if (np) {
		PyObject * array = PyByteArray_FromStringAndSize(0, expected_size);
		gl.ReadPixels(x, y, width, height, base_format, pixel_type, PyByteArray_AS_STRING(array));
		PyObject * res = PyObject_CallFunctionObjArgs(numpy_frombuffer, array, data_type->numpy_dtype, 0);
		Py_DECREF(array);
		return res;
	} else {
		PyObject * res = PyBytes_FromStringAndSize(0, expected_size);
		gl.ReadPixels(x, y, width, height, base_format, pixel_type, PyBytes_AS_STRING(res));
		return res;
	}

	// gl.BindFramebuffer(GL_FRAMEBUFFER, self->context->bound_framebuffer->framebuffer_obj);
	Py_RETURN_NONE;
}

PyTypeObject * MGLFramebuffer_define(int version_code) {
	PyMethodDef MGLFramebuffer_methods[] = {
		{"clear", (PyCFunction)MGLFramebuffer_meth_clear, METH_VARARGS, 0},
		{"use", (PyCFunction)MGLFramebuffer_meth_use, METH_NOARGS, 0},
		{"read", (PyCFunction)MGLFramebuffer_meth_read, METH_VARARGS, 0},
		{0},
	};

	PyGetSetDef MGLFramebuffer_getset[] = {
		{"viewport", 0, (setter)MGLFramebuffer_set_viewport, 0},
		{0},
	};

	PyType_Slot MGLFramebuffer_slots[] = {
		{Py_tp_methods, dup(MGLFramebuffer_methods)},
		{Py_tp_getset, dup(MGLFramebuffer_getset)},
		{0},
	};

	PyType_Spec MGLFramebuffer_spec = {"moderngl.mgl.Framebuffer", sizeof(MGLFramebuffer), 0, Py_TPFLAGS_DEFAULT, MGLFramebuffer_slots};
	return (PyTypeObject *)PyType_FromSpec(&MGLFramebuffer_spec);
}
