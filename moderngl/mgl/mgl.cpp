#include "mgl.hpp"
#include "extensions.hpp"
#include "internal/modules.hpp"
#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"
#include "generated/methods.hpp"

/* moderngl.core.initialize()
 * Initializes internal objects that cannot be initialized import time.
 */
PyObject * meth_initialize(PyObject * self) {
    static bool initialized = false;
    if (initialized) {
        Py_RETURN_NONE;
    }

    initialized = true;

    if (!load_modules()) {
        return 0;
    }

    /* Define MGLContext only.
     * The rest of the internal types will be defined by MGLContext.
     */

    MGLContext_class = (PyTypeObject *)PyType_FromSpec(&MGLContext_spec);

    /* Detect wrapper classes for internal types */

    init_wrappers();

    /* Errors are not recoverable at this point */

    if (PyErr_Occurred()) {
        return 0;
    }

    Py_RETURN_NONE;
}

/* Module definition */

PyModuleDef mgl_def = {PyModuleDef_HEAD_INIT, mgl_name, 0, -1, module_methods, 0, 0, 0, 0};

extern "C" PyObject * PyInit_mgl() {
    PyObject * module = PyModule_Create(&mgl_def);
    return module;
}
