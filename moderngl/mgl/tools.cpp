#include "tools.hpp"

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

int prepare_buffer(PyObject * data, Py_buffer * view) {
	if (data->ob_type->tp_as_buffer && data->ob_type->tp_as_buffer->bf_getbuffer) {
		if (data->ob_type->tp_as_buffer->bf_getbuffer(data, view, PyBUF_STRIDED_RO) < 0) {
			return -1;
		}
	} else {
		PyObject * bytes = PyObject_CallMethodObjArgs(data, tobytes_str, 0);
		if (!bytes) {
			return -1;
		}
		if (bytes->ob_type != &PyBytes_Type) {
			PyErr_Format(module_error, "tobytes returned %s not bytes", bytes->ob_type->tp_name);
			Py_DECREF(bytes);
			return -1;
		}
		view->buf = PyBytes_AS_STRING(bytes);
		view->len = PyBytes_GET_SIZE(bytes);
		view->obj = bytes;
		view->readonly = true;
	}
	return 0;
}
