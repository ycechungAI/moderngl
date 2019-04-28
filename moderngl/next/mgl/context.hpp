#pragma once
#include "mgl.hpp"

#include "internal/opengl/gl_context.hpp"
#include "internal/opengl/gl_methods.hpp"

enum MGLEnableFlag {
    MGL_NOTHING = 0,
    MGL_BLEND = 1,
    MGL_DEPTH_TEST = 2,
    MGL_CULL_FACE = 4,
    MGL_RASTERIZER_DISCARD = 8,
};

struct MGLBaseObject {
    PyObject_HEAD
    struct {
        MGLBaseObject * prev;
        MGLBaseObject * next;
    } chain;
};

inline void chain_objects(MGLBaseObject * a, MGLBaseObject * b) {
    b->chain.prev = a->chain.prev;
    b->chain.next = a;
    a->chain.prev->chain.next = b;
    a->chain.prev = b;
}

struct MGLFramebuffer;
struct MGLScope;

struct MGLContext : public MGLBaseObject {
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

struct MGLContextObject : public MGLBaseObject {
    PyObject * wrapper;
    MGLContext * context;
};

extern PyType_Spec MGLContext_spec;
extern PyTypeObject * MGLContext_class;

PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs);

struct MGLObject {
    PyObject_HEAD
    PyObject * wrapper;
    MGLContext * context;
};
