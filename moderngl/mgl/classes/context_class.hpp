#pragma once
#include "../mgl.hpp"
#include "../context.hpp"

extern PyTypeObject * MGLContext_class;
extern PyTypeObject * Context_class;
extern int Context_class_mglo;

void MGLContext_init_wrapper();

struct MGLObject {
	PyObject_HEAD
	PyObject * wrapper;
	MGLContext * context;
};

inline MGLObject * _MGLContext_new_object(MGLContext * self, PyTypeObject * type, PyTypeObject * cls, int slot) {
	MGLObject * res = new_object(MGLObject, type);
	res->wrapper = new_object(PyObject, cls);
	SLOT(res->wrapper, MGLObject, slot) = res;
	res->context = self;
	Py_INCREF(self);
	return res;
}

#define MGLContext_new_object(self, name) (MGL ## name *)_MGLContext_new_object(self, self->MGL ## name ## _type, name ## _class, name ## _class_mglo)
