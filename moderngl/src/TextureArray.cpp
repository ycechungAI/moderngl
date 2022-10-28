#include "Types.hpp"

#include "InlineMethods.hpp"

PyObject * MGLContext_texture_array(MGLContext * self, PyObject * args) {
	int width;
	int height;
	int layers;

	int components;

	PyObject * data;

	int alignment;

	const char * dtype;
	Py_ssize_t dtype_size;

	int args_ok = PyArg_ParseTuple(
		args,
		"(III)IOIs#",
		&width,
		&height,
		&layers,
		&components,
		&data,
		&alignment,
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

	if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
		MGLError_Set("the alignment must be 1, 2, 4 or 8");
		return 0;
	}

	MGLDataType * data_type = from_dtype(dtype, dtype_size);

	if (!data_type) {
		MGLError_Set("invalid dtype");
		return 0;
	}

	int expected_size = width * components * data_type->size;
	expected_size = (expected_size + alignment - 1) / alignment * alignment;
	expected_size = expected_size * height * layers;

	Py_buffer buffer_view;

	if (data != Py_None) {
		int get_buffer = PyObject_GetBuffer(data, &buffer_view, PyBUF_SIMPLE);
		if (get_buffer < 0) {
			// Propagate the default error
			return 0;
		}
	} else {
		buffer_view.len = expected_size;
		buffer_view.buf = 0;
	}

	if (buffer_view.len != expected_size) {
		MGLError_Set("data size mismatch %d != %d", buffer_view.len, expected_size);
		if (data != Py_None) {
			PyBuffer_Release(&buffer_view);
		}
		return 0;
	}

	int pixel_type = data_type->gl_type;
	int base_format = data_type->base_format[components];
	int internal_format = data_type->internal_format[components];

	const GLMethods & gl = self->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->default_texture_unit);

    MGLTextureArray * texture = PyObject_New(MGLTextureArray, MGLTextureArray_type);
    texture->released = false;

	texture->texture_obj = 0;
	gl.GenTextures(1, (GLuint *)&texture->texture_obj);

	if (!texture->texture_obj) {
		MGLError_Set("cannot create texture");
		Py_DECREF(texture);
		return 0;
	}

	gl.BindTexture(GL_TEXTURE_2D_ARRAY, texture->texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexImage3D(GL_TEXTURE_2D_ARRAY, 0, internal_format, width, height, layers, 0, base_format, pixel_type, buffer_view.buf);
	if (data_type->float_type) {
		gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else {
		gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	if (data != Py_None) {
		PyBuffer_Release(&buffer_view);
	}

	texture->width = width;
	texture->height = height;
	texture->layers = layers;
	texture->components = components;
	texture->data_type = data_type;

	texture->min_filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
	texture->mag_filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;

	texture->repeat_x = true;
	texture->repeat_y = true;
	texture->anisotropy = 1.0;

	Py_INCREF(self);
	texture->context = self;

	Py_INCREF(texture);

	PyObject * result = PyTuple_New(2);
	PyTuple_SET_ITEM(result, 0, (PyObject *)texture);
	PyTuple_SET_ITEM(result, 1, PyLong_FromLong(texture->texture_obj));
	return result;
}

PyObject * MGLTextureArray_read(MGLTextureArray * self, PyObject * args) {
	int alignment;

	int args_ok = PyArg_ParseTuple(
		args,
		"I",
		&alignment
	);

	if (!args_ok) {
		return 0;
	}

	if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
		MGLError_Set("the alignment must be 1, 2, 4 or 8");
		return 0;
	}

	int expected_size = self->width * self->components * self->data_type->size;
	expected_size = (expected_size + alignment - 1) / alignment * alignment;
	expected_size = expected_size * self->height * self->layers;

	PyObject * result = PyBytes_FromStringAndSize(0, expected_size);
	char * data = PyBytes_AS_STRING(result);

	int pixel_type = self->data_type->gl_type;
	int base_format = self->data_type->base_format[self->components];

	const GLMethods & gl = self->context->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);

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
	// gl.GetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &level_width);
	// gl.GetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_HEIGHT, &level_height);
	// printf("level_width: %d\n", level_width);
	// printf("level_height: %d\n", level_height);

	gl.GetTexImage(GL_TEXTURE_2D_ARRAY, 0, base_format, pixel_type, data);

	return result;
}

