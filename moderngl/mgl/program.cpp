#include "program.hpp"
#include "classes/all.hpp"
#include "internal/modules.hpp"

const char * SHADER_NAME[] = {
	"vertex_shader",
	"fragment_shader",
	"geometry_shader",
	"tess_control_shader",
	"tess_evaluation_shader",
};

const int SHADER_TYPE[] = {
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_GEOMETRY_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER,
};

PyObject * MGLContext_meth_program(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 6) {
        // TODO: error
        return 0;
    }

	MGLProgram * program = MGLContext_new_object(self, Program);

	const GLMethods & gl = self->gl;

	int program_obj = gl.CreateProgram();
	if (!program_obj) {
		PyErr_Format(moderngl_error, "cannot create program");
		Py_DECREF(program);
		return 0;
	}

	program->program_obj = program_obj;

	for (int i = 0; i < 5; ++i) {
		if (args[i] == Py_None) {
			continue;
		}

		if (!PyUnicode_Check(args[i])) {
			PyErr_Format(PyExc_TypeError, "%s must be a str not %s", SHADER_NAME[i], args[i]->ob_type->tp_name);
			// Py_DECREF(program);
			return 0;
		}

		int shader_obj = gl.CreateShader(SHADER_TYPE[i]);
		if (!shader_obj) {
			PyErr_Format(moderngl_error, "cannot create shader");
			// Py_DECREF(program);
			return 0;
		}

		program->shader_obj[i] = shader_obj;

		int compiled = GL_FALSE;
		const char * source_str = PyUnicode_AsUTF8(args[i]);
		gl.ShaderSource(shader_obj, 1, &source_str, 0);
		gl.CompileShader(shader_obj);
		gl.GetShaderiv(shader_obj, GL_COMPILE_STATUS, &compiled);

		if (!compiled) {
			int log_len = 0;
			gl.GetShaderiv(shader_obj, GL_INFO_LOG_LENGTH, &log_len);

			char * log_text = (char *)malloc(log_len);
			gl.GetShaderInfoLog(shader_obj, log_len, &log_len, log_text);
			PyObject * name = PyUnicode_FromString(SHADER_NAME[i]);
			PyObject * info = PyUnicode_FromStringAndSize(log_text, log_len);
			// PyObject_CallFunctionObjArgs(moderngl_compiler_error, name, args[i], info, 0);
			// Py_DECREF(name);
			// Py_DECREF(info);
			// Py_DECREF(program);
			free(log_text);
			return 0;
		}

		gl.AttachShader(program_obj, shader_obj);
	}

    PyObject * varyings = args[5];
	if (PyUnicode_Check(varyings)) {
		varyings = PyTuple_Pack(1, varyings);
	} else {
		varyings = PySequence_Fast(varyings, "varyings is not iterable");
		if (!varyings) {
			return 0;
		}
	}

	int varyings_len = (int)PySequence_Fast_GET_SIZE(varyings);

	if (varyings_len) {
		const char ** varyings_str = new const char * [varyings_len];
		for (int i = 0; i < varyings_len; ++i) {
			PyObject * varying = PySequence_Fast_GET_ITEM(varyings, i);
			if (!PyUnicode_Check(varying)) {
				PyErr_Format(PyExc_TypeError, "expected str, got %s", varying->ob_type->tp_name);
				Py_DECREF(varyings);
				Py_DECREF(program);
				delete[] varyings_str;
				return 0;
			}
			varyings_str[i] = PyUnicode_AsUTF8(varying);
		}
		gl.TransformFeedbackVaryings(program_obj, varyings_len, varyings_str, GL_INTERLEAVED_ATTRIBS);
		delete[] varyings_str;
	}

	Py_DECREF(varyings);

	int linked = GL_FALSE;
	gl.LinkProgram(program_obj);
	gl.GetProgramiv(program_obj, GL_LINK_STATUS, &linked);

	if (!linked) {
		int log_len = 0;
		gl.GetProgramiv(program_obj, GL_INFO_LOG_LENGTH, &log_len);

		char * log_text = (char *)malloc(log_len);
		gl.GetProgramInfoLog(program_obj, log_len, &log_len, log_text);
		PyObject * info = PyUnicode_FromStringAndSize(log_text, log_len);
		// PyObject_CallFunctionObjArgs(moderngl_linker_error, args[0], args[1], args[2], args[3], args[4], info, 0);
        PyObject * args = PyTuple_Pack(6, args[0], args[1], args[2], args[3], args[4], info);
		PyObject_Call(moderngl_linker_error, args, 0);
		// Py_DECREF(info);
		// Py_DECREF(program);
		free(log_text);
		return 0;
	}

	PyObject * uniforms = PyDict_New();
	PyObject * attributes = PyDict_New();

	int attributes_len = 0;
	int uniforms_len = 0;
	int uniform_blocks_len = 0;

	gl.GetProgramiv(program_obj, GL_ACTIVE_ATTRIBUTES, &attributes_len);
	gl.GetProgramiv(program_obj, GL_ACTIVE_UNIFORMS, &uniforms_len);
	gl.GetProgramiv(program_obj, GL_ACTIVE_UNIFORM_BLOCKS, &uniform_blocks_len);

	// for (int i = 0; i < attributes_len; ++i) {
	// 	int type = 0;
	// 	int size = 0;
	// 	int name_len = 0;
	// 	char name[256];

	// 	gl.GetActiveAttrib(program_obj, i, 256, &name_len, &size, (GLenum *)&type, name);
	// 	int location = gl.GetAttribLocation(program_obj, name);
	// 	clean_glsl_name(name, name_len);

	// 	GLTypeInfo info = type_info(type);

	// 	PyObject * attrib = new_object(PyObject, Attribute_class);
	// 	SLOT(attrib, PyObject, Attribute_class_type) = PyLong_FromLong(type);
	// 	SLOT(attrib, PyObject, Attribute_class_location) = PyLong_FromLong(location);
	// 	SLOT(attrib, PyObject, Attribute_class_shape) = PyLong_FromLong(info.shape);
	// 	SLOT(attrib, PyObject, Attribute_class_cols) = PyLong_FromLong(info.cols);
	// 	SLOT(attrib, PyObject, Attribute_class_rows) = PyLong_FromLong(info.rows);
	// 	SLOT(attrib, PyObject, Attribute_class_size) = PyLong_FromLong(size);
	// 	PyDict_SetItemString(attributes, name, attrib);
	// }

	// for (int i = 0; i < uniforms_len; ++i) {
	// 	int type = 0;
	// 	int size = 0;
	// 	int name_len = 0;
	// 	char name[256];

	// 	gl.GetActiveUniform(program->program_obj, i, 256, &name_len, &size, (GLenum *)&type, name);
	// 	int location = gl.GetUniformLocation(program->program_obj, name);
	// 	clean_glsl_name(name, name_len);

	// 	if (location < 0) {
	// 		continue;
	// 	}

	// 	GLTypeInfo info = type_info(type);

	// 	PyObject * uniform = new_object(PyObject, Uniform_class);
	// 	SLOT(uniform, PyObject, Uniform_class_type) = PyLong_FromLong(type);
	// 	SLOT(uniform, PyObject, Uniform_class_location) = PyLong_FromLong(location);
	// 	SLOT(uniform, PyObject, Uniform_class_shape) = PyLong_FromLong(info.shape);
	// 	SLOT(uniform, PyObject, Uniform_class_cols) = PyLong_FromLong(info.cols);
	// 	SLOT(uniform, PyObject, Uniform_class_rows) = PyLong_FromLong(info.rows);
	// 	SLOT(uniform, PyObject, Uniform_class_size) = PyLong_FromLong(size);
	// 	PyDict_SetItemString(uniforms, name, uniform);
	// }

	// for (int i = 0; i < uniform_blocks_len; ++i) {
	// 	int size = 0;
	// 	int name_len = 0;
	// 	char name[256];

	// 	gl.GetActiveUniformBlockName(program->program_obj, i, 256, &name_len, name);
	// 	int index = gl.GetUniformBlockIndex(program->program_obj, name);
	// 	gl.GetActiveUniformBlockiv(program->program_obj, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
	// 	clean_glsl_name(name, name_len);

	// 	PyObject * uniform = new_object(PyObject, Uniform_class);
	// 	SLOT(uniform, PyObject, Uniform_class_location) = PyLong_FromLong(index);
	// 	SLOT(uniform, PyObject, Uniform_class_shape) = PyLong_FromLong(0);
	// 	SLOT(uniform, PyObject, Uniform_class_size) = PyLong_FromLong(size);
	// 	PyDict_SetItemString(uniforms, name, uniform);
	// }

	SLOT(program->wrapper, PyObject, Program_class_attributes) = attributes;
	SLOT(program->wrapper, PyObject, Program_class_uniforms) = uniforms;
	return NEW_REF(program->wrapper);
    Py_RETURN_NONE;
}

