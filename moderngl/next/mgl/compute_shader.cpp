#include "compute_shader.hpp"
#include "context.hpp"

#include "internal/wrapper.hpp"

#include "internal/tools.hpp"
#include "internal/uniform.hpp"
#include "internal/glsl.hpp"

/* MGLContext.compute_shader(texture)
 */
PyObject * MGLContext_meth_compute_shader(MGLContext * self, PyObject * source) {
    MGLComputeShader * compute_shader = PyObject_New(MGLComputeShader, MGLComputeShader_class);
    chain_objects(self, compute_shader);
    compute_shader->context = self;

    const GLMethods & gl = self->gl;

	int program_obj = gl.CreateProgram();
	if (!program_obj) {
		PyErr_Format(moderngl_error, "cannot create program");
        Py_DECREF(compute_shader);
		return 0;
	}

	int shader_obj = gl.CreateShader(GL_COMPUTE_SHADER);
	if (!shader_obj) {
		PyErr_Format(moderngl_error, "cannot create shader");
        gl.DeleteProgram(program_obj);
        Py_DECREF(compute_shader);
		return 0;
	}

    compute_shader->program_obj = program_obj;
    compute_shader->shader_obj = shader_obj;

	int compiled = GL_FALSE;
	const char * source_str = PyUnicode_AsUTF8(source);
	gl.ShaderSource(shader_obj, 1, &source_str, 0);
	gl.CompileShader(shader_obj);
	gl.GetShaderiv(shader_obj, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
        int log_len = 0;
        gl.GetShaderiv(shader_obj, GL_INFO_LOG_LENGTH, &log_len);

        char * log_text = (char *)malloc(log_len + 1);
        gl.GetShaderInfoLog(shader_obj, log_len, 0, log_text);

        PyObject * res = PyObject_CallFunction(self->glsl_compiler_error, "ss#", "ComputeShader", log_text, log_len);
        Py_XDECREF(res);

        gl.DeleteProgram(program_obj);
        gl.DeleteShader(shader_obj);
        Py_DECREF(compute_shader);
        free(log_text);
        return 0;
	}

	int linked = GL_FALSE;
	gl.AttachShader(program_obj, shader_obj);
	gl.LinkProgram(program_obj);
	gl.GetProgramiv(program_obj, GL_LINK_STATUS, &linked);

	if (!linked) {
        int log_len = 0;
        gl.GetProgramiv(program_obj, GL_INFO_LOG_LENGTH, &log_len);

        char * log_text = (char *)malloc(log_len + 1);
        gl.GetProgramInfoLog(program_obj, log_len, 0, log_text);

        PyObject * res = PyObject_CallFunction(self->glsl_linker_error, "s#", log_text, log_len);
        Py_XDECREF(res);

        gl.DeleteProgram(program_obj);
        gl.DeleteShader(shader_obj);
        Py_DECREF(compute_shader);
        free(log_text);
        return 0;
	}

    self->use_program(program_obj);

    PyObject * uniforms = PyDict_New();

    int uniforms_len = 0;
    int uniform_blocks_len = 0;

    gl.GetProgramiv(program_obj, GL_ACTIVE_UNIFORMS, &uniforms_len);
    gl.GetProgramiv(program_obj, GL_ACTIVE_UNIFORM_BLOCKS, &uniform_blocks_len);

    for (int i = 0; i < uniforms_len; ++i) {
        int type = 0;
        int size = 0;
        int name_len = 0;
        char name[256];

        gl.GetActiveUniform(compute_shader->program_obj, i, 256, &name_len, &size, (GLenum *)&type, name);
        int location = gl.GetUniformLocation(compute_shader->program_obj, name);
        clean_glsl_name2(name, name_len);

        if (location < 0) {
            continue;
        }

        GLTypeInfo info = type_info(type);

        PyObject * uniform = Uniform_New("iiiiii", type, location, info.cols, info.rows, size, info.shape);
        PyDict_SetItemString(uniforms, name, uniform);
    }

    for (int i = 0; i < uniform_blocks_len; ++i) {
        int size = 0;
        int name_len = 0;
        char name[256];

        gl.GetActiveUniformBlockName(compute_shader->program_obj, i, 256, &name_len, name);
        int index = gl.GetUniformBlockIndex(compute_shader->program_obj, name);
        gl.GetActiveUniformBlockiv(compute_shader->program_obj, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
        clean_glsl_name2(name, name_len);

        PyObject * uniform = Uniform_New("OiOOii", Py_None, index, Py_None, Py_None, size, 0);
        PyDict_SetItemString(uniforms, name, uniform);
    }

    compute_shader->wrapper = ComputeShader_New("OO", compute_shader, uniforms);
    return compute_shader->wrapper;
}

/* MGLComputeShader.uniform(...)
 */
PyObject * MGLComputeShader_meth_uniform(MGLComputeShader * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 1 && nargs != 2) {
        PyErr_BadInternalCall();
        return 0;
    }
    const GLMethods & gl = self->context->gl;
    self->context->use_program(self->program_obj);
    return getset_uniform(gl, self->program_obj, args[0], nargs == 2 ? args[1] : NULL);
}

/* MGLComputeShader.run(group_x, group_y, group_z)
 */
PyObject * MGLComputeShader_meth_run(MGLComputeShader * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(3);

    unsigned x = PyLong_AsUnsignedLong(args[0]);
    unsigned y = PyLong_AsUnsignedLong(args[1]);
    unsigned z = PyLong_AsUnsignedLong(args[2]);

    ensure_no_error();

    self->context->use_program(self->program_obj);
	self->context->gl.DispatchCompute(x, y, z);
    Py_RETURN_NONE;
}

void MGLComputeShader_dealloc(MGLComputeShader * self) {
    printf("MGLComputeShader_dealloc\n");
    Py_TYPE(self)->tp_free(self);
}

fastcallable(MGLComputeShader_meth_uniform)
fastcallable(MGLComputeShader_meth_run)

PyMethodDef MGLComputeShader_methods[] = {
    {"uniform", fastcall(MGLComputeShader_meth_uniform), fastcall_flags, NULL},
    {"run", fastcall(MGLComputeShader_meth_run), fastcall_flags, NULL},
    {0},
};

PyType_Slot MGLComputeShader_slots[] = {
    {Py_tp_methods, MGLComputeShader_methods},
    {Py_tp_dealloc, (void *)MGLComputeShader_dealloc},
    {0},
};

PyType_Spec MGLComputeShader_spec = {
    "moderngl.mgl.new.MGLComputeShader",
    sizeof(MGLComputeShader),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLComputeShader_slots,
};

PyTypeObject * MGLComputeShader_class;
