#include "texture.hpp"

#include "context.hpp"
#include "data_type.hpp"
#include "tools.hpp"

PyObject * MGLContext_meth_texture(MGLContext * self, PyObject * args) { TRACE_VARAGS
	PyObject * size_or_img;
	int components;
	PyObject * data;
	int levels;
	int samples;
	int alignment;
	PyObject * dtype;

	int args_ok = PyArg_ParseTuple(
		args,
		"OIOIIIO",
		&size_or_img,
		&components,
		&data,
		&levels,
		&samples,
		&alignment,
		&dtype
	);

	if (!args_ok) {
		return 0;
	}

	PyObject * size = size_or_img;

	if ((PyObject *)size_or_img->ob_type == pillow_image) {
		size = PyObject_GetAttr(size_or_img, size_str);
		if (!size) {
			return 0;
		}
		PyObject * mode = PyObject_GetAttr(size_or_img, mode_str);
		if (!mode) {
			return 0;
		}
		const char * img_mode = PyUnicode_AsUTF8(mode);
		if (!strcmp(img_mode, "RGBA")) {
			components = 4;
		} else if (!strcmp(img_mode, "RGB")) {
			components = 3;
		} else if (!strcmp(img_mode, "L")) {
			components = 1;
		} else {
		}
		data = size_or_img;
		alignment = 1;
	}

	size = PySequence_Fast(size, "size is not iterable");
	if (!size) {
		return 0;
	}

	if (PySequence_Fast_GET_SIZE(size) != 2) {
		return 0;
	}

	int width = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 0));
	int height = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 1));

	if (components < 1 || components > 4) {
		PyErr_Format(module_error, "the components must be 1, 2, 3 or 4");
		return 0;
	}

	// if ((samples & (samples - 1)) || samples > self->max_samples) {
	// 	PyErr_Format(module_error, "the number of samples is invalid");
	// 	return 0;
	// }

	if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
		PyErr_Format(module_error, "the alignment must be 1, 2, 4 or 8");
		return 0;
	}

	if (data != Py_None && samples) {
		PyErr_Format(module_error, "multisample textures are not writable directly");
		return 0;
	}

	MGLDataType * data_type = from_dtype(dtype);

	if (!data_type) {
		PyErr_Format(module_error, "invalid dtype");
		return 0;
	}

	int max_levels = -1;
	{
		int size = width > height ? width : height;
		while (size) {
			max_levels += 1;
			size >>= 1;
		}
	}

	if (levels < 0 || levels > max_levels) {
		levels = max_levels;
	}

	int expected_size = width * components * data_type->size;
	expected_size = (expected_size + alignment - 1) / alignment * alignment;
	expected_size = expected_size * height;

	int texture_target = samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	int pixel_type = data_type->gl_type;
	int base_format = data_type->base_format[components];
	int internal_format = data_type->internal_format[components];

	MGLTexture * texture = MGLContext_new_object(self, Texture);
	texture->width = width;
	texture->height = height;
	texture->components = components;
	texture->levels = levels;
	texture->samples = samples;
	texture->data_type = data_type;

	const GLMethods & gl = self->gl;
	gl.GenTextures(1, (GLuint *)&texture->texture_obj);
	gl.ActiveTexture(GL_TEXTURE0 + self->default_texture_unit);
	gl.BindTexture(texture_target, texture->texture_obj);
	gl.TexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl.TexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (!texture->texture_obj) {
		PyErr_Format(module_error, "cannot create texture");
		Py_DECREF(texture);
		return 0;
	}

	if (data != Py_None) {
		Py_buffer view = {};
		if (prepare_buffer(data, &view) < 0) {
			return 0;
		}
		void * buf = view.buf;
		bool contiguos = PyBuffer_IsContiguous(&view, 'C');
		gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
		gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
		if (!contiguos) {
			buf = malloc(expected_size);
			PyBuffer_ToContiguous(buf, &view, view.len, 'C');
		}
		if (gl.TexStorage2D) {
			gl.TexStorage2D(texture_target, levels, internal_format, width, height);
			gl.TexSubImage2D(texture_target, 0, 0, 0, width, height, base_format, pixel_type, buf);
		} else {
			gl.TexImage2D(texture_target, 0, internal_format, width, height, 0, base_format, pixel_type, buf);
		}
		if (!contiguos) {
			free(buf);
		}
	} else if (samples) {
		gl.TexImage2DMultisample(texture_target, samples, internal_format, width, height, true);
	} else {
		gl.TexImage2D(texture_target, 0, internal_format, width, height, 0, base_format, pixel_type, 0);
	}

	SLOT(texture->wrapper, PyObject, Texture_class_level) = PyLong_FromLong(0);
	SLOT(texture->wrapper, PyObject, Texture_class_size) = int_tuple(width, height);
	return NEW_REF(texture->wrapper);
}

