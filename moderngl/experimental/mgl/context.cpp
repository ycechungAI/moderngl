#include "context.hpp"

#include "buffer.hpp"
#include "recorder.hpp"
#include "compute_shader.hpp"
#include "configuration.hpp"
#include "framebuffer.hpp"
#include "limits.hpp"
#include "program.hpp"
#include "query.hpp"
#include "renderbuffer.hpp"
#include "sampler.hpp"
#include "scope.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"

#include "internal/wrapper.hpp"
#include "internal/tools.hpp"

/* moderngl.core.create_context(standalone, debug, glhook, gc)
 * Returns a Context object.
 */
PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 4) {
        // TODO: error
        return 0;
    }

    bool standalone = (bool)PyObject_IsTrue(args[0]);
    bool debug = (bool)PyObject_IsTrue(args[1]);
    PyObject * glhook = args[2];
    PyObject * gc = args[3];

    MGLContext * context = new_object(MGLContext, MGLContext_class);
    memset((char *)context + sizeof(PyObject), 0, sizeof(MGLContext) - sizeof(PyObject));

    // context->glsl_compiler_error = moderngl_compiler_error;
    // context->glsl_linker_error = moderngl_linker_error;

    if (!context->gl_context.load(standalone)) {
        return 0;
    }

    if (!context->gl.load()) {
        return 0;
    }

    if (gc != Py_None) {
        context->gc = NEW_REF(gc);
    }

    const GLMethods & gl = context->gl;

    if (glhook != Py_None) {
        PyObject * dtype = PyUnicode_FromFormat("u%d", sizeof(void *));
        PyObject * glprocs = PyMemoryView_FromMemory((char *)&context->gl, sizeof(context->gl), PyBUF_WRITE);
        PyObject * result = call_function(glhook, glprocs, dtype);
        if (!result) {
            return 0;
        }
        Py_DECREF(dtype);
        Py_DECREF(glprocs);
        Py_DECREF(result);
    }

    int major = 0;
    int minor = 0;
    gl.GetIntegerv(GL_MAJOR_VERSION, &major);
    gl.GetIntegerv(GL_MINOR_VERSION, &minor);
    int version_code = major * 100 + minor * 10;

    if (version_code == 0) {
        gl.GetError();
        const char * ver = (const char *)gl.GetString(GL_VERSION);
        if (ver && '0' <= ver[0] && ver[0] <= '9' && ver[1] == '.' && '0' <= ver[2] && ver[2] <= '9') {
            version_code = (ver[0] - '0') * 100 + (ver[2] - '0') * 10;
        }
    }

    int default_texture_unit = 0;
    gl.GetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &default_texture_unit);
    default_texture_unit = GL_TEXTURE0 + (default_texture_unit > 0 ? default_texture_unit - 1 : 0);

    context->default_texture_unit = default_texture_unit;
    context->current_enable_only = 0;
    context->current_color_mask = 0xffffffffffffffff;
    context->current_depth_mask = true;

    gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl.Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    gl.Enable(GL_PRIMITIVE_RESTART);
    gl.PrimitiveRestartIndex(-1);

    context->MGLBuffer_class = (PyTypeObject *)PyType_FromSpec(&MGLBuffer_spec);
    context->MGLComputeShader_class = (PyTypeObject *)PyType_FromSpec(&MGLComputeShader_spec);
    context->MGLFramebuffer_class = (PyTypeObject *)PyType_FromSpec(&MGLFramebuffer_spec);
    context->MGLProgram_class = (PyTypeObject *)PyType_FromSpec(&MGLProgram_spec);
    context->MGLQuery_class = (PyTypeObject *)PyType_FromSpec(&MGLQuery_spec);
    context->MGLRenderbuffer_class = (PyTypeObject *)PyType_FromSpec(&MGLRenderbuffer_spec);
    context->MGLSampler_class = (PyTypeObject *)PyType_FromSpec(&MGLSampler_spec);
    context->MGLScope_class = (PyTypeObject *)PyType_FromSpec(&MGLScope_spec);
    context->MGLTexture_class = (PyTypeObject *)PyType_FromSpec(&MGLTexture_spec);
    context->MGLVertexArray_class = (PyTypeObject *)PyType_FromSpec(&MGLVertexArray_spec);

    MGLFramebuffer * default_framebuffer = MGLContext_new_object(context, Framebuffer);

    default_framebuffer->framebuffer_obj = 0;
    default_framebuffer->components = 4;
    default_framebuffer->color_mask = 0xF;
    default_framebuffer->depth_mask = true;
    default_framebuffer->attachments = 1;
    default_framebuffer->attachment_type = "f";

    default_framebuffer->viewport[0] = 0;
    default_framebuffer->viewport[1] = 0;
    default_framebuffer->viewport[2] = default_framebuffer->width;
    default_framebuffer->viewport[3] = default_framebuffer->height;
    SLOT(default_framebuffer->wrapper, PyObject, Framebuffer_class_viewport) = int_tuple(0, 0, default_framebuffer->width, default_framebuffer->height);

    context->default_framebuffer = default_framebuffer;
    context->bound_framebuffer = NEW_REF(default_framebuffer);

    MGLScope * default_scope = MGLContext_new_object(context, Scope);

    default_scope->framebuffer = NEW_REF(default_framebuffer);
    default_scope->old_scope = default_scope;
    default_scope->enable_only = 0;

    context->default_scope = default_scope;
    context->active_scope = NEW_REF(default_scope);
    context->bound_scope = NEW_REF(default_scope);

    MGLRecorder * recorder = PyObject_New(MGLRecorder, MGLRecorder_class);
    // memset((char *)recorder + sizeof(PyObject), 0, sizeof(MGLRecorder) - sizeof(PyObject));
    recorder->context = context;

    context->wrapper = new_object(PyObject, Context_class);
    clear_slots(context->wrapper);

    SLOT(context->wrapper, MGLContext, Context_class_mglo) = context;
    SLOT(context->wrapper, PyObject, Context_class_version_code) = PyLong_FromLong(version_code);
    SLOT(context->wrapper, PyObject, Context_class_limits) = get_limits(gl, version_code);
    SLOT(context->wrapper, PyObject, Context_class_screen) = NEW_REF(context->default_framebuffer->wrapper);
    SLOT(context->wrapper, PyObject, Context_class_fbo) = NEW_REF(context->bound_framebuffer->wrapper);
    SLOT(context->wrapper, MGLRecorder, Context_class_recorder) = recorder;

    return NEW_REF(context->wrapper);
}

