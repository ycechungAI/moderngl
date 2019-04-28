#include "configuration.hpp"
#include "context.hpp"

/* MGLContext.configure(configuration)
 */
PyObject * MGLContext_meth_configure(MGLContext * self, PyObject * configuration) {
    // if (PyObject * formatter = PyDict_GetItemString(configuration, "glsl_compiler_error")) {
    //     replace_object(self->glsl_compiler_error, formatter);
    // }

    // if (PyObject * formatter = PyDict_GetItemString(configuration, "glsl_linker_error")) {
    //     replace_object(self->glsl_linker_error, formatter);
    // }

    Py_RETURN_NONE;
}
