#include "buffer.hpp"

#include "context.hpp"
#include "tools.hpp"

int MGLBuffer_core_write(MGLBuffer * self, const Py_ssize_t & offset, Py_buffer * view, bool contiguos) {
	const GLMethods & gl = self->context->gl;
	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);

	if (contiguos) {
		gl.BufferSubData(GL_ARRAY_BUFFER, offset, view->len, view->buf);
	} else {
		void * map = gl.MapBufferRange(GL_ARRAY_BUFFER, offset, view->len, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
		if (!map) {
			PyErr_Format(module_error, "cannot map the buffer");
			return -1;
		}
		PyBuffer_ToContiguous(map, view, view->len, 'C');
		gl.UnmapBuffer(GL_ARRAY_BUFFER);
	}

	return 0;
}

PyObject * MGLContext_meth_buffer(MGLContext * self, PyObject * args) { TRACE_VARAGS
	PyObject * data;
	PyObject * reserve;
	int readable;
	int writable;
	int local;

	int args_ok = PyArg_ParseTuple(
		args,
		"OOppp",
		&data,
		&reserve,
		&readable,
		&writable,
		&local
	);

	if (!args_ok) {
		return 0;
	}

	Py_ssize_t reserve_size = unpack_size(reserve);

	if ((data == Py_None) ^ (reserve_size != 0)) {
		if (reserve_size) {
			PyErr_Format(module_error, "data and reserve are mutually exclusive");
			return 0;
		}
		PyErr_Format(module_error, "empty buffer");
		return 0;
	}

	MGLBuffer * buffer = MGLContext_new_object(self, Buffer);
	buffer->flags = (readable ? MGL_BUFFER_READABLE : 0) | (writable ? MGL_BUFFER_WRITABLE : 0) | (local ? MGL_BUFFER_LOCAL : 0);

	const GLMethods & gl = self->gl;
	gl.GenBuffers(1, (GLuint *)&buffer->buffer_obj);

	if (!buffer->buffer_obj) {
		PyErr_Format(module_error, "cannot create buffer");
		Py_DECREF(buffer);
		return 0;
	}

	gl.BindBuffer(GL_ARRAY_BUFFER, buffer->buffer_obj);

	unsigned flags;
	PROC_glBufferData initializer;

	if (gl.BufferStorage) {
		initializer = gl.BufferStorage;
		buffer->flags |= MGL_BUFFER_IMMUTABLE;
		flags = (readable ? GL_MAP_READ_BIT : 0) | (writable ? GL_MAP_WRITE_BIT : 0);
		flags |= (writable ? (GL_DYNAMIC_STORAGE_BIT) : 0) | (local ? GL_CLIENT_STORAGE_BIT : 0);
	} else {
		initializer = gl.BufferData;
		flags = writable ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
	}

	if (reserve_size) {
		buffer->size = reserve_size;
		initializer(GL_ARRAY_BUFFER, reserve_size, 0, flags);
	} else {
		Py_buffer view = {};
		if (prepare_buffer(data, &view) < 0) {
			Py_DECREF(buffer);
			return 0;
		}
		buffer->size = view.len;
		if (PyBuffer_IsContiguous(&view, 'C')) {
			initializer(GL_ARRAY_BUFFER, view.len, view.buf, flags);
		} else {
			initializer(GL_ARRAY_BUFFER, view.len, 0, flags);
			if (MGLBuffer_core_write(buffer, 0, &view, false) < 0) {
				PyBuffer_Release(&view);
				Py_DECREF(buffer);
				return 0;
			}
		}
		PyBuffer_Release(&view);
	}

	SLOT(buffer->wrapper, PyObject, Buffer_class_size) = PyLong_FromSsize_t(buffer->size);
	return NEW_REF(buffer->wrapper);
}

PyObject * MGLBuffer_meth_write(MGLBuffer * self, PyObject * args) { TRACE_VARAGS
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

	if (OPEN_ERROR(self->flags)) {
		PyErr_Format(module_error, "buffer is open");
		return 0;
	}

	if (WRITE_ERROR(self->flags)) {
		PyErr_Format(module_error, "buffer is not writable");
		return 0;
	}

	Py_buffer view = {};
	if (prepare_buffer(data, &view) < 0) {
		return 0;
	}

	bool contiguos = PyBuffer_IsContiguous(&view, 'C');
	if (MGLBuffer_core_write(self, offset, &view, contiguos) < 0) {
		PyBuffer_Release(&view);
		return 0;
	}

	PyBuffer_Release(&view);
	Py_RETURN_NONE;
}

PyObject * MGLBuffer_meth_read(MGLBuffer * self, PyObject * args) { TRACE_VARAGS
	Py_ssize_t size;
	Py_ssize_t offset;
	PyObject * dtype;

	int args_ok = PyArg_ParseTuple(
		args,
		"nnO",
		&size,
		&offset,
		&dtype
	);

	if (!args_ok) {
		return 0;
	}

	if (size < 0) {
		size = self->size - offset;
	}

	if (OPEN_ERROR(self->flags)) {
		PyErr_Format(module_error, "buffer is open");
		return 0;
	}

	if (READ_ERROR(self->flags)) {
		PyErr_Format(module_error, "buffer is not readable");
		return 0;
	}

	if (RANGE_ERROR(offset, size, self->size)) {
		PyErr_Format(module_error, "out of range offset = %d or size = %d", offset, size);
		return 0;
	}

	if (DTYPE_ERROR(dtype)) {
		PyErr_Format(module_error, "dtype is set but numpy is not installed");
		return 0;
	}

	const GLMethods & gl = self->context->gl;

	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);
	void * map = gl.MapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_READ_BIT);

	if (!map) {
		PyErr_Format(module_error, "cannot map the buffer");
		return 0;
	}

	PyObject * res;
	if (dtype == Py_None) {
		res = PyBytes_FromStringAndSize((const char *)map, size);
	} else {
		PyObject * array = PyByteArray_FromStringAndSize((const char *)map, size);
		res = PyObject_CallFunctionObjArgs(numpy_frombuffer, array, dtype, 0);
		Py_DECREF(array);
	}

	gl.UnmapBuffer(GL_ARRAY_BUFFER);
	return res;
}