/* _MGLContext_new_object(...)
 */
MGLObject * _MGLContext_new_object(MGLContext * self, PyTypeObject * type, PyTypeObject * cls, int slot, int size) {
    MGLObject * res = new_object(MGLObject, type);
    memset((char *)res + sizeof(PyObject), 0, size - sizeof(PyObject));
    res->wrapper = new_object(PyObject, cls);
    clear_slots(res->wrapper);
    SLOT(res->wrapper, MGLObject, slot) = NEW_REF(res);
    res->context = NEW_REF(self);
    if (self->gc) {
        PyObject * track = PyObject_GetAttrString(self->gc, "append");
        if (track) {
            PyObject * result = call_function(track, res->wrapper);
            if (!result) {
                return 0;
            }
            Py_DECREF(result);
        }
    }
    return res;
}

/* _MGLObject_release(...)
 */
PyObject * _MGLObject_release(MGLObject * self) {
    if (self->context->gc) {
        PyObject * untrack = PyObject_GetAttrString(self->context->gc, "remove");
        if (untrack) {
            PyObject * result = call_function(untrack, self->wrapper);
            if (!result) {
                return 0;
            }
            Py_DECREF(result);
        }
    }
    Py_DECREF(self->wrapper);
    Py_DECREF(self->context);
    Py_DECREF(self);
    Py_RETURN_NONE;
}

