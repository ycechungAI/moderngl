#pragma once
#include "python.hpp"

struct GLTypeInfo {
    int cols;
    int rows;
    int shape;
};

void clean_glsl_name2(char * name, int & name_len);

int swizzle_from_chr(char c);
char chr_from_swizzle(int c);

GLTypeInfo type_info(int type);
