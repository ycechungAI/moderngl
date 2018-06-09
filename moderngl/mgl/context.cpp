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

	Py_RETURN_NONE;
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
