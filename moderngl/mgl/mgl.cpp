#include "mgl.hpp"

PyObject * module_error;

#ifdef MGL_DEBUG
PyObject * moderngl_debugger;
#endif
PyObject * moderngl;
PyObject * numpy;
PyObject * pillow;

PyTypeObject * Context_class;
int Context_class_mglo;
int Context_class_framebuffers;
int Context_class_limits;
int Context_class_screen;
int Context_class_fbo;
int Context_class_extra;

PyTypeObject * MGLContext_type;

int slot_offset(PyTypeObject * type, const char * name, int & counter) {
	if (!type || PyErr_Occurred()) {
		return 0;
	}
	counter -= 1;
	for (int i = 0; type->tp_members[i].name; ++i) {
		if (!strcmp(type->tp_members[i].name, name)) {
			return type->tp_members[i].offset;
		}
	}

	PyErr_Format(PyExc_Exception, "missing slot %s in %s", name, type->tp_name);
	return 0;
}

PyTypeObject * detect_class(PyObject * module, const char * name, int & slots_len) {
	if (!module || PyErr_Occurred()) {
		return 0;
	}

	PyObject * cls = PyObject_GetAttrString(module, name);
	if (!cls) {
		return 0;
	}

	PyObject * slots = PyObject_GetAttrString(cls, "__slots__");
	if (!slots) {
		return 0;
	}

	slots_len = (int)PyObject_Size(slots);
	return (PyTypeObject *)cls;
}

void assert_slots_len(PyTypeObject * type, int slots_len) {
	if (!slots_len || PyErr_Occurred()) {
		return;
	}

	PyErr_Format(PyExc_Exception, "remaining slots in %s", type->tp_name);
}

void protect_slot(PyTypeObject * type, const char * name) {
	if (!type || PyErr_Occurred()) {
		return;
	}
	for (int i = 0; type->tp_members[i].name; ++i) {
		if (!strcmp(type->tp_members[i].name, name)) {
			type->tp_members[i].flags |= READONLY;
			return;
		}
	}
}

void remove_init(PyTypeObject * type) {
	if (!type || PyErr_Occurred()) {
		return;
	}
	type->tp_init = 0;
	type->tp_new = 0;
}

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

	Py_RETURN_NONE;
}

PyMethodDef mgl_methods[] = {
	{"create_context", (PyCFunction)meth_create_context, METH_VARARGS, 0},
	{0},
};

PyModuleDef mgl_def = {PyModuleDef_HEAD_INIT, "moderngl.mgl", 0, -1, mgl_methods, 0, 0, 0, 0};

void initialize_module() {
	static bool initialized = false;
	if (initialized) {
		return;
	}

	moderngl = PyImport_ImportModule("moderngl");
	if (!moderngl) {
		return;
	}

	#ifdef MGL_DEBUG
	moderngl_debugger = PyImport_ImportModule("moderngl_debugger");
	if (!moderngl_debugger) {
		PyErr_Clear();
	}
	#endif

	numpy = PyImport_ImportModule("numpy");
	if (!numpy) {
		PyErr_Clear();
	}

	pillow = PyImport_ImportModule("PIL.Image");
	if (!pillow) {
		PyErr_Clear();
	}

	module_error = PyObject_GetAttrString(moderngl, "Error");

	int Context_slots = 0;
	Context_class = detect_class(moderngl, "Context", Context_slots);
	Context_class_mglo = slot_offset(Context_class, "_Context__mglo", Context_slots);
	Context_class_framebuffers = slot_offset(Context_class, "_Context__framebuffers", Context_slots);
	Context_class_limits = slot_offset(Context_class, "limits", Context_slots);
	Context_class_screen = slot_offset(Context_class, "screen", Context_slots);
	Context_class_fbo = slot_offset(Context_class, "fbo", Context_slots);
	Context_class_extra = slot_offset(Context_class, "extra", Context_slots);
	assert_slots_len(Context_class, Context_slots);
	protect_slot(Context_class, "limits");
	protect_slot(Context_class, "screen");
	protect_slot(Context_class, "fbo");
	remove_init(Context_class);

	initialized = true;
}

extern "C" PyObject * PyInit_mgl() {
	PyObject * module = PyModule_Create(&mgl_def);
	return module;
}