/* _MGLObject_pop_mglo(...)
 */
MGLObject * _MGLObject_pop_mglo(PyObject * wrapper, int slot) {
    MGLObject * mglo = SLOT(wrapper, MGLObject, slot);
    SLOT(wrapper, MGLObject, slot) = 0;
    return mglo;
}

/*inline*/ void MGLContext::enable(int enable_only) {
    if (int changed_flags = current_enable_only ^ enable_only) {
        if (changed_flags & MGL_BLEND) {
            if (enable_only & MGL_BLEND) {
                gl.Enable(GL_BLEND);
            } else {
                gl.Disable(GL_BLEND);
            }
        }

        if (changed_flags & MGL_DEPTH_TEST) {
            if (enable_only & MGL_DEPTH_TEST) {
                gl.Enable(GL_DEPTH_TEST);
            } else {
                gl.Disable(GL_DEPTH_TEST);
            }
        }

        if (changed_flags & MGL_CULL_FACE) {
            if (enable_only & MGL_CULL_FACE) {
                gl.Enable(GL_CULL_FACE);
            } else {
                gl.Disable(GL_CULL_FACE);
            }
        }

        if (changed_flags & MGL_RASTERIZER_DISCARD) {
            if (enable_only & MGL_RASTERIZER_DISCARD) {
                gl.Enable(GL_RASTERIZER_DISCARD);
            } else {
                gl.Disable(GL_RASTERIZER_DISCARD);
            }
        }

        current_enable_only = enable_only;
    }
}

/*inline*/ void MGLContext::use_program(int program_obj) {
    if (current_program_obj != program_obj) {
        current_program_obj = program_obj;
        gl.UseProgram(program_obj);
    }
}

/*inline*/ void MGLContext::bind_array_buffer(int buffer_obj) {
    if (current_array_buffer_obj != buffer_obj) {
        current_array_buffer_obj = buffer_obj;
        gl.BindBuffer(GL_ARRAY_BUFFER, buffer_obj);
    }
}

/*inline*/ void MGLContext::bind_vertex_array(int vertex_array_obj) {
    if (current_vertex_array_obj != vertex_array_obj) {
        current_vertex_array_obj = vertex_array_obj;
        gl.BindVertexArray(vertex_array_obj);
    }
}

/*inline*/ void MGLContext::bind_framebuffer(int framebuffer_obj) {
    if (current_framebuffer_obj != framebuffer_obj) {
        current_framebuffer_obj = framebuffer_obj;
        gl.BindFramebuffer(GL_FRAMEBUFFER, framebuffer_obj);
    }
}

/*inline*/ void MGLContext::bind_temp_texture(int texture_target, int texture_obj) {
    if (current_temp_texture_obj != texture_obj) {
        current_temp_texture_obj = texture_obj;
        gl.ActiveTexture(default_texture_unit);
        gl.BindTexture(texture_target, texture_obj);
    }
}

/*inline*/ void MGLContext::bind_sampler(int location, int texture_target, int texture_obj, int sampler_obj) {
    int & current = current_sampler_obj[location & 0x1f];
    if (current != sampler_obj) {
        current = sampler_obj;
        gl.ActiveTexture(GL_TEXTURE0 + location);
        gl.BindTexture(texture_target, texture_obj);
        gl.BindSampler(location, sampler_obj);
    }
}

/*inline*/ void MGLContext::set_alignment(int alignment) {
    if (current_alignment != alignment) {
        current_alignment = alignment;
        gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
        gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    }
}

/*inline*/ void MGLContext::set_write_mask(unsigned long long color_mask, bool depth_mask) {
    if (unsigned long long mask_diff = (current_color_mask ^ color_mask) & bound_framebuffer->color_mask) {
        current_color_mask = color_mask;

        for (int i = 0; mask_diff; ++i) {
            if (mask_diff & 0xF) {
                gl.ColorMaski(i, color_mask & 1, color_mask & 2, color_mask & 4, color_mask & 8);
            }
            color_mask >>= 4;
            mask_diff >>= 4;
        }
    }

    if ((current_depth_mask ^ depth_mask) & bound_framebuffer->depth_mask) {
        current_depth_mask = depth_mask;
        gl.DepthMask(depth_mask);
    }
}

