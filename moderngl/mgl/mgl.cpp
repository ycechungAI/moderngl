#include "mgl.hpp"
#include "internal/modules.hpp"
#include "classes/all.hpp"

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

    MGLContext_define();

    /* Detect wrapper classes for internal types */

    MGLContext_init_wrapper();
    MGLBuffer_init_wrapper();

    /* Errors are not recoverable at this point */

    if (PyErr_Occurred()) {
        return 0;
    }

    Py_RETURN_NONE;
}

/* Backward compatible methods */

BC2(meth_create_context);

/* Module methods */

PyMethodDef mgl_methods[] = {
    DEF2(create_context),
    DEF1(initialize),
    {0},
};

/* Module definition */

PyModuleDef mgl_def = {PyModuleDef_HEAD_INIT, mgl_name, 0, -1, mgl_methods, 0, 0, 0, 0};

extern "C" PyObject * PyInit_mgl() {
    PyObject * module = PyModule_Create(&mgl_def);
    return module;
}
