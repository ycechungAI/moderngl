#pragma once

#include "opengl/gl_methods.hpp"

enum MGLEnableFlag {
    MGL_NOTHING = 0,
    MGL_BLEND = 1,
    MGL_DEPTH_TEST = 2,
    MGL_CULL_FACE = 4,
    MGL_RASTERIZER_DISCARD = 8,
};

int read_enable_only(const GLMethods & gl);
void gl_force_enable_only(const GLMethods & gl, int flags);
void gl_diff_enable_only(const GLMethods & gl, int flags, int current_flags);

// #define gl_enable_only(gl, flags, current_flags) gl_force_enable_only(gl, flags)
#define gl_enable_only(gl, flags, current_flags) gl_diff_enable_only(gl, flags, current_flags)
