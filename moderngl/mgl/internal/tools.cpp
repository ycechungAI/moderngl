#include "tools.hpp"

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
