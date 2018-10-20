#include "sampler.hpp"
#include "context.hpp"
#include "texture.hpp"

#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"

#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

/* MGLContext.sampler(texture)
 */
PyObject * MGLContext_meth_sampler(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 1) {
        // TODO: error
        return 0;
    }

    PyObject * texture = args[0];

    MGLSampler * sampler = MGLContext_new_object(self, Sampler);

    const GLMethods & gl = self->gl;
    gl.GenSamplers(1, (GLuint *)&sampler->sampler_obj);

    if (!sampler->sampler_obj) {
        PyErr_Format(moderngl_error, "cannot create sampler");
        Py_DECREF(sampler);
        return 0;
    }

    gl.SamplerParameteri(sampler->sampler_obj, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl.SamplerParameteri(sampler->sampler_obj, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SLOT(sampler->wrapper, PyObject, Sampler_class_texture) = texture;
    return NEW_REF(sampler->wrapper);
}

/* MGLSampler.use(location)
 */
PyObject * MGLSampler_meth_use(MGLSampler * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 1) {
        // TODO: error
        return 0;
    }

    PyObject * wrapper = SLOT(self->wrapper, PyObject, Sampler_class_texture);
    MGLTexture * texture = SLOT(wrapper, MGLTexture, Texture_class_mglo);

    int location = PyLong_AsLong(args[0]);
    self->context->bind_sampler(location, texture->texture_target, texture->texture_obj, self->sampler_obj);
    Py_RETURN_NONE;
}
