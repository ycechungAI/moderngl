#include "extensions.hpp"
#include "classes/context_class.hpp"

/* MGLContext.extensions(...)
 * Returns a list of supported extensions.
 */
PyObject * meth_extensions(PyObject * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 1) {
        // TODO: error
        return 0;
    }

    if (args[0]->ob_type != Context_class) {
        // TODO: error
        return 0;
    }

    MGLContext * ctx = SLOT(args[0], MGLContext, Context_class_mglo);
    int version_code = PyLong_AsLong(SLOT(args[0], PyObject, Context_class_version_code));
    const GLMethods & gl = ctx->gl;

    if (version_code >= 300) {
        int num_extensions = 0;
        gl.GetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
        PyObject * lst = PyList_New(num_extensions);
        for (int i = 0; i < num_extensions; ++i) {
            const char * ext = (const char *)gl.GetStringi(GL_EXTENSIONS, i);
            PyList_SET_ITEM(lst, i, PyUnicode_FromString(ext));
        }
        return lst;
    } else {
        const char * ext = (const char *)gl.GetString(GL_EXTENSIONS);
        if (!ext) {
            return PyList_New(0);
        }
        PyObject * extensions = PyUnicode_FromString(ext);
        PyObject * lst = PyUnicode_Split(extensions, 0, -1);
        Py_DECREF(extensions);
        return lst;
    }
}
