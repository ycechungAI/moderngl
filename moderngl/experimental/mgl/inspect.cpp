#include "buffer.hpp"
#include "compute_shader.hpp"
#include "context.hpp"
#include "framebuffer.hpp"
#include "program.hpp"
#include "query.hpp"
#include "renderbuffer.hpp"
#include "sampler.hpp"
#include "scope.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"

#include "internal/wrapper.hpp"
#include "internal/tools.hpp"

inline void dict_add_obj(PyObject * dict, const char * key, PyObject * value) {
    if (value) {
        PyDict_SetItemString(dict, key, value);
    }
}

inline void dict_add_obj_decref(PyObject * dict, const char * key, PyObject * value) {
    if (value) {
        PyDict_SetItemString(dict, key, value);
        Py_DECREF(value);
    }
}

inline void dict_add_int(PyObject * dict, const char * key, int value) {
    PyObject * item = PyLong_FromLong(value);
    PyDict_SetItemString(dict, key, item);
    Py_DECREF(item);
}

inline void dict_add_ull(PyObject * dict, const char * key, unsigned long long value) {
    PyObject * item = PyLong_FromUnsignedLongLong(value);
    PyDict_SetItemString(dict, key, item);
    Py_DECREF(item);
}

inline void dict_add_ssizet(PyObject * dict, const char * key, Py_ssize_t value) {
    PyObject * item = PyLong_FromSsize_t(value);
    PyDict_SetItemString(dict, key, item);
    Py_DECREF(item);
}

inline void dict_add_bool(PyObject * dict, const char * key, bool value) {
    PyObject * item = PyBool_FromLong(value);
    PyDict_SetItemString(dict, key, item);
    Py_DECREF(item);
}

inline void dict_add_str(PyObject * dict, const char * key, const char * value, int length) {
    PyObject * item = PyUnicode_FromStringAndSize(value, length);
    PyDict_SetItemString(dict, key, item);
    Py_DECREF(item);
}

inline void dict_add_int_tuple(PyObject * dict, const char * key, int size, int * values) {
    PyObject * item = PyTuple_New(size);
    for (int i = 0; i < size; ++i) {
        PyTuple_SET_ITEM(item, i, PyLong_FromLong(values[i]));
    }
    PyDict_SetItemString(dict, key, item);
    Py_DECREF(item);
}

/* moderngl.core.inspect(obj)
 */
