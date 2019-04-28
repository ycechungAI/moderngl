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

/* moderngl.core.create_context(standalone, debug, require, glhook, gc)
 * Returns a Context object.
 */
PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(3);

    bool standalone = (bool)PyObject_IsTrue(args[0]);
    PyObject * require = args[1];
    PyObject * glhook = args[2];

    static bool first_run = true;

    if (first_run) {
        PyObject * sys = must_have(PyImport_ImportModule("sys"));
        PyObject * sys_modules = must_have(PyObject_GetAttrString(sys, "modules"));
        PyObject * moderngl = must_have(PyDict_GetItemString(sys_modules, "moderngl"));
        moderngl = must_have(PyObject_GetAttrString(moderngl, "new"));

        Attribute_class = intern(PyObject_GetAttrString(moderngl, "Attribute"));
        Buffer_class = intern(PyObject_GetAttrString(moderngl, "Buffer"));
        ComputeShader_class = intern(PyObject_GetAttrString(moderngl, "ComputeShader"));
        Context_class = intern(PyObject_GetAttrString(moderngl, "Context"));
        Framebuffer_class = intern(PyObject_GetAttrString(moderngl, "Framebuffer"));
        Limits_class = intern(PyObject_GetAttrString(moderngl, "Limits"));
        Program_class = intern(PyObject_GetAttrString(moderngl, "Program"));
        Query_class = intern(PyObject_GetAttrString(moderngl, "Query"));
        Renderbuffer_class = intern(PyObject_GetAttrString(moderngl, "Renderbuffer"));
        Sampler_class = intern(PyObject_GetAttrString(moderngl, "Sampler"));
        Scope_class = intern(PyObject_GetAttrString(moderngl, "Scope"));
        Texture_class = intern(PyObject_GetAttrString(moderngl, "Texture"));
        Uniform_class = intern(PyObject_GetAttrString(moderngl, "Uniform"));
        VertexArray_class = intern(PyObject_GetAttrString(moderngl, "VertexArray"));

        moderngl_error = intern(PyObject_GetAttrString(moderngl, "Error"));
        moderngl_compiler_error = intern(PyObject_GetAttrString(moderngl, "compiler_error"));
        moderngl_linker_error = intern(PyObject_GetAttrString(moderngl, "linker_error"));
        moderngl_split_format = intern(PyObject_GetAttrString(moderngl, "split_format"));

        numpy = xintern(PyImport_ImportModule("numpy"));
        if (!numpy) {
            PyErr_Clear();
        }

        if (numpy) {
            numpy_frombuffer = xintern(PyObject_GetAttrString(numpy, "frombuffer"));
        }

        pillow = xintern(PyImport_ImportModule("PIL.Image"));
        if (!pillow) {
            PyErr_Clear();
        }

        if (pillow) {
            pillow_image = xintern(PyObject_GetAttrString(pillow, "Image"));
        }

        tobytes_str = intern(PyUnicode_FromString("tobytes"));
        size_str = intern(PyUnicode_FromString("size"));
        mode_str = intern(PyUnicode_FromString("mode"));
        points_long = intern(PyLong_FromLong(GL_POINTS));
        triangles_long = intern(PyLong_FromLong(GL_TRIANGLES));

        /* Errors are not recoverable at this point */

        ensure_no_error();

        first_run = false;
    }

    MGLContext * context = PyObject_New(MGLContext, MGLContext_class);
    context->chain.prev = context;
    context->chain.next = context;

    context->glsl_compiler_error = new_ref(moderngl_compiler_error);
    context->glsl_linker_error = new_ref(moderngl_linker_error);

    if (!context->gl_context.load(standalone)) {
        return 0;
    }

    if (!context->gl.load()) {
        return 0;
    }

    const GLMethods & gl = context->gl;

    if (glhook != Py_None) {
        PyObject * dtype = PyUnicode_FromFormat("u%d", sizeof(void *));
        PyObject * glprocs = PyMemoryView_FromMemory((char *)&context->gl, sizeof(context->gl), PyBUF_WRITE);
        PyObject * result = PyObject_CallFunction(glhook, "OO", glprocs, dtype);
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

    MGLFramebuffer * default_framebuffer = PyObject_New(MGLFramebuffer, MGLFramebuffer_class);
    chain_objects(context, default_framebuffer);
    default_framebuffer->context = context;

    default_framebuffer->framebuffer_obj = 0;
    default_framebuffer->components = 4;
    default_framebuffer->color_mask = 0xF;
    default_framebuffer->depth_mask = true;
    default_framebuffer->attachments = 1;
    default_framebuffer->attachment_type[0] = 'f';

    int scrissor_box[4] = {};
    gl.GetIntegerv(GL_SCISSOR_BOX, scrissor_box);
    default_framebuffer->width = scrissor_box[2];
    default_framebuffer->height = scrissor_box[3];

    default_framebuffer->viewport[0] = 0;
    default_framebuffer->viewport[1] = 0;
    default_framebuffer->viewport[2] = default_framebuffer->width;
    default_framebuffer->viewport[3] = default_framebuffer->height;

    default_framebuffer->wrapper = Framebuffer_New("O(ii)", default_framebuffer, default_framebuffer->width, default_framebuffer->height);

    context->default_framebuffer = default_framebuffer;
    context->bound_framebuffer = default_framebuffer;

    MGLScope * default_scope = PyObject_New(MGLScope, MGLScope_class);
    chain_objects(context, default_scope);
    default_scope->context = context;

    default_scope->framebuffer = default_framebuffer;
    default_scope->old_scope = default_scope;
    default_scope->enable_only = 0;

    context->default_scope = default_scope;
    context->active_scope = default_scope;
    context->bound_scope = default_scope;

    default_scope->wrapper = Scope_New("O", default_scope);

    MGLRecorder * recorder = PyObject_New(MGLRecorder, MGLRecorder_class);
    chain_objects(context, recorder);
    recorder->context = context;

    PyObject * limits = get_limits(gl, version_code);

    context->wrapper = Context_New("OiOOOO", context, version_code, limits, context->default_framebuffer->wrapper, context->bound_framebuffer->wrapper, recorder);
    return new_ref(context->wrapper);
}

