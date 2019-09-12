#include "Types.hpp"
#include "Error.hpp"

#include "BufferFormat.hpp"

PyObject * strsize(PyObject * self, PyObject * args) {
	const char * str;

	int args_ok = PyArg_ParseTuple(
		args,
		"s",
		&str
	);

	if (!args_ok) {
		return 0;
	}

	char first_chr = *str++;
	if (first_chr < '1' || first_chr > '9') {
		return 0;
	}

	long long value = first_chr - '0';

	while (char chr = *str++) {
		if (chr < '0' || chr > '9') {
			switch (chr) {
				case 'G':
					value *= 1024;

				case 'M':
					value *= 1024;

				case 'K':
					value *= 1024;

					if (*str++ != 'B') {
						return 0;
					}

				case 'B':
					if (*str++) {
						return 0;
					}

				case 0:
					break;

				default:
					return 0;
			}
			break;
		}

		value = value * 10 + chr - '0';
	}

	return PyLong_FromLongLong(value);
}

PyObject * fmtdebug(PyObject * self, PyObject * args) {
	const char * str;

	int args_ok = PyArg_ParseTuple(
		args,
		"s",
		&str
	);

	if (!args_ok) {
		return 0;
	}

	FormatIterator it = FormatIterator(str);
	FormatInfo format_info = it.info();

	PyObject * nodes = PyList_New(0);

	if (format_info.valid) {
		while (FormatNode * node = it.next()) {
			PyObject * obj = PyTuple_New(4);
			PyTuple_SET_ITEM(obj, 0, PyLong_FromLong(node->size));
			PyTuple_SET_ITEM(obj, 1, PyLong_FromLong(node->count));
			PyTuple_SET_ITEM(obj, 2, PyLong_FromLong(node->type));
			PyTuple_SET_ITEM(obj, 3, PyBool_FromLong(node->normalize));
			PyList_Append(nodes, obj);
		}
	}

	PyObject * res = PyTuple_New(5);
	PyTuple_SET_ITEM(res, 0, PyLong_FromLong(format_info.size));
	PyTuple_SET_ITEM(res, 1, PyLong_FromLong(format_info.nodes));
	PyTuple_SET_ITEM(res, 2, PyLong_FromLong(format_info.divisor));
	PyTuple_SET_ITEM(res, 3, PyBool_FromLong(format_info.valid));
	PyTuple_SET_ITEM(res, 4, PyList_AsTuple(nodes));
	Py_DECREF(nodes);
	return res;
}

PyObject * create_standalone_context(PyObject * self, PyObject * args) {
	int egl = false;

	int args_ok = PyArg_ParseTuple(
		args,
		"p",
		&egl
	);

	if (!args_ok) {
		return 0;
	}

	MGLContext * ctx = (MGLContext *)MGLContext_Type.tp_alloc(&MGLContext_Type, 0);

	ctx->gl_context.error = "unknown error";
	if (!ctx->gl_context.load(true, !!egl)) {
		MGLError_Set(ctx->gl_context.error);
		return 0;
	}
	ctx->wireframe = false;

	if (PyErr_Occurred()) {
		return 0;
	}

	MGLContext_Initialize(ctx);

	if (PyErr_Occurred()) {
		return 0;
	}

	Py_INCREF(ctx);

	PyObject * result = PyTuple_New(2);
	PyTuple_SET_ITEM(result, 0, (PyObject *)ctx);
	PyTuple_SET_ITEM(result, 1, PyLong_FromLong(ctx->version_code));
	return result;
}

PyObject * create_context(PyObject * self) {
	MGLContext * ctx = (MGLContext *)MGLContext_Type.tp_alloc(&MGLContext_Type, 0);

	ctx->gl_context.load(false, NULL);
	ctx->wireframe = false;

	if (PyErr_Occurred()) {
		return 0;
	}

	MGLContext_Initialize(ctx);

	if (PyErr_Occurred()) {
		return 0;
	}

	Py_INCREF(ctx);

	PyObject * result = PyTuple_New(2);
	PyTuple_SET_ITEM(result, 0, (PyObject *)ctx);
	PyTuple_SET_ITEM(result, 1, PyLong_FromLong(ctx->version_code));
	return result;
}

PyMethodDef MGL_module_methods[] = {
	{"strsize", (PyCFunction)strsize, METH_VARARGS, 0},
	{"create_standalone_context", (PyCFunction)create_standalone_context, METH_VARARGS, 0},
	{"create_context", (PyCFunction)create_context, METH_NOARGS, 0},
	{"fmtdebug", (PyCFunction)fmtdebug, METH_VARARGS, 0},
	{0},
};

