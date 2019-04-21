#include "Types.hpp"

struct ProgramAttributeInfo {
	void * gl_attrib_ptr_proc;
	int program_obj;
	int number;
	int location;
	int type;
	int scalar_type;
	int dimension;
	int array_length;
	int rows_length;
	int row_length;
	char shape;
	bool normalizable;
};

static void _fill_attrib_info(ProgramAttributeInfo * self, const GLMethods & gl) {
	switch (self->type) {
		case GL_INT:
			self->dimension = 1;

			self->scalar_type = GL_INT;

			self->rows_length = self->array_length * 1;
			self->row_length = 1;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribIPointer;

			self->normalizable = false;
			self->shape = 'i';
			break;

		case GL_INT_VEC2:
			self->dimension = 2;

			self->scalar_type = GL_INT;

			self->rows_length = self->array_length * 1;
			self->row_length = 2;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribIPointer;

			self->normalizable = false;
			self->shape = 'i';
			break;

		case GL_INT_VEC3:
			self->dimension = 3;

			self->scalar_type = GL_INT;

			self->rows_length = self->array_length * 1;
			self->row_length = 3;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribIPointer;

			self->normalizable = false;
			self->shape = 'i';
			break;

		case GL_INT_VEC4:
			self->dimension = 4;

			self->scalar_type = GL_INT;

			self->rows_length = self->array_length * 1;
			self->row_length = 4;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribIPointer;

			self->normalizable = false;
			self->shape = 'i';
			break;

		case GL_UNSIGNED_INT:
			self->dimension = 1;

			self->scalar_type = GL_UNSIGNED_INT;

			self->rows_length = self->array_length * 1;
			self->row_length = 1;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribIPointer;

			self->normalizable = false;
			self->shape = 'I';
			break;

		case GL_UNSIGNED_INT_VEC2:
			self->dimension = 2;

			self->scalar_type = GL_UNSIGNED_INT;

			self->rows_length = self->array_length * 1;
			self->row_length = 2;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribIPointer;

			self->normalizable = false;
			self->shape = 'I';
			break;

		case GL_UNSIGNED_INT_VEC3:
			self->dimension = 3;

			self->scalar_type = GL_UNSIGNED_INT;

			self->rows_length = self->array_length * 1;
			self->row_length = 3;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribIPointer;

			self->normalizable = false;
			self->shape = 'I';
			break;

		case GL_UNSIGNED_INT_VEC4:
			self->dimension = 4;

			self->scalar_type = GL_UNSIGNED_INT;

			self->rows_length = self->array_length * 1;
			self->row_length = 4;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribIPointer;

			self->normalizable = false;
			self->shape = 'I';
			break;

		case GL_FLOAT:
			self->dimension = 1;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 1;
			self->row_length = 1;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_FLOAT_VEC2:
			self->dimension = 2;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 1;
			self->row_length = 2;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_FLOAT_VEC3:
			self->dimension = 3;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 1;
			self->row_length = 3;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_FLOAT_VEC4:
			self->dimension = 4;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 1;
			self->row_length = 4;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_DOUBLE:
			self->dimension = 1;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 1;
			self->row_length = 1;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		case GL_DOUBLE_VEC2:
			self->dimension = 2;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 1;
			self->row_length = 2;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		case GL_DOUBLE_VEC3:
			self->dimension = 3;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 1;
			self->row_length = 3;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		case GL_DOUBLE_VEC4:
			self->dimension = 4;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 1;
			self->row_length = 4;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		case GL_FLOAT_MAT2:
			self->dimension = 4;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 2;
			self->row_length = 2;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_FLOAT_MAT2x3:
			self->dimension = 6;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 2;
			self->row_length = 3;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_FLOAT_MAT2x4:
			self->dimension = 8;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 2;
			self->row_length = 4;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_FLOAT_MAT3x2:
			self->dimension = 6;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 3;
			self->row_length = 2;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_FLOAT_MAT3:
			self->dimension = 9;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 3;
			self->row_length = 3;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_FLOAT_MAT3x4:
			self->dimension = 12;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 3;
			self->row_length = 4;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_FLOAT_MAT4x2:
			self->dimension = 8;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 4;
			self->row_length = 2;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_FLOAT_MAT4x3:
			self->dimension = 12;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 4;
			self->row_length = 3;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_FLOAT_MAT4:
			self->dimension = 16;

			self->scalar_type = GL_FLOAT;

			self->rows_length = self->array_length * 4;
			self->row_length = 4;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = true;
			self->shape = 'f';
			break;

		case GL_DOUBLE_MAT2:
			self->dimension = 4;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 2;
			self->row_length = 2;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		case GL_DOUBLE_MAT2x3:
			self->dimension = 6;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 2;
			self->row_length = 3;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		case GL_DOUBLE_MAT2x4:
			self->dimension = 8;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 2;
			self->row_length = 4;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		case GL_DOUBLE_MAT3x2:
			self->dimension = 6;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 3;
			self->row_length = 2;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		case GL_DOUBLE_MAT3:
			self->dimension = 9;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 3;
			self->row_length = 3;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		case GL_DOUBLE_MAT3x4:
			self->dimension = 12;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 3;
			self->row_length = 4;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		case GL_DOUBLE_MAT4x2:
			self->dimension = 8;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 4;
			self->row_length = 2;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		case GL_DOUBLE_MAT4x3:
			self->dimension = 12;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 4;
			self->row_length = 3;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		case GL_DOUBLE_MAT4:
			self->dimension = 16;

			self->scalar_type = GL_DOUBLE;

			self->rows_length = self->array_length * 4;
			self->row_length = 4;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribLPointer;

			self->normalizable = false;
			self->shape = 'd';
			break;

		default:
			self->dimension = 1;

			self->scalar_type = 0;

			self->rows_length = self->array_length * 1;
			self->row_length = 1;

			self->gl_attrib_ptr_proc = (void *)gl.VertexAttribPointer;

			self->normalizable = false;
			self->shape = 0;
			break;
	}
}
