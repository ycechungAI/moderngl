#include "gl_tracer.hpp"
#include "../python.hpp"
#include "../modules.hpp"

PyObject * tracer = 0;

void trace_gl_method(const char * name, int error) {
    if (!tracer) {
        PyObject * moderngl_tracing = PyImport_ImportModule("moderngl.tracing");
        tracer = PyObject_GetAttrString(moderngl_tracing, "Tracer");
    }

    PyObject * callback = PyObject_GetAttrString(tracer, "callback");
    if (callback == Py_None) {
        return;
    }

    PyObject * py_name = PyUnicode_FromString(name);
    PyObject * py_error = PyLong_FromLong(error);
    PyObject_CallFunctionObjArgs(callback, py_name, py_error, (void *)0);
    Py_DECREF(py_name);
    Py_DECREF(py_error);
    Py_DECREF(callback);
}