PyObject * MGLTextureArray_read_into(MGLTextureArray * self, PyObject * args) {
	PyObject * data;
	int alignment;
	Py_ssize_t write_offset;

	int args_ok = PyArg_ParseTuple(
		args,
		"OIn",
		&data,
		&alignment,
		&write_offset
	);

	if (!args_ok) {
		return 0;
	}

	if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
		MGLError_Set("the alignment must be 1, 2, 4 or 8");
		return 0;
	}

	int expected_size = self->width * self->components * self->data_type->size;
	expected_size = (expected_size + alignment - 1) / alignment * alignment;
	expected_size = expected_size * self->height * self->layers;

	int pixel_type = self->data_type->gl_type;
	int format = self->data_type->base_format[self->components];

	if (Py_TYPE(data) == MGLBuffer_type) {

		MGLBuffer * buffer = (MGLBuffer *)data;

		const GLMethods & gl = self->context->gl;

		gl.BindBuffer(GL_PIXEL_PACK_BUFFER, buffer->buffer_obj);
		gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
		gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);
		gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
		gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
		gl.GetTexImage(GL_TEXTURE_2D_ARRAY, 0, format, pixel_type, (void *)write_offset);
		gl.BindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	} else {

		Py_buffer buffer_view;

		int get_buffer = PyObject_GetBuffer(data, &buffer_view, PyBUF_WRITABLE);
		if (get_buffer < 0) {
			// Propagate the default error
			return 0;
		}

		if (buffer_view.len < write_offset + expected_size) {
			MGLError_Set("the buffer is too small");
			PyBuffer_Release(&buffer_view);
			return 0;
		}

		char * ptr = (char *)buffer_view.buf + write_offset;

		const GLMethods & gl = self->context->gl;

		gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
		gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);
		gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
		gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
		gl.GetTexImage(GL_TEXTURE_2D_ARRAY, 0, format, pixel_type, ptr);

		PyBuffer_Release(&buffer_view);

	}

	Py_RETURN_NONE;
}

PyObject * MGLTextureArray_write(MGLTextureArray * self, PyObject * args) {
	PyObject * data;
	PyObject * viewport;
	int alignment;

	int args_ok = PyArg_ParseTuple(
		args,
		"OOI",
		&data,
		&viewport,
		&alignment
	);

	if (!args_ok) {
		return 0;
	}

	if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
		MGLError_Set("the alignment must be 1, 2, 4 or 8");
		return 0;
	}

	int x = 0;
	int y = 0;
	int z = 0;
	int width = self->width;
	int height = self->height;
	int layers = self->layers;

	Py_buffer buffer_view;

	if (viewport != Py_None) {
		if (Py_TYPE(viewport) != &PyTuple_Type) {
			MGLError_Set("the viewport must be a tuple not %s", Py_TYPE(viewport)->tp_name);
			return 0;
		}

		if (PyTuple_GET_SIZE(viewport) == 6) {

			x = PyLong_AsLong(PyTuple_GET_ITEM(viewport, 0));
			y = PyLong_AsLong(PyTuple_GET_ITEM(viewport, 1));
			z = PyLong_AsLong(PyTuple_GET_ITEM(viewport, 2));
			width = PyLong_AsLong(PyTuple_GET_ITEM(viewport, 3));
			height = PyLong_AsLong(PyTuple_GET_ITEM(viewport, 4));
			layers = PyLong_AsLong(PyTuple_GET_ITEM(viewport, 5));

		} else if (PyTuple_GET_SIZE(viewport) == 3) {

			width = PyLong_AsLong(PyTuple_GET_ITEM(viewport, 0));
			height = PyLong_AsLong(PyTuple_GET_ITEM(viewport, 1));
			layers = PyLong_AsLong(PyTuple_GET_ITEM(viewport, 2));

		} else {

			MGLError_Set("the viewport size %d is invalid", PyTuple_GET_SIZE(viewport));
			return 0;

		}

		if (PyErr_Occurred()) {
			MGLError_Set("wrong values in the viewport");
			return 0;
		}

	}

	int expected_size = width * self->components * self->data_type->size;
	expected_size = (expected_size + alignment - 1) / alignment * alignment;
	expected_size = expected_size * height * layers;

	int pixel_type = self->data_type->gl_type;
	int format = self->data_type->base_format[self->components];

	if (Py_TYPE(data) == MGLBuffer_type) {

		MGLBuffer * buffer = (MGLBuffer *)data;

		const GLMethods & gl = self->context->gl;

		gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer->buffer_obj);
		gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
		gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);
		gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
		gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
		gl.TexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, x, y, z, width, height, layers, format, pixel_type, 0);
		gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	} else {

		int get_buffer = PyObject_GetBuffer(data, &buffer_view, PyBUF_SIMPLE);
		if (get_buffer < 0) {
			// Propagate the default error
			return 0;
		}

		if (buffer_view.len != expected_size) {
			MGLError_Set("data size mismatch %d != %d", buffer_view.len, expected_size);
			if (data != Py_None) {
				PyBuffer_Release(&buffer_view);
			}
			return 0;
		}

		const GLMethods & gl = self->context->gl;

		gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
		gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);
		gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
		gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
		gl.TexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, x, y, z, width, height, layers, format, pixel_type, buffer_view.buf);

		PyBuffer_Release(&buffer_view);

	}

	Py_RETURN_NONE;
}

