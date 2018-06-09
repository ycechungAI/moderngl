#pragma once
#include "mgl.hpp"
#include "types.hpp"
#include "tools.hpp"

PyObject * meth_create_context(PyObject * self, PyObject * args);

PyTypeObject * MGLContext_define();

inline MGLObject * _MGLContext_new_object(MGLContext * self, PyTypeObject * type, PyTypeObject * cls, int slot) {
	MGLObject * res = new_object(MGLObject, type);
	res->wrapper = new_object(PyObject, cls);
	SLOT(res->wrapper, MGLObject, slot) = res;
	res->context = self;
	Py_INCREF(self);
	return res;
}

#define MGLContext_new_object(self, name) (MGL ## name *)_MGLContext_new_object(self, self->MGL ## name ## _type, name ## _class, name ## _class_mglo)
