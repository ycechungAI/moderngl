#include "Types.hpp"

PyObject * MGLContext_buffer(MGLContext * self, PyObject * args) {
	PyObject * data;
	int reserve;
	int dynamic;

	int args_ok = PyArg_ParseTuple(
		args,
		"OIp",
		&data,
		&reserve,
		&dynamic
	);

	if (!args_ok) {
		return 0;
	}

	if (data == Py_None && !reserve) {
		MGLError_Set("missing data or reserve");
		return 0;
	}

	if (data != Py_None && reserve) {
		MGLError_Set("data and reserve are mutually exclusive");
		return 0;
	}

	Py_buffer buffer_view;

	if (data != Py_None) {
		int get_buffer = PyObject_GetBuffer(data, &buffer_view, PyBUF_SIMPLE);
		if (get_buffer < 0) {
			// Propagate the default error
			return 0;
		}
	} else {
		buffer_view.len = reserve;
		buffer_view.buf = 0;
	}

	if (!buffer_view.len) {
		if (data != Py_None) {
			PyBuffer_Release(&buffer_view);
		}
		MGLError_Set("the buffer cannot be empty");
		return 0;
	}

    MGLBuffer * buffer = PyObject_New(MGLBuffer, MGLBuffer_type);
    buffer->released = false;

	buffer->size = (int)buffer_view.len;
	buffer->dynamic = dynamic ? true : false;

	const GLMethods & gl = self->gl;

	buffer->buffer_obj = 0;
	gl.GenBuffers(1, (GLuint *)&buffer->buffer_obj);

	if (!buffer->buffer_obj) {
		MGLError_Set("cannot create buffer");
		Py_DECREF(buffer);
		return 0;
	}

	gl.BindBuffer(GL_ARRAY_BUFFER, buffer->buffer_obj);
	gl.BufferData(GL_ARRAY_BUFFER, buffer->size, buffer_view.buf, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	Py_INCREF(self);
	buffer->context = self;

	if (data != Py_None) {
		PyBuffer_Release(&buffer_view);
	}

	Py_INCREF(buffer);

	PyObject * result = PyTuple_New(3);
	PyTuple_SET_ITEM(result, 0, (PyObject *)buffer);
	PyTuple_SET_ITEM(result, 1, PyLong_FromSsize_t(buffer->size));
	PyTuple_SET_ITEM(result, 2, PyLong_FromLong(buffer->buffer_obj));
	return result;
}

PyObject * MGLBuffer_write(MGLBuffer * self, PyObject * args) {
	PyObject * data;
	Py_ssize_t offset;

	int args_ok = PyArg_ParseTuple(
		args,
		"On",
		&data,
		&offset
	);

	if (!args_ok) {
		return 0;
	}

	Py_buffer buffer_view;

	int get_buffer = PyObject_GetBuffer(data, &buffer_view, PyBUF_SIMPLE);
	if (get_buffer < 0) {		
		// Propagate the default error
		return 0;
	}

	if (offset < 0 || buffer_view.len + offset > self->size) {
		MGLError_Set("out of range offset = %d or size = %d", offset, buffer_view.len);
		PyBuffer_Release(&buffer_view);
		return 0;
	}

	const GLMethods & gl = self->context->gl;
	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);
	gl.BufferSubData(GL_ARRAY_BUFFER, (GLintptr)offset, buffer_view.len, buffer_view.buf);
	PyBuffer_Release(&buffer_view);
	Py_RETURN_NONE;
}

PyObject * MGLBuffer_read(MGLBuffer * self, PyObject * args) {
	Py_ssize_t size;
	Py_ssize_t offset;

	int args_ok = PyArg_ParseTuple(
		args,
		"nn",
		&size,
		&offset
	);

	if (!args_ok) {
		return 0;
	}

	if (size < 0) {
		size = self->size - offset;
	}

	if (offset < 0 || offset + size > self->size) {
		MGLError_Set("out of rangeoffset = %d or size = %d", offset, size);
		return 0;
	}

	const GLMethods & gl = self->context->gl;

	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);
	void * map = gl.MapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_READ_BIT);

	if (!map) {
		MGLError_Set("cannot map the buffer");
		return 0;
	}

	PyObject * data = PyBytes_FromStringAndSize((const char *)map, size);

	gl.UnmapBuffer(GL_ARRAY_BUFFER);

	return data;
}

