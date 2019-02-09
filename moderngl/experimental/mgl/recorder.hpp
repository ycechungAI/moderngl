#pragma once
#include "mgl.hpp"

#include "internal/opengl/gl_methods.hpp"

struct MGLContext;
struct MGLFramebuffer;
struct MGLScope;

struct MGLRecorder {
    PyObject_HEAD
    MGLContext * context;

    int old_enable_only;
    int old_program_obj;
    int old_array_buffer_obj;
    int old_vertex_array_obj;
    int old_framebuffer_obj;
    int old_temp_texture_obj;
    int old_sampler_obj[32];
    int old_alignment;

    unsigned long long old_color_mask;
    bool old_depth_mask;

    MGLFramebuffer * old_bound_framebuffer;
    MGLScope * old_active_scope;
    MGLScope * old_bound_scope;
};

extern PyTypeObject * MGLRecorder_class;
PyObject * MGLContext_meth_replay(MGLContext * self, PyObject * bytecode);
void init_recording();
