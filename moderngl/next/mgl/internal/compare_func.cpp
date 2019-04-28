#include "opengl/opengl.hpp"

int compare_func_from_str(const char * str) {
	if (!str || !str[0] || (str[1] && str[2])) {
		return 0;
	}

	switch (str[0] * 256 + str[1]) {
		case ('<' * 256 + '='):
			return GL_LEQUAL;

		case ('<' * 256):
			return GL_LESS;

		case ('>' * 256 + '='):
			return GL_GEQUAL;

		case ('>' * 256):
			return GL_GREATER;

		case ('=' * 256 + '='):
			return GL_EQUAL;

		case ('!' * 256 + '='):
			return GL_NOTEQUAL;

		case ('0' * 256):
			return GL_NEVER;

		case ('1' * 256):
			return GL_ALWAYS;

		default:
			return 0;
	}
}
