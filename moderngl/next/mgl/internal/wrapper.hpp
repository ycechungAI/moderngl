#pragma once

#include "python.hpp"
#include "intern.hpp"

#define Attribute_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)Attribute_class)
#define Buffer_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)Buffer_class)
#define ComputeShader_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)ComputeShader_class)
#define Context_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)Context_class)
#define Framebuffer_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)Framebuffer_class)
#define Limits_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)Limits_class)
#define Program_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)Program_class)
#define Query_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)Query_class)
#define Renderbuffer_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)Renderbuffer_class)
#define Sampler_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)Sampler_class)
#define Scope_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)Scope_class)
#define Texture_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)Texture_class)
#define Uniform_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)Uniform_class)
#define VertexArray_Check(obj) (Py_TYPE(obj) == (PyTypeObject *)VertexArray_class)

#define Attribute_New(...) PyObject_CallFunction(Attribute_class, __VA_ARGS__)
#define Buffer_New(...) PyObject_CallFunction(Buffer_class, __VA_ARGS__)
#define ComputeShader_New(...) PyObject_CallFunction(ComputeShader_class, __VA_ARGS__)
#define Context_New(...) PyObject_CallFunction(Context_class, __VA_ARGS__)
#define Framebuffer_New(...) PyObject_CallFunction(Framebuffer_class, __VA_ARGS__)
#define Limits_New(...) PyObject_CallFunction(Limits_class, __VA_ARGS__)
#define Program_New(...) PyObject_CallFunction(Program_class, __VA_ARGS__)
#define Query_New(...) PyObject_CallFunction(Query_class, __VA_ARGS__)
#define Renderbuffer_New(...) PyObject_CallFunction(Renderbuffer_class, __VA_ARGS__)
#define Sampler_New(...) PyObject_CallFunction(Sampler_class, __VA_ARGS__)
#define Scope_New(...) PyObject_CallFunction(Scope_class, __VA_ARGS__)
#define Texture_New(...) PyObject_CallFunction(Texture_class, __VA_ARGS__)
#define Uniform_New(...) PyObject_CallFunction(Uniform_class, __VA_ARGS__)
#define VertexArray_New(...) PyObject_CallFunction(VertexArray_class, __VA_ARGS__)
