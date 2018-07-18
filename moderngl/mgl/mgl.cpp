#include "mgl.hpp"

#include "internal/modules.hpp"
#include "classes/all.hpp"

PyObject * meth_initialize(PyObject * self) {
    static bool initialized = false;
    if (initialized) {
        Py_RETURN_NONE;
    }

    initialized = true;

    if (!load_modules()) {
        return 0;
    }

    MGLContext_define();

    MGLContext_init_wrapper();
    MGLBuffer_init_wrapper();

    if (PyErr_Occurred()) {
        return 0;
    }

    Py_RETURN_NONE;
}

BC2(meth_create_context);

PyMethodDef mgl_methods[] = {
    DEF2(create_context),
    DEF1(initialize),
    {0},
};

PyModuleDef mgl_def = {PyModuleDef_HEAD_INIT, mgl_name, 0, -1, mgl_methods, 0, 0, 0, 0};

extern "C" PyObject * PyInit_mgl() {
    PyObject * module = PyModule_Create(&mgl_def);
    return module;
}
