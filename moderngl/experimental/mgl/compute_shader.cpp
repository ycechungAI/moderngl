#include "compute_shader.hpp"
#include "context.hpp"

#include "internal/wrapper.hpp"

#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

/* MGLContext.compute_shader(texture)
 */
PyObject * MGLContext_meth_compute_shader(MGLContext * self, PyObject * source) {
    MGLComputeShader * compute_shader = MGLContext_new_object(self, ComputeShader);

    const GLMethods & gl = self->gl;

	int program_obj = gl.CreateProgram();
	if (!program_obj) {
		PyErr_Format(moderngl_error, "cannot create program");
        Py_DECREF(compute_shader);
		return 0;
	}

	int shader_obj = gl.CreateShader(GL_COMPUTE_SHADER);
	if (!shader_obj) {
		PyErr_Format(moderngl_error, "cannot create the shader object");
        // Py_DECREF(compute_shader);
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
        PyObject * name = PyUnicode_FromString("ComputeShader");
        PyObject * info = PyUnicode_FromStringAndSize(log_text, log_len);
        call_function(self->glsl_compiler_error, name, source, info);
        Py_DECREF(name);
        Py_DECREF(info);
        // Py_DECREF(compute_shader);
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
        PyObject * info = PyUnicode_FromStringAndSize(log_text, log_len);
        call_function(self->glsl_linker_error, info);
        Py_DECREF(info);
        // Py_DECREF(compute_shader);
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
        clean_glsl_name(name, name_len);

        if (location < 0) {
            continue;
        }

        GLTypeInfo info = type_info(type);

        PyObject * uniform = new_object(PyObject, Uniform_class);
        SLOT(uniform, PyObject, Uniform_class_type) = PyLong_FromLong(type);
        SLOT(uniform, PyObject, Uniform_class_location) = PyLong_FromLong(location);
        SLOT(uniform, PyObject, Uniform_class_cols) = PyLong_FromLong(info.cols);
        SLOT(uniform, PyObject, Uniform_class_rows) = PyLong_FromLong(info.rows);
        SLOT(uniform, PyObject, Uniform_class_size) = PyLong_FromLong(size);
        SLOT(uniform, PyObject, Uniform_class_shape) = PyLong_FromLong(info.shape);
        PyDict_SetItemString(uniforms, name, uniform);
    }

    for (int i = 0; i < uniform_blocks_len; ++i) {
        int size = 0;
        int name_len = 0;
        char name[256];

        gl.GetActiveUniformBlockName(compute_shader->program_obj, i, 256, &name_len, name);
        int index = gl.GetUniformBlockIndex(compute_shader->program_obj, name);
        gl.GetActiveUniformBlockiv(compute_shader->program_obj, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
        clean_glsl_name(name, name_len);

        PyObject * uniform = new_object(PyObject, Uniform_class);
        SLOT(uniform, PyObject, Uniform_class_type) = 0;
        SLOT(uniform, PyObject, Uniform_class_location) = PyLong_FromLong(index);
        SLOT(uniform, PyObject, Uniform_class_cols) = 0;
        SLOT(uniform, PyObject, Uniform_class_rows) = 0;
        SLOT(uniform, PyObject, Uniform_class_size) = PyLong_FromLong(size);
        SLOT(uniform, PyObject, Uniform_class_shape) = PyLong_FromLong(0);
        PyDict_SetItemString(uniforms, name, uniform);
    }

    SLOT(compute_shader->wrapper, PyObject, ComputeShader_class_uniforms) = uniforms;
    return NEW_REF(compute_shader->wrapper);
}

/* MGLComputeShader.run(group_x, group_y, group_z)
 */
PyObject * MGLComputeShader_meth_run(MGLComputeShader * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 3) {
        // TODO: error
        return 0;
    }

    unsigned x = PyLong_AsUnsignedLong(args[0]);
    unsigned y = PyLong_AsUnsignedLong(args[1]);
    unsigned z = PyLong_AsUnsignedLong(args[2]);

    const GLMethods & gl = self->context->gl;
    self->context->use_program(self->program_obj);
	// gl.UseProgram(self->program_obj);
	gl.DispatchCompute(x, y, z);
    Py_RETURN_NONE;
}

#if PY_VERSION_HEX >= 0x03070000

PyMethodDef MGLComputeShader_methods[] = {
    {"run", (PyCFunction)MGLComputeShader_meth_run, METH_FASTCALL, 0},
    {0},
};

#else

PyObject * MGLComputeShader_meth_run_va(MGLComputeShader * self, PyObject * args) {
    return MGLComputeShader_meth_run(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyMethodDef MGLComputeShader_methods[] = {
    {"run", (PyCFunction)MGLComputeShader_meth_run_va, METH_VARARGS, 0},
    {0},
};

#endif

PyType_Slot MGLComputeShader_slots[] = {
    {Py_tp_methods, MGLComputeShader_methods},
    {0},
};

PyType_Spec MGLComputeShader_spec = {
    mgl_name ".ComputeShader",
    sizeof(MGLComputeShader),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLComputeShader_slots,
};
