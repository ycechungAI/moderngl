#include "context.hpp"

#include "classes/all.hpp"

/* MGLContext.create_context(...)
 * Returns a Context object.
 */
PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs) {
    Py_INCREF(MGLContext_class);
    MGLContext * context = PyObject_New(MGLContext, MGLContext_class);

    MGLBuffer_define(context);

    context->wrapper = new_object(PyObject, Context_class);
    SLOT(context->wrapper, MGLContext, Context_class_mglo) = context;
    return NEW_REF(context->wrapper);
}

/* Backward compatible methods */

BC4(MGLContext, buffer);

/* Definition of MGLContext internal type */

void MGLContext_define() {
	PyMethodDef MGLContext_methods[] = {
		DEF4(MGLContext, buffer),
		{0},
	};

	PyType_Slot MGLContext_slots[] = {
		{Py_tp_methods, dup(MGLContext_methods)},
		{0},
	};

	PyType_Spec MGLContext_spec = {mgl_name ".Context", sizeof(MGLContext), 0, Py_TPFLAGS_DEFAULT, MGLContext_slots};
	MGLContext_class = (PyTypeObject *)PyType_FromSpec(&MGLContext_spec);
}
