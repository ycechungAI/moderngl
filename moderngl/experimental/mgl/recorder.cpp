#include "recorder.hpp"

#include "context.hpp"

#include "internal/bytecode.hpp"
#include "internal/modules.hpp"
#include "internal/wrapper.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

PyTypeObject * MGLRecorder_class;

PyObject * MGLContext_meth_replay(MGLContext * self, PyObject * bytecode) {
    if (MGLBytecode::ptr != MGLBytecode::buffer) {
        return 0;
    }

    memcpy(&MGLBytecode::gl, &self->gl, sizeof(GLMethods));
    MGLScope_begin_core(self->default_scope);

    Py_buffer view = {};
    PyObject_GetBuffer(bytecode, &view, PyBUF_STRIDED_RO);
    PyBuffer_ToContiguous(MGLBytecode::buffer, &view, view.len, 'C');
    PyBuffer_Release(&view);

    MGLBytecode::evaluate((int)view.len);

    MGLScope_end_core(self->default_scope);
    Py_RETURN_NONE;
}

MGLRecorder * MGLRecorder_meth_enter(MGLRecorder * self) {
    if (!MGLBytecode::buffer) {
        MGLBytecode::buffer = (char *)malloc(16384);
        MGLBytecode::ptr = MGLBytecode::buffer;
    }

    if (MGLBytecode::ptr != MGLBytecode::buffer) {
        return 0;
    }

    self->old_enable_only = self->context->current_enable_only;
    self->old_program_obj = self->context->current_program_obj;
    self->old_array_buffer_obj = self->context->current_array_buffer_obj;
    self->old_vertex_array_obj = self->context->current_vertex_array_obj;
    self->old_framebuffer_obj = self->context->current_framebuffer_obj;
    self->old_temp_texture_obj = self->context->current_temp_texture_obj;
    self->old_alignment = self->context->current_alignment;
    self->old_color_mask = self->context->current_color_mask;
    self->old_depth_mask = self->context->current_depth_mask;

    self->context->current_enable_only = 0;
    self->context->current_program_obj = 0;
    self->context->current_array_buffer_obj = 0;
    self->context->current_vertex_array_obj = 0;
    self->context->current_framebuffer_obj = 0;
    self->context->current_temp_texture_obj = 0;
    self->context->current_alignment = 0;
    self->context->current_color_mask = 0xffffffffffffffff;
    self->context->current_depth_mask = true;

    self->old_bound_framebuffer = self->context->bound_framebuffer;
    self->old_active_scope = self->context->active_scope;
    self->old_bound_scope = self->context->bound_scope;

    self->context->bound_framebuffer = self->context->default_framebuffer;
    self->context->active_scope = self->context->default_scope;
    self->context->bound_scope = self->context->default_scope;

    for (int i = 0; i < 32; ++i) {
        self->old_sampler_obj[i] = self->context->current_sampler_obj[i];
        self->context->current_sampler_obj[i] = 0;
    }

    memcpy(&MGLBytecode::gl, &self->context->gl, sizeof(GLMethods));
    void ** src = (void **)&MGLBytecode::rec;
    void ** dst = (void **)&self->context->gl;
    int size = sizeof(GLMethods) / sizeof(void *);
    for (int i = 0; i < size; ++i) {
        if (dst[i]) {
            dst[i] = src[i];
        }
    }
    return NEW_REF(self);
}

PyObject * MGLRecorder_meth_exit(MGLRecorder * self) {
    self->context->current_enable_only = self->old_enable_only;
    self->context->current_program_obj = self->old_program_obj;
    self->context->current_array_buffer_obj = self->old_array_buffer_obj;
    self->context->current_vertex_array_obj = self->old_vertex_array_obj;
    self->context->current_framebuffer_obj = self->old_framebuffer_obj;
    self->context->current_temp_texture_obj = self->old_temp_texture_obj;
    self->context->current_alignment = self->old_alignment;
    self->context->current_color_mask = self->old_color_mask;
    self->context->current_depth_mask = self->old_depth_mask;

    self->context->bound_framebuffer = self->old_bound_framebuffer;
    self->context->active_scope = self->old_active_scope;
    self->context->bound_scope = self->old_bound_scope;

    for (int i = 0; i < 32; ++i) {
        self->context->current_sampler_obj[i] = self->old_sampler_obj[i];
    }

    memcpy(&self->context->gl, &MGLBytecode::gl, sizeof(GLMethods));
    Py_RETURN_NONE;
}

PyObject * MGLRecorder_meth_dump(MGLRecorder * self) {
    if (MGLBytecode::error) {
        MGLBytecode::error = false;
        MGLBytecode::ptr = MGLBytecode::buffer;
        return 0;
    }

    PyObject * res = PyBytes_FromStringAndSize(MGLBytecode::buffer, MGLBytecode::ptr - MGLBytecode::buffer);
    MGLBytecode::ptr = MGLBytecode::buffer;
    return res;
}

PyObject * MGLRecorder_meth_resize(MGLContext * self, PyObject * size) {
    if (MGLBytecode::ptr != MGLBytecode::buffer) {
        return 0;
    }

    Py_ssize_t alloc_size = unpack_size(size);
    free(MGLBytecode::buffer);
    if (alloc_size) {
        MGLBytecode::buffer = (char *)malloc(alloc_size);
    } else {
        MGLBytecode::buffer = 0;
    }
    MGLBytecode::ptr = MGLBytecode::buffer;
    Py_RETURN_NONE;
}

PyMethodDef MGLRecorder_methods[] = {
    {"__enter__", (PyCFunction)MGLRecorder_meth_enter, METH_NOARGS, 0},
    {"__exit__", (PyCFunction)MGLRecorder_meth_exit, METH_VARARGS, 0},
    {"dump", (PyCFunction)MGLRecorder_meth_dump, METH_NOARGS, 0},
    {"resize", (PyCFunction)MGLRecorder_meth_resize, METH_O, 0},
    {0},
};

PyType_Slot MGLRecorder_slots[] = {
    {Py_tp_methods, MGLRecorder_methods},
    {0},
};

PyType_Spec MGLRecorder_spec = {
    mgl_name ".Recorder",
    sizeof(MGLRecorder),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLRecorder_slots,
};

void init_recording() {
    MGLBytecode::init();
    MGLRecorder_class = (PyTypeObject *)PyType_FromSpec(&MGLRecorder_spec);
}
