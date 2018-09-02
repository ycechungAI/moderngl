#pragma once
#include "internal/python.hpp"

#ifdef MGL_DEBUG
#define mgl_name "moderngl.mgl_dbg"
#define PyInit_mgl PyInit_mgl_dbg
#else
#define mgl_name "moderngl.mgl"
#endif

enum MGLEnableFlag {
    MGL_NOTHING = 0,
    MGL_BLEND = 1,
    MGL_DEPTH_TEST = 2,
    MGL_CULL_FACE = 4,
    MGL_RASTERIZER_DISCARD = 8,
};