PyObject * MGLBuffer_meth_map(MGLBuffer * self, PyObject * args) { TRACE_VARAGS
	Py_ssize_t size;
	Py_ssize_t offset;
	int readable;
	int writable;
	PyObject * dtype;

	int args_ok = PyArg_ParseTuple(
		args,
		"nnppO",
		&size,
		&offset,
		&readable,
		&writable,
		&dtype
	);

	if (!args_ok) {
		return 0;
	}

	if (size < 0) {
		size = self->size - offset;
	}

	if (!readable && !writable) {
		readable = self->flags & MGL_BUFFER_READABLE;
		writable = self->flags & MGL_BUFFER_WRITABLE;
	}

	if (OPEN_ERROR(self->flags)) {
		PyErr_Format(module_error, "buffer is open");
		return 0;
	}

	if (readable && READ_ERROR(self->flags)) {
		PyErr_Format(module_error, "buffer is not readable");
		return 0;
	}

	if (writable && WRITE_ERROR(self->flags)) {
		PyErr_Format(module_error, "buffer is not writable");
		return 0;
	}

	if (RANGE_ERROR(offset, size, self->size)) {
		PyErr_Format(module_error, "out of range offset = %d or size = %d", offset, size);
		return 0;
	}

	if (DTYPE_ERROR(dtype)) {
		PyErr_Format(module_error, "dtype is set but numpy is not installed");
		return 0;
	}

	const GLMethods & gl = self->context->gl;

	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);
	unsigned flags = (readable ? GL_MAP_READ_BIT : 0) | (writable ? GL_MAP_WRITE_BIT : 0);
	void * map = gl.MapBufferRange(GL_ARRAY_BUFFER, offset, size, flags);

	if (!map) {
		PyErr_Format(module_error, "cannot map the buffer");
		return 0;
	}

	self->flags |= MGL_BUFFER_OPEN | (readable ? MGL_BUFFER_READING : 0) | (writable ? MGL_BUFFER_WRITING : 0);
	PyObject * mem = PyMemoryView_FromMemory((char *)map, size, writable ? PyBUF_WRITE : PyBUF_READ);
	if (dtype == Py_None) {
		return mem;
	}

	PyObject * array = PyObject_CallFunctionObjArgs(numpy_frombuffer, mem, dtype, 0);
	if (!array) {
		gl.UnmapBuffer(GL_ARRAY_BUFFER);
	}
	Py_DECREF(mem);
	return array;
}

PyObject * MGLBuffer_meth_unmap(MGLBuffer * self) { TRACE_NOARGS
	if (self->flags & MGL_BUFFER_OPEN) {
		self->flags &= ~(MGL_BUFFER_OPEN | MGL_BUFFER_READING | MGL_BUFFER_WRITING);
		const GLMethods & gl = self->context->gl;
		gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);
		gl.UnmapBuffer(GL_ARRAY_BUFFER);
	}
	Py_RETURN_NONE;
}

PyObject * MGLBuffer_meth_clear(MGLBuffer * self) { TRACE_NOARGS
	if (OPEN_ERROR(self->flags)) {
		PyErr_Format(module_error, "buffer is open");
		return 0;
	}

	if (WRITE_ERROR(self->flags)) {
		PyErr_Format(module_error, "buffer is not writable");
		return 0;
	}

	const GLMethods & gl = self->context->gl;
	gl.BindBuffer(GL_ARRAY_BUFFER, self->buffer_obj);

	if (gl.ClearBufferData) {
		char zero = 0;
		gl.ClearBufferData(GL_ARRAY_BUFFER, GL_R8I, GL_RED, GL_UNSIGNED_BYTE, &zero);
	} else {
		void * map = gl.MapBuffer(GL_ARRAY_BUFFER, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		if (!map) {
			PyErr_Format(module_error, "cannot map the buffer");
			return 0;
		}
		memset(map, 0, self->size);
		gl.UnmapBuffer(GL_ARRAY_BUFFER);
	}
	Py_RETURN_NONE;
}

PyTypeObject * MGLBuffer_define(int version_code) {
	PyMethodDef MGLBuffer_methods[] = {
		{"write", (PyCFunction)MGLBuffer_meth_write, METH_VARARGS, 0},
		{"read", (PyCFunction)MGLBuffer_meth_read, METH_VARARGS, 0},
		{"map", (PyCFunction)MGLBuffer_meth_map, METH_VARARGS, 0},
		{"unmap", (PyCFunction)MGLBuffer_meth_unmap, METH_NOARGS, 0},
		{"clear", (PyCFunction)MGLBuffer_meth_clear, METH_NOARGS, 0},
		{0},
	};

	PyType_Slot MGLBuffer_slots[] = {
		{Py_tp_methods, dup(MGLBuffer_methods)},
		{0},
	};

	PyType_Spec MGLBuffer_spec = {"moderngl.mgl.Buffer", sizeof(MGLBuffer), 0, Py_TPFLAGS_DEFAULT, MGLBuffer_slots};
	return (PyTypeObject *)PyType_FromSpec(&MGLBuffer_spec);
}
