#include "tools.hpp"

/* Detects a class defined in python.
 */
PyTypeObject * detect_class(PyObject * module, const char * name, int & slots_len) {
    if (!module || PyErr_Occurred()) {
        return 0;
    }

    PyObject * cls = PyObject_GetAttrString(module, name);
    if (!cls) {
        return 0;
    }

    PyObject * slots = PyObject_GetAttrString(cls, "__slots__");
    if (!slots) {
        return 0;
    }

    slots_len = (int)PyObject_Size(slots);
    return (PyTypeObject *)cls;
}

/* Returns the offset of a given slot.
 */
int slot_offset(PyTypeObject * type, const char * name, int & counter) {
    if (!type || PyErr_Occurred()) {
        return 0;
    }
    counter -= 1;
    for (int i = 0; type->tp_members[i].name; ++i) {
        if (!strcmp(type->tp_members[i].name, name)) {
            return type->tp_members[i].offset;
        }
    }

    PyErr_Format(PyExc_Exception, "missing slot %s in %s", name, type->tp_name);
    return 0;
}

/* Ensures that all the slots are processed.
 */
void assert_slots_len(PyTypeObject * type, int slots_len) {
    if (!slots_len || PyErr_Occurred()) {
        return;
    }

    PyErr_Format(PyExc_Exception, "remaining slots in %s", type->tp_name);
}

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
