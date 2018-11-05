#include "scope.hpp"
#include "buffer.hpp"
#include "context.hpp"
#include "sampler.hpp"
#include "texture.hpp"

#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"

#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

/* MGLContext.scope(framebuffer, enable_only, samplers, uniform_buffers, storage_buffers)
 */
PyObject * MGLContext_meth_scope(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 5) {
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

    PyObject * samplers = 0;
    PyObject * uniform_buffers = 0;
    PyObject * storage_buffers = 0;

    int num_samplers = 0;
    int num_uniform_buffers = 0;
    int num_storage_buffers = 0;

    if (args[2] != Py_None) {
        samplers = PySequence_Fast(args[2], "samplers is not iterable");
        if (!samplers) {
            return 0;
        }

        num_samplers = (int)PySequence_Fast_GET_SIZE(samplers);
    }

    if (args[3] != Py_None) {
        uniform_buffers = PySequence_Fast(args[3], "uniform_buffers is not iterable");
        if (!uniform_buffers) {
            return 0;
        }

        num_uniform_buffers = (int)PySequence_Fast_GET_SIZE(uniform_buffers);
    }
    if (args[4] != Py_None) {
        storage_buffers = PySequence_Fast(args[4], "storage_buffers is not iterable");
        if (!storage_buffers) {
            return 0;
        }

        num_storage_buffers = (int)PySequence_Fast_GET_SIZE(storage_buffers);
    }

    int num_bindings = num_samplers + num_uniform_buffers + num_storage_buffers;

    if (num_bindings) {
        scope->num_samplers = num_samplers;
        scope->num_uniform_buffers = num_uniform_buffers;
        scope->num_storage_buffers = num_storage_buffers;
        scope->bindings = (MGLScopeBinding *)malloc(sizeof(MGLScopeBinding) * num_bindings);

        for (int i = 0; i < num_samplers; ++i) {
            PyObject * tuple = PySequence_Fast_GET_ITEM(samplers, i);
            tuple = PySequence_Fast(tuple, "samplers is not iterable");
            if (!tuple) {
                return 0;
            }
            if (PySequence_Fast_GET_SIZE(tuple) != 2) {
                return 0;
            }
            PyObject * wrapper = PySequence_Fast_GET_ITEM(tuple, 0);
            int binding = PyLong_AsLong(PySequence_Fast_GET_ITEM(tuple, 1));
            Py_DECREF(tuple);
            if (wrapper->ob_type != Sampler_class) {
                return 0;
            }
            scope->bindings[i].sampler = SLOT(wrapper, MGLSampler, Sampler_class_mglo);
            scope->bindings[i].binding = binding;
        }

        for (int i = 0; i < num_uniform_buffers; ++i) {
            PyObject * tuple = PySequence_Fast_GET_ITEM(uniform_buffers, i);
            tuple = PySequence_Fast(tuple, "uniform_buffers is not iterable");
            if (!tuple) {
                return 0;
            }
            if (PySequence_Fast_GET_SIZE(tuple) != 2) {
                return 0;
            }
            PyObject * wrapper = PySequence_Fast_GET_ITEM(tuple, 0);
            int binding = PyLong_AsLong(PySequence_Fast_GET_ITEM(tuple, 1));
            Py_DECREF(tuple);
            if (wrapper->ob_type != Buffer_class) {
                return 0;
            }
            scope->bindings[num_samplers + i].buffer = SLOT(wrapper, MGLBuffer, Buffer_class_mglo);
            scope->bindings[num_samplers + i].binding = binding;
        }

        for (int i = 0; i < num_storage_buffers; ++i) {
            PyObject * tuple = PySequence_Fast_GET_ITEM(storage_buffers, i);
            tuple = PySequence_Fast(tuple, "storage_buffers is not iterable");
            if (!tuple) {
                return 0;
            }
            if (PySequence_Fast_GET_SIZE(tuple) != 2) {
                return 0;
            }
            PyObject * wrapper = PySequence_Fast_GET_ITEM(tuple, 0);
            int binding = PyLong_AsLong(PySequence_Fast_GET_ITEM(tuple, 1));
            Py_DECREF(tuple);
            if (wrapper->ob_type != Buffer_class) {
                return 0;
            }
            scope->bindings[num_samplers + num_uniform_buffers + i].buffer = SLOT(wrapper, MGLBuffer, Buffer_class_mglo);
            scope->bindings[num_samplers + num_uniform_buffers + i].binding = binding;
        }
    }

    Py_XDECREF(samplers);
    Py_XDECREF(uniform_buffers);
    Py_XDECREF(storage_buffers);

    return NEW_REF(scope->wrapper);
}

/* MGLScope.begin()
 */
PyObject * MGLScope_meth_begin(MGLScope * self) {
    const GLMethods & gl = self->context->gl;
    if (self->enable_only >= 0) {
        self->old_enable_only = self->context->current_enable_only;
        self->context->enable(self->enable_only);
    }
    // return MGLFramebuffer_meth_use(self->framebuffer);

    if (self->bindings) {
        MGLScopeBinding * ptr = self->bindings;

        for (int i = 0; i < self->num_samplers; ++i) {
            PyObject * wrapper = SLOT(ptr->sampler->wrapper, PyObject, Sampler_class_texture);
            MGLTexture * texture = SLOT(wrapper, MGLTexture, Texture_class_mglo);
            self->context->bind_sampler(ptr->binding, texture->texture_target, texture->texture_obj, ptr->sampler->sampler_obj);
            ++ptr;
        }

        for (int i = 0; i < self->num_uniform_buffers; ++i) {
            ++ptr;
        }

        for (int i = 0; i < self->num_storage_buffers; ++i) {
            ++ptr;
        }
    }

    Py_RETURN_NONE;
}

/* MGLScope.end()
 */
PyObject * MGLScope_meth_end(MGLScope * self) {
    const GLMethods & gl = self->context->gl;
    if (self->enable_only >= 0) {
        self->context->enable(self->enable_only);
    }
    // return MGLFramebuffer_meth_use(self->old_framebuffer);
    Py_RETURN_NONE;
}