PyObject * MGLBuffer_read_into(MGLBuffer * self, PyObject * args) {
	PyObject * data;
	Py_ssize_t size;
	Py_ssize_t offset;
	Py_ssize_t write_offset;

	int args_ok = PyArg_ParseTuple(
		args,
		"Onnn",
		&data,
		&size,
		&offset,
		&write_offset
	);

	if (!args_ok) {
		return 0;
	}

	if (size < 0) {
		size = self->size - offset;
	}

	if (offset < 0 || write_offset < 0 || offset + size > self->size) {
		MGLError_Set("out of range");
		return 0;
	}

	Py_buffer buffer_view;

	int get_buffer = PyObject_GetBuffer(data, &buffer_view, PyBUF_WRITABLE);
	if (get_buffer < 0) {
		// Propagate the default error
		return 0;
	}

	if (buffer_view.len < write_offset + size) {
		MGLError_Set("the buffer is too small");
		PyBuffer_Release(&buffer_view);
		return 0;
	}

	const GLMethods & gl = self->context->gl;

	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);
	void * map = gl.MapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_READ_BIT);

	char * ptr = (char *)buffer_view.buf + write_offset;
	memcpy(ptr, map, size);

	gl.UnmapBuffer(GL_ARRAY_BUFFER);

	PyBuffer_Release(&buffer_view);
	Py_RETURN_NONE;
}

PyObject * MGLBuffer_write_chunks(MGLBuffer * self, PyObject * args) {
	PyObject * data;
	Py_ssize_t start;
	Py_ssize_t step;
	Py_ssize_t count;

	int args_ok = PyArg_ParseTuple(
		args,
		"Onnn",
		&data,
		&start,
		&step,
		&count
	);

	if (!args_ok) {
		return 0;
	}

	Py_ssize_t abs_step = step > 0 ? step : -step;

	Py_buffer buffer_view;

	int get_buffer = PyObject_GetBuffer(data, &buffer_view, PyBUF_SIMPLE);
	if (get_buffer < 0) {
		// Propagate the default error
		return 0;
	}

	const GLMethods & gl = self->context->gl;
	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);

	Py_ssize_t chunk_size = buffer_view.len / count;

	if (buffer_view.len != chunk_size * count) {
		MGLError_Set("data (%d bytes) cannot be divided to %d equal chunks", buffer_view.len, count);
		PyBuffer_Release(&buffer_view);
		return 0;
	}

	if (start < 0) {
		start = self->size + start;
	}

	if (start < 0 || chunk_size > abs_step || start + chunk_size > self->size || start + count * step - step < 0 || start + count * step - step + chunk_size > self->size) {
		MGLError_Set("buffer overflow");
		PyBuffer_Release(&buffer_view);
		return 0;
	}

	char * write_ptr = (char *)gl.MapBufferRange(GL_ARRAY_BUFFER, 0, self->size, GL_MAP_WRITE_BIT);
	char * read_ptr = (char *)buffer_view.buf;

	if (!write_ptr) {
		MGLError_Set("cannot map the buffer");
		PyBuffer_Release(&buffer_view);
		return 0;
	}

	write_ptr += start;
	for (Py_ssize_t i = 0; i < count; ++i) {
		memcpy(write_ptr, read_ptr, chunk_size);
		read_ptr += chunk_size;
		write_ptr += step;
	}

	gl.UnmapBuffer(GL_ARRAY_BUFFER);
	PyBuffer_Release(&buffer_view);
	Py_RETURN_NONE;
}

