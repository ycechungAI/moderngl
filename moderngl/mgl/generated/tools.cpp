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
            return (int)type->tp_members[i].offset;
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
