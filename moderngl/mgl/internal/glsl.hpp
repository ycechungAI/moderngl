#pragma once
#include "python.hpp"

struct GLTypeInfo {
    int cols;
    int rows;
    int shape;
};

void clean_glsl_name(char * name, int & name_len);
GLTypeInfo type_info(int type);

void read_float(float *& ptr, PyObject * value);
void read_int(int *& ptr, PyObject * value);
void read_unsigned(unsigned *& ptr, PyObject * value);
void read_double(double *& ptr, PyObject * value);
void read_bool(int *& ptr, PyObject * value);

typedef void (* read_value)(void *& ptr, PyObject * value);
