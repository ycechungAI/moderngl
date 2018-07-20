#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

/* Defining methods
 * DEF1(...) is a method with no arguments
 * DEF2(...) is a method with positional arguments (fastcall in python 3.7)
 * DEF3(...) is a method of a class with no arguments
 * DEF4(...) is a method of a class with positional arguments (fastcall in python 3.7)
 */

#if PY_VERSION_HEX >= 0x03070000
#define DEF1(meth) { "" # meth, (PyCFunction)meth_ ## meth, METH_NOARGS, 0}
#define DEF2(meth) { "" # meth, (PyCFunction)meth_ ## meth, METH_FASTCALL, 0}
#define DEF3(type, meth) { "" # meth, (PyCFunction)type ## _meth_ ## meth, METH_NOARGS, 0}
#define DEF4(type, meth) { "" # meth, (PyCFunction)type ## _meth_ ## meth, METH_FASTCALL, 0}
#else
#define DEF1(meth) { "" # meth, (PyCFunction)meth_ ## meth, METH_NOARGS, 0}
#define DEF2(meth) { "" # meth, (PyCFunction)meth_ ## meth ## _slow, METH_VARARGS, 0}
#define DEF3(type, meth) { "" # meth, (PyCFunction)type ## _meth_ ## meth, METH_NOARGS, 0}
#define DEF4(type, meth) { "" # meth, (PyCFunction)type ## _meth_ ## meth ## _slow, METH_VARARGS, 0}
#endif

/* Methods defined with DEF2(...) and DEF4(...) require backward compatibility.
 * All the methods must be defined using the fastcall calling convention.
 * A method with the regular calling convention can be created using BC2(...) and BC4(...) macros.
 */

#define BC2(meth) PyObject * meth ## _slow(PyObject * self, PyObject * args) { return meth(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size); }
#define BC4(type, meth) PyObject * type ## _meth_ ## meth ## _slow(PyObject * self, PyObject * args) { return type ## _meth_ ## meth((type *)self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size); }

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
    // PyObject_GC_Track(wrapper);
    memset((char *)res + sizeof(PyObject), 0, type->tp_basicsize - sizeof(PyObject));
    return res;
}

#define new_object(type, typeobj) (type *)_new_object(typeobj)
