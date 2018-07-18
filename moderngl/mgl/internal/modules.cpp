#include "modules.hpp"

PyObject * moderngl;

bool load_modules() {
    moderngl = PyImport_ImportModule("moderngl");
    if (!moderngl) {
        return false;
    }

    return true;
}