PyObject * MGLTexture_meth_level(MGLTexture * self, PyObject * args) { TRACE_VARAGS
	int level;

	int args_ok = PyArg_ParseTuple(
		args,
		"I",
		&level
	);

	if (!args_ok) {
		return 0;
	}

	if (level > self->levels) {
		PyErr_Format(module_error, "invalid level");
		return 0;
	}

	int width = self->width >> level;
	int height = self->height >> level;

	PyObject * size = PyTuple_New(2);
	PyTuple_SET_ITEM(size, 0, PyLong_FromLong(width > 1 ? width : 1));
	PyTuple_SET_ITEM(size, 1, PyLong_FromLong(height > 1 ? height : 1));

	PyObject * wrapper = new_object(PyObject, Texture_class);
	SLOT(wrapper, MGLTexture, Texture_class_mglo) = NEW_REF(self);
	SLOT(wrapper, PyObject, Texture_class_level) = PyLong_FromLong(level);
	SLOT(wrapper, PyObject, Texture_class_size) = size;
	return wrapper;
}

PyObject * MGLTexture_meth_write(MGLTexture * self, PyObject * args) { TRACE_VARAGS
	PyObject * data;
	PyObject * viewport;
	int alignment;
	int level;

	int args_ok = PyArg_ParseTuple(
		args,
		"OOII",
		&data,
		&viewport,
		&alignment,
		&level
	);

	if (!args_ok) {
		return 0;
	}

	if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
		PyErr_Format(module_error, "the alignment must be 1, 2, 4 or 8");
		return 0;
	}

	if (level > self->levels) {
		PyErr_Format(module_error, "invalid level");
		return 0;
	}

	if (self->samples) {
		PyErr_Format(module_error, "multisample textures cannot be read directly");
		return 0;
	}

	int max_width = self->width / (1 << level);
	int max_height = self->height / (1 << level);

	max_width = max_width > 1 ? max_width : 1;
	max_height = max_height > 1 ? max_height : 1;

	int x = 0;
	int y = 0;
	int width = max_width;
	int height = max_height;

	if (!unpack_viewport(viewport, x, y, width, height)) {
		return 0;
	}

	int expected_size = width * self->components * self->data_type->size;
	expected_size = (expected_size + alignment - 1) / alignment * alignment;
	expected_size = expected_size * height;

	int texture_target = self->samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	int pixel_type = self->data_type->gl_type;
	int format = self->data_type->base_format[self->components];

	const GLMethods & gl = self->context->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(texture_target, self->texture_obj);
	gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
	gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	if (data->ob_type == Buffer_class) {
		MGLBuffer * buffer = SLOT(data, MGLBuffer, Buffer_class_mglo);
		gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer->buffer_obj);
		gl.TexSubImage2D(texture_target, level, x, y, width, height, format, pixel_type, 0);
		gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	} else {
		Py_buffer view = {};
		if (prepare_buffer(data, &view) < 0) {
			return 0;
		}
		void * buf = view.buf;
		bool contiguos = PyBuffer_IsContiguous(&view, 'C');
		if (!contiguos) {
			buf = malloc(expected_size);
			PyBuffer_ToContiguous(buf, &view, view.len, 'C');
		}
		gl.TexSubImage2D(texture_target, level, x, y, width, height, format, pixel_type, buf);
		if (!contiguos) {
			free(buf);
		}
	}

	Py_RETURN_NONE;
}