PyObject * meth_inspect(PyObject * self, PyObject * obj) {
    if (PyLong_CheckExact(obj)) {
        return meth_inspect(0, (PyObject *)PyLong_AsVoidPtr(obj));
    }

    if (obj->ob_type == Attribute_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj(res, "type", SLOT(obj, PyObject, Attribute_class_type));
        dict_add_obj(res, "location", SLOT(obj, PyObject, Attribute_class_location));
        dict_add_obj(res, "cols", SLOT(obj, PyObject, Attribute_class_cols));
        dict_add_obj(res, "rows", SLOT(obj, PyObject, Attribute_class_rows));
        dict_add_obj(res, "size", SLOT(obj, PyObject, Attribute_class_size));
        dict_add_obj(res, "shape", SLOT(obj, PyObject, Attribute_class_shape));
        return res;
    }

    if (obj->ob_type == Buffer_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj_decref(res, "mglo", meth_inspect(0, SLOT(obj, PyObject, Buffer_class_mglo)));
        dict_add_obj(res, "size", SLOT(obj, PyObject, Buffer_class_size));
        return res;
    }

    if (obj->ob_type == ComputeShader_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj_decref(res, "mglo", meth_inspect(0, SLOT(obj, PyObject, ComputeShader_class_mglo)));
        dict_add_obj(res, "uniforms", SLOT(obj, PyObject, ComputeShader_class_uniforms));
        return res;
    }

    if (obj->ob_type == Context_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj_decref(res, "mglo", meth_inspect(0, SLOT(obj, PyObject, Context_class_mglo)));
        dict_add_obj(res, "version_code", SLOT(obj, PyObject, Context_class_version_code));
        dict_add_obj(res, "limits", SLOT(obj, PyObject, Context_class_limits));
        dict_add_obj(res, "screen", SLOT(obj, PyObject, Context_class_screen));
        dict_add_obj(res, "fbo", SLOT(obj, PyObject, Context_class_fbo));
        dict_add_obj(res, "recorder", SLOT(obj, PyObject, Context_class_recorder));
        return res;
    }

    if (obj->ob_type == Framebuffer_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj_decref(res, "mglo", meth_inspect(0, SLOT(obj, PyObject, Framebuffer_class_mglo)));
        dict_add_obj(res, "viewport", SLOT(obj, PyObject, Framebuffer_class_viewport));
        dict_add_obj(res, "size", SLOT(obj, PyObject, Framebuffer_class_size));
        return res;
    }

    if (obj->ob_type == Program_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj_decref(res, "mglo", meth_inspect(0, SLOT(obj, PyObject, Program_class_mglo)));
        dict_add_obj(res, "uniforms", SLOT(obj, PyObject, Program_class_uniforms));
        dict_add_obj(res, "attributes", SLOT(obj, PyObject, Program_class_attributes));
        return res;
    }

    if (obj->ob_type == Query_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj_decref(res, "mglo", meth_inspect(0, SLOT(obj, PyObject, Query_class_mglo)));
        dict_add_obj(res, "elapsed", SLOT(obj, PyObject, Query_class_elapsed));
        dict_add_obj(res, "primitives", SLOT(obj, PyObject, Query_class_primitives));
        dict_add_obj(res, "samples", SLOT(obj, PyObject, Query_class_samples));
        return res;
    }

    if (obj->ob_type == Renderbuffer_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj_decref(res, "mglo", meth_inspect(0, SLOT(obj, PyObject, Renderbuffer_class_mglo)));
        dict_add_obj(res, "size", SLOT(obj, PyObject, Renderbuffer_class_size));
        return res;
    }

    if (obj->ob_type == Sampler_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj_decref(res, "mglo", meth_inspect(0, SLOT(obj, PyObject, Sampler_class_mglo)));
        dict_add_obj(res, "swizzle", SLOT(obj, PyObject, Sampler_class_swizzle));
        dict_add_obj(res, "filter", SLOT(obj, PyObject, Sampler_class_filter));
        dict_add_obj(res, "texture", SLOT(obj, PyObject, Sampler_class_texture));
        return res;
    }

    if (obj->ob_type == Scope_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj_decref(res, "mglo", meth_inspect(0, SLOT(obj, PyObject, Scope_class_mglo)));
        return res;
    }

    if (obj->ob_type == Texture_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj_decref(res, "mglo", meth_inspect(0, SLOT(obj, PyObject, Texture_class_mglo)));
        dict_add_obj(res, "level", SLOT(obj, PyObject, Texture_class_level));
        dict_add_obj(res, "layer", SLOT(obj, PyObject, Texture_class_layer));
        dict_add_obj(res, "size", SLOT(obj, PyObject, Texture_class_size));
        return res;
    }

    if (obj->ob_type == Uniform_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj(res, "type", SLOT(obj, PyObject, Uniform_class_type));
        dict_add_obj(res, "location", SLOT(obj, PyObject, Uniform_class_location));
        dict_add_obj(res, "cols", SLOT(obj, PyObject, Uniform_class_cols));
        dict_add_obj(res, "rows", SLOT(obj, PyObject, Uniform_class_rows));
        dict_add_obj(res, "size", SLOT(obj, PyObject, Uniform_class_size));
        dict_add_obj(res, "shape", SLOT(obj, PyObject, Uniform_class_shape));
        return res;
    }

    if (obj->ob_type == VertexArray_class) {
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj_decref(res, "mglo", meth_inspect(0, SLOT(obj, PyObject, VertexArray_class_mglo)));
        dict_add_obj(res, "ibo", SLOT(obj, PyObject, VertexArray_class_ibo));
        dict_add_obj(res, "program", SLOT(obj, PyObject, VertexArray_class_program));
        dict_add_obj(res, "scope", SLOT(obj, PyObject, VertexArray_class_scope));
        dict_add_obj(res, "mode", SLOT(obj, PyObject, VertexArray_class_mode));
        dict_add_obj(res, "vertices", SLOT(obj, PyObject, VertexArray_class_vertices));
        return res;
    }

    if (obj->ob_type == MGLContext_class) {
        MGLContext * context = (MGLContext *)obj;
        PyObject * res = PyDict_New();
        dict_add_obj(res, "self", obj);
        dict_add_obj(res, "wrapper", context->wrapper);
        // TODO: ...
        return res;
    }

    if (starts_with(obj->ob_type->tp_name, mgl_name)) {
        MGLObject * mglo = (MGLObject *)obj;

        if (mglo->ob_base.ob_type == mglo->context->MGLBuffer_class) {
            MGLBuffer * buffer = (MGLBuffer *)obj;
            PyObject * res = PyDict_New();
            dict_add_obj(res, "self", obj);
            dict_add_obj(res, "wrapper", buffer->wrapper);
            dict_add_obj(res, "context", (PyObject *)buffer->context);
            dict_add_int(res, "buffer_obj", buffer->buffer_obj);
            dict_add_int(res, "flags", buffer->flags);
            dict_add_ssizet(res, "size", buffer->size);
            return res;
        }

        if (mglo->ob_base.ob_type == mglo->context->MGLComputeShader_class) {
            MGLComputeShader * compute_shader = (MGLComputeShader *)obj;
            PyObject * res = PyDict_New();
            dict_add_obj(res, "self", obj);
            dict_add_obj(res, "wrapper", compute_shader->wrapper);
            dict_add_obj(res, "context", (PyObject *)compute_shader->context);
            dict_add_int(res, "program_obj", compute_shader->program_obj);
            dict_add_int(res, "shader_obj", compute_shader->shader_obj);
            return res;
        }

        if (mglo->ob_base.ob_type == mglo->context->MGLFramebuffer_class) {
            MGLFramebuffer * framebuffer = (MGLFramebuffer *)obj;
            PyObject * res = PyDict_New();
            dict_add_obj(res, "self", obj);
            dict_add_obj(res, "wrapper", framebuffer->wrapper);
            dict_add_obj(res, "context", (PyObject *)framebuffer->context);
            dict_add_int(res, "framebuffer_obj", framebuffer->framebuffer_obj);
            dict_add_int(res, "width", framebuffer->width);
            dict_add_int(res, "height", framebuffer->height);
            dict_add_int(res, "depth", framebuffer->depth);
            dict_add_int(res, "components", framebuffer->components);
            dict_add_int(res, "levels", framebuffer->levels);
            dict_add_int(res, "samples", framebuffer->samples);
            dict_add_int_tuple(res, "viewport", 4, framebuffer->viewport);
            dict_add_int(res, "attachments", framebuffer->attachments);
            dict_add_str(res, "attachment_type", framebuffer->attachment_type, framebuffer->attachments);
            dict_add_ull(res, "color_mask", framebuffer->color_mask);
            dict_add_bool(res, "depth_mask", framebuffer->depth_mask);
            return res;
        }

        if (mglo->ob_base.ob_type == mglo->context->MGLProgram_class) {
            MGLProgram * program = (MGLProgram *)obj;
            PyObject * res = PyDict_New();
            dict_add_obj(res, "self", obj);
            dict_add_obj(res, "wrapper", program->wrapper);
            dict_add_obj(res, "context", (PyObject *)program->context);
            dict_add_int(res, "program_obj", program->program_obj);
            dict_add_int_tuple(res, "shader_obj", 5, program->shader_obj);
            return res;
        }

        if (mglo->ob_base.ob_type == mglo->context->MGLQuery_class) {
            MGLQuery * query = (MGLQuery *)obj;
            PyObject * res = PyDict_New();
            dict_add_obj(res, "self", obj);
            dict_add_obj(res, "wrapper", query->wrapper);
            dict_add_obj(res, "context", (PyObject *)query->context);
            dict_add_int_tuple(res, "query_obj", 4, query->query_obj);
            return res;
        }

        if (mglo->ob_base.ob_type == mglo->context->MGLRenderbuffer_class) {
            MGLRenderbuffer * renderbuffer = (MGLRenderbuffer *)obj;
            PyObject * res = PyDict_New();
            dict_add_obj(res, "self", obj);
            dict_add_obj(res, "wrapper", renderbuffer->wrapper);
            dict_add_obj(res, "context", (PyObject *)renderbuffer->context);
            dict_add_int(res, "renderbuffer_obj", renderbuffer->renderbuffer_obj);
            dict_add_int(res, "width", renderbuffer->width);
            dict_add_int(res, "height", renderbuffer->height);
            dict_add_int(res, "depth", renderbuffer->depth);
            dict_add_int(res, "components", renderbuffer->components);
            dict_add_int(res, "levels", renderbuffer->levels);
            dict_add_int(res, "samples", renderbuffer->samples);
            return res;
        }

        if (mglo->ob_base.ob_type == mglo->context->MGLSampler_class) {
            MGLSampler * sampler = (MGLSampler *)obj;
            PyObject * res = PyDict_New();
            dict_add_obj(res, "self", obj);
            dict_add_obj(res, "wrapper", sampler->wrapper);
            dict_add_obj(res, "context", (PyObject *)sampler->context);
            dict_add_int(res, "sampler_obj", sampler->sampler_obj);
            return res;
        }

        if (mglo->ob_base.ob_type == mglo->context->MGLScope_class) {
            MGLScope * scope = (MGLScope *)obj;
            PyObject * res = PyDict_New();
            dict_add_obj(res, "self", obj);
            dict_add_obj(res, "wrapper", scope->wrapper);
            dict_add_obj(res, "context", (PyObject *)scope->context);
            dict_add_obj(res, "framebuffer", (PyObject *)scope->framebuffer);
            dict_add_obj(res, "old_framebuffer", (PyObject *)scope->old_framebuffer);
            dict_add_int(res, "num_samplers", scope->num_samplers);
            dict_add_int(res, "num_uniform_buffers", scope->num_uniform_buffers);
            dict_add_int(res, "num_storage_buffers", scope->num_storage_buffers);
            dict_add_int(res, "enable_only", scope->enable_only);
            dict_add_int(res, "old_enable_only", scope->old_enable_only);

            PyObject * bindings = PyTuple_New(3);
            PyObject * bindings_samplers = PyTuple_New(scope->num_samplers);
            MGLScopeBinding * it = scope->bindings;
            for (int i = 0; i < scope->num_samplers; ++i) {
                PyObject * binding = PyTuple_New(2);
                PyTuple_SET_ITEM(binding, 0, it->object);
                PyTuple_SET_ITEM(binding, 1, PyLong_FromLong(it->binding));
                PyTuple_SET_ITEM(bindings_samplers, 0, binding);
                it += 1;
            }
            PyObject * bindings_uniform_buffers = PyTuple_New(scope->num_uniform_buffers);
            for (int i = 0; i < scope->num_uniform_buffers; ++i) {
                PyObject * binding = PyTuple_New(2);
                PyTuple_SET_ITEM(binding, 0, it->object);
                PyTuple_SET_ITEM(binding, 1, PyLong_FromLong(it->binding));
                PyTuple_SET_ITEM(bindings_samplers, 0, binding);
                it += 1;
            }
            PyObject * bindings_storage_buffers = PyTuple_New(scope->num_storage_buffers);
            for (int i = 0; i < scope->num_storage_buffers; ++i) {
                PyObject * binding = PyTuple_New(2);
                PyTuple_SET_ITEM(binding, 0, it->object);
                PyTuple_SET_ITEM(binding, 1, PyLong_FromLong(it->binding));
                PyTuple_SET_ITEM(bindings_samplers, 0, binding);
                it += 1;
            }
            PyTuple_SET_ITEM(bindings, 0, bindings_samplers);
            PyTuple_SET_ITEM(bindings, 1, bindings_uniform_buffers);
            PyTuple_SET_ITEM(bindings, 2, bindings_storage_buffers);
            dict_add_obj_decref(res, "bindings", bindings);
            return res;
        }

        if (mglo->ob_base.ob_type == mglo->context->MGLTexture_class) {
            MGLTexture * texture = (MGLTexture *)obj;
            PyObject * res = PyDict_New();
            dict_add_obj(res, "self", obj);
            dict_add_obj(res, "wrapper", texture->wrapper);
            dict_add_obj(res, "context", (PyObject *)texture->context);
            dict_add_int(res, "texture_obj", texture->texture_obj);
            dict_add_int(res, "texture_target", texture->texture_target);
            dict_add_int(res, "width", texture->width);
            dict_add_int(res, "height", texture->height);
            dict_add_int(res, "depth", texture->depth);
            dict_add_int(res, "components", texture->components);
            dict_add_int(res, "levels", texture->levels);
            dict_add_int(res, "samples", texture->samples);
            return res;
        }

        if (mglo->ob_base.ob_type == mglo->context->MGLVertexArray_class) {
            MGLVertexArray * vertex_array = (MGLVertexArray *)obj;
            PyObject * res = PyDict_New();
            dict_add_obj(res, "self", obj);
            dict_add_obj(res, "wrapper", vertex_array->wrapper);
            dict_add_obj(res, "context", (PyObject *)vertex_array->context);
            dict_add_int(res, "vertex_array_obj", vertex_array->vertex_array_obj);
            dict_add_int(res, "max_vertices", vertex_array->max_vertices);
            return res;
        }
    }

    return 0;
}
