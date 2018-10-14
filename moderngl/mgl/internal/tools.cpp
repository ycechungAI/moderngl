#include "tools.hpp"
#include "modules.hpp"

/* Takes an int or str and converts it into a Py_ssize_t.
 * Examples: 1000 -> 1000, '32KB' -> 32768, '1MB' -> 1048576.
 */
Py_ssize_t unpack_size(PyObject * size) {
    if (PyLong_Check(size)) {
        return PyLong_AsSsize_t(size);
    }

    if (!PyUnicode_Check(size)) {
        PyErr_Format(PyExc_TypeError, "invalid size");
        return 0;
    }

    const char * str = PyUnicode_AsUTF8(size);

    char first_chr = *str++;
    if (first_chr < '1' || first_chr > '9') {
        PyErr_Format(PyExc_TypeError, "invalid size");
        return 0;
    }

    Py_ssize_t value = first_chr - '0';

    while (char chr = *str++) {
        if (chr < '0' || chr > '9') {
            switch (chr) {
                case 'G':
                    value *= 1024;

                case 'M':
                    value *= 1024;

                case 'K':
                    value *= 1024;
                    if (*str++ != 'B') {
                        return 0;
                    }

                case 'B':
                    if (*str++) {
                        return 0;
                    }

                case 0:
                    break;

                default:
                    return 0;
            }
            break;
        }
        value = value * 10 + chr - '0';
    }
    return value;
}

int prepare_buffer(PyObject * data, Py_buffer * view) {
    if (data->ob_type->tp_as_buffer && data->ob_type->tp_as_buffer->bf_getbuffer) {
        if (data->ob_type->tp_as_buffer->bf_getbuffer(data, view, PyBUF_STRIDED_RO) < 0) {
            return -1;
        }
    } else {
        PyObject * bytes = PyObject_CallMethodObjArgs(data, tobytes_str, 0);
        if (!bytes) {
            return -1;
        }
        if (bytes->ob_type != &PyBytes_Type) {
            PyErr_Format(PyExc_TypeError, "tobytes returned %s not bytes", bytes->ob_type->tp_name);
            Py_DECREF(bytes);
            return -1;
        }
        view->buf = PyBytes_AS_STRING(bytes);
        view->len = PyBytes_GET_SIZE(bytes);
        view->obj = bytes;
        view->readonly = true;
    }
    return 0;
}

bool unpack_viewport(PyObject * viewport, int & x, int & y, int & width, int & height) {
    if (viewport == Py_None) {
        return true;
    }

    viewport = PySequence_Fast(viewport, "viewport is not iterable");
    if (!viewport) {
        return false;
    }

    int size = (int)PySequence_Fast_GET_SIZE(viewport);

    if (size == 4) {
        x = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 0));
        y = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 1));
        width = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 2));
        height = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 3));
    } else if (size == 2) {
        width = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 0));
        height = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 1));
    } else {
        PyErr_Format(moderngl_error, "viewport size must be 2 or 4");
        Py_DECREF(viewport);
        return false;
    }

    Py_DECREF(viewport);
    if (PyErr_Occurred()) {
        return false;
    }

    return true;
}

bool unpack_viewport(PyObject * viewport, int & x, int & y, int & z, int & width, int & height, int & depth) {
    if (viewport == Py_None) {
        return true;
    }

    viewport = PySequence_Fast(viewport, "viewport is not iterable");
    if (!viewport) {
        return false;
    }

    int size = (int)PySequence_Fast_GET_SIZE(viewport);

    if (size == 6) {
        x = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 0));
        y = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 1));
        z = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 2));
        width = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 3));
        height = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 4));
        depth = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 5));
    } else if (size == 3) {
        width = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 0));
        height = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 1));
        depth = PyLong_AsLong(PySequence_Fast_GET_ITEM(viewport, 2));
    } else {
        PyErr_Format(moderngl_error, "viewport size must be 3 or 6");
        Py_DECREF(viewport);
        return false;
    }

    Py_DECREF(viewport);
    if (PyErr_Occurred()) {
        return false;
    }

    return true;
}
