#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

#define new_ref(obj) (Py_INCREF(obj), obj)

typedef PyObject * (* fastcallmethod)(PyObject * self, PyObject * const * args, Py_ssize_t nargs);
typedef void (* read_value)(void *& ptr, PyObject * value);

void read_float(float *& ptr, PyObject * value);
void read_int(int *& ptr, PyObject * value);
void read_unsigned(unsigned *& ptr, PyObject * value);
void read_double(double *& ptr, PyObject * value);
void read_bool(int *& ptr, PyObject * value);

void set_slot(PyObject * obj, const char * name, PyObject * value);
PyObject * get_slot(PyObject * obj, const char * name);

#if 0 && PY_VERSION_HEX >= 0x03070000
#define fastcallable(func)
#define fastcall(func) (PyCFunction)func
#define fastcall_flags METH_FASTCALL
#else
#define fastcallable(func) PyObject * func ## _va(PyObject * self, PyObject * args) { return ((fastcallmethod)func)(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size); }
#define fastcall(func) (PyCFunction)(func ## _va)
#define fastcall_flags METH_VARARGS
#endif

#define ensure_num_args(count) if (nargs != count) { PyErr_BadInternalCall(); return NULL; }

struct ErrorResult {
    operator int() {
        return -1;
    }
    operator PyObject * () {
        return NULL;
    }
};

#define ensure_no_error() if (PyErr_Occurred()) { return ErrorResult(); }
#define return_error(...) PyErr_Fromat(moderngl_error, __VA_ARGS__); return ErrorResult();

static PyObject * _must_have(PyObject * obj, const char * filename, int line) {
    if (!obj) {
        printf("missing object %s:%d\n", filename, line);
        exit(0);
    }
    return obj;
}

#define must_have(obj) _must_have(obj, __FILE__, __LINE__)