PyObject * MGLTextureArray_meth_bind(MGLTextureArray * self, PyObject * args) {
	int unit;
	int read;
	int write;
	int level;
	int format;

	int args_ok = PyArg_ParseTuple(
		args,
		"IppII",
		&unit,
		&read,
		&write,
		&level,
		&format
	);

	if (!args_ok) {
		return NULL;
	}

	int access = GL_READ_WRITE;
	if (read && !write) access = GL_READ_ONLY;
	else if (!read && write) access = GL_WRITE_ONLY;
	else if (!read && !write) {
		MGLError_Set("Illegal access mode. Read or write needs to be enabled.");
		return NULL;
	}

	int frmt = format ? format : self->data_type->internal_format[self->components];

    const GLMethods & gl = self->context->gl;
	// NOTE: Texture array must be bound as layered to expose all layers
	gl.BindImageTexture(unit, self->texture_obj, level, GL_TRUE, 0, access, frmt);
    Py_RETURN_NONE;
}

PyObject * MGLTextureArray_use(MGLTextureArray * self, PyObject * args) {
	int index;

	int args_ok = PyArg_ParseTuple(
		args,
		"I",
		&index
	);

	if (!args_ok) {
		return 0;
	}


	const GLMethods & gl = self->context->gl;
	gl.ActiveTexture(GL_TEXTURE0 + index);
	gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);

	Py_RETURN_NONE;
}

PyObject * MGLTextureArray_build_mipmaps(MGLTextureArray * self, PyObject * args) {
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

	if (base > self->max_level) {
		MGLError_Set("invalid base");
		return 0;
	}

	const GLMethods & gl = self->context->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);

	gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, base);
	gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, max);

	gl.GenerateMipmap(GL_TEXTURE_2D_ARRAY);

	gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	self->min_filter = GL_LINEAR_MIPMAP_LINEAR;
	self->mag_filter = GL_LINEAR;
	self->max_level = max;

	Py_RETURN_NONE;
}

PyObject * MGLTextureArray_release(MGLTextureArray * self) {
	MGLTextureArray_Invalidate(self);
	Py_RETURN_NONE;
}

PyObject * MGLTextureArray_get_repeat_x(MGLTextureArray * self) {
	return PyBool_FromLong(self->repeat_x);
}

int MGLTextureArray_set_repeat_x(MGLTextureArray * self, PyObject * value) {

	const GLMethods & gl = self->context->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);

	if (value == Py_True) {
		gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		self->repeat_x = true;
		return 0;
	} else if (value == Py_False) {
		gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		self->repeat_x = false;
		return 0;
	} else {
		MGLError_Set("invalid value for texture_x");
		return -1;
	}
}

PyObject * MGLTextureArray_get_repeat_y(MGLTextureArray * self) {
	return PyBool_FromLong(self->repeat_y);
}

