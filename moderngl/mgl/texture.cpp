#include "texture.hpp"
#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"
#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

PyObject * MGLContext_meth_texture(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 6) {
        // TODO: error
        return 0;
    }

    MGLTexture * texture = MGLContext_new_object(self, Texture);
    return NEW_REF(texture->wrapper);
}

PyObject * MGLTexture_meth_write(MGLTexture * self, PyObject * const * args, Py_ssize_t nargs) {
    Py_RETURN_NONE;
}

/* Definition of MGLTexture internal type */

void MGLTexture_define(MGLContext * ctx) {
    ctx->MGLTexture_class = (PyTypeObject *)PyType_FromSpec(&MGLTexture_spec);
}
