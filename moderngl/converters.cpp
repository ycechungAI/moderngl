#include "converters.hpp"
#include "moderngl.hpp"

int parse_texture_size(PyObject * obj, int * size) {
    PyObject * seq = PySequence_Fast(obj, "not iterable");
    if (!seq) {
        return 0;
    }
    int len = (int)PySequence_Fast_GET_SIZE(seq);
    PyObject ** items = PySequence_Fast_ITEMS(seq);
    if (len == 1) {
        size[0] = PyLong_AsLong(items[0]);
    } else if (len == 2) {
        size[0] = PyLong_AsLong(items[0]);
        size[1] = PyLong_AsLong(items[1]);
    } else if (len == 3) {
        size[0] = PyLong_AsLong(items[0]);
        size[1] = PyLong_AsLong(items[1]);
        size[2] = PyLong_AsLong(items[2]);
    } else {
        PyErr_Format(PyExc_ValueError, "invalid size");
        return 0;
    }
    Py_DECREF(seq);
    if (PyErr_Occurred()) {
        return 0;
    }
    return 1;
}

int parse_reserve(PyObject * obj, int * reserve) {
    if (PyLong_Check(obj)) {
        *reserve = PyLong_AsLong(obj);
        return 1;
    }
    if (!PyUnicode_Check(obj)) {
        PyErr_Format(PyExc_ValueError, "invalid reserve");
        return 0;
    }
    int size = 0;
    const char * str = PyUnicode_AsUTF8(obj);
    while (char chr = *str) {
        if (chr >= '0' && chr <= '9') {
            size = size * 10 + chr - '0';
        } else {
            switch (chr) {
                case 'G':
                    size *= 1024;
                case 'M':
                    size *= 1024;
                case 'K':
                    size *= 1024;
                    if (str[1] != 'B' || str[2]) {
                        PyErr_Format(PyExc_ValueError, "invalid reserve");
                        return 0;
                    }
                case 'B':
                    if (str[1]) {
                        PyErr_Format(PyExc_ValueError, "invalid reserve");
                        return 0;
                    }
                default:
                    PyErr_Format(PyExc_ValueError, "invalid reserve");
                    return 0;
            }
        }
    }
    if (size < 1) {
        PyErr_Format(PyExc_ValueError, "invalid reserve");
        return 0;
    }
    *reserve = size;
    return 1;
}

int parse_viewport2d(PyObject * obj, int * viewport) {
    PyObject * seq = PySequence_Fast(obj, "not iterable");
    if (!seq) {
        return 0;
    }
    int len = (int)PySequence_Fast_GET_SIZE(seq);
    PyObject ** items = PySequence_Fast_ITEMS(seq);
    if (len == 2) {
        viewport[0] = 0;
        viewport[1] = 0;
        viewport[2] = PyLong_AsLong(items[0]);
        viewport[3] = PyLong_AsLong(items[1]);
    } else if (len == 4) {
        viewport[0] = PyLong_AsLong(items[0]);
        viewport[1] = PyLong_AsLong(items[1]);
        viewport[2] = PyLong_AsLong(items[2]);
        viewport[3] = PyLong_AsLong(items[3]);
    } else {
        return 0;
    }
    Py_DECREF(seq);
    if (PyErr_Occurred()) {
        return 0;
    }
    return 1;
}

int parse_viewport3d(PyObject * obj, int * viewport) {
    PyObject * seq = PySequence_Fast(obj, "not iterable");
    if (!seq) {
        return 0;
    }
    int len = (int)PySequence_Fast_GET_SIZE(seq);
    PyObject ** items = PySequence_Fast_ITEMS(seq);
    if (len == 3) {
        viewport[0] = 0;
        viewport[1] = 0;
        viewport[2] = 0;
        viewport[3] = PyLong_AsLong(items[0]);
        viewport[4] = PyLong_AsLong(items[1]);
        viewport[5] = PyLong_AsLong(items[2]);
    } else if (len == 6) {
        viewport[0] = PyLong_AsLong(items[0]);
        viewport[1] = PyLong_AsLong(items[1]);
        viewport[2] = PyLong_AsLong(items[2]);
        viewport[3] = PyLong_AsLong(items[3]);
        viewport[4] = PyLong_AsLong(items[4]);
        viewport[5] = PyLong_AsLong(items[5]);
    } else {
        return 0;
    }
    Py_DECREF(seq);
    if (PyErr_Occurred()) {
        return 0;
    }
    return 1;
}

int parse_float_color(PyObject * obj, float * color) {
    PyObject * seq = PySequence_Fast(obj, "not iterable");
    if (!seq) {
        return 0;
    }
    int size = (int)PySequence_Fast_GET_SIZE(seq);
    PyObject ** items = PySequence_Fast_ITEMS(seq);
    if (size < 1 || size > 4) {
        PyErr_Format(PyExc_ValueError, "invalid color");
        return 0;
    }
    for (int i = 0; i < size; ++i) {
        color[i] = (float)PyFloat_AsDouble(items[i]);
    }
    Py_DECREF(seq);
    if (PyErr_Occurred()) {
        return 0;
    }
    return 1;
}

int parse_int_color(PyObject * obj, int * color) {
    PyObject * seq = PySequence_Fast(obj, "not iterable");
    if (!seq) {
        return 0;
    }
    int size = (int)PySequence_Fast_GET_SIZE(seq);
    PyObject ** items = PySequence_Fast_ITEMS(seq);
    if (size < 1 || size > 4) {
        PyErr_Format(PyExc_ValueError, "invalid color");
        return 0;
    }
    for (int i = 0; i < size; ++i) {
        color[i] = PyLong_AsLong(items[i]);
    }
    Py_DECREF(seq);
    if (PyErr_Occurred()) {
        return 0;
    }
    return 1;
}

