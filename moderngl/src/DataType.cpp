#include "Types.hpp"

static int float_base_format[5] = {0, GL_RED, GL_RG, GL_RGB, GL_RGBA};
static int int_base_format[5] = {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER};

static int f1_internal_format[5] = {0, GL_R8, GL_RG8, GL_RGB8, GL_RGBA8};
static int f2_internal_format[5] = {0, GL_R16F, GL_RG16F, GL_RGB16F, GL_RGBA16F};
static int f4_internal_format[5] = {0, GL_R32F, GL_RG32F, GL_RGB32F, GL_RGBA32F};
static int u1_internal_format[5] = {0, GL_R8UI, GL_RG8UI, GL_RGB8UI, GL_RGBA8UI};
static int u2_internal_format[5] = {0, GL_R16UI, GL_RG16UI, GL_RGB16UI, GL_RGBA16UI};
static int u4_internal_format[5] = {0, GL_R32UI, GL_RG32UI, GL_RGB32UI, GL_RGBA32UI};
static int i1_internal_format[5] = {0, GL_R8I, GL_RG8I, GL_RGB8I, GL_RGBA8I};
static int i2_internal_format[5] = {0, GL_R16I, GL_RG16I, GL_RGB16I, GL_RGBA16I};
static int i4_internal_format[5] = {0, GL_R32I, GL_RG32I, GL_RGB32I, GL_RGBA32I};

static MGLDataType f1 = {float_base_format, f1_internal_format, GL_UNSIGNED_BYTE, 1, true};
static MGLDataType f2 = {float_base_format, f2_internal_format, GL_HALF_FLOAT, 2, true};
static MGLDataType f4 = {float_base_format, f4_internal_format, GL_FLOAT, 4, true};
static MGLDataType u1 = {int_base_format, u1_internal_format, GL_UNSIGNED_BYTE, 1, false};
static MGLDataType u2 = {int_base_format, u2_internal_format, GL_UNSIGNED_SHORT, 2, false};
static MGLDataType u4 = {int_base_format, u4_internal_format, GL_UNSIGNED_INT, 4, false};
static MGLDataType i1 = {int_base_format, i1_internal_format, GL_BYTE, 1, false};
static MGLDataType i2 = {int_base_format, i2_internal_format, GL_SHORT, 2, false};
static MGLDataType i4 = {int_base_format, i4_internal_format, GL_INT, 4, false};

MGLDataType * from_dtype(const char * dtype) {
	if (!dtype[0] || (dtype[1] && dtype[2])) {
		return 0;
	}

	switch (dtype[0] * 256 + dtype[1]) {
		case ('f' * 256 + '1'):
			return &f1;

		case ('f' * 256 + '2'):
			return &f2;

		case ('f' * 256 + '4'):
			return &f4;

		case ('u' * 256 + '1'):
			return &u1;

		case ('u' * 256 + '2'):
			return &u2;

		case ('u' * 256 + '4'):
			return &u4;

		case ('i' * 256 + '1'):
			return &i1;

		case ('i' * 256 + '2'):
			return &i2;

		case ('i' * 256 + '4'):
			return &i4;

		default:
			return 0;
	}
}
