#include "Types.hpp"

#include "InlineMethods.hpp"

PyObject * MGLContext_compute_shader(MGLContext * self, PyObject * args) {
	PyObject * source;

	int args_ok = PyArg_ParseTuple(
		args,
		"O",
		&source
	);

	if (!args_ok) {
		return 0;
	}

	if (!PyUnicode_Check(source)) {
		MGLError_Set("the source must be a string not %s", Py_TYPE(source)->tp_name);
		return 0;
	}

	const char * source_str = PyUnicode_AsUTF8(source);

    MGLComputeShader * compute_shader = PyObject_New(MGLComputeShader, MGLComputeShader_type);
    compute_shader->released = false;

	Py_INCREF(self);
	compute_shader->context = self;

	const GLMethods & gl = self->gl;

	int program_obj = gl.CreateProgram();

	if (!program_obj) {
		MGLError_Set("cannot create program");
		return 0;
	}

	int shader_obj = gl.CreateShader(GL_COMPUTE_SHADER);

	if (!shader_obj) {
		MGLError_Set("cannot create the shader object");
		return 0;
	}

	gl.ShaderSource(shader_obj, 1, &source_str, 0);
	gl.CompileShader(shader_obj);

	int compiled = GL_FALSE;
	gl.GetShaderiv(shader_obj, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		const char * message = "GLSL Compiler failed";
		const char * title = "ComputeShader";
		const char * underline = "=============";

		int log_len = 0;
		gl.GetShaderiv(shader_obj, GL_INFO_LOG_LENGTH, &log_len);

		char * log = new char[log_len];
		gl.GetShaderInfoLog(shader_obj, log_len, &log_len, log);

		gl.DeleteShader(shader_obj);

		MGLError_Set("%s\n\n%s\n%s\n%s\n", message, title, underline, log);

		delete[] log;
		return 0;
	}

	gl.AttachShader(program_obj, shader_obj);
	gl.LinkProgram(program_obj);

	int linked = GL_FALSE;
	gl.GetProgramiv(program_obj, GL_LINK_STATUS, &linked);

	if (!linked) {
		const char * message = "GLSL Linker failed";
		const char * title = "ComputeShader";
		const char * underline = "=============";

		int log_len = 0;
		gl.GetProgramiv(program_obj, GL_INFO_LOG_LENGTH, &log_len);

		char * log = new char[log_len];
		gl.GetProgramInfoLog(program_obj, log_len, &log_len, log);

		gl.DeleteProgram(program_obj);

		MGLError_Set("%s\n\n%s\n%s\n%s\n", message, title, underline, log);

		delete[] log;
		return 0;
	}

	compute_shader->shader_obj = shader_obj;
	compute_shader->program_obj = program_obj;

	Py_INCREF(compute_shader);

	int num_uniforms = 0;
	int num_uniform_blocks = 0;

	gl.GetProgramiv(program_obj, GL_ACTIVE_UNIFORMS, &num_uniforms);
	gl.GetProgramiv(program_obj, GL_ACTIVE_UNIFORM_BLOCKS, &num_uniform_blocks);

	PyObject * members_dict = PyDict_New();

	for (int i = 0; i < num_uniforms; ++i) {
		int type = 0;
		int array_length = 0;
		int name_len = 0;
		char name[256];

		gl.GetActiveUniform(program_obj, i, 256, &name_len, &array_length, (GLenum *)&type, name);
		int location = gl.GetUniformLocation(program_obj, name);

		clean_glsl_name(name, name_len);

		if (location < 0) {
			continue;
		}

        PyObject * item = PyObject_CallMethod(
            helper, "make_uniform", "(siiiiO)",
            name, type, program_obj, location, array_length, self
        );

        PyDict_SetItemString(members_dict, name, item);
        Py_DECREF(item);
	}

	for (int i = 0; i < num_uniform_blocks; ++i) {
		int size = 0;
		int name_len = 0;
		char name[256];

		gl.GetActiveUniformBlockName(program_obj, i, 256, &name_len, name);
		int index = gl.GetUniformBlockIndex(program_obj, name);
		gl.GetActiveUniformBlockiv(program_obj, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);

		clean_glsl_name(name, name_len);

        PyObject * item = PyObject_CallMethod(
            helper, "make_uniform_block", "(siiiO)",
            name, program_obj, index, size, self
        );

        PyDict_SetItemString(members_dict, name, item);
        Py_DECREF(item);
	}

	PyObject * result = PyTuple_New(3);
	PyTuple_SET_ITEM(result, 0, (PyObject *)compute_shader);
	PyTuple_SET_ITEM(result, 1, members_dict);
	PyTuple_SET_ITEM(result, 2, PyLong_FromLong(compute_shader->program_obj));
	return result;
}

PyObject * MGLComputeShader_run(MGLComputeShader * self, PyObject * args) {
	unsigned x;
	unsigned y;
	unsigned z;

	int args_ok = PyArg_ParseTuple(
		args,
		"III",
		&x,
		&y,
		&z
	);

	if (!args_ok) {
		return 0;
	}

	const GLMethods & gl = self->context->gl;

	gl.UseProgram(self->program_obj);
	gl.DispatchCompute(x, y, z);

	Py_RETURN_NONE;
}

PyObject * MGLComputeShader_release(MGLComputeShader * self) {
	MGLComputeShader_Invalidate(self);
	Py_RETURN_NONE;
}

void MGLComputeShader_Invalidate(MGLComputeShader * compute_shader) {
	if (compute_shader->released) {
		return;
	}
	compute_shader->released = true;

	// TODO: decref

	const GLMethods & gl = compute_shader->context->gl;
	gl.DeleteShader(compute_shader->shader_obj);
	gl.DeleteProgram(compute_shader->program_obj);

	Py_DECREF(compute_shader->context);
	Py_DECREF(compute_shader);
}