bool MGL_InitializeModule(PyObject * module) {
	{
		if (PyType_Ready(&MGLAttribute_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register Attribute in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLAttribute_Type);

		PyModule_AddObject(module, "Attribute", (PyObject *)&MGLAttribute_Type);
	}

	{
		if (PyType_Ready(&MGLBuffer_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register Buffer in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLBuffer_Type);

		PyModule_AddObject(module, "Buffer", (PyObject *)&MGLBuffer_Type);
	}

	{
		if (PyType_Ready(&MGLComputeShader_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register ComputeShader in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLComputeShader_Type);

		PyModule_AddObject(module, "ComputeShader", (PyObject *)&MGLComputeShader_Type);
	}

	{
		if (PyType_Ready(&MGLContext_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register Context in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLContext_Type);

		PyModule_AddObject(module, "Context", (PyObject *)&MGLContext_Type);
	}

	{
		if (PyType_Ready(&MGLFramebuffer_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register Framebuffer in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLFramebuffer_Type);

		PyModule_AddObject(module, "Framebuffer", (PyObject *)&MGLFramebuffer_Type);
	}

	{
		if (PyType_Ready(&MGLInvalidObject_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register InvalidObject in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLInvalidObject_Type);

		PyModule_AddObject(module, "InvalidObject", (PyObject *)&MGLInvalidObject_Type);
	}

	{
		if (PyType_Ready(&MGLProgram_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register Program in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLProgram_Type);

		PyModule_AddObject(module, "Program", (PyObject *)&MGLProgram_Type);
	}

	{
		if (PyType_Ready(&MGLQuery_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register Query in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLQuery_Type);

		PyModule_AddObject(module, "Query", (PyObject *)&MGLQuery_Type);
	}

	{
		if (PyType_Ready(&MGLRenderbuffer_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register Renderbuffer in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLRenderbuffer_Type);

		PyModule_AddObject(module, "Renderbuffer", (PyObject *)&MGLRenderbuffer_Type);
	}

	{
		if (PyType_Ready(&MGLScope_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register Scope in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLScope_Type);

		PyModule_AddObject(module, "Scope", (PyObject *)&MGLScope_Type);
	}

	{
		if (PyType_Ready(&MGLTexture_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register Texture in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLTexture_Type);

		PyModule_AddObject(module, "Texture", (PyObject *)&MGLTexture_Type);
	}

	{
		if (PyType_Ready(&MGLTextureArray_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register TextureArray in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLTextureArray_Type);

		PyModule_AddObject(module, "TextureArray", (PyObject *)&MGLTextureArray_Type);
	}

	{
		if (PyType_Ready(&MGLTextureCube_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register TextureCube in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLTextureCube_Type);

		PyModule_AddObject(module, "TextureCube", (PyObject *)&MGLTextureCube_Type);
	}

	{
		if (PyType_Ready(&MGLTexture3D_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register Texture3D in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLTexture3D_Type);

		PyModule_AddObject(module, "Texture3D", (PyObject *)&MGLTexture3D_Type);
	}

	{
		if (PyType_Ready(&MGLUniform_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register Uniform in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLUniform_Type);

		PyModule_AddObject(module, "Uniform", (PyObject *)&MGLUniform_Type);
	}

	{
		if (PyType_Ready(&MGLUniformBlock_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register UniformBlock in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLUniformBlock_Type);

		PyModule_AddObject(module, "UniformBlock", (PyObject *)&MGLUniformBlock_Type);
	}

	{
		if (PyType_Ready(&MGLVertexArray_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register VertexArray in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLVertexArray_Type);

		PyModule_AddObject(module, "VertexArray", (PyObject *)&MGLVertexArray_Type);
	}

	{
		if (PyType_Ready(&MGLSampler_Type) < 0) {
			PyErr_Format(PyExc_ImportError, "Cannot register Sampler in %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}

		Py_INCREF(&MGLSampler_Type);

		PyModule_AddObject(module, "Sampler", (PyObject *)&MGLSampler_Type);
	}

	return true;
}

PyModuleDef MGL_moduledef = {
	PyModuleDef_HEAD_INIT,
	"mgl",
	0,
	-1,
	MGL_module_methods,
	0,
	0,
	0,
	0,
};

extern "C" PyObject * PyInit_mgl() {
	PyObject * module = PyModule_Create(&MGL_moduledef);

	if (!MGL_InitializeModule(module)) {
		return 0;
	}

	return module;
}
