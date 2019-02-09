#pragma once
#include "mgl.hpp"
#include "framebuffer.hpp"
#include "scope.hpp"

#include "internal/opengl/gl_context.hpp"
#include "internal/opengl/gl_methods.hpp"

enum MGLEnableFlag {
    MGL_NOTHING = 0,
    MGL_BLEND = 1,
    MGL_DEPTH_TEST = 2,
    MGL_CULL_FACE = 4,
    MGL_RASTERIZER_DISCARD = 8,
};

struct MGLContext {
    PyObject_HEAD
    PyObject * wrapper;
    GLContext gl_context;
    GLMethods gl;

    int version_code;
    int default_texture_unit;

    int current_enable_only;
    int current_program_obj;
    int current_array_buffer_obj;
    int current_vertex_array_obj;
    int current_framebuffer_obj;
    int current_temp_texture_obj;
    int current_sampler_obj[32];
    int current_alignment;

    unsigned long long current_color_mask;
    bool current_depth_mask;

    MGLFramebuffer * bound_framebuffer;
    MGLFramebuffer * default_framebuffer;

    MGLScope * default_scope;
    MGLScope * active_scope;
    MGLScope * bound_scope;

    PyTypeObject * MGLBuffer_class;
    PyTypeObject * MGLComputeShader_class;
    PyTypeObject * MGLFramebuffer_class;
    PyTypeObject * MGLProgram_class;
    PyTypeObject * MGLQuery_class;
    PyTypeObject * MGLRenderbuffer_class;
    PyTypeObject * MGLSampler_class;
    PyTypeObject * MGLScope_class;
    PyTypeObject * MGLTexture_class;
    PyTypeObject * MGLVertexArray_class;

    PyObject * glsl_compiler_error;
    PyObject * glsl_linker_error;
    PyObject * gc;

    void enable(int enable_only);

    void use_program(int program_obj);

    void bind_array_buffer(int buffer_obj);
    void bind_vertex_array(int vertex_array_obj);
    void bind_framebuffer(int framebuffer_obj);
    void bind_temp_texture(int texture_target, int texture_obj);
    void bind_sampler(int location, int texture_target, int texture_obj, int sampler_obj);

    void set_alignment(int alignment);
    void set_write_mask(unsigned long long color_mask, bool depth_mask);
};

extern PyType_Spec MGLContext_spec;
PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs);

/* Every objects derived from a Context must have the following HEAD */

struct MGLObject {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
};

/* Every internal object has its own wrapper class defined in the python layer.
 * An internal object holds a ref to the instance of its wrapper class.
 * The wrapper class holds a ref to the internal object.
 * Such a pair can be created using MGLContext_new_object.
 * The circular reference must be resolved when releasing objects.
 */

MGLObject * _MGLContext_new_object(MGLContext * self, PyTypeObject * type, PyTypeObject * cls, int slot, int size);
MGLObject * _MGLObject_pop_mglo(PyObject * wrapper, int slot);
PyObject * _MGLObject_release(MGLObject * self);

#define MGLContext_new_object(self, name) (MGL ## name *)_MGLContext_new_object(self, self->MGL ## name ## _class, name ## _class, name ## _class_mglo, sizeof(MGL ## name))
#define MGLObject_pop_mglo(name, obj) (MGL ## name *)_MGLObject_pop_mglo(obj, name ## _class_mglo)
#define MGLObject_release(obj) _MGLObject_release((MGLObject *)obj);

extern PyTypeObject * MGLContext_class;

/* Macros */

#ifdef MGL_DEBUG
#define ENSURE_CONTEXT(ctx, ret) if (!ctx->gl_context.active()) { PyErr_Format(PyExc_AssertionError, "the current context is not active"); return ret; }
#else
#define ENSURE_CONTEXT(ctx, ret)
#endif
