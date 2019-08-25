#pragma once

#include "common.hpp"
#include "opengl.hpp"

struct DataType {
    int base_format[5];
    int internal_format[5];
    int gl_type;
    char shape;
    int size;
};

static DataType f1 = {
    {0, GL_RED, GL_RG, GL_RGB, GL_RGBA},
    {0, GL_R8, GL_RG8, GL_RGB8, GL_RGBA8},
    GL_UNSIGNED_BYTE, 'f', 1,
};

static DataType f2 = {
    {0, GL_RED, GL_RG, GL_RGB, GL_RGBA},
    {0, GL_R16F, GL_RG16F, GL_RGB16F, GL_RGBA16F},
    GL_HALF_FLOAT, 'f', 2,
};

static DataType f4 = {
    {0, GL_RED, GL_RG, GL_RGB, GL_RGBA},
    {0, GL_R32F, GL_RG32F, GL_RGB32F, GL_RGBA32F},
    GL_FLOAT, 'f', 4,
};

static DataType u1 = {
    {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
    {0, GL_R8UI, GL_RG8UI, GL_RGB8UI, GL_RGBA8UI},
    GL_UNSIGNED_BYTE, 'u', 1,
};

static DataType u2 = {
    {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
    {0, GL_R16UI, GL_RG16UI, GL_RGB16UI, GL_RGBA16UI},
    GL_UNSIGNED_SHORT, 'u', 2,
};

static DataType u4 = {
    {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
    {0, GL_R32UI, GL_RG32UI, GL_RGB32UI, GL_RGBA32UI},
    GL_UNSIGNED_INT, 'u', 4,
};

static DataType i1 = {
    {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
    {0, GL_R8I, GL_RG8I, GL_RGB8I, GL_RGBA8I},
    GL_BYTE, 'i', 1,
};

static DataType i2 = {
    {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
    {0, GL_R16I, GL_RG16I, GL_RGB16I, GL_RGBA16I},
    GL_SHORT, 'i', 2,
};

static DataType i4 = {
    {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
    {0, GL_R32I, GL_RG32I, GL_RGB32I, GL_RGBA32I},
    GL_INT, 'i', 4,
};

static DataType d2 = {
    {0, GL_DEPTH_COMPONENT, 0, 0, 0},
    {0, GL_DEPTH_COMPONENT16, 0, 0, 0},
    GL_HALF_FLOAT, 'd', 2,
};

static DataType d3 = {
    {0, GL_DEPTH_COMPONENT, 0, 0, 0},
    {0, GL_DEPTH_COMPONENT24, 0, 0, 0},
    GL_FLOAT, 'd', 4,
};

static DataType d4 = {
    {0, GL_DEPTH_COMPONENT, 0, 0, 0},
    {0, GL_DEPTH_COMPONENT32, 0, 0, 0},
    GL_FLOAT, 'd', 4,
};
