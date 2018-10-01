#include "framebuffer.hpp"
#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"
#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

PyObject * MGLContext_meth_framebuffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 0) {
        // TODO: error
        return 0;
    }

    MGLFramebuffer * framebuffer = MGLContext_new_object(self, Framebuffer);
    return NEW_REF(framebuffer->wrapper);
}

PyObject * MGLFramebuffer_meth_write(MGLFramebuffer * self, PyObject * const * args, Py_ssize_t nargs) {
    Py_RETURN_NONE;
}
