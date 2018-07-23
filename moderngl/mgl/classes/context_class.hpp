#pragma once
#include "../mgl.hpp"
#include "../context.hpp"

extern PyTypeObject * MGLContext_class;
extern PyTypeObject * Context_class;
extern int Context_class_mglo;
extern int Context_class_version_code;
extern int Context_class_limits;
extern int Context_class_extra;

void MGLContext_init_wrapper();

/* Every objects derived from a Context must have the following HEAD */

struct MGLObject {
	PyObject_HEAD
	PyObject * wrapper;
	MGLContext * context;
};

/* Every internal object has its own wrapper class defined in the python layer.
 * An internal object holds a ref to the instance of its wrapper class.
 * The wrapper class holds a ref to the internal object.
 * Such a pair can be created using MGLContext_new_object.
 * The circular reference must be resolved when releasing objects.
 */

inline MGLObject * _MGLContext_new_object(MGLContext * self, PyTypeObject * type, PyTypeObject * cls, int slot) {
	MGLObject * res = new_object(MGLObject, type);
	res->wrapper = new_object(PyObject, cls);
	SLOT(res->wrapper, MGLObject, slot) = res;
	res->context = self;
	Py_INCREF(self);
	return res;
}

#define MGLContext_new_object(self, name) (MGL ## name *)_MGLContext_new_object(self, self->MGL ## name ## _type, name ## _class, name ## _class_mglo)
