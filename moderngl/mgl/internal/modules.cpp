#include "modules.hpp"

PyObject * moderngl;

/* Loading external modules.
 * This function must not be called on import.
 * Circular imports are not supported in this case.
 */
bool load_modules() {
    moderngl = PyImport_ImportModule("moderngl");
    if (!moderngl) {
        return false;
    }

    return true;
}
