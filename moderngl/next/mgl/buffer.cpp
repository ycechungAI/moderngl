#include "buffer.hpp"
#include "context.hpp"
#include "internal/wrapper.hpp"

#include "internal/tools.hpp"

/* MGLBuffer_core_write(...)
 */
int MGLBuffer_core_write(MGLBuffer * self, const Py_ssize_t & offset, Py_buffer * view, bool contiguous) {
    const GLMethods & gl = self->context->gl;
    self->context->bind_array_buffer(self->buffer_obj);

    if (contiguous) {
        gl.BufferSubData(GL_ARRAY_BUFFER, offset, view->len, view->buf);
    } else {
        void * map = gl.MapBufferRange(GL_ARRAY_BUFFER, offset, view->len, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
        if (!map) {
            PyErr_Format(moderngl_error, "cannot map the buffer");
            return -1;
        }
        PyBuffer_ToContiguous(map, view, view->len, 'C');
        gl.UnmapBuffer(GL_ARRAY_BUFFER);
    }

    return 0;
}

/* MGLContext.copy_buffer(dst, src, size, read_offset, write_offset)
 */
PyObject * MGLContext_meth_copy_buffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(5);

    PyObject * dst = args[0];
    PyObject * src = args[1];

    dst = get_new_wrapper(dst);
    src = get_new_wrapper(src);

    if (!Buffer_Check(dst)) {
        PyErr_Format(moderngl_error, "not a Buffer");
        return 0;
    }

    if (!Buffer_Check(src)) {
        PyErr_Format(moderngl_error, "not a Buffer");
        return 0;
    }

    Py_ssize_t size = PyLong_AsSsize_t(args[2]);
    Py_ssize_t read_offset = PyLong_AsSsize_t(args[3]);
    Py_ssize_t write_offset = PyLong_AsSsize_t(args[4]);

    MGLBuffer * src_buffer = (MGLBuffer *)get_slot(src, "mglo");
    MGLBuffer * dst_buffer = (MGLBuffer *)get_slot(dst, "mglo");

    if (size < 0) {
        size = src_buffer->size - read_offset;
    }

    if (size < 0 || read_offset < 0 || write_offset < 0) {
        PyErr_Format(moderngl_error, "overflow");
        return 0;
    }

    if (read_offset + size > src_buffer->size || write_offset + size > dst_buffer->size) {
        PyErr_Format(moderngl_error, "overflow");
        return 0;
    }

	const GLMethods & gl = self->gl;
	gl.BindBuffer(GL_COPY_READ_BUFFER, src_buffer->buffer_obj);
	gl.BindBuffer(GL_COPY_WRITE_BUFFER, dst_buffer->buffer_obj);
	gl.CopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, read_offset, write_offset, size);
    Py_RETURN_NONE;
}

/* MGLContext.buffer(data, reserve, readable, writable, local)
 * Returns a Buffer object.
 */
PyObject * MGLContext_meth_buffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(5);

    PyObject * data = args[0];
    PyObject * reserve = args[1];
    int readable = PyObject_IsTrue(args[2]);
    int writable = PyObject_IsTrue(args[3]);
    int local = PyObject_IsTrue(args[4]);

    Py_ssize_t reserve_size = unpack_size(reserve);

    if ((data == Py_None) ^ (reserve_size != 0)) {
        if (reserve_size) {
            PyErr_Format(moderngl_error, "data and reserve are mutually exclusive");
            return 0;
        }
        PyErr_Format(moderngl_error, "empty buffer");
        return 0;
    }

    MGLBuffer * buffer = PyObject_New(MGLBuffer, MGLBuffer_class);
    chain_objects(self, buffer);
    buffer->context = self;

    buffer->flags = (readable ? MGL_BUFFER_READABLE : 0) | (writable ? MGL_BUFFER_WRITABLE : 0) | (local ? MGL_BUFFER_LOCAL : 0);

    const GLMethods & gl = self->gl;
    gl.GenBuffers(1, (GLuint *)&buffer->buffer_obj);

    if (!buffer->buffer_obj) {
        PyErr_Format(moderngl_error, "cannot create buffer");
        Py_DECREF(buffer);
        return 0;
    }

    self->bind_array_buffer(buffer->buffer_obj);

    unsigned flags;

    if (gl.BufferStorage) {
        buffer->flags |= MGL_BUFFER_IMMUTABLE;
        flags = (readable ? GL_MAP_READ_BIT : 0) | (writable ? GL_MAP_WRITE_BIT : 0);
        flags |= (writable ? (GL_DYNAMIC_STORAGE_BIT) : 0) | (local ? GL_CLIENT_STORAGE_BIT : 0);
    } else {
        flags = writable ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    }

    if (reserve_size) {
        buffer->size = reserve_size;
        if (gl.BufferStorage) {
            gl.BufferStorage(GL_ARRAY_BUFFER, reserve_size, 0, flags);
        } else {
            gl.BufferData(GL_ARRAY_BUFFER, reserve_size, 0, flags);
        }
    } else {
        Py_buffer view = {};
        if (prepare_buffer(data, &view) < 0) {
            Py_DECREF(buffer);
            return 0;
        }
        buffer->size = view.len;
        if (PyBuffer_IsContiguous(&view, 'C')) {
            if (gl.BufferStorage) {
                gl.BufferStorage(GL_ARRAY_BUFFER, view.len, view.buf, flags);
            } else {
                gl.BufferData(GL_ARRAY_BUFFER, view.len, view.buf, flags);
            }
        } else {
            if (gl.BufferStorage) {
                gl.BufferStorage(GL_ARRAY_BUFFER, view.len, 0, flags);
            } else {
                gl.BufferData(GL_ARRAY_BUFFER, view.len, 0, flags);
            }
            if (MGLBuffer_core_write(buffer, 0, &view, false) < 0) {
                PyBuffer_Release(&view);
                Py_DECREF(buffer);
                return 0;
            }
        }
        PyBuffer_Release(&view);
    }

    buffer->wrapper = Buffer_New("On", buffer, buffer->size);
    return buffer->wrapper;
}

