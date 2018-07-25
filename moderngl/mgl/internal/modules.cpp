#include "modules.hpp"
#include "opengl/opengl.hpp"

PyObject * moderngl;
PyObject * moderngl_error;
PyObject * moderngl_compiler_error;
PyObject * moderngl_linker_error;
PyObject * moderngl_split_format;

PyObject * numpy;
PyObject * numpy_frombuffer;

PyObject * tobytes_str;
PyObject * points_long;
PyObject * triangles_long;

/* Loading external modules.
 * This function must not be called on import.
 * Circular imports are not supported in this case.
 */
bool load_modules() {
    moderngl = PyImport_ImportModule("moderngl");
    if (!moderngl) {
        return false;
    }

    moderngl_error = PyObject_GetAttrString(moderngl, "Error");

    PyObject * moderngl_tools = PyImport_ImportModule("moderngl.tools");
    if (!moderngl_tools) {
        return false;
    }

    moderngl_compiler_error = PyObject_GetAttrString(moderngl_tools, "compiler_error");
    moderngl_linker_error = PyObject_GetAttrString(moderngl_tools, "linker_error");
    moderngl_split_format = PyObject_GetAttrString(moderngl_tools, "split_format");

    numpy = PyImport_ImportModule("numpy");
    if (!numpy) {
        PyErr_Clear();
    }

    if (numpy) {
        numpy_frombuffer = PyObject_GetAttrString(numpy, "frombuffer");
    }

    tobytes_str = PyUnicode_FromString("tobytes");
    points_long = PyLong_FromLong(GL_POINTS);
    triangles_long = PyLong_FromLong(GL_TRIANGLES);

    if (PyErr_Occurred()) {
        return false;
    }

    return true;
}
