#include "cpp_classes.hpp"

#include "../buffer.hpp"
#include "../configuration.hpp"
#include "../context.hpp"
#include "../framebuffer.hpp"
#include "../program.hpp"
#include "../query.hpp"
#include "../renderbuffer.hpp"
#include "../sampler.hpp"
#include "../scope.hpp"
#include "../texture.hpp"
#include "../vertex_array.hpp"

extern PyObject * MGLBuffer_meth_write(MGLBuffer * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLBuffer_meth_read(MGLBuffer * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLBuffer_meth_map(MGLBuffer * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLBuffer_meth_unmap(MGLBuffer * self);
extern PyObject * MGLBuffer_meth_clear(MGLBuffer * self);

extern PyObject * MGLContext_meth_buffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLContext_meth_configure(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLContext_meth_framebuffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLContext_meth_program(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLContext_meth_query(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLContext_meth_renderbuffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLContext_meth_sampler(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLContext_meth_scope(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLContext_meth_texture(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLContext_meth_vertex_array(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);

extern PyObject * MGLFramebuffer_meth_read(MGLFramebuffer * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLFramebuffer_meth_use(MGLFramebuffer * self);
extern PyObject * MGLFramebuffer_meth_clear(MGLFramebuffer * self, PyObject * const * args, Py_ssize_t nargs);

extern PyObject * MGLProgram_meth_uniform(MGLProgram * self, PyObject * const * args, Py_ssize_t nargs);

extern PyObject * MGLQuery_meth_begin(MGLQuery * self);
extern PyObject * MGLQuery_meth_end(MGLQuery * self);
extern PyObject * MGLQuery_meth_begin_render(MGLQuery * self);
extern PyObject * MGLQuery_meth_end_render(MGLQuery * self);

extern PyObject * MGLRenderbuffer_meth_write(MGLRenderbuffer * self, PyObject * const * args, Py_ssize_t nargs);

extern PyObject * MGLSampler_meth_use(MGLSampler * self, PyObject * const * args, Py_ssize_t nargs);

extern PyObject * MGLScope_meth_begin(MGLScope * self);
extern PyObject * MGLScope_meth_end(MGLScope * self);

extern PyObject * MGLTexture_meth_write(MGLTexture * self, PyObject * const * args, Py_ssize_t nargs);

extern PyObject * MGLVertexArray_meth_render(MGLVertexArray * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLVertexArray_meth_transform(MGLVertexArray * self, PyObject * const * args, Py_ssize_t nargs);

extern int MGLVertexArray_set_ibo(MGLVertexArray * self, PyObject * value);

#if PY_VERSION_HEX >= 0x03070000

PyMethodDef MGLBuffer_methods[] = {
    {"write", (PyCFunction)MGLBuffer_meth_write, METH_FASTCALL, 0},
    {"read", (PyCFunction)MGLBuffer_meth_read, METH_FASTCALL, 0},
    {"map", (PyCFunction)MGLBuffer_meth_map, METH_FASTCALL, 0},
    {"unmap", (PyCFunction)MGLBuffer_meth_unmap, METH_NOARGS, 0},
    {"clear", (PyCFunction)MGLBuffer_meth_clear, METH_NOARGS, 0},
    {0},
};

PyMethodDef MGLContext_methods[] = {
    {"buffer", (PyCFunction)MGLContext_meth_buffer, METH_FASTCALL, 0},
    {"configure", (PyCFunction)MGLContext_meth_configure, METH_FASTCALL, 0},
    {"framebuffer", (PyCFunction)MGLContext_meth_framebuffer, METH_FASTCALL, 0},
    {"program", (PyCFunction)MGLContext_meth_program, METH_FASTCALL, 0},
    {"query", (PyCFunction)MGLContext_meth_query, METH_FASTCALL, 0},
    {"renderbuffer", (PyCFunction)MGLContext_meth_renderbuffer, METH_FASTCALL, 0},
    {"sampler", (PyCFunction)MGLContext_meth_sampler, METH_FASTCALL, 0},
    {"scope", (PyCFunction)MGLContext_meth_scope, METH_FASTCALL, 0},
    {"texture", (PyCFunction)MGLContext_meth_texture, METH_FASTCALL, 0},
    {"vertex_array", (PyCFunction)MGLContext_meth_vertex_array, METH_FASTCALL, 0},
    {0},
};

PyMethodDef MGLFramebuffer_methods[] = {
    {"read", (PyCFunction)MGLFramebuffer_meth_read, METH_FASTCALL, 0},
    {"use", (PyCFunction)MGLFramebuffer_meth_use, METH_NOARGS, 0},
    {"clear", (PyCFunction)MGLFramebuffer_meth_clear, METH_FASTCALL, 0},
    {0},
};

PyMethodDef MGLProgram_methods[] = {
    {"uniform", (PyCFunction)MGLProgram_meth_uniform, METH_FASTCALL, 0},
    {0},
};

PyMethodDef MGLQuery_methods[] = {
    {"begin", (PyCFunction)MGLQuery_meth_begin, METH_NOARGS, 0},
    {"end", (PyCFunction)MGLQuery_meth_end, METH_NOARGS, 0},
    {"begin_render", (PyCFunction)MGLQuery_meth_begin_render, METH_NOARGS, 0},
    {"end_render", (PyCFunction)MGLQuery_meth_end_render, METH_NOARGS, 0},
    {0},
};

PyMethodDef MGLRenderbuffer_methods[] = {
    {"write", (PyCFunction)MGLRenderbuffer_meth_write, METH_FASTCALL, 0},
    {0},
};

PyMethodDef MGLSampler_methods[] = {
    {"use", (PyCFunction)MGLSampler_meth_use, METH_FASTCALL, 0},
    {0},
};

PyMethodDef MGLScope_methods[] = {
    {"begin", (PyCFunction)MGLScope_meth_begin, METH_NOARGS, 0},
    {"end", (PyCFunction)MGLScope_meth_end, METH_NOARGS, 0},
    {0},
};

PyMethodDef MGLTexture_methods[] = {
    {"write", (PyCFunction)MGLTexture_meth_write, METH_FASTCALL, 0},
    {0},
};

PyMethodDef MGLVertexArray_methods[] = {
    {"render", (PyCFunction)MGLVertexArray_meth_render, METH_FASTCALL, 0},
    {"transform", (PyCFunction)MGLVertexArray_meth_transform, METH_FASTCALL, 0},
    {0},
};

#else

PyObject * MGLBuffer_meth_write_va(MGLBuffer * self, PyObject * args) {
    return MGLBuffer_meth_write(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLBuffer_meth_read_va(MGLBuffer * self, PyObject * args) {
    return MGLBuffer_meth_read(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLBuffer_meth_map_va(MGLBuffer * self, PyObject * args) {
    return MGLBuffer_meth_map(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}


PyObject * MGLContext_meth_buffer_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_buffer(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_configure_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_configure(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_framebuffer_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_framebuffer(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_program_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_program(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_query_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_query(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_renderbuffer_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_renderbuffer(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_sampler_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_sampler(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_scope_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_scope(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_texture_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_texture(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_vertex_array_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_vertex_array(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLFramebuffer_meth_read_va(MGLFramebuffer * self, PyObject * args) {
    return MGLFramebuffer_meth_read(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLFramebuffer_meth_clear_va(MGLFramebuffer * self, PyObject * args) {
    return MGLFramebuffer_meth_clear(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLProgram_meth_uniform_va(MGLProgram * self, PyObject * args) {
    return MGLProgram_meth_uniform(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}


PyObject * MGLRenderbuffer_meth_write_va(MGLRenderbuffer * self, PyObject * args) {
    return MGLRenderbuffer_meth_write(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLSampler_meth_use_va(MGLSampler * self, PyObject * args) {
    return MGLSampler_meth_use(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}


PyObject * MGLTexture_meth_write_va(MGLTexture * self, PyObject * args) {
    return MGLTexture_meth_write(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLVertexArray_meth_render_va(MGLVertexArray * self, PyObject * args) {
    return MGLVertexArray_meth_render(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLVertexArray_meth_transform_va(MGLVertexArray * self, PyObject * args) {
    return MGLVertexArray_meth_transform(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyMethodDef MGLBuffer_methods[] = {
    {"write", (PyCFunction)MGLBuffer_meth_write_va, METH_VARARGS, 0},
    {"read", (PyCFunction)MGLBuffer_meth_read_va, METH_VARARGS, 0},
    {"map", (PyCFunction)MGLBuffer_meth_map_va, METH_VARARGS, 0},
    {"unmap", (PyCFunction)MGLBuffer_meth_unmap, METH_NOARGS, 0},
    {"clear", (PyCFunction)MGLBuffer_meth_clear, METH_NOARGS, 0},
    {0},
};

PyMethodDef MGLContext_methods[] = {
    {"buffer", (PyCFunction)MGLContext_meth_buffer_va, METH_VARARGS, 0},
    {"configure", (PyCFunction)MGLContext_meth_configure_va, METH_VARARGS, 0},
    {"framebuffer", (PyCFunction)MGLContext_meth_framebuffer_va, METH_VARARGS, 0},
    {"program", (PyCFunction)MGLContext_meth_program_va, METH_VARARGS, 0},
    {"query", (PyCFunction)MGLContext_meth_query_va, METH_VARARGS, 0},
    {"renderbuffer", (PyCFunction)MGLContext_meth_renderbuffer_va, METH_VARARGS, 0},
    {"sampler", (PyCFunction)MGLContext_meth_sampler_va, METH_VARARGS, 0},
    {"scope", (PyCFunction)MGLContext_meth_scope_va, METH_VARARGS, 0},
    {"texture", (PyCFunction)MGLContext_meth_texture_va, METH_VARARGS, 0},
    {"vertex_array", (PyCFunction)MGLContext_meth_vertex_array_va, METH_VARARGS, 0},
    {0},
};

PyMethodDef MGLFramebuffer_methods[] = {
    {"read", (PyCFunction)MGLFramebuffer_meth_read_va, METH_VARARGS, 0},
    {"use", (PyCFunction)MGLFramebuffer_meth_use, METH_NOARGS, 0},
    {"clear", (PyCFunction)MGLFramebuffer_meth_clear_va, METH_VARARGS, 0},
    {0},
};

PyMethodDef MGLProgram_methods[] = {
    {"uniform", (PyCFunction)MGLProgram_meth_uniform_va, METH_VARARGS, 0},
    {0},
};

PyMethodDef MGLQuery_methods[] = {
    {"begin", (PyCFunction)MGLQuery_meth_begin, METH_NOARGS, 0},
    {"end", (PyCFunction)MGLQuery_meth_end, METH_NOARGS, 0},
    {"begin_render", (PyCFunction)MGLQuery_meth_begin_render, METH_NOARGS, 0},
    {"end_render", (PyCFunction)MGLQuery_meth_end_render, METH_NOARGS, 0},
    {0},
};

PyMethodDef MGLRenderbuffer_methods[] = {
    {"write", (PyCFunction)MGLRenderbuffer_meth_write_va, METH_VARARGS, 0},
    {0},
};

PyMethodDef MGLSampler_methods[] = {
    {"use", (PyCFunction)MGLSampler_meth_use_va, METH_VARARGS, 0},
    {0},
};

PyMethodDef MGLScope_methods[] = {
    {"begin", (PyCFunction)MGLScope_meth_begin, METH_NOARGS, 0},
    {"end", (PyCFunction)MGLScope_meth_end, METH_NOARGS, 0},
    {0},
};

PyMethodDef MGLTexture_methods[] = {
    {"write", (PyCFunction)MGLTexture_meth_write_va, METH_VARARGS, 0},
    {0},
};

PyMethodDef MGLVertexArray_methods[] = {
    {"render", (PyCFunction)MGLVertexArray_meth_render_va, METH_VARARGS, 0},
    {"transform", (PyCFunction)MGLVertexArray_meth_transform_va, METH_VARARGS, 0},
    {0},
};

#endif

PyGetSetDef MGLBuffer_getset[] = {
    {0},
};

PyGetSetDef MGLContext_getset[] = {
    {0},
};

PyGetSetDef MGLFramebuffer_getset[] = {
    {0},
};

PyGetSetDef MGLProgram_getset[] = {
    {0},
};

PyGetSetDef MGLQuery_getset[] = {
    {0},
};

PyGetSetDef MGLRenderbuffer_getset[] = {
    {0},
};

PyGetSetDef MGLSampler_getset[] = {
    {0},
};

PyGetSetDef MGLScope_getset[] = {
    {0},
};

PyGetSetDef MGLTexture_getset[] = {
    {0},
};

PyGetSetDef MGLVertexArray_getset[] = {
    {"ibo", 0, (setter)MGLVertexArray_set_ibo, 0, 0},
    {0},
};

PyType_Slot MGLBuffer_slots[] = {
    {Py_tp_methods, MGLBuffer_methods},
    {Py_tp_getset, MGLBuffer_getset},
    {0},
};

PyType_Slot MGLContext_slots[] = {
    {Py_tp_methods, MGLContext_methods},
    {Py_tp_getset, MGLContext_getset},
    {0},
};

PyType_Slot MGLFramebuffer_slots[] = {
    {Py_tp_methods, MGLFramebuffer_methods},
    {Py_tp_getset, MGLFramebuffer_getset},
    {0},
};

PyType_Slot MGLProgram_slots[] = {
    {Py_tp_methods, MGLProgram_methods},
    {Py_tp_getset, MGLProgram_getset},
    {0},
};

PyType_Slot MGLQuery_slots[] = {
    {Py_tp_methods, MGLQuery_methods},
    {Py_tp_getset, MGLQuery_getset},
    {0},
};

PyType_Slot MGLRenderbuffer_slots[] = {
    {Py_tp_methods, MGLRenderbuffer_methods},
    {Py_tp_getset, MGLRenderbuffer_getset},
    {0},
};

PyType_Slot MGLSampler_slots[] = {
    {Py_tp_methods, MGLSampler_methods},
    {Py_tp_getset, MGLSampler_getset},
    {0},
};

PyType_Slot MGLScope_slots[] = {
    {Py_tp_methods, MGLScope_methods},
    {Py_tp_getset, MGLScope_getset},
    {0},
};

PyType_Slot MGLTexture_slots[] = {
    {Py_tp_methods, MGLTexture_methods},
    {Py_tp_getset, MGLTexture_getset},
    {0},
};

PyType_Slot MGLVertexArray_slots[] = {
    {Py_tp_methods, MGLVertexArray_methods},
    {Py_tp_getset, MGLVertexArray_getset},
    {0},
};

PyType_Spec MGLBuffer_spec = {
    mgl_name ".Buffer",
    sizeof(MGLBuffer),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLBuffer_slots,
};

PyType_Spec MGLContext_spec = {
    mgl_name ".Context",
    sizeof(MGLContext),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLContext_slots,
};

PyType_Spec MGLFramebuffer_spec = {
    mgl_name ".Framebuffer",
    sizeof(MGLFramebuffer),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLFramebuffer_slots,
};

PyType_Spec MGLProgram_spec = {
    mgl_name ".Program",
    sizeof(MGLProgram),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLProgram_slots,
};

PyType_Spec MGLQuery_spec = {
    mgl_name ".Query",
    sizeof(MGLQuery),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLQuery_slots,
};

PyType_Spec MGLRenderbuffer_spec = {
    mgl_name ".Renderbuffer",
    sizeof(MGLRenderbuffer),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLRenderbuffer_slots,
};

PyType_Spec MGLSampler_spec = {
    mgl_name ".Sampler",
    sizeof(MGLSampler),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLSampler_slots,
};

PyType_Spec MGLScope_spec = {
    mgl_name ".Scope",
    sizeof(MGLScope),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLScope_slots,
};

PyType_Spec MGLTexture_spec = {
    mgl_name ".Texture",
    sizeof(MGLTexture),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLTexture_slots,
};

PyType_Spec MGLVertexArray_spec = {
    mgl_name ".VertexArray",
    sizeof(MGLVertexArray),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLVertexArray_slots,
};
