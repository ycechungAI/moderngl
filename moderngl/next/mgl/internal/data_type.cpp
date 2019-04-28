#include "data_type.hpp"

#include "opengl/opengl.hpp"

MGLDataType f1 = {
    NULL,
    {0, GL_RED, GL_RG, GL_RGB, GL_RGBA},
    {0, GL_R8, GL_RG8, GL_RGB8, GL_RGBA8},
    GL_UNSIGNED_BYTE,
    1,
    'f',
};

MGLDataType f2 = {
    NULL,
    {0, GL_RED, GL_RG, GL_RGB, GL_RGBA},
    {0, GL_R16F, GL_RG16F, GL_RGB16F, GL_RGBA16F},
    GL_HALF_FLOAT,
    2,
    'f',
};

MGLDataType f4 = {
    NULL,
    {0, GL_RED, GL_RG, GL_RGB, GL_RGBA},
    {0, GL_R32F, GL_RG32F, GL_RGB32F, GL_RGBA32F},
    GL_FLOAT,
    4,
    'f',
};

MGLDataType u1 = {
    NULL,
    {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
    {0, GL_R8UI, GL_RG8UI, GL_RGB8UI, GL_RGBA8UI},
    GL_UNSIGNED_BYTE,
    1,
    'u',
};

MGLDataType u2 = {
    NULL,
    {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
    {0, GL_R16UI, GL_RG16UI, GL_RGB16UI, GL_RGBA16UI},
    GL_UNSIGNED_SHORT,
    2,
    'u',
};

MGLDataType u4 = {
    NULL,
    {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
    {0, GL_R32UI, GL_RG32UI, GL_RGB32UI, GL_RGBA32UI},
    GL_UNSIGNED_INT,
    4,
    'u',
};

MGLDataType i1 = {
    NULL,
    {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
    {0, GL_R8I, GL_RG8I, GL_RGB8I, GL_RGBA8I},
    GL_BYTE,
    1,
    'i',
};

MGLDataType i2 = {
    NULL,
    {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
    {0, GL_R16I, GL_RG16I, GL_RGB16I, GL_RGBA16I},
    GL_SHORT,
    2,
    'i',
};

MGLDataType i4 = {
    NULL,
    {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER},
    {0, GL_R32I, GL_RG32I, GL_RGB32I, GL_RGBA32I},
    GL_INT,
    4,
    'i',
};

MGLDataType d2 = {
    NULL,
    {0, GL_DEPTH_COMPONENT, 0, 0, 0},
    {0, GL_DEPTH_COMPONENT16, 0, 0, 0},
    GL_HALF_FLOAT,
    2,
    'd',
};

MGLDataType d3 = {
    NULL,
    {0, GL_DEPTH_COMPONENT, 0, 0, 0},
    {0, GL_DEPTH_COMPONENT24, 0, 0, 0},
    GL_FLOAT,
    4,
    'd',
};

MGLDataType d4 = {
    NULL,
    {0, GL_DEPTH_COMPONENT, 0, 0, 0},
    {0, GL_DEPTH_COMPONENT32, 0, 0, 0},
    GL_FLOAT,
    4,
    'd',
};

MGLDataType * from_dtype(PyObject * dtype) {
    if (PyUnicode_Check(dtype) && PyUnicode_GET_LENGTH(dtype) == 2) {
        short * code = (short *)PyUnicode_AsUTF8(dtype);
        switch (*code) {
            case 'f' | '1' << 8:
                return &f1;

            case 'f' | '2' << 8:
                return &f2;

            case 'f' | '4' << 8:
                return &f4;

            case 'u' | '1' << 8:
                return &u1;

            case 'u' | '2' << 8:
                return &u2;

            case 'u' | '4' << 8:
                return &u4;

            case 'i' | '1' << 8:
                return &i1;

            case 'i' | '2' << 8:
                return &i2;

            case 'i' | '4' << 8:
                return &i4;

            case 'd' | '2' << 8:
                return &d2;

            case 'd' | '3' << 8:
                return &d3;

            case 'd' | '4' << 8:
                return &d4;
        }
    }
    return NULL;
}