void write_uni(const GLMethods & gl, int program_obj, int location, int type, int size, void * ptr) {
	switch (type) {
		case GL_BOOL: gl.ProgramUniform1iv(program_obj, location, size, (int *)ptr); break;
		case GL_BOOL_VEC2: gl.ProgramUniform2iv(program_obj, location, size, (int *)ptr); break;
		case GL_BOOL_VEC3: gl.ProgramUniform3iv(program_obj, location, size, (int *)ptr); break;
		case GL_BOOL_VEC4: gl.ProgramUniform4iv(program_obj, location, size, (int *)ptr); break;
		case GL_INT: gl.ProgramUniform1iv(program_obj, location, size, (int *)ptr); break;
		case GL_INT_VEC2: gl.ProgramUniform2iv(program_obj, location, size, (int *)ptr); break;
		case GL_INT_VEC3: gl.ProgramUniform3iv(program_obj, location, size, (int *)ptr); break;
		case GL_INT_VEC4: gl.ProgramUniform4iv(program_obj, location, size, (int *)ptr); break;
		case GL_UNSIGNED_INT: gl.ProgramUniform1uiv(program_obj, location, size, (unsigned *)ptr); break;
		case GL_UNSIGNED_INT_VEC2: gl.ProgramUniform2uiv(program_obj, location, size, (unsigned *)ptr); break;
		case GL_UNSIGNED_INT_VEC3: gl.ProgramUniform3uiv(program_obj, location, size, (unsigned *)ptr); break;
		case GL_UNSIGNED_INT_VEC4: gl.ProgramUniform4uiv(program_obj, location, size, (unsigned *)ptr); break;
		case GL_FLOAT: gl.ProgramUniform1fv(program_obj, location, size, (float *)ptr); break;
		case GL_FLOAT_VEC2: gl.ProgramUniform2fv(program_obj, location, size, (float *)ptr); break;
		case GL_FLOAT_VEC3: gl.ProgramUniform3fv(program_obj, location, size, (float *)ptr); break;
		case GL_FLOAT_VEC4: gl.ProgramUniform4fv(program_obj, location, size, (float *)ptr); break;
		case GL_DOUBLE: gl.ProgramUniform1dv(program_obj, location, size, (double *)ptr); break;
		case GL_DOUBLE_VEC2: gl.ProgramUniform2dv(program_obj, location, size, (double *)ptr); break;
		case GL_DOUBLE_VEC3: gl.ProgramUniform3dv(program_obj, location, size, (double *)ptr); break;
		case GL_DOUBLE_VEC4: gl.ProgramUniform4dv(program_obj, location, size, (double *)ptr); break;
		case GL_FLOAT_MAT2: gl.ProgramUniformMatrix2fv(program_obj, location, size, false, (float *)ptr); break;
		case GL_FLOAT_MAT2x3: gl.ProgramUniformMatrix2x3fv(program_obj, location, size, false, (float *)ptr); break;
		case GL_FLOAT_MAT2x4: gl.ProgramUniformMatrix2x4fv(program_obj, location, size, false, (float *)ptr); break;
		case GL_FLOAT_MAT3x2: gl.ProgramUniformMatrix3x2fv(program_obj, location, size, false, (float *)ptr); break;
		case GL_FLOAT_MAT3: gl.ProgramUniformMatrix3fv(program_obj, location, size, false, (float *)ptr); break;
		case GL_FLOAT_MAT3x4: gl.ProgramUniformMatrix3x4fv(program_obj, location, size, false, (float *)ptr); break;
		case GL_FLOAT_MAT4x2: gl.ProgramUniformMatrix4x2fv(program_obj, location, size, false, (float *)ptr); break;
		case GL_FLOAT_MAT4x3: gl.ProgramUniformMatrix4x3fv(program_obj, location, size, false, (float *)ptr); break;
		case GL_FLOAT_MAT4: gl.ProgramUniformMatrix4fv(program_obj, location, size, false, (float *)ptr); break;
		case GL_DOUBLE_MAT2: gl.ProgramUniformMatrix2dv(program_obj, location, size, false, (double *)ptr); break;
		case GL_DOUBLE_MAT2x3: gl.ProgramUniformMatrix2x3dv(program_obj, location, size, false, (double *)ptr); break;
		case GL_DOUBLE_MAT2x4: gl.ProgramUniformMatrix2x4dv(program_obj, location, size, false, (double *)ptr); break;
		case GL_DOUBLE_MAT3x2: gl.ProgramUniformMatrix3x2dv(program_obj, location, size, false, (double *)ptr); break;
		case GL_DOUBLE_MAT3: gl.ProgramUniformMatrix3dv(program_obj, location, size, false, (double *)ptr); break;
		case GL_DOUBLE_MAT3x4: gl.ProgramUniformMatrix3x4dv(program_obj, location, size, false, (double *)ptr); break;
		case GL_DOUBLE_MAT4x2: gl.ProgramUniformMatrix4x2dv(program_obj, location, size, false, (double *)ptr); break;
		case GL_DOUBLE_MAT4x3: gl.ProgramUniformMatrix4x3dv(program_obj, location, size, false, (double *)ptr); break;
		case GL_DOUBLE_MAT4: gl.ProgramUniformMatrix4dv(program_obj, location, size, false, (double *)ptr); break;
		case GL_SAMPLER_2D: gl.ProgramUniform1iv(program_obj, location, size, (int *)ptr); break;
		case GL_SAMPLER_3D: gl.ProgramUniform1iv(program_obj, location, size, (int *)ptr); break;
		case GL_SAMPLER_2D_ARRAY: gl.ProgramUniform1iv(program_obj, location, size, (int *)ptr); break;
		case GL_SAMPLER_2D_SHADOW: gl.ProgramUniform1iv(program_obj, location, size, (int *)ptr); break;
		case GL_SAMPLER_2D_MULTISAMPLE: gl.ProgramUniform1iv(program_obj, location, size, (int *)ptr); break;
		case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: gl.ProgramUniform1iv(program_obj, location, size, (int *)ptr); break;
		case GL_SAMPLER_CUBE: gl.ProgramUniform1iv(program_obj, location, size, (int *)ptr); break;
		case GL_SAMPLER_CUBE_SHADOW: gl.ProgramUniform1iv(program_obj, location, size, (int *)ptr); break;
		case GL_SAMPLER_CUBE_MAP_ARRAY: gl.ProgramUniform1iv(program_obj, location, size, (int *)ptr); break;
		case GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW: gl.ProgramUniform1iv(program_obj, location, size, (int *)ptr); break;
	}
}

