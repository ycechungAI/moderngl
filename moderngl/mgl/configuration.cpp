#include "configuration.hpp"
#include "context.hpp"

#include "internal/modules.hpp"

/* MGLContext.configure(configuration)
 */
PyObject * MGLContext_meth_configure(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 1) {
        // TODO: error
        return 0;
    }

    PyObject * configuration = args[0];

    if (PyObject * formatter = PyDict_GetItemString(configuration, "glsl_compiler_error")) {
        replace_object(self->glsl_compiler_error, formatter);
    }

    if (PyObject * formatter = PyDict_GetItemString(configuration, "glsl_linker_error")) {
        replace_object(self->glsl_linker_error, formatter);
    }

    if (PyObject * gc = PyDict_GetItemString(configuration, "gc")) {
        replace_object(self->gc, gc);
    }

    Py_RETURN_NONE;
}
