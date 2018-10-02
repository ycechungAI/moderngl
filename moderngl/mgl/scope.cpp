#include "scope.hpp"
#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"
#include "internal/modules.hpp"
#include "internal/enable.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

/* MGLContext.scope(...)
 */
PyObject * MGLContext_meth_scope(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 2) {
        // TODO: error
        return 0;
    }

    if (args[0] != Py_None && Py_TYPE(args[0]) != Framebuffer_class) {
        return 0;
    }

    MGLScope * scope = MGLContext_new_object(self, Scope);

    if (args[0] == Py_None) {
        scope->framebuffer = 0;
    } else {
        scope->framebuffer = NEW_REF(SLOT(args[0], MGLFramebuffer, Framebuffer_class_mglo));
    }

    scope->enable_only = PyLong_AsLong(args[1]);

    return NEW_REF(scope->wrapper);
}

/* MGLScope.begin()
 */
PyObject * MGLScope_meth_begin(MGLScope * self) {
    const GLMethods & gl = self->context->gl;
    gl_enable_only(gl, self->enable_only, self->context->enable_only);
    self->context->enable_only = self->enable_only;
    // return MGLFramebuffer_meth_use(self->framebuffer);
    Py_RETURN_NONE;
}

/* MGLScope.end()
 */
PyObject * MGLScope_meth_end(MGLScope * self) {
    const GLMethods & gl = self->context->gl;
    gl_enable_only(gl, self->old_enable_only, self->context->enable_only);
    self->context->enable_only = self->old_enable_only;
    // return MGLFramebuffer_meth_use(self->old_framebuffer);
    Py_RETURN_NONE;
}