PyObject * MGLTexture_meth_read(MGLTexture * self, PyObject * args) {
	int level;
	int alignment;
	int np;

	int args_ok = PyArg_ParseTuple(
		args,
		"IIp",
		&level,
		&alignment,
		&np
	);

	if (!args_ok) {
		return 0;
	}

	if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
		PyErr_Format(module_error, "the alignment must be 1, 2, 4 or 8");
		return 0;
	}

	if (level > self->levels) {
		PyErr_Format(module_error, "invalid level");
		return 0;
	}

	if (self->samples) {
		PyErr_Format(module_error, "multisample textures cannot be read directly");
		return 0;
	}

	int width = self->width / (1 << level);
	int height = self->height / (1 << level);

	width = width > 1 ? width : 1;
	height = height > 1 ? height : 1;

	int expected_size = width * self->components * self->data_type->size;
	expected_size = (expected_size + alignment - 1) / alignment * alignment;
	expected_size = expected_size * height;

	const int base_formats[] = {0, GL_RED, GL_RG, GL_RGB, GL_RGBA};

	int texture_target = self->samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	int pixel_type = self->data_type->gl_type;
	int base_format = base_formats[self->components];

	const GLMethods & gl = self->context->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(texture_target, self->texture_obj);

	gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
	gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	// To determine the required size of pixels, use glGetTexLevelParameter to determine
	// the dimensions of the internal texture image, then scale the required number of pixels
	// by the storage required for each pixel, based on format and type. Be sure to take the
	// pixel storage parameters into account, especially GL_PACK_ALIGNMENT.

	// int pack = 0;
	// gl.GetIntegerv(GL_PACK_ALIGNMENT, &pack);
	// printf("GL_PACK_ALIGNMENT: %d\n", pack);

	// glGetTexLevelParameter with argument GL_TEXTURE_WIDTH
	// glGetTexLevelParameter with argument GL_TEXTURE_HEIGHT
	// glGetTexLevelParameter with argument GL_TEXTURE_INTERNAL_FORMAT

	// int level_width = 0;
	// int level_height = 0;
	// gl.GetTexLevelParameteriv(texture_target, 0, GL_TEXTURE_WIDTH, &level_width);
	// gl.GetTexLevelParameteriv(texture_target, 0, GL_TEXTURE_HEIGHT, &level_height);
	// printf("level_width: %d\n", level_width);
	// printf("level_height: %d\n", level_height);

	if (np) {
		PyObject * array = PyByteArray_FromStringAndSize(0, expected_size);
		gl.GetTexImage(texture_target, level, base_format, pixel_type, PyByteArray_AS_STRING(array));
		PyObject * res = PyObject_CallFunctionObjArgs(numpy_frombuffer, array, self->data_type->numpy_dtype, 0);
		Py_DECREF(array);
		return res;
	}

	PyObject * res = PyBytes_FromStringAndSize(0, expected_size);
	gl.GetTexImage(texture_target, level, base_format, pixel_type, PyBytes_AS_STRING(res));
	return res;
}

PyObject * MGLTexture_meth_use(MGLTexture * self, PyObject * args) { TRACE_VARAGS
	int location;

	int args_ok = PyArg_ParseTuple(
		args,
		"I",
		&location
	);

	if (!args_ok) {
		return 0;
	}

	const GLMethods & gl = self->context->gl;
	int texture_target = self->samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	gl.ActiveTexture(GL_TEXTURE0 + location);
	gl.BindTexture(texture_target, self->texture_obj);
	Py_RETURN_NONE;
}

PyObject * MGLTexture_meth_build_mipmaps(MGLTexture * self, PyObject * args) { TRACE_VARAGS
	int base = 0;
	int max = 1000;

	int args_ok = PyArg_ParseTuple(
		args,
		"II",
		&base,
		&max
	);

	if (!args_ok) {
		return 0;
	}

	if (max < 0) {
		max = self->levels - base;
	}

	const GLMethods & gl = self->context->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(GL_TEXTURE_2D, self->texture_obj);
	gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, base);
	gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, max);
	gl.GenerateMipmap(GL_TEXTURE_2D);
	gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gl.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// self->min_filter = GL_LINEAR_MIPMAP_LINEAR;
	// self->mag_filter = GL_LINEAR;
	// self->max_level = max;
	Py_RETURN_NONE;
}