PyTypeObject * MGLContext_class;

#if PY_VERSION_HEX >= 0x03070000

PyMethodDef MGLContext_methods[] = {
    {"copy_buffer", (PyCFunction)MGLContext_meth_copy_buffer, METH_FASTCALL, 0},
    {"buffer", (PyCFunction)MGLContext_meth_buffer, METH_FASTCALL, 0},
    {"compute_shader", (PyCFunction)MGLContext_meth_compute_shader, METH_O, 0},
    {"configure", (PyCFunction)MGLContext_meth_configure, METH_O, 0},
    {"framebuffer", (PyCFunction)MGLContext_meth_framebuffer, METH_FASTCALL, 0},
    {"program", (PyCFunction)MGLContext_meth_program, METH_FASTCALL, 0},
    {"query", (PyCFunction)MGLContext_meth_query, METH_FASTCALL, 0},
    {"renderbuffer", (PyCFunction)MGLContext_meth_renderbuffer, METH_FASTCALL, 0},
    {"sampler", (PyCFunction)MGLContext_meth_sampler, METH_FASTCALL, 0},
    {"scope", (PyCFunction)MGLContext_meth_scope, METH_FASTCALL, 0},
    {"texture", (PyCFunction)MGLContext_meth_texture, METH_FASTCALL, 0},
    {"vertex_array", (PyCFunction)MGLContext_meth_vertex_array, METH_FASTCALL, 0},
    {"replay", (PyCFunction)MGLContext_meth_replay, METH_O, 0},
    {0},
};

#else

PyObject * MGLContext_meth_copy_buffer_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_copy_buffer(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_buffer_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_buffer(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_framebuffer_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_framebuffer(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_program_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_program(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_query_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_query(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_renderbuffer_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_renderbuffer(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_sampler_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_sampler(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_scope_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_scope(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_texture_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_texture(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyObject * MGLContext_meth_vertex_array_va(MGLContext * self, PyObject * args) {
    return MGLContext_meth_vertex_array(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}

PyMethodDef MGLContext_methods[] = {
    {"copy_buffer", (PyCFunction)MGLContext_meth_copy_buffer_va, METH_VARARGS, 0},
    {"buffer", (PyCFunction)MGLContext_meth_buffer_va, METH_VARARGS, 0},
    {"compute_shader", (PyCFunction)MGLContext_meth_compute_shader, METH_O, 0},
    {"configure", (PyCFunction)MGLContext_meth_configure, METH_O, 0},
    {"framebuffer", (PyCFunction)MGLContext_meth_framebuffer_va, METH_VARARGS, 0},
    {"program", (PyCFunction)MGLContext_meth_program_va, METH_VARARGS, 0},
    {"query", (PyCFunction)MGLContext_meth_query_va, METH_VARARGS, 0},
    {"renderbuffer", (PyCFunction)MGLContext_meth_renderbuffer_va, METH_VARARGS, 0},
    {"sampler", (PyCFunction)MGLContext_meth_sampler_va, METH_VARARGS, 0},
    {"scope", (PyCFunction)MGLContext_meth_scope_va, METH_VARARGS, 0},
    {"texture", (PyCFunction)MGLContext_meth_texture_va, METH_VARARGS, 0},
    {"vertex_array", (PyCFunction)MGLContext_meth_vertex_array_va, METH_VARARGS, 0},
    {"replay", (PyCFunction)MGLContext_meth_replay, METH_O, 0},
    {0},
};

#endif

PyType_Slot MGLContext_slots[] = {
    {Py_tp_methods, MGLContext_methods},
    {0},
};

PyType_Spec MGLContext_spec = {
    mgl_name ".Context",
    sizeof(MGLContext),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLContext_slots,
};
