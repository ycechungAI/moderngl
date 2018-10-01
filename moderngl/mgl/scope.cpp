#include "scope.hpp"
#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"
#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

PyObject * MGLContext_meth_scope(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 0) {
        // TODO: error
        return 0;
    }

    MGLScope * scope = MGLContext_new_object(self, Scope);
    return NEW_REF(scope->wrapper);
}

PyObject * MGLScope_meth_write(MGLScope * self, PyObject * const * args, Py_ssize_t nargs) {
    Py_RETURN_NONE;
}
