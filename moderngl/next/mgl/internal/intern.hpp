#pragma once
#include "python.hpp"

extern PyObject * moderngl_error;
extern PyObject * moderngl_compiler_error;
extern PyObject * moderngl_linker_error;
extern PyObject * moderngl_split_format;

extern PyObject * numpy;
extern PyObject * numpy_frombuffer;

extern PyObject * pillow;
extern PyObject * pillow_image;

extern PyObject * tobytes_str;
extern PyObject * size_str;
extern PyObject * mode_str;
extern PyObject * points_long;
extern PyObject * triangles_long;

extern PyObject * Attribute_class;
extern PyObject * Buffer_class;
extern PyObject * ComputeShader_class;
extern PyObject * Context_class;
extern PyObject * Framebuffer_class;
extern PyObject * Limits_class;
extern PyObject * Program_class;
extern PyObject * Query_class;
extern PyObject * Renderbuffer_class;
extern PyObject * Sampler_class;
extern PyObject * Scope_class;
extern PyObject * Texture_class;
extern PyObject * Uniform_class;
extern PyObject * VertexArray_class;

extern PyObject * interns;
inline PyObject * _intern(PyObject * obj) {
    if (obj) {
        PyList_Append(interns, obj);
        Py_DECREF(obj);
    }
    return obj;
}

#define intern(obj) _intern(must_have(obj))
#define xintern(obj) _intern(obj)
