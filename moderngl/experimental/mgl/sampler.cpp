#include "sampler.hpp"
#include "context.hpp"
#include "texture.hpp"

#include "internal/wrapper.hpp"

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
PyObject * MGLSampler_meth_use(MGLSampler * self, PyObject * arg) {
    PyObject * wrapper = SLOT(self->wrapper, PyObject, Sampler_class_texture);
    MGLTexture * texture = SLOT(wrapper, MGLTexture, Texture_class_mglo);

    int location = PyLong_AsLong(arg);
    self->context->bind_sampler(location, texture->texture_target, texture->texture_obj, self->sampler_obj);
    Py_RETURN_NONE;
}

#if PY_VERSION_HEX >= 0x03070000

PyMethodDef MGLSampler_methods[] = {
    {"use", (PyCFunction)MGLSampler_meth_use, METH_O, 0},
    {0},
};

#else

PyMethodDef MGLSampler_methods[] = {
    {"use", (PyCFunction)MGLSampler_meth_use, METH_O, 0},
    {0},
};

#endif

PyType_Slot MGLSampler_slots[] = {
    {Py_tp_methods, MGLSampler_methods},
    {0},
};

PyType_Spec MGLSampler_spec = {
    mgl_name ".Sampler",
    sizeof(MGLSampler),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLSampler_slots,
};
