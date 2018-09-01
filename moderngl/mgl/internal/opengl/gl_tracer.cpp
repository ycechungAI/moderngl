#include "gl_tracer.hpp"
#include "../python.hpp"
#include "../modules.hpp"

PyObject * tracer = 0;

void trace_gl_method(int error, const char * format, ...) {
    char function[256];

    va_list vl;
    va_start(vl, format);
    vsnprintf(function, 256 - 1, format, vl);
    va_end(vl);

    if (!tracer) {
        PyObject * moderngl_tracing = PyImport_ImportModule("moderngl.tracing");
        tracer = PyObject_GetAttrString(moderngl_tracing, "Tracer");
    }

    PyObject * callback = PyObject_GetAttrString(tracer, "callback");
    if (callback == Py_None) {
        return;
    }

    PyObject * py_function = PyUnicode_FromString(function);
    PyObject * py_error = PyLong_FromLong(error);
    PyObject_CallFunctionObjArgs(callback, py_function, py_error, (void *)0);
    Py_DECREF(py_function);
    Py_DECREF(py_error);
    Py_DECREF(callback);
}
