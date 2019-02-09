#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

/* Wrapper classes for internal objects are defined in python. They must have __slots__ defined.
 * A slot can be accessed in O(1) once detect_class(...) and slot_offset(...) is called.
 */

#define SLOT(obj, type, offset) (*(type **)((char *)obj + offset))

/* Shortcuts */

#define NEW_REF(obj) (Py_INCREF(obj), obj)

/* Classes defined in python must be instantiated using new_object(...)
 * The allocated memory is initialized to zero.
 * Slots can be set using the SLOT(...) macro.
 */
inline PyObject * _new_object(PyTypeObject * type) {
    PyObject * res = 0;
    Py_INCREF(type);
    if (type->tp_flags & Py_TPFLAGS_HAVE_GC) {
        res = PyObject_GC_New(PyObject, type);
    } else {
        res = PyObject_New(PyObject, type);
    }
    return res;
}

#define new_object(type, typeobj) (type *)_new_object(typeobj)
#define call_function(function, ...) PyObject_CallFunctionObjArgs(function, __VA_ARGS__, (void *)0)

inline void replace_object(PyObject *& src, PyObject * dst) {
    Py_INCREF(dst);
    Py_DECREF(src);
    src = dst;
}
