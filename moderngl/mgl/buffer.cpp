#include "buffer.hpp"

PyObject * MGLContext_meth_buffer(PyObject * self, PyObject * const * args, Py_ssize_t nargs) {
    Py_RETURN_NONE;
}

void MGLBuffer_define(MGLContext * ctx) {
	PyType_Slot MGLBuffer_slots[] = {
		{0},
	};

	PyType_Spec MGLBuffer_spec = {mgl_name ".Buffer", sizeof(MGLBuffer), 0, Py_TPFLAGS_DEFAULT, MGLBuffer_slots};
	ctx->MGLBuffer_class = (PyTypeObject *)PyType_FromSpec(&MGLBuffer_spec);
}
