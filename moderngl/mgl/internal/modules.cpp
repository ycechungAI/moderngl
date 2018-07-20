#include "modules.hpp"

PyObject * moderngl;
PyObject * moderngl_error;

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

    if (PyErr_Occurred()) {
        return false;
    }

    return true;
}
