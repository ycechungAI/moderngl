#include "tools.hpp"

#include "opengl.hpp"

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

void clean_glsl_name(char * name, int & name_len) {
	if (name_len && name[name_len - 1] == ']') {
		name_len -= 1;
		while (name_len && name[name_len] != '[') {
			name_len -= 1;
		}
	}
	name[name_len] = 0;
}

bool unpack_viewport(PyObject * viewport, int & x, int & y, int & width, int & height) {
	if (viewport == Py_None) {
		return true;
	}

	viewport = PySequence_Fast(viewport, "viewport is not iterable");
	if (!viewport) {
		return false;
	}

	int size = (int)PySequence_Fast_GET_SIZE(viewport);

	if (size == 4) {
		x = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 0));
		y = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 1));
		width = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 2));
		height = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 3));
	} else if (size == 2) {
		width = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 0));
		height = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 1));
	} else {
		PyErr_Format(module_error, "viewport size must be 2 or 4");
		Py_DECREF(viewport);
		return false;
	}

	Py_DECREF(viewport);
	if (PyErr_Occurred()) {
		return false;
	}

	return true;
}

GLTypeInfo type_info(int type) {
	GLTypeInfo info = {};

	int & cols = info.cols;
	int & rows = info.rows;
	int & shape = info.shape;

	switch (type) {
		case GL_FLOAT: cols = 1; rows = 1; shape = 'f'; break;
		case GL_FLOAT_VEC2: cols = 2; rows = 1; shape = 'f'; break;
		case GL_FLOAT_VEC3: cols = 3; rows = 1; shape = 'f'; break;
		case GL_FLOAT_VEC4: cols = 4; rows = 1; shape = 'f'; break;
		case GL_DOUBLE: cols = 1; rows = 1; shape = 'd'; break;
		case GL_DOUBLE_VEC2: cols = 2; rows = 1; shape = 'd'; break;
		case GL_DOUBLE_VEC3: cols = 3; rows = 1; shape = 'd'; break;
		case GL_DOUBLE_VEC4: cols = 4; rows = 1; shape = 'd'; break;
		case GL_INT: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_VEC2: cols = 2; rows = 1; shape = 'i'; break;
		case GL_INT_VEC3: cols = 3; rows = 1; shape = 'i'; break;
		case GL_INT_VEC4: cols = 4; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT: cols = 1; rows = 1; shape = 'u'; break;
		case GL_UNSIGNED_INT_VEC2: cols = 2; rows = 1; shape = 'u'; break;
		case GL_UNSIGNED_INT_VEC3: cols = 3; rows = 1; shape = 'u'; break;
		case GL_UNSIGNED_INT_VEC4: cols = 4; rows = 1; shape = 'u'; break;
		case GL_BOOL: cols = 1; rows = 1; shape = 'p'; break;
		case GL_BOOL_VEC2: cols = 2; rows = 1; shape = 'p'; break;
		case GL_BOOL_VEC3: cols = 3; rows = 1; shape = 'p'; break;
		case GL_BOOL_VEC4: cols = 4; rows = 1; shape = 'p'; break;
		case GL_FLOAT_MAT2: cols = 2; rows = 2; shape = 'f'; break;
		case GL_FLOAT_MAT3: cols = 3; rows = 3; shape = 'f'; break;
		case GL_FLOAT_MAT4: cols = 4; rows = 4; shape = 'f'; break;
		case GL_FLOAT_MAT2x3: cols = 3; rows = 2; shape = 'f'; break;
		case GL_FLOAT_MAT2x4: cols = 4; rows = 2; shape = 'f'; break;
		case GL_FLOAT_MAT3x2: cols = 2; rows = 3; shape = 'f'; break;
		case GL_FLOAT_MAT3x4: cols = 4; rows = 3; shape = 'f'; break;
		case GL_FLOAT_MAT4x2: cols = 2; rows = 4; shape = 'f'; break;
		case GL_FLOAT_MAT4x3: cols = 3; rows = 4; shape = 'f'; break;
		case GL_DOUBLE_MAT2: cols = 2; rows = 2; shape = 'd'; break;
		case GL_DOUBLE_MAT3: cols = 3; rows = 3; shape = 'd'; break;
		case GL_DOUBLE_MAT4: cols = 4; rows = 4; shape = 'd'; break;
		case GL_DOUBLE_MAT2x3: cols = 3; rows = 2; shape = 'd'; break;
		case GL_DOUBLE_MAT2x4: cols = 4; rows = 2; shape = 'd'; break;
		case GL_DOUBLE_MAT3x2: cols = 2; rows = 3; shape = 'd'; break;
		case GL_DOUBLE_MAT3x4: cols = 4; rows = 3; shape = 'd'; break;
		case GL_DOUBLE_MAT4x2: cols = 2; rows = 4; shape = 'd'; break;
		case GL_DOUBLE_MAT4x3: cols = 3; rows = 4; shape = 'd'; break;
		case GL_SAMPLER_1D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_2D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_3D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_CUBE: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_1D_SHADOW: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_2D_SHADOW: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_1D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_2D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_1D_ARRAY_SHADOW: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_2D_ARRAY_SHADOW: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_2D_MULTISAMPLE: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_CUBE_SHADOW: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_BUFFER: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_2D_RECT: cols = 1; rows = 1; shape = 'i'; break;
		case GL_SAMPLER_2D_RECT_SHADOW: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_SAMPLER_1D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_SAMPLER_2D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_SAMPLER_3D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_SAMPLER_CUBE: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_SAMPLER_1D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_SAMPLER_2D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_SAMPLER_2D_MULTISAMPLE: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_SAMPLER_BUFFER: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_SAMPLER_2D_RECT: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_SAMPLER_1D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_SAMPLER_2D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_SAMPLER_3D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_SAMPLER_CUBE: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_SAMPLER_BUFFER: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_SAMPLER_2D_RECT: cols = 1; rows = 1; shape = 'i'; break;
		case GL_IMAGE_1D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_IMAGE_2D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_IMAGE_3D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_IMAGE_2D_RECT: cols = 1; rows = 1; shape = 'i'; break;
		case GL_IMAGE_CUBE: cols = 1; rows = 1; shape = 'i'; break;
		case GL_IMAGE_BUFFER: cols = 1; rows = 1; shape = 'i'; break;
		case GL_IMAGE_1D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_IMAGE_2D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_IMAGE_2D_MULTISAMPLE: cols = 1; rows = 1; shape = 'i'; break;
		case GL_IMAGE_2D_MULTISAMPLE_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_IMAGE_1D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_IMAGE_2D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_IMAGE_3D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_IMAGE_2D_RECT: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_IMAGE_CUBE: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_IMAGE_BUFFER: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_IMAGE_1D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_IMAGE_2D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_IMAGE_2D_MULTISAMPLE: cols = 1; rows = 1; shape = 'i'; break;
		case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_IMAGE_1D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_IMAGE_2D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_IMAGE_3D: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_IMAGE_2D_RECT: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_IMAGE_CUBE: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_IMAGE_BUFFER: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_IMAGE_1D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_IMAGE_2D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
		case GL_UNSIGNED_INT_ATOMIC_COUNTER: cols = 1; rows = 1; shape = 'i'; break;
	}

	return info;
}

void enable_only(const GLMethods & gl, int flags) {
	if (flags < 0) {
		return;
	}

	if (flags & MGL_BLEND) {
		gl.Enable(GL_BLEND);
	} else {
		gl.Disable(GL_BLEND);
	}

	if (flags & MGL_DEPTH_TEST) {
		gl.Enable(GL_DEPTH_TEST);
	} else {
		gl.Disable(GL_DEPTH_TEST);
	}

	if (flags & MGL_CULL_FACE) {
		gl.Enable(GL_CULL_FACE);
	} else {
		gl.Disable(GL_CULL_FACE);
	}

	if (flags & MGL_RASTERIZER_DISCARD) {
		gl.Enable(GL_RASTERIZER_DISCARD);
	} else {
		gl.Disable(GL_RASTERIZER_DISCARD);
	}
}
