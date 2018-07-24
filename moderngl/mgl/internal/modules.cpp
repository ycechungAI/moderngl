#include "modules.hpp"

PyObject * moderngl;
PyObject * moderngl_error;
PyObject * moderngl_compiler_error;
PyObject * moderngl_linker_error;

/* Loading external modules.
 * This function must not be called on import.
 * Circular imports are not supported in this case.
 */
bool load_modules() {
    moderngl = PyImport_ImportModule("moderngl");
    if (!moderngl) {
        return false;
    }

    moderngl_error = PyObject_GetAttrString(moderngl, "Error");

    PyObject * moderngl_compiler = PyImport_ImportModule("moderngl.compiler");
    if (!moderngl_compiler) {
        return false;
    }

    moderngl_compiler_error = PyObject_GetAttrString(moderngl_compiler, "compiler_error");
    moderngl_linker_error = PyObject_GetAttrString(moderngl_compiler, "linker_error");

    if (PyErr_Occurred()) {
        return false;
    }

    return true;
}
