#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

#ifdef MGL_DEBUG
#define PyInit_mgl PyInit_mgl_dbg
#define SLOT(obj, type, offset) (*(type **)((char *)obj + offset))
#define TRACE_VARAGS _trace_call(__FUNCTION__, __FILE__, __LINE__, "meth", self, args);
#define TRACE_NOARGS _trace_call(__FUNCTION__, __FILE__, __LINE__, "meth", self, 0);
#define TRACE_GETTER _trace_call(__FUNCTION__, __FILE__, __LINE__, "get", self, 0);
#define TRACE_SETTER _trace_call(__FUNCTION__, __FILE__, __LINE__, "set", self, value);
#else
#define SLOT(obj, type, offset) (*(type **)((char *)obj + offset))
#define TRACE_VARAGS
#define TRACE_NOARGS
#define TRACE_GETTER
#define TRACE_SETTER
#endif

extern PyObject * module_error;

extern PyObject * tobytes_str;

#ifdef MGL_DEBUG
extern PyObject * moderngl_debugger;
#endif
extern PyObject * moderngl;
extern PyObject * numpy;
extern PyObject * pillow;

extern PyObject * numpy_frombuffer;

extern PyTypeObject * Context_class;
extern int Context_class_mglo;
extern int Context_class_framebuffers;
extern int Context_class_limits;
extern int Context_class_screen;
extern int Context_class_fbo;
extern int Context_class_extra;

extern PyTypeObject * Buffer_class;
extern int Buffer_class_mglo;
extern int Buffer_class_size;
extern int Buffer_class_extra;

extern PyTypeObject * MGLContext_type;

#ifdef MGL_DEBUG
template <typename T>
static PyObject * _wrapper(T * obj) {
	return obj->wrapper;
}

template <>
static PyObject * _wrapper<PyObject>(PyObject * obj) {
	return Py_None;
}

template <typename T>
static void _trace_call(const char * func, const char * file, int line, const char * proc, T * self, PyObject * args) {
	if (!moderngl_debugger) {
		return;
	}

	PyObject * trace = PyObject_GetAttrString(moderngl_debugger, "_trace");
	if (!trace) {
		return;
	}

	PyObject * wrapper = self ? _wrapper(self) : Py_None;
	PyObject * mglo = self ? (PyObject *)self : Py_None;
	if (args) {
		Py_INCREF(args);
	} else {
		args = PyTuple_New(0);
	}
	Py_INCREF(wrapper);
	Py_INCREF(mglo);

	PyObject * trace_args = PyTuple_New(7);
	PyTuple_SET_ITEM(trace_args, 0, PyUnicode_FromString(func));
	PyTuple_SET_ITEM(trace_args, 1, PyUnicode_FromString(file));
	PyTuple_SET_ITEM(trace_args, 2, PyLong_FromLong(line));
	PyTuple_SET_ITEM(trace_args, 3, PyUnicode_FromString(proc));
	PyTuple_SET_ITEM(trace_args, 4, wrapper);
	PyTuple_SET_ITEM(trace_args, 5, mglo);
	PyTuple_SET_ITEM(trace_args, 6, args);
	PyObject_CallObject(trace, trace_args);
	Py_DECREF(trace_args);
	Py_DECREF(trace);
	PyErr_Clear();
}
#endif

void initialize_module();