PyObject * MGLContext_meth_make_current(MGLContext * self) {
    self->gl_context.enter();
    Py_RETURN_NONE;
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

void MGLContext_dealloc(MGLContext * self) {
    printf("MGLContext_dealloc\n");
    Py_TYPE(self)->tp_free(self);
}

fastcallable(MGLContext_meth_copy_buffer)
fastcallable(MGLContext_meth_buffer)
fastcallable(MGLContext_meth_framebuffer)
fastcallable(MGLContext_meth_program)
fastcallable(MGLContext_meth_query)
fastcallable(MGLContext_meth_renderbuffer)
fastcallable(MGLContext_meth_depth_renderbuffer)
fastcallable(MGLContext_meth_sampler)
fastcallable(MGLContext_meth_scope)
fastcallable(MGLContext_meth_texture)
fastcallable(MGLContext_meth_vertex_array)

PyMethodDef MGLContext_methods[] = {
    {"copy_buffer", fastcall(MGLContext_meth_copy_buffer), fastcall_flags, NULL},
    {"buffer", fastcall(MGLContext_meth_buffer), fastcall_flags, NULL},
    {"compute_shader", (PyCFunction)MGLContext_meth_compute_shader, METH_O, 0},
    {"configure", (PyCFunction)MGLContext_meth_configure, METH_O, 0},
    {"framebuffer", fastcall(MGLContext_meth_framebuffer), fastcall_flags, NULL},
    {"program", fastcall(MGLContext_meth_program), fastcall_flags, NULL},
    {"query", fastcall(MGLContext_meth_query), fastcall_flags, NULL},
    {"renderbuffer", fastcall(MGLContext_meth_renderbuffer), fastcall_flags, NULL},
    {"depth_renderbuffer", fastcall(MGLContext_meth_depth_renderbuffer), fastcall_flags, NULL},
    {"sampler", fastcall(MGLContext_meth_sampler), fastcall_flags, NULL},
    {"scope", fastcall(MGLContext_meth_scope), fastcall_flags, NULL},
    {"texture", fastcall(MGLContext_meth_texture), fastcall_flags, NULL},
    {"vertex_array", fastcall(MGLContext_meth_vertex_array), fastcall_flags, NULL},
    {"replay", (PyCFunction)MGLContext_meth_replay, METH_O, 0},
    {"make_current", (PyCFunction)MGLContext_meth_make_current, METH_NOARGS, 0},
    {0},
};

PyType_Slot MGLContext_slots[] = {
    {Py_tp_methods, MGLContext_methods},
    {Py_tp_dealloc, (void *)MGLContext_dealloc},
    {0},
};

PyType_Spec MGLContext_spec = {
    "moderngl.mgl.new.MGLContext",
    sizeof(MGLContext),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLContext_slots,
};

PyTypeObject * MGLContext_class;
