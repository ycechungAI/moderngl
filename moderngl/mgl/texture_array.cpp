#include "texture_array.hpp"

#include "context.hpp"

PyObject * MGLContext_meth_texture_array(MGLContext * self, PyObject * args) { TRACE_VARAGS
	MGLTextureArray * texture_array = MGLContext_new_object(self, TextureArray);
	return NEW_REF(texture_array->wrapper);
}

PyTypeObject * MGLTextureArray_define(int version_code) {
	PyMethodDef MGLTextureArray_methods[] = {
		{0},
	};

	PyType_Slot MGLTextureArray_slots[] = {
		{Py_tp_methods, dup(MGLTextureArray_methods)},
		{0},
	};

	PyType_Spec MGLTextureArray_spec = {"moderngl.mgl.TextureArray", sizeof(MGLTextureArray), 0, Py_TPFLAGS_DEFAULT, MGLTextureArray_slots};
	return (PyTypeObject *)PyType_FromSpec(&MGLTextureArray_spec);
}