int parse_uint_color(PyObject * obj, unsigned * color) {
    PyObject * seq = PySequence_Fast(obj, "not iterable");
    if (!seq) {
        return 0;
    }
    int size = (int)PySequence_Fast_GET_SIZE(seq);
    PyObject ** items = PySequence_Fast_ITEMS(seq);
    if (size < 1 || size > 4) {
        PyErr_Format(PyExc_ValueError, "invalid color");
        return 0;
    }
    for (int i = 0; i < size; ++i) {
        color[i] = PyLong_AsUnsignedLong(items[i]);
    }
    Py_DECREF(seq);
    if (PyErr_Occurred()) {
        return 0;
    }
    return 1;
}

int parse_dtype(PyObject * obj, DataType ** dtype) {
    if (!PyUnicode_Check(obj) || PyUnicode_GET_LENGTH(obj) != 2) {
        PyErr_Format(PyExc_ValueError, "invalid dtype");
        return 0;
    }

    short * code = (short *)PyUnicode_AsUTF8(obj);
    switch (*code) {
        case 'f' | '1' << 8: *dtype = &f1; return 1;
        case 'f' | '2' << 8: *dtype = &f2; return 1;
        case 'f' | '4' << 8: *dtype = &f4; return 1;
        case 'u' | '1' << 8: *dtype = &u1; return 1;
        case 'u' | '2' << 8: *dtype = &u2; return 1;
        case 'u' | '4' << 8: *dtype = &u4; return 1;
        case 'i' | '1' << 8: *dtype = &i1; return 1;
        case 'i' | '2' << 8: *dtype = &i2; return 1;
        case 'i' | '4' << 8: *dtype = &i4; return 1;
        case 'd' | '2' << 8: *dtype = &d2; return 1;
        case 'd' | '3' << 8: *dtype = &d3; return 1;
        case 'd' | '4' << 8: *dtype = &d4; return 1;
    }
    return 1;
}

int parse_components(PyObject * obj, int * components) {
    *components = PyLong_AsLong(obj);
    if (PyErr_Occurred()) {
        return 0;
    }
    if (*components < 1 || *components > 4) {
        PyErr_Format(PyExc_ValueError, "invalid components");
        return 0;
    }
    return 1;
}

int parse_samples(PyObject * obj, int * samples) {
    *samples = PyLong_AsLong(obj);
    if (PyErr_Occurred()) {
        return 0;
    }
    if (*samples & (*samples - 1)) {
        PyErr_Format(PyExc_ValueError, "invalid samples");
        return 0;
    }
    return 1;
}

int parse_swizzle(PyObject * obj, int * swizzle) {
    if (!PyUnicode_Check(obj)) {
        PyErr_Format(PyExc_ValueError, "invalid swizzle");
        return 0;
    }

    const char * str = PyUnicode_AsUTF8(obj);
    int size = (int)PyUnicode_GET_SIZE(obj);

    if (size < 1 || size > 4) {
        PyErr_Format(PyExc_ValueError, "invalid swizzle");
        return 0;
    }

    swizzle[0] = GL_ZERO;
    swizzle[1] = GL_ZERO;
    swizzle[2] = GL_ZERO;
    swizzle[3] = GL_ONE;

    for (int i = 0; str[i]; ++i) {
        switch (str[i]) {
            case 'R':
            case 'r':
                swizzle[i] = GL_RED;
                break;

            case 'G':
            case 'g':
                swizzle[i] = GL_GREEN;
                break;

            case 'B':
            case 'b':
                swizzle[i] = GL_BLUE;
                break;

            case 'A':
            case 'a':
                swizzle[i] = GL_ALPHA;
                break;

            case '0':
                swizzle[i] = GL_ZERO;
                break;

            case '1':
                swizzle[i] = GL_ONE;
                break;

            default:
                PyErr_Format(PyExc_ValueError, "invalid swizzle");
                return 0;
        }
    }
    return 1;
}

int parse_filter(PyObject * obj, int * filter) {
    PyObject * seq = PySequence_Fast(obj, "not iterable");
    if (!seq) {
        return 0;
    }

    int size = (int)PySequence_Fast_GET_SIZE(seq);
    if (size != 2) {
        return 0;
    }

    filter[0] = PyLong_AsLong(PySequence_Fast_GET_ITEM(seq, 0));
    filter[1] = PyLong_AsLong(PySequence_Fast_GET_ITEM(seq, 1));

    if (PyErr_Occurred()) {
        return 0;
    }

    return 1;
}

int parse_wrap(PyObject * obj, int * wrap) {
    if (!PyLong_Check(obj)) {
        PyErr_Format(PyExc_ValueError, "invalid wrap");
        return 0;
    }
    int value = PyLong_AsLong(obj);
    for (int i = 0; i < 3; ++i) {
        switch (((unsigned char *)&value)[i]) {
            case 0:
            case MGL_CLAMP_TO_EDGE: wrap[i] = GL_CLAMP_TO_EDGE; break;
            case MGL_REPEAT: wrap[i] = GL_REPEAT; break;
            case MGL_MIRRORED_REPEAT: wrap[i] = GL_MIRRORED_REPEAT; break;
            case MGL_MIRROR_CLAMP_TO_EDGE: wrap[i] = GL_MIRROR_CLAMP_TO_EDGE; break;
            case MGL_CLAMP_TO_BORDER: wrap[i] = GL_CLAMP_TO_BORDER; break;
            default:
                PyErr_Format(PyExc_ValueError, "invalid wrap");
                return 0;
        }
    }
    return 1;
}
