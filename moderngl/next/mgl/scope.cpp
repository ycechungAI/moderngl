#include "scope.hpp"
#include "buffer.hpp"
#include "context.hpp"
#include "sampler.hpp"
#include "texture.hpp"
#include "framebuffer.hpp"

#include "internal/wrapper.hpp"

#include "internal/tools.hpp"
#include "internal/glsl.hpp"

void MGLScope_begin_core(MGLScope * self) {
    const GLMethods & gl = self->context->gl;
    self->old_scope = self->context->active_scope;
    self->context->active_scope = self;
    self->context->bound_scope = self;

    if (self->enable_only >= 0) {
        self->old_enable_only = self->context->current_enable_only;
        self->context->enable(self->enable_only);
    }

    if (self->framebuffer) {
        MGLFramebuffer_use_core(self->framebuffer);
    }

    if (self->samplers) {
        MGLScopeSamplerBinding * ptr = self->samplers;
        for (int i = 0; i < self->num_samplers; ++i) {
            MGLTexture * texture = ptr->sampler->texture;
            self->context->bind_sampler(ptr->binding, texture->texture_target, texture->texture_obj, ptr->sampler->sampler_obj);
            ++ptr;
        }
    }

    if (self->buffers) {
        MGLScopeBufferBinding * ptr = self->buffers;
        for (int i = 0; i < self->num_buffers; ++i) {
        	gl.BindBufferRange(ptr->target, ptr->binding, ptr->buffer->buffer_obj, ptr->offset, ptr->size);
            ++ptr;
        }
    }
}

void MGLScope_end_core(MGLScope * self) {
    self->context->active_scope = self->old_scope;
    self->old_scope = 0;
}

/* MGLContext.scope(enable_only, framebuffer, samplers, uniform_buffers, storage_buffers)
 */
PyObject * MGLContext_meth_scope(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(5);

    MGLScope * scope = PyObject_New(MGLScope, MGLScope_class);
    chain_objects(self, scope);
    scope->context = self;

    scope->framebuffer = 0;
    scope->samplers = 0;
    scope->buffers = 0;

    int enable_only = PyLong_AsLong(args[0]);
    ensure_no_error();

    scope->enable_only = enable_only;

    if (args[1] != Py_None && !Framebuffer_Check(args[1])) {
        return 0;
    }

    if (args[1] == Py_None) {
        scope->framebuffer = 0;
    } else {
        scope->framebuffer = (MGLFramebuffer *)get_slot(args[1], "mglo");
    }

    PyObject * samplers = 0;
    PyObject * buffers = 0;

    int num_samplers = 0;
    int num_buffers = 0;

    if (args[2] != Py_None) {
        samplers = PySequence_Fast(args[2], "samplers is not iterable");
        if (!samplers) {
            return 0;
        }

        num_samplers = (int)PySequence_Fast_GET_SIZE(samplers);
    }

    if (args[3] != Py_None) {
        buffers = PySequence_Fast(args[3], "buffers is not iterable");
        if (!buffers) {
            return 0;
        }

        num_buffers = (int)PySequence_Fast_GET_SIZE(buffers);
    }

    scope->num_samplers = num_samplers;
    scope->num_buffers = num_buffers;

    if (num_samplers) {
        scope->samplers = (MGLScopeSamplerBinding *)malloc(sizeof(MGLScopeSamplerBinding) * num_samplers);

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
            if (!Sampler_Check(wrapper)) {
                return 0;
            }
            scope->samplers[i].sampler = (MGLSampler *)get_slot(wrapper, "mglo");
            scope->samplers[i].binding = binding;
        }
    }

    if (num_buffers) {
        scope->buffers = (MGLScopeBufferBinding *)malloc(sizeof(MGLScopeBufferBinding) * num_buffers);

        for (int i = 0; i < num_buffers; ++i) {
            PyObject * tuple = PySequence_Fast_GET_ITEM(buffers, i);
            tuple = PySequence_Fast(tuple, "buffers is not iterable");
            if (!tuple) {
                return 0;
            }
            if (PySequence_Fast_GET_SIZE(tuple) != 2) {
                return 0;
            }
            PyObject * wrapper = PySequence_Fast_GET_ITEM(tuple, 0);
            int binding = PyLong_AsLong(PySequence_Fast_GET_ITEM(tuple, 1));
            Py_DECREF(tuple);
            if (!Buffer_Check(wrapper)) {
                return 0;
            }
            scope->buffers[i].buffer = (MGLBuffer *)get_slot(wrapper, "mglo");
            scope->buffers[i].binding = binding;
        }
    }

    Py_XDECREF(samplers);
    Py_XDECREF(buffers);

    scope->wrapper = Scope_New("O", scope);
    return scope->wrapper;
}

/* MGLScope.begin()
 */
PyObject * MGLScope_meth_begin(MGLScope * self) {
    MGLScope_begin_core(self);
    Py_RETURN_NONE;
}

/* MGLScope.end()
 */
PyObject * MGLScope_meth_end(MGLScope * self) {
    MGLScope_end_core(self);
    Py_RETURN_NONE;
}

void MGLScope_dealloc(MGLScope * self) {
    printf("MGLScope_dealloc\n");
    Py_TYPE(self)->tp_free(self);
}

PyMethodDef MGLScope_methods[] = {
    {"begin", (PyCFunction)MGLScope_meth_begin, METH_NOARGS, 0},
    {"end", (PyCFunction)MGLScope_meth_end, METH_NOARGS, 0},
    {0},
};

PyType_Slot MGLScope_slots[] = {
    {Py_tp_methods, MGLScope_methods},
    {Py_tp_dealloc, (void *)MGLScope_dealloc},
    {0},
};

PyType_Spec MGLScope_spec = {
    "moderngl.mgl.new.MGLScope",
    sizeof(MGLScope),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLScope_slots,
};

PyTypeObject * MGLScope_class;