PyObject * MGLBuffer_read_chunks(MGLBuffer * self, PyObject * args) {
	Py_ssize_t chunk_size;
	Py_ssize_t start;
	Py_ssize_t step;
	Py_ssize_t count;

	int args_ok = PyArg_ParseTuple(
		args,
		"nnnn",
		&chunk_size,
		&start,
		&step,
		&count
	);

	if (!args_ok) {
		return 0;
	}

	Py_ssize_t abs_step = step > 0 ? step : -step;

	if (start < 0) {
		start = self->size + start;
	}

	if (start < 0 || chunk_size < 0 || chunk_size > abs_step || start + chunk_size > self->size || start + count * step - step < 0 || start + count * step - step + chunk_size > self->size) {
		MGLError_Set("size error");
		return 0;
	}

	const GLMethods & gl = self->context->gl;

	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);

	char * read_ptr = (char *)gl.MapBufferRange(GL_ARRAY_BUFFER, 0, self->size, GL_MAP_READ_BIT);

	if (!read_ptr) {
		MGLError_Set("cannot map the buffer");
		return 0;
	}

	PyObject * data = PyBytes_FromStringAndSize(0, chunk_size * count);
	char * write_ptr = PyBytes_AS_STRING(data);

	read_ptr += start;
	for (Py_ssize_t i = 0; i < count; ++i) {
		memcpy(write_ptr, read_ptr, chunk_size);
		write_ptr += chunk_size;
		read_ptr += step;
	}

	gl.UnmapBuffer(GL_ARRAY_BUFFER);
	return data;
}

PyObject * MGLBuffer_read_chunks_into(MGLBuffer * self, PyObject * args) {
	PyObject * data;
	Py_ssize_t chunk_size;
	Py_ssize_t start;
	Py_ssize_t step;
	Py_ssize_t count;
	Py_ssize_t write_offset;

	int args_ok = PyArg_ParseTuple(
		args,
		"Onnnnn",
		&data,
		&chunk_size,
		&start,
		&step,
		&count,
		&write_offset
	);

	if (!args_ok) {
		return 0;
	}

	Py_buffer buffer_view;

	int get_buffer = PyObject_GetBuffer(data, &buffer_view, PyBUF_WRITABLE);
	if (get_buffer < 0) {
		// Propagate the default error
		return 0;
	}

	const GLMethods & gl = self->context->gl;

	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);

	char * read_ptr = (char *)gl.MapBufferRange(GL_ARRAY_BUFFER, 0, self->size, GL_MAP_READ_BIT);
	char * write_ptr = (char *)buffer_view.buf + write_offset;

	if (!read_ptr) {
		MGLError_Set("cannot map the buffer");
		return 0;
	}

	read_ptr += start;
	for (Py_ssize_t i = 0; i < count; ++i) {
		memcpy(write_ptr, read_ptr, chunk_size);
		write_ptr += chunk_size;
		read_ptr += step;
	}

	gl.UnmapBuffer(GL_ARRAY_BUFFER);
	PyBuffer_Release(&buffer_view);
	Py_RETURN_NONE;
}

PyObject * MGLBuffer_clear(MGLBuffer * self, PyObject * args) {
	Py_ssize_t size;
	Py_ssize_t offset;
	PyObject * chunk;

	int args_ok = PyArg_ParseTuple(
		args,
		"nnO",
		&size,
		&offset,
		&chunk
	);

	if (!args_ok) {
		return 0;
	}

	if (size < 0) {
		size = self->size - offset;
	}

	Py_buffer buffer_view;

	if (chunk != Py_None) {
		int get_buffer = PyObject_GetBuffer(chunk, &buffer_view, PyBUF_SIMPLE);
		if (get_buffer < 0) {
			// Propagate the default error
			return 0;
		}

		if (size % buffer_view.len != 0) {
			MGLError_Set("the chunk does not fit the size");
			PyBuffer_Release(&buffer_view);
			return 0;
		}
	} else {
		buffer_view.len = 0;
		buffer_view.buf = 0;
	}

	const GLMethods & gl = self->context->gl;
	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);

	char * map = (char *)gl.MapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_WRITE_BIT);

	if (!map) {
		MGLError_Set("cannot map the buffer");
		return 0;
	}

	if (buffer_view.len) {
		char * src = (char *)buffer_view.buf;
		Py_ssize_t divisor = buffer_view.len;

		for (Py_ssize_t i = 0; i < size; ++i) {
			map[i] = src[i % divisor];
		}
	} else {
		memset(map + offset, 0, size);
	}

	gl.UnmapBuffer(GL_ARRAY_BUFFER);

	if (chunk != Py_None) {
		PyBuffer_Release(&buffer_view);
	}

	Py_RETURN_NONE;
}

