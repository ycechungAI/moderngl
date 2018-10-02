#include "enable.hpp"

int read_enable_only(const GLMethods & gl) {
    int enable_only = 0;
    if (gl.IsEnabled(GL_BLEND)) {
        enable_only |= MGL_BLEND;
    }
    if (gl.IsEnabled(GL_DEPTH_TEST)) {
        enable_only |= MGL_DEPTH_TEST;
    }
    if (gl.IsEnabled(GL_CULL_FACE)) {
        enable_only |= MGL_CULL_FACE;
    }
    if (gl.IsEnabled(GL_RASTERIZER_DISCARD)) {
        enable_only |= MGL_RASTERIZER_DISCARD;
    }
    return enable_only;
}

void gl_force_enable_only(const GLMethods & gl, int flags) {
    if (flags < 0) {
        return;
    }

    if (flags & MGL_BLEND) {
        gl.Enable(GL_BLEND);
    } else {
        gl.Disable(GL_BLEND);
    }

    if (flags & MGL_DEPTH_TEST) {
        gl.Enable(GL_DEPTH_TEST);
    } else {
        gl.Disable(GL_DEPTH_TEST);
    }

    if (flags & MGL_CULL_FACE) {
        gl.Enable(GL_CULL_FACE);
    } else {
        gl.Disable(GL_CULL_FACE);
    }

    if (flags & MGL_RASTERIZER_DISCARD) {
        gl.Enable(GL_RASTERIZER_DISCARD);
    } else {
        gl.Disable(GL_RASTERIZER_DISCARD);
    }
}

void gl_diff_enable_only(const GLMethods & gl, int flags, int current_flags) {
    if (flags < 0 || flags == current_flags) {
        return;
    }

    int changed_flags = flags ^ current_flags;

    if (changed_flags & MGL_BLEND) {
        if (flags & MGL_BLEND) {
            gl.Enable(GL_BLEND);
        } else {
            gl.Disable(GL_BLEND);
        }
    }

    if (changed_flags & MGL_DEPTH_TEST) {
        if (flags & MGL_DEPTH_TEST) {
            gl.Enable(GL_DEPTH_TEST);
        } else {
            gl.Disable(GL_DEPTH_TEST);
        }
    }

    if (changed_flags & MGL_CULL_FACE) {
        if (flags & MGL_CULL_FACE) {
            gl.Enable(GL_CULL_FACE);
        } else {
            gl.Disable(GL_CULL_FACE);
        }
    }

    if (changed_flags & MGL_RASTERIZER_DISCARD) {
        if (flags & MGL_RASTERIZER_DISCARD) {
            gl.Enable(GL_RASTERIZER_DISCARD);
        } else {
            gl.Disable(GL_RASTERIZER_DISCARD);
        }
    }
}
