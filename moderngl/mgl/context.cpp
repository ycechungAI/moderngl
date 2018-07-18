#include "context.hpp"

#include "classes/all.hpp"

PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs) {
    Py_INCREF(MGLContext_class);
    MGLContext * context = PyObject_New(MGLContext, MGLContext_class);

    MGLBuffer_define(context);

    context->wrapper = new_object(PyObject, Context_class);
    SLOT(context->wrapper, MGLContext, Context_class_mglo) = context;
    return NEW_REF(context->wrapper);
}