int MGLTextureArray_set_repeat_y(MGLTextureArray * self, PyObject * value) {

	const GLMethods & gl = self->context->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);

	if (value == Py_True) {
		gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		self->repeat_y = true;
		return 0;
	} else if (value == Py_False) {
		gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		self->repeat_y = false;
		return 0;
	} else {
		MGLError_Set("invalid value for texture_y");
		return -1;
	}
}

PyObject * MGLTextureArray_get_filter(MGLTextureArray * self) {
	PyObject * res = PyTuple_New(2);
	PyTuple_SET_ITEM(res, 0, PyLong_FromLong(self->min_filter));
	PyTuple_SET_ITEM(res, 1, PyLong_FromLong(self->mag_filter));
	return res;
}

int MGLTextureArray_set_filter(MGLTextureArray * self, PyObject * value) {
	if (PyTuple_GET_SIZE(value) != 2) {
		MGLError_Set("invalid filter");
		return -1;
	}

	self->min_filter = PyLong_AsLong(PyTuple_GET_ITEM(value, 0));
	self->mag_filter = PyLong_AsLong(PyTuple_GET_ITEM(value, 1));


	const GLMethods & gl = self->context->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);
	gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, self->min_filter);
	gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, self->mag_filter);

	return 0;
}

PyObject * MGLTextureArray_get_swizzle(MGLTextureArray * self, void * closure) {

	const GLMethods & gl = self->context->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);

	int swizzle_r = 0;
	int swizzle_g = 0;
	int swizzle_b = 0;
	int swizzle_a = 0;

	gl.GetTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_R, &swizzle_r);
	gl.GetTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, &swizzle_g);
	gl.GetTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, &swizzle_b);
	gl.GetTexParameteriv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_A, &swizzle_a);

	char swizzle[5] = {
		char_from_swizzle(swizzle_r),
		char_from_swizzle(swizzle_g),
		char_from_swizzle(swizzle_b),
		char_from_swizzle(swizzle_a),
		0,
	};

	return PyUnicode_FromStringAndSize(swizzle, 4);
}

int MGLTextureArray_set_swizzle(MGLTextureArray * self, PyObject * value, void * closure) {
	const char * swizzle = PyUnicode_AsUTF8(value);

	if (!swizzle[0]) {
		MGLError_Set("the swizzle is empty");
		return -1;
	}

	int tex_swizzle[4] = {-1, -1, -1, -1};

	for (int i = 0; swizzle[i]; ++i) {
		if (i > 3) {
			MGLError_Set("the swizzle is too long");
			return -1;
		}

		tex_swizzle[i] = swizzle_from_char(swizzle[i]);

		if (tex_swizzle[i] == -1) {
			MGLError_Set("'%c' is not a valid swizzle parameter", swizzle[i]);
			return -1;
		}
	}


	const GLMethods & gl = self->context->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);

	gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_R, tex_swizzle[0]);
	if (tex_swizzle[1] != -1) {
		gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, tex_swizzle[1]);
		if (tex_swizzle[2] != -1) {
			gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, tex_swizzle[2]);
			if (tex_swizzle[3] != -1) {
				gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_A, tex_swizzle[3]);
			}
		}
	}

	return 0;
}

PyObject * MGLTextureArray_get_anisotropy(MGLTextureArray * self) {
	return PyFloat_FromDouble(self->anisotropy);
}

int MGLTextureArray_set_anisotropy(MGLTextureArray * self, PyObject * value) {
	self->anisotropy = (float)min(max(PyFloat_AsDouble(value), 1.0), self->context->max_anisotropy);

	const GLMethods & gl = self->context->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(GL_TEXTURE_2D_ARRAY, self->texture_obj);
	gl.TexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY, self->anisotropy);

	return 0;
}

void MGLTextureArray_Invalidate(MGLTextureArray * texture) {
	if (texture->released) {
		return;
	}
	texture->released = true;

	// TODO: decref

	const GLMethods & gl = texture->context->gl;
	gl.DeleteTextures(1, (GLuint *)&texture->texture_obj);

	Py_DECREF(texture->context);
	Py_DECREF(texture);
}