PyObject * MGLBuffer_orphan(MGLBuffer * self, PyObject * args) {
	Py_ssize_t size;

	int args_ok = PyArg_ParseTuple(
		args,
		"n",
		&size
	);

	if (!args_ok) {
		return 0;
	}

	if (size > 0) {
		self->size = size;
	}

	const GLMethods & gl = self->context->gl;
	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);
	gl.BufferData(GL_ARRAY_BUFFER, self->size, 0, self->dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	Py_RETURN_NONE;
}

PyObject * MGLBuffer_bind_to_uniform_block(MGLBuffer * self, PyObject * args) {
	int binding;
	Py_ssize_t offset;
	Py_ssize_t size;

	int args_ok = PyArg_ParseTuple(
		args,
		"Inn",
		&binding,
		&offset,
		&size
	);

	if (!args_ok) {
		return 0;
	}

	if (size < 0) {
		size = self->size - offset;
	}

	const GLMethods & gl = self->context->gl;
	gl.BindBufferRange(GL_UNIFORM_BUFFER, binding, self->buffer_obj, offset, size);
	Py_RETURN_NONE;
}

PyObject * MGLBuffer_bind_to_storage_buffer(MGLBuffer * self, PyObject * args) {
	int binding;
	Py_ssize_t offset;
	Py_ssize_t size;

	int args_ok = PyArg_ParseTuple(
		args,
		"Inn",
		&binding,
		&offset,
		&size
	);

	if (!args_ok) {
		return 0;
	}

	if (size < 0) {
		size = self->size - offset;
	}

	const GLMethods & gl = self->context->gl;
	gl.BindBufferRange(GL_SHADER_STORAGE_BUFFER, binding, self->buffer_obj, offset, size);
	Py_RETURN_NONE;
}

PyObject * MGLBuffer_release(MGLBuffer * self) {
	MGLBuffer_Invalidate(self);
	Py_RETURN_NONE;
}

PyObject * MGLBuffer_size(MGLBuffer * self) {
	return PyLong_FromLong(self->size); 
}

int MGLBuffer_tp_as_buffer_get_view(MGLBuffer * self, Py_buffer * view, int flags) {
	int access = (flags == PyBUF_SIMPLE) ? GL_MAP_READ_BIT : (GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	const GLMethods & gl = self->context->gl;
	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);
	void * map = gl.MapBufferRange(GL_ARRAY_BUFFER, 0, self->size, access);

	if (!map) {
		PyErr_Format(PyExc_BufferError, "Cannot map buffer");
		view->obj = 0;
		return -1;
	}

	view->buf = map;
	view->len = self->size;
	view->itemsize = 1;

	view->format = 0;
	view->ndim = 0;
	view->shape = 0;
	view->strides = 0;
	view->suboffsets = 0;

	Py_INCREF(self);
	view->obj = (PyObject *)self;
	return 0;
}

void MGLBuffer_tp_as_buffer_release_view(MGLBuffer * self, Py_buffer * view) {
	const GLMethods & gl = self->context->gl;
	gl.UnmapBuffer(GL_ARRAY_BUFFER);
}

void MGLBuffer_Invalidate(MGLBuffer * buffer) {
	if (buffer->released) {
		return;
	}
	buffer->released = true;

	const GLMethods & gl = buffer->context->gl;
	gl.DeleteBuffers(1, (GLuint *)&buffer->buffer_obj);

	Py_DECREF(buffer->context);
	Py_DECREF(buffer);
}
