#include "cpp_classes.hpp"

#include "../buffer.hpp"
#include "../context.hpp"
#include "../program.hpp"
#include "../texture.hpp"
#include "../vertex_array.hpp"

extern PyObject * MGLBuffer_meth_write(MGLBuffer * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLBuffer_meth_read(MGLBuffer * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLBuffer_meth_map(MGLBuffer * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLBuffer_meth_unmap(MGLBuffer * self);
extern PyObject * MGLBuffer_meth_clear(MGLBuffer * self);

extern PyObject * MGLContext_meth_buffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLContext_meth_program(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLContext_meth_texture(MGLContext * self, PyObject * const * args, Py_ssize_t nargs);

extern PyObject * MGLProgram_meth_uniform(MGLProgram * self, PyObject * const * args, Py_ssize_t nargs);

extern PyObject * MGLTexture_meth_write(MGLTexture * self, PyObject * const * args, Py_ssize_t nargs);

extern PyObject * MGLVertexArray_meth_render(MGLVertexArray * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * MGLVertexArray_meth_transform(MGLVertexArray * self, PyObject * const * args, Py_ssize_t nargs);

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
    {"program", (PyCFunction)MGLContext_meth_program, METH_FASTCALL, 0},
    {"texture", (PyCFunction)MGLContext_meth_texture, METH_FASTCALL, 0},
    {0},
};

PyMethodDef MGLProgram_methods[] = {
    {"uniform", (PyCFunction)MGLProgram_meth_uniform, METH_FASTCALL, 0},
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

PyObject * MGLContext_meth_program_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_program(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_texture_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_texture(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLProgram_meth_uniform_va(MGLProgram * self, PyObject * args) {
    return MGLProgram_meth_uniform(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
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
    {"program", (PyCFunction)MGLContext_meth_program_va, METH_VARARGS, 0},
    {"texture", (PyCFunction)MGLContext_meth_texture_va, METH_VARARGS, 0},
    {0},
};

PyMethodDef MGLProgram_methods[] = {
    {"uniform", (PyCFunction)MGLProgram_meth_uniform_va, METH_VARARGS, 0},
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

PyType_Slot MGLBuffer_slots[] = {
    {Py_tp_methods, MGLBuffer_methods},
    {0},
};

PyType_Slot MGLContext_slots[] = {
    {Py_tp_methods, MGLContext_methods},
    {0},
};

PyType_Slot MGLProgram_slots[] = {
    {Py_tp_methods, MGLProgram_methods},
    {0},
};

PyType_Slot MGLTexture_slots[] = {
    {Py_tp_methods, MGLTexture_methods},
    {0},
};

PyType_Slot MGLVertexArray_slots[] = {
    {Py_tp_methods, MGLVertexArray_methods},
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

PyType_Spec MGLProgram_spec = {
    mgl_name ".Program",
    sizeof(MGLProgram),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLProgram_slots,
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