PyObject * MGLProgram_meth_uniform(MGLProgram * self, PyObject * const * args, Py_ssize_t nargs) {
	// PyObject * uniform;
	// PyObject * value;

	// int args_ok = PyArg_ParseTuple(
	// 	args,
	// 	"OO",
	// 	&uniform,
	// 	&value
	// );

	// if (!args_ok) {
	// 	return 0;
	// }

	// if (uniform == Py_None) {
	// 	Py_RETURN_NONE;
	// }

	// const GLMethods & gl = self->context->gl;

	// int location = PyLong_AsLong(SLOT(uniform, PyObject, Uniform_class_location));
	// int shape = PyLong_AsLong(SLOT(uniform, PyObject, Uniform_class_shape));
	// int type = PyLong_AsLong(SLOT(uniform, PyObject, Uniform_class_type));

	// if (shape) {
	// 	int cols = PyLong_AsLong(SLOT(uniform, PyObject, Uniform_class_cols));
	// 	int rows = PyLong_AsLong(SLOT(uniform, PyObject, Uniform_class_rows));
	// 	int size = PyLong_AsLong(SLOT(uniform, PyObject, Uniform_class_size));

	// 	if (value->ob_type->tp_as_buffer && value->ob_type->tp_as_buffer->bf_getbuffer) {
	// 		Py_buffer view = {};
	// 		if (value->ob_type->tp_as_buffer->bf_getbuffer(value, &view, PyBUF_STRIDED_RO) < 0) {
	// 			return 0;
	// 		}
	// 		if (PyBuffer_IsContiguous(&view, 'C')) {
	// 			write_uni(gl, self->program_obj, location, type, size, view.buf);
	// 		} else {
	// 			void * cache = malloc(view.len);
	// 			PyBuffer_ToContiguous(cache, &view, view.len, 'C');
	// 			write_uni(gl, self->program_obj, location, type, size, cache);
	// 			free(cache);
	// 		}
	// 		PyBuffer_Release(&view);
	// 	} else {
	// 		read_value func;
	// 		switch (shape) {
	// 			case 'f': func = (read_value)read_float; break;
	// 			case 'i': func = (read_value)read_int; break;
	// 			case 'u': func = (read_value)read_unsigned; break;
	// 			case 'd': func = (read_value)read_double; break;
	// 			case 'p': func = (read_value)read_bool; break;
	// 		}
	// 		if (size == 1 && rows == 1 && cols == 1) {
	// 			char cache[8];
	// 			void * ptr = cache;
	// 			func(ptr, value);
	// 			if (PyErr_Occurred()) {
	// 				return 0;
	// 			}
	// 			write_uni(gl, self->program_obj, location, type, size, cache);
	// 		} else {
	// 			PyObject * seq = PySequence_Fast(value, "fail");
	// 			if (!seq) {
	// 				return 0;
	// 			}
	// 			int seq_len = (int)PySequence_Fast_GET_SIZE(seq);
	// 			void * cache = malloc(seq_len * (shape == 'd' ? 8 : 4));
	// 			void * ptr = cache;
	// 			for (int i = 0; i < seq_len; ++i) {
	// 				func(ptr, PySequence_Fast_GET_ITEM(seq, i));
	// 			}
	// 			if (PyErr_Occurred()) {
	// 				Py_DECREF(seq);
	// 				free(cache);
	// 				return 0;
	// 			}
	// 			write_uni(gl, self->program_obj, location, type, size, cache);
	// 			Py_DECREF(seq);
	// 			free(cache);
	// 		}
	// 	}
	// } else {
	// 	int binding = PyLong_AsLong(value);

	// 	if (PyErr_Occurred()) {
	// 		return 0;
	// 	}

	// 	self->context->gl.UniformBlockBinding(self->program_obj, location, binding);
	// }

	Py_RETURN_NONE;
}

/* Backward compatible methods */

BC4(MGLProgram, uniform);

/* Definition of MGLProgram internal type */

void MGLProgram_define(MGLContext * ctx) {
	PyMethodDef MGLProgram_methods[] = {
        DEF4(MGLProgram, uniform),
		{0},
	};

	PyType_Slot MGLProgram_slots[] = {
		{Py_tp_methods, MGLProgram_methods},
		{0},
	};

    ctx->MGLProgram_class = define_python_class(mgl_name ".Program", sizeof(MGLProgram), MGLProgram_slots);
}
