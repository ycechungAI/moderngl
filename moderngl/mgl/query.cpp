#include "query.hpp"
#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"
#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

/* MGLContext.query(...)
 */
PyObject * MGLContext_meth_query(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 0) {
        // TODO: error
        return 0;
    }

    MGLQuery * query = MGLContext_new_object(self, Query);
    return NEW_REF(query->wrapper);
}

PyObject * MGLQuery_meth_write(MGLQuery * self, PyObject * const * args, Py_ssize_t nargs) {
    Py_RETURN_NONE;
}