int MGLTexture_set_swizzle(MGLTexture * self, PyObject * value) { TRACE_SETTER
	const GLMethods & gl = self->context->gl;
	int texture_target = self->samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	int tex_swizzle[4] = {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA};

	const char * swizzle = PyUnicode_AsUTF8(value);
	int size = PyUnicode_GetSize(value);
	bool error = false;

	if (size > 4) {
		return -1;
	}

	for (int i = 0; i < size; ++i) {
		switch (swizzle[i]) {
			case 'R': tex_swizzle[i] = GL_RED; break;
			case 'r': tex_swizzle[i] = GL_RED; break;
			case 'G': tex_swizzle[i] = GL_GREEN; break;
			case 'g': tex_swizzle[i] = GL_GREEN; break;
			case 'B': tex_swizzle[i] = GL_BLUE; break;
			case 'b': tex_swizzle[i] = GL_BLUE; break;
			case 'A': tex_swizzle[i] = GL_ALPHA; break;
			case 'a': tex_swizzle[i] = GL_ALPHA; break;
			case '0': tex_swizzle[i] = GL_ZERO; break;
			case '1': tex_swizzle[i] = GL_ONE; break;
			default: error = true; break;
		}
	}

	if (error) {
		return -1;
	}

	gl.TexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_RGBA, tex_swizzle);
	return 0;
}

int MGLTexture_set_filter(MGLTexture * self, PyObject * value) { TRACE_SETTER
	const GLMethods & gl = self->context->gl;
	int texture_target = self->samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(GL_TEXTURE_2D, self->texture_obj);

	int min_filter = 0;
	int mag_filter = 0;

	if (PyLong_Check(value)) {
		int filter = PyLong_AsLong(value);
		switch (filter) {
			case GL_NEAREST: min_filter = GL_NEAREST; mag_filter = GL_NEAREST; break;
			case GL_LINEAR: min_filter = GL_LINEAR; mag_filter = GL_LINEAR; break;
			case GL_NEAREST_MIPMAP_NEAREST: min_filter = GL_NEAREST_MIPMAP_NEAREST; mag_filter = GL_NEAREST; break;
			case GL_LINEAR_MIPMAP_NEAREST: min_filter = GL_LINEAR_MIPMAP_NEAREST; mag_filter = GL_LINEAR; break;
			case GL_NEAREST_MIPMAP_LINEAR: min_filter = GL_NEAREST_MIPMAP_LINEAR; mag_filter = GL_NEAREST; break;
			case GL_LINEAR_MIPMAP_LINEAR: min_filter = GL_LINEAR_MIPMAP_LINEAR; mag_filter = GL_LINEAR; break;
		}
	} else {
		value = PySequence_Fast(value, "filter is not iterable");
		if (!value) {
			return -1;
		}

		int size = PySequence_Fast_GET_SIZE(value);
		if (size != 2) {
			return -1;
		}

		min_filter = PyLong_AsLong(PySequence_Fast_GET_ITEM(value, 0));
		mag_filter = PyLong_AsLong(PySequence_Fast_GET_ITEM(value, 1));
	}

	// TODO: check

	gl.TexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, min_filter);
	gl.TexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, mag_filter);

	return 0;
}

PyTypeObject * MGLTexture_define(int version_code) {
	PyMethodDef MGLTexture_methods[] = {
		{"level", (PyCFunction)MGLTexture_meth_level, METH_VARARGS, 0},
		{"write", (PyCFunction)MGLTexture_meth_write, METH_VARARGS, 0},
		{"read", (PyCFunction)MGLTexture_meth_read, METH_VARARGS, 0},
		{"use", (PyCFunction)MGLTexture_meth_use, METH_VARARGS, 0},
		{"build_mipmaps", (PyCFunction)MGLTexture_meth_build_mipmaps, METH_VARARGS, 0},
		{0},
	};

	PyGetSetDef MGLTexture_getset[] = {
		{"swizzle", 0, (setter)MGLTexture_set_swizzle, 0},
		{"filter", 0, (setter)MGLTexture_set_filter, 0},
		{0},
	};

	PyType_Slot MGLTexture_slots[] = {
		{Py_tp_methods, dup(MGLTexture_methods)},
		{Py_tp_getset, dup(MGLTexture_getset)},
		{0},
	};

	PyType_Spec MGLTexture_spec = {"moderngl.mgl.Texture", sizeof(MGLTexture), 0, Py_TPFLAGS_DEFAULT, MGLTexture_slots};
	return (PyTypeObject *)PyType_FromSpec(&MGLTexture_spec);
}
