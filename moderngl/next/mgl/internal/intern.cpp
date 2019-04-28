#include "intern.hpp"

PyObject * moderngl_error;
PyObject * moderngl_compiler_error;
PyObject * moderngl_linker_error;
PyObject * moderngl_split_format;

PyObject * numpy;
PyObject * numpy_frombuffer;

PyObject * pillow;
PyObject * pillow_image;

PyObject * tobytes_str;
PyObject * size_str;
PyObject * mode_str;
PyObject * points_long;
PyObject * triangles_long;

PyObject * Attribute_class;
PyObject * Buffer_class;
PyObject * ComputeShader_class;
PyObject * Context_class;
PyObject * Framebuffer_class;
PyObject * Limits_class;
PyObject * Program_class;
PyObject * Query_class;
PyObject * Renderbuffer_class;
PyObject * Sampler_class;
PyObject * Scope_class;
PyObject * Texture_class;
PyObject * Uniform_class;
PyObject * VertexArray_class;

PyObject * interns;
