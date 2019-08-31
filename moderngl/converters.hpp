#include "common.hpp"

#include "data_types.hpp"

int parse_reserve(PyObject * obj, int * reserve);
int parse_texture_size(PyObject * obj, int * size);
int parse_viewport2d(PyObject * obj, int * viewport);
int parse_viewport3d(PyObject * obj, int * viewport);
int parse_float_color(PyObject * obj, float * color);
int parse_int_color(PyObject * obj, int * color);
int parse_uint_color(PyObject * obj, unsigned * color);
int parse_dtype(PyObject * obj, DataType ** dtype);
int parse_components(PyObject * obj, int * components);
int parse_samples(PyObject * obj, int * samples);
int parse_swizzle(PyObject * obj, int * swizzle);
int parse_filter(PyObject * obj, int * filter);
int parse_wrap(PyObject * obj, int * wrap);
int parse_lod_range(PyObject * obj, float * lod_range);
int parse_offset2d(PyObject * obj, int * offset);