/* MGLBuffer.write(data, offset)
 */
PyObject * MGLBuffer_meth_write(MGLBuffer * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(2);

    PyObject * data = args[0];
    Py_ssize_t offset = PyLong_AsSsize_t(args[1]);

    if (OPEN_ERROR(self->flags)) {
        PyErr_Format(moderngl_error, "buffer is open");
        return 0;
    }

    if (WRITE_ERROR(self->flags)) {
        PyErr_Format(moderngl_error, "buffer is not writable");
        return 0;
    }

    Py_buffer view = {};
    if (prepare_buffer(data, &view) < 0) {
        return 0;
    }

    if (RANGE_ERROR(offset, view.len, self->size)) {
        PyErr_Format(moderngl_error, "out of range offset = %d or size = %d", offset, view.len);
        return 0;
    }

    bool contiguous = PyBuffer_IsContiguous(&view, 'C');
    if (MGLBuffer_core_write(self, offset, &view, contiguous) < 0) {
        PyBuffer_Release(&view);
        return 0;
    }

    PyBuffer_Release(&view);
    Py_RETURN_NONE;
}

/* MGLBuffer.read(size, offset, dtype)
 */
PyObject * MGLBuffer_meth_read(MGLBuffer * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(3);

    Py_ssize_t size = PyLong_AsSsize_t(args[0]);
    Py_ssize_t offset = PyLong_AsSsize_t(args[1]);
    PyObject * dtype = args[2];

    if (size < 0) {
        size = self->size - offset;
    }

    if (OPEN_ERROR(self->flags)) {
        PyErr_Format(moderngl_error, "buffer is open");
        return 0;
    }

    if (READ_ERROR(self->flags)) {
        PyErr_Format(moderngl_error, "buffer is not readable");
        return 0;
    }

    if (RANGE_ERROR(offset, size, self->size)) {
        PyErr_Format(moderngl_error, "out of range offset = %d or size = %d", offset, size);
        return 0;
    }

    if (DTYPE_ERROR(dtype)) {
        PyErr_Format(moderngl_error, "dtype is set but numpy is not installed");
        return 0;
    }

    const GLMethods & gl = self->context->gl;

    self->context->bind_array_buffer(self->buffer_obj);
    void * map = gl.MapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_READ_BIT);

    if (!map) {
        PyErr_Format(moderngl_error, "cannot map the buffer");
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

/* MGLBuffer.map(size, offset, readable, writable, dtype)
 */
PyObject * MGLBuffer_meth_map(MGLBuffer * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(5);

    Py_ssize_t size = PyLong_AsSsize_t(args[0]);
    Py_ssize_t offset = PyLong_AsSsize_t(args[1]);
    int readable = PyObject_IsTrue(args[2]);
    int writable = PyObject_IsTrue(args[3]);
    PyObject * dtype = args[4];

    if (size < 0) {
        size = self->size - offset;
    }

    if (size < 0 || offset < 0) {
        PyErr_Format(moderngl_error, "overflow");
        return 0;
    }

    if (!readable && !writable) {
        readable = self->flags & MGL_BUFFER_READABLE;
        writable = self->flags & MGL_BUFFER_WRITABLE;
    }

    if (OPEN_ERROR(self->flags)) {
        PyErr_Format(moderngl_error, "buffer is open");
        return 0;
    }

    if (readable && READ_ERROR(self->flags)) {
        PyErr_Format(moderngl_error, "buffer is not readable");
        return 0;
    }

    if (writable && WRITE_ERROR(self->flags)) {
        PyErr_Format(moderngl_error, "buffer is not writable");
        return 0;
    }

    if (RANGE_ERROR(offset, size, self->size)) {
        PyErr_Format(moderngl_error, "out of range offset = %d or size = %d", offset, size);
        return 0;
    }

    if (DTYPE_ERROR(dtype)) {
        PyErr_Format(moderngl_error, "dtype is set but numpy is not installed");
        return 0;
    }

    const GLMethods & gl = self->context->gl;

    self->context->bind_array_buffer(self->buffer_obj);
    unsigned flags = (readable ? GL_MAP_READ_BIT : 0) | (writable ? GL_MAP_WRITE_BIT : 0);
    void * map = gl.MapBufferRange(GL_ARRAY_BUFFER, offset, size, flags);

    if (!map) {
        PyErr_Format(moderngl_error, "cannot map the buffer");
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

/* MGLBuffer.unmap()
 */
PyObject * MGLBuffer_meth_unmap(MGLBuffer * self) {
    if (self->flags & MGL_BUFFER_OPEN) {
        self->flags &= ~(MGL_BUFFER_OPEN | MGL_BUFFER_READING | MGL_BUFFER_WRITING);
        const GLMethods & gl = self->context->gl;
        self->context->bind_array_buffer(self->buffer_obj);
        gl.UnmapBuffer(GL_ARRAY_BUFFER);
    }
    Py_RETURN_NONE;
}

/* MGLBuffer.clear()
 */
PyObject * MGLBuffer_meth_clear(MGLBuffer * self) {
    if (OPEN_ERROR(self->flags)) {
        PyErr_Format(moderngl_error, "buffer is open");
        return 0;
    }

    if (WRITE_ERROR(self->flags)) {
        PyErr_Format(moderngl_error, "buffer is not writable");
        return 0;
    }

    const GLMethods & gl = self->context->gl;
    self->context->bind_array_buffer(self->buffer_obj);

    if (gl.ClearBufferData) {
        char zero = 0;
        gl.ClearBufferData(GL_ARRAY_BUFFER, GL_R8I, GL_RED, GL_UNSIGNED_BYTE, &zero);
    } else {
        void * map = gl.MapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        if (!map) {
            PyErr_Format(moderngl_error, "cannot map the buffer");
            return 0;
        }
        memset(map, 0, self->size);
        gl.UnmapBuffer(GL_ARRAY_BUFFER);
    }
    Py_RETURN_NONE;
}

/* MGLBuffer.bind(binding, offset, size)
 */
PyObject * MGLBuffer_meth_bind(MGLBuffer * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(4);

	int binding = PyLong_AsLong(args[0]);
	Py_ssize_t offset = PyLong_AsSsize_t(args[1]);
	Py_ssize_t size = PyLong_AsSsize_t(args[2]);
	int ssbo = PyLong_AsLong(args[3]);

    const GLMethods & gl = self->context->gl;
	gl.BindBufferRange(ssbo ? GL_SHADER_STORAGE_BUFFER : GL_UNIFORM_BUFFER, binding, self->buffer_obj, offset, size);
    Py_RETURN_NONE;
}

void MGLBuffer_dealloc(MGLBuffer * self) {
    printf("MGLBuffer_dealloc\n");
    Py_TYPE(self)->tp_free(self);
}

fastcallable(MGLBuffer_meth_write)
fastcallable(MGLBuffer_meth_read)
fastcallable(MGLBuffer_meth_map)
fastcallable(MGLBuffer_meth_bind)

PyMethodDef MGLBuffer_methods[] = {
    {"write", fastcall(MGLBuffer_meth_write), fastcall_flags, NULL},
    {"read", fastcall(MGLBuffer_meth_read), fastcall_flags, NULL},
    {"map", fastcall(MGLBuffer_meth_map), fastcall_flags, NULL},
    {"unmap", (PyCFunction)MGLBuffer_meth_unmap, METH_NOARGS, 0},
    {"clear", (PyCFunction)MGLBuffer_meth_clear, METH_NOARGS, 0},
    {"bind", fastcall(MGLBuffer_meth_bind), fastcall_flags, NULL},
    {0},
};

PyType_Slot MGLBuffer_slots[] = {
    {Py_tp_methods, MGLBuffer_methods},
    {Py_tp_dealloc, (void *)MGLBuffer_dealloc},
    {0},
};

PyType_Spec MGLBuffer_spec = {
    "moderngl.mgl.new.MGLBuffer",
    sizeof(MGLBuffer),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLBuffer_slots,
};

PyTypeObject * MGLBuffer_class;
