#include "texture_cube.hpp"

#include "context.hpp"

PyObject * MGLContext_meth_texture_cube(MGLContext * self, PyObject * args) { TRACE_VARAGS
	MGLTextureCube * texture_cube = MGLContext_new_object(self, TextureCube);
	return NEW_REF(texture_cube->wrapper);
}

PyTypeObject * MGLTextureCube_define(int version_code) {
	PyMethodDef MGLTextureCube_methods[] = {
		{0},
	};

	PyType_Slot MGLTextureCube_slots[] = {
		{Py_tp_methods, dup(MGLTextureCube_methods)},
		{0},
	};

	PyType_Spec MGLTextureCube_spec = {"moderngl.mgl.TextureCube", sizeof(MGLTextureCube), 0, Py_TPFLAGS_DEFAULT, MGLTextureCube_slots};
	return (PyTypeObject *)PyType_FromSpec(&MGLTextureCube_spec);
}
