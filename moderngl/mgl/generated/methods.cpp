#include "methods.hpp"

extern PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * meth_extensions(PyObject * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * meth_hwinfo(PyObject * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * meth_initialize(PyObject * self);
extern PyObject * meth_glprocs(PyObject * self, PyObject * const * args, Py_ssize_t nargs);
extern PyObject * meth_release(PyObject * self, PyObject * const * args, Py_ssize_t nargs);

#if PY_VERSION_HEX >= 0x03070000

PyMethodDef module_methods[] = {
    {"create_context", (PyCFunction)meth_create_context, METH_FASTCALL, 0},
    {"extensions", (PyCFunction)meth_extensions, METH_FASTCALL, 0},
    {"hwinfo", (PyCFunction)meth_hwinfo, METH_FASTCALL, 0},
    {"initialize", (PyCFunction)meth_initialize, METH_NOARGS, 0},
    {"glprocs", (PyCFunction)meth_glprocs, METH_FASTCALL, 0},
    {"release", (PyCFunction)meth_release, METH_FASTCALL, 0},
    {0},
};

#else

PyObject * meth_create_context_va(PyObject * self, PyObject * args) {
    return meth_create_context(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * meth_extensions_va(PyObject * self, PyObject * args) {
    return meth_extensions(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * meth_hwinfo_va(PyObject * self, PyObject * args) {
    return meth_hwinfo(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * meth_glprocs_va(PyObject * self, PyObject * args) {
    return meth_glprocs(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * meth_release_va(PyObject * self, PyObject * args) {
    return meth_release(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyMethodDef module_methods[] = {
    {"create_context", (PyCFunction)meth_create_context_va, METH_VARARGS, 0},
    {"extensions", (PyCFunction)meth_extensions_va, METH_VARARGS, 0},
    {"hwinfo", (PyCFunction)meth_hwinfo_va, METH_VARARGS, 0},
    {"initialize", (PyCFunction)meth_initialize, METH_NOARGS, 0},
    {"glprocs", (PyCFunction)meth_glprocs_va, METH_VARARGS, 0},
    {"release", (PyCFunction)meth_release_va, METH_VARARGS, 0},
    {0},
};

#endif
