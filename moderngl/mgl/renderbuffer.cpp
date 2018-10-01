#include "renderbuffer.hpp"
#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"
#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

PyObject * MGLContext_meth_renderbuffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 0) {
        // TODO: error
        return 0;
    }

    MGLRenderbuffer * renderbuffer = MGLContext_new_object(self, Renderbuffer);
    return NEW_REF(renderbuffer->wrapper);
}

PyObject * MGLRenderbuffer_meth_write(MGLRenderbuffer * self, PyObject * const * args, Py_ssize_t nargs) {
    Py_RETURN_NONE;
}
