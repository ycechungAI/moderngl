#include "context.hpp"

#include "tools.hpp"

PyObject * meth_create_context(PyObject * self, PyObject * args) { TRACE_VARAGS
	int standalone;

	int args_ok = PyArg_ParseTuple(
		args,
		"p",
		&standalone
	);

	if (!args_ok) {
		return 0;
	}

	initialize_module();

	if (PyErr_Occurred()) {
		return 0;
	}

	Py_INCREF(MGLContext_type);
	MGLContext * context = PyObject_New(MGLContext, MGLContext_type);

	if (!context->gl_context.load(standalone)) {
		return 0;
	}

	if (!context->gl.load()) {
		return 0;
	}

	const GLMethods & gl = context->gl;

	PyObject * framebuffers = PyDict_New();

	context->wrapper = new_object(PyObject, Context_class);
	SLOT(context->wrapper, MGLContext, Context_class_mglo) = context;
	SLOT(context->wrapper, PyObject, Context_class_framebuffers) = framebuffers;
	SLOT(context->wrapper, PyObject, Context_class_screen) = 0;
	SLOT(context->wrapper, PyObject, Context_class_fbo) = 0;
	return NEW_REF(context->wrapper);
}

PyTypeObject * MGLContext_define() {
	PyMethodDef MGLContext_methods[] = {
		{0},
	};

	PyGetSetDef MGLContext_getset[] = {
		{0},
	};

	PyType_Slot MGLContext_slots[] = {
		{Py_tp_methods, dup(MGLContext_methods)},
		{Py_tp_getset, dup(MGLContext_getset)},
		{0},
	};

	PyType_Spec MGLContext_spec = {"moderngl.mgl.Context", sizeof(MGLContext), 0, Py_TPFLAGS_DEFAULT, MGLContext_slots};
	return (PyTypeObject *)PyType_FromSpec(&MGLContext_spec);
}
