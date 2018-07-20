#include "buffer.hpp"

/* MGLContext.buffer(...)
 * Returns a Buffer object.
 */
PyObject * MGLContext_meth_buffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
	ENSURE_CONTEXT(self, 0);
    Py_RETURN_NONE;
}

/* Backward compatible methods */

// BC4(MGLBuffer, write);

/* Definition of MGLBuffer internal type */

void MGLBuffer_define(MGLContext * ctx) {
	PyType_Slot MGLBuffer_slots[] = {
		{0},
	};

	PyType_Spec MGLBuffer_spec = {mgl_name ".Buffer", sizeof(MGLBuffer), 0, Py_TPFLAGS_DEFAULT, MGLBuffer_slots};
	ctx->MGLBuffer_class = (PyTypeObject *)PyType_FromSpec(&MGLBuffer_spec);
}
