#include "moderngl/mgl/internal/opengl/gl_methods.hpp"
#include <Python.h>
#include <cstdio>

GLMethods gl;

void print_error() {
    GLenum error = gl.GetError();
    switch (error) {
        case GL_NO_ERROR: break;
        case GL_INVALID_ENUM: printf(" // GL_INVALID_ENUM"); break;
        case GL_INVALID_VALUE: printf(" // GL_INVALID_VALUE"); break;
        case GL_INVALID_OPERATION: printf(" // GL_INVALID_OPERATION"); break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: printf(" // GL_INVALID_FRAMEBUFFER_OPERATION"); break;
        case GL_OUT_OF_MEMORY: printf(" // GL_OUT_OF_MEMORY"); break;
        case GL_STACK_UNDERFLOW: printf(" // GL_STACK_UNDERFLOW"); break;
        case GL_STACK_OVERFLOW: printf(" // GL_STACK_OVERFLOW"); break;
        default: printf(" // UNKNOWN_ERROR: %04x", error);
    }
}

void print_render_mode(GLenum mode) {
    switch (mode) {
        case GL_POINTS: printf("GL_POINTS"); break;
        case GL_LINE_STRIP: printf("GL_LINE_STRIP"); break;
        case GL_LINE_LOOP: printf("GL_LINE_LOOP"); break;
        case GL_LINES: printf("GL_LINES"); break;
        case GL_LINE_STRIP_ADJACENCY: printf("GL_LINE_STRIP_ADJACENCY"); break;
        case GL_LINES_ADJACENCY: printf("GL_LINES_ADJACENCY"); break;
        case GL_TRIANGLE_STRIP: printf("GL_TRIANGLE_STRIP"); break;
        case GL_TRIANGLE_FAN: printf("GL_TRIANGLE_FAN"); break;
        case GL_TRIANGLES: printf("GL_TRIANGLES"); break;
        case GL_TRIANGLE_STRIP_ADJACENCY: printf("GL_TRIANGLE_STRIP_ADJACENCY"); break;
        case GL_TRIANGLES_ADJACENCY: printf("GL_TRIANGLES_ADJACENCY"); break;
        case GL_PATCHES: printf("GL_PATCHES"); break;
        default: printf("%04x", mode);
    }
}

void print_enable_flag(GLenum mode) {
    switch (mode) {
        case GL_BLEND: printf("GL_BLEND"); break;
        case GL_COLOR_LOGIC_OP: printf("GL_COLOR_LOGIC_OP"); break;
        case GL_CULL_FACE: printf("GL_CULL_FACE"); break;
        case GL_DEBUG_OUTPUT: printf("GL_DEBUG_OUTPUT"); break;
        case GL_DEBUG_OUTPUT_SYNCHRONOUS: printf("GL_DEBUG_OUTPUT_SYNCHRONOUS"); break;
        case GL_DEPTH_CLAMP: printf("GL_DEPTH_CLAMP"); break;
        case GL_DEPTH_TEST: printf("GL_DEPTH_TEST"); break;
        case GL_DITHER: printf("GL_DITHER"); break;
        case GL_FRAMEBUFFER_SRGB: printf("GL_FRAMEBUFFER_SRGB"); break;
        case GL_LINE_SMOOTH: printf("GL_LINE_SMOOTH"); break;
        case GL_MULTISAMPLE: printf("GL_MULTISAMPLE"); break;
        case GL_POLYGON_OFFSET_FILL: printf("GL_POLYGON_OFFSET_FILL"); break;
        case GL_POLYGON_OFFSET_LINE: printf("GL_POLYGON_OFFSET_LINE"); break;
        case GL_POLYGON_OFFSET_POINT: printf("GL_POLYGON_OFFSET_POINT"); break;
        case GL_POLYGON_SMOOTH: printf("GL_POLYGON_SMOOTH"); break;
        case GL_PRIMITIVE_RESTART: printf("GL_PRIMITIVE_RESTART"); break;
        case GL_PRIMITIVE_RESTART_FIXED_INDEX: printf("GL_PRIMITIVE_RESTART_FIXED_INDEX"); break;
        case GL_RASTERIZER_DISCARD: printf("GL_RASTERIZER_DISCARD"); break;
        case GL_SAMPLE_ALPHA_TO_COVERAGE: printf("GL_SAMPLE_ALPHA_TO_COVERAGE"); break;
        case GL_SAMPLE_ALPHA_TO_ONE: printf("GL_SAMPLE_ALPHA_TO_ONE"); break;
        case GL_SAMPLE_COVERAGE: printf("GL_SAMPLE_COVERAGE"); break;
        case GL_SAMPLE_SHADING: printf("GL_SAMPLE_SHADING"); break;
        case GL_SAMPLE_MASK: printf("GL_SAMPLE_MASK"); break;
        case GL_SCISSOR_TEST: printf("GL_SCISSOR_TEST"); break;
        case GL_STENCIL_TEST: printf("GL_STENCIL_TEST"); break;
        case GL_TEXTURE_CUBE_MAP_SEAMLESS: printf("GL_TEXTURE_CUBE_MAP_SEAMLESS"); break;
        case GL_PROGRAM_POINT_SIZE: printf("GL_PROGRAM_POINT_SIZE"); break;
        default: printf("%04x", mode);
    }
}

void print_buffer_access_mode(GLenum access) {
    switch (access) {
        case GL_READ_ONLY: printf("GL_READ_ONLY"); break;
        case GL_WRITE_ONLY: printf("GL_WRITE_ONLY"); break;
        case GL_READ_WRITE: printf("GL_READ_WRITE"); break;
        default: printf("%04x", access);
    }
}

void print_buffer_access_bits(GLbitfield flags) {
    if (flags & GL_DYNAMIC_STORAGE_BIT) {
        flags &= ~GL_DYNAMIC_STORAGE_BIT;
        printf("GL_DYNAMIC_STORAGE_BIT");
        if (flags) {
            printf("|");
        }
    }
    if (flags & GL_MAP_READ_BIT) {
        flags &= ~GL_MAP_READ_BIT;
        printf("GL_MAP_READ_BIT");
        if (flags) {
            printf("|");
        }
    }
    if (flags & GL_MAP_WRITE_BIT) {
        flags &= ~GL_MAP_WRITE_BIT;
        printf("GL_MAP_WRITE_BIT");
        if (flags) {
            printf("|");
        }
    }
    if (flags & GL_MAP_PERSISTENT_BIT) {
        flags &= ~GL_MAP_PERSISTENT_BIT;
        printf("GL_MAP_PERSISTENT_BIT");
        if (flags) {
            printf("|");
        }
    }
    if (flags & GL_MAP_COHERENT_BIT) {
        flags &= ~GL_MAP_COHERENT_BIT;
        printf("GL_MAP_COHERENT_BIT");
        if (flags) {
            printf("|");
        }
    }
    if (flags & GL_CLIENT_STORAGE_BIT) {
        flags &= ~GL_CLIENT_STORAGE_BIT;
        printf("GL_CLIENT_STORAGE_BIT");
        if (flags) {
            printf("|");
        }
    }
    if (flags) {
        printf("0x%x", flags);
    }
}

void print_buffer_target(GLenum target) {
    switch (target) {
        case GL_ARRAY_BUFFER: printf("GL_ARRAY_BUFFER"); break;
        case GL_ATOMIC_COUNTER_BUFFER: printf("GL_ATOMIC_COUNTER_BUFFER"); break;
        case GL_COPY_READ_BUFFER: printf("GL_COPY_READ_BUFFER"); break;
        case GL_COPY_WRITE_BUFFER: printf("GL_COPY_WRITE_BUFFER"); break;
        case GL_DISPATCH_INDIRECT_BUFFER: printf("GL_DISPATCH_INDIRECT_BUFFER"); break;
        case GL_DRAW_INDIRECT_BUFFER: printf("GL_DRAW_INDIRECT_BUFFER"); break;
        case GL_ELEMENT_ARRAY_BUFFER: printf("GL_ELEMENT_ARRAY_BUFFER"); break;
        case GL_PIXEL_PACK_BUFFER: printf("GL_PIXEL_PACK_BUFFER"); break;
        case GL_PIXEL_UNPACK_BUFFER: printf("GL_PIXEL_UNPACK_BUFFER"); break;
        case GL_QUERY_BUFFER: printf("GL_QUERY_BUFFER"); break;
        case GL_SHADER_STORAGE_BUFFER: printf("GL_SHADER_STORAGE_BUFFER"); break;
        case GL_TEXTURE_BUFFER: printf("GL_TEXTURE_BUFFER"); break;
        case GL_TRANSFORM_FEEDBACK_BUFFER: printf("GL_TRANSFORM_FEEDBACK_BUFFER"); break;
        case GL_UNIFORM_BUFFER: printf("GL_UNIFORM_BUFFER"); break;
        default: printf("%04x", target);
    }
}

void print_texture_target(GLenum target) {
    switch (target) {
        case GL_TEXTURE_1D: printf("GL_TEXTURE_1D"); break;
        case GL_TEXTURE_2D: printf("GL_TEXTURE_2D"); break;
        case GL_TEXTURE_3D: printf("GL_TEXTURE_3D"); break;
        case GL_TEXTURE_1D_ARRAY: printf("GL_TEXTURE_1D_ARRAY"); break;
        case GL_TEXTURE_2D_ARRAY: printf("GL_TEXTURE_2D_ARRAY"); break;
        case GL_TEXTURE_RECTANGLE: printf("GL_TEXTURE_RECTANGLE"); break;
        case GL_TEXTURE_CUBE_MAP: printf("GL_TEXTURE_CUBE_MAP"); break;
        case GL_TEXTURE_CUBE_MAP_ARRAY: printf("GL_TEXTURE_CUBE_MAP_ARRAY"); break;
        case GL_TEXTURE_BUFFER: printf("GL_TEXTURE_BUFFER"); break;
        case GL_TEXTURE_2D_MULTISAMPLE: printf("GL_TEXTURE_2D_MULTISAMPLE"); break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY: printf("GL_TEXTURE_2D_MULTISAMPLE_ARRAY"); break;
        default: printf("%04x", target);
    }
}

void print_gl_error(GLenum error) {
    switch (error) {
        case GL_NO_ERROR: printf("GL_NO_ERROR"); break;
        case GL_INVALID_ENUM: printf("GL_INVALID_ENUM"); break;
        case GL_INVALID_VALUE: printf("GL_INVALID_VALUE"); break;
        case GL_INVALID_OPERATION: printf("GL_INVALID_OPERATION"); break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: printf("GL_INVALID_FRAMEBUFFER_OPERATION"); break;
        case GL_OUT_OF_MEMORY: printf("GL_OUT_OF_MEMORY"); break;
        case GL_STACK_UNDERFLOW: printf("GL_STACK_UNDERFLOW"); break;
        case GL_STACK_OVERFLOW: printf("GL_STACK_OVERFLOW"); break;
        default: printf("%04x", error);
    }
}

void print_cull_face(GLenum face) {
    switch (face) {
        case GL_FRONT: printf("GL_FRONT"); break;
        case GL_BACK: printf("GL_BACK"); break;
        case GL_FRONT_AND_BACK: printf("GL_FRONT_AND_BACK"); break;
        default: printf("%04x", face);
    }
}

void print_buffer_mode(GLenum mode) {
    switch (mode) {
        case GL_INTERLEAVED_ATTRIBS: printf("GL_INTERLEAVED_ATTRIBS"); break;
        case GL_SEPARATE_ATTRIBS: printf("GL_SEPARATE_ATTRIBS"); break;
        default: printf("%04x", mode);
    }
}

void print_clear_buffer_format(GLenum format) {
    switch (format) {
        case GL_RED: printf("GL_RED"); break;
        case GL_RG: printf("GL_RG"); break;
        case GL_RGB: printf("GL_RGB"); break;
        case GL_BGR: printf("GL_BGR"); break;
        case GL_RGBA: printf("GL_RGBA"); break;
        case GL_BGRA: printf("GL_BGRA"); break;
        case GL_RED_INTEGER: printf("GL_RED_INTEGER"); break;
        case GL_RG_INTEGER: printf("GL_RG_INTEGER"); break;
        case GL_RGB_INTEGER: printf("GL_RGB_INTEGER"); break;
        case GL_BGR_INTEGER: printf("GL_BGR_INTEGER"); break;
        case GL_RGBA_INTEGER: printf("GL_RGBA_INTEGER"); break;
        case GL_BGRA_INTEGER: printf("GL_BGRA_INTEGER"); break;
        case GL_STENCIL_INDEX: printf("GL_STENCIL_INDEX"); break;
        case GL_DEPTH_COMPONENT: printf("GL_DEPTH_COMPONENT"); break;
        case GL_DEPTH_STENCIL: printf("GL_DEPTH_STENCIL"); break;
        default: printf("%04x", format);
    }
}

void print_clear_buffer_type(GLenum type) {
    switch (type) {
        case GL_UNSIGNED_BYTE: printf("GL_UNSIGNED_BYTE"); break;
        case GL_BYTE: printf("GL_BYTE"); break;
        case GL_UNSIGNED_SHORT: printf("GL_UNSIGNED_SHORT"); break;
        case GL_SHORT: printf("GL_SHORT"); break;
        case GL_UNSIGNED_INT: printf("GL_UNSIGNED_INT"); break;
        case GL_INT: printf("GL_INT"); break;
        case GL_FLOAT: printf("GL_FLOAT"); break;
        case GL_UNSIGNED_BYTE_3_3_2: printf("GL_UNSIGNED_BYTE_3_3_2"); break;
        case GL_UNSIGNED_BYTE_2_3_3_REV: printf("GL_UNSIGNED_BYTE_2_3_3_REV"); break;
        case GL_UNSIGNED_SHORT_5_6_5: printf("GL_UNSIGNED_SHORT_5_6_5"); break;
        case GL_UNSIGNED_SHORT_5_6_5_REV: printf("GL_UNSIGNED_SHORT_5_6_5_REV"); break;
        case GL_UNSIGNED_SHORT_4_4_4_4: printf("GL_UNSIGNED_SHORT_4_4_4_4"); break;
        case GL_UNSIGNED_SHORT_4_4_4_4_REV: printf("GL_UNSIGNED_SHORT_4_4_4_4_REV"); break;
        case GL_UNSIGNED_SHORT_5_5_5_1: printf("GL_UNSIGNED_SHORT_5_5_5_1"); break;
        case GL_UNSIGNED_SHORT_1_5_5_5_REV: printf("GL_UNSIGNED_SHORT_1_5_5_5_REV"); break;
        case GL_UNSIGNED_INT_8_8_8_8: printf("GL_UNSIGNED_INT_8_8_8_8"); break;
        case GL_UNSIGNED_INT_8_8_8_8_REV: printf("GL_UNSIGNED_INT_8_8_8_8_REV"); break;
        case GL_UNSIGNED_INT_10_10_10_2: printf("GL_UNSIGNED_INT_10_10_10_2"); break;
        case GL_UNSIGNED_INT_2_10_10_10_REV: printf("GL_UNSIGNED_INT_2_10_10_10_REV"); break;
        default: printf("%04x", type);
    }
}

void print_clear_buffer_internalformat(GLenum internalformat) {
    switch (internalformat) {
        case GL_R8: printf("GL_R8"); break;
        case GL_R16: printf("GL_R16"); break;
        case GL_R16F: printf("GL_R16F"); break;
        case GL_R32F: printf("GL_R32F"); break;
        case GL_R8I: printf("GL_R8I"); break;
        case GL_R16I: printf("GL_R16I"); break;
        case GL_R32I: printf("GL_R32I"); break;
        case GL_R8UI: printf("GL_R8UI"); break;
        case GL_R16UI: printf("GL_R16UI"); break;
        case GL_R32UI: printf("GL_R32UI"); break;
        case GL_RG8: printf("GL_RG8"); break;
        case GL_RG16: printf("GL_RG16"); break;
        case GL_RG16F: printf("GL_RG16F"); break;
        case GL_RG32F: printf("GL_RG32F"); break;
        case GL_RG8I: printf("GL_RG8I"); break;
        case GL_RG16I: printf("GL_RG16I"); break;
        case GL_RG32I: printf("GL_RG32I"); break;
        case GL_RG8UI: printf("GL_RG8UI"); break;
        case GL_RG16UI: printf("GL_RG16UI"); break;
        case GL_RG32UI: printf("GL_RG32UI"); break;
        case GL_RGB32F: printf("GL_RGB32F"); break;
        case GL_RGB32I: printf("GL_RGB32I"); break;
        case GL_RGB32UI: printf("GL_RGB32UI"); break;
        case GL_RGBA8: printf("GL_RGBA8"); break;
        case GL_RGBA16: printf("GL_RGBA16"); break;
        case GL_RGBA16F: printf("GL_RGBA16F"); break;
        case GL_RGBA32F: printf("GL_RGBA32F"); break;
        case GL_RGBA8I: printf("GL_RGBA8I"); break;
        case GL_RGBA16I: printf("GL_RGBA16I"); break;
        case GL_RGBA32I: printf("GL_RGBA32I"); break;
        case GL_RGBA8UI: printf("GL_RGBA8UI"); break;
        case GL_RGBA16UI: printf("GL_RGBA16UI"); break;
        case GL_RGBA32UI: printf("GL_RGBA32UI"); break;
        default: printf("%04x", internalformat);
    }
}

void print_shader(GLenum shader) {
    switch (shader) {
        case GL_COMPUTE_SHADER: printf("GL_COMPUTE_SHADER"); break;
        case GL_VERTEX_SHADER: printf("GL_VERTEX_SHADER"); break;
        case GL_TESS_CONTROL_SHADER: printf("GL_TESS_CONTROL_SHADER"); break;
        case GL_TESS_EVALUATION_SHADER: printf("GL_TESS_EVALUATION_SHADER"); break;
        case GL_GEOMETRY_SHADER: printf("GL_GEOMETRY_SHADER"); break;
        case GL_FRAGMENT_SHADER: printf("GL_FRAGMENT_SHADER"); break;
        default: printf("%04x", shader);
    }
}

void print_shader_pname(GLenum pname) {
    switch (pname) {
        case GL_SHADER_TYPE: printf("GL_SHADER_TYPE"); break;
        case GL_DELETE_STATUS: printf("GL_DELETE_STATUS"); break;
        case GL_COMPILE_STATUS: printf("GL_COMPILE_STATUS"); break;
        case GL_INFO_LOG_LENGTH: printf("GL_INFO_LOG_LENGTH"); break;
        case GL_SHADER_SOURCE_LENGTH: printf("GL_SHADER_SOURCE_LENGTH"); break;
        default: printf("%04x", pname);
    }
}

void print_program_pname(GLenum pname) {
    switch (pname) {
        case GL_DELETE_STATUS: printf("GL_DELETE_STATUS"); break;
        case GL_LINK_STATUS: printf("GL_LINK_STATUS"); break;
        case GL_VALIDATE_STATUS: printf("GL_VALIDATE_STATUS"); break;
        case GL_INFO_LOG_LENGTH: printf("GL_INFO_LOG_LENGTH"); break;
        case GL_ATTACHED_SHADERS: printf("GL_ATTACHED_SHADERS"); break;
        case GL_ACTIVE_ATOMIC_COUNTER_BUFFERS: printf("GL_ACTIVE_ATOMIC_COUNTER_BUFFERS"); break;
        case GL_ACTIVE_ATTRIBUTES: printf("GL_ACTIVE_ATTRIBUTES"); break;
        case GL_ACTIVE_ATTRIBUTE_MAX_LENGTH: printf("GL_ACTIVE_ATTRIBUTE_MAX_LENGTH"); break;
        case GL_ACTIVE_UNIFORMS: printf("GL_ACTIVE_UNIFORMS"); break;
        case GL_ACTIVE_UNIFORM_MAX_LENGTH: printf("GL_ACTIVE_UNIFORM_MAX_LENGTH"); break;
        case GL_ACTIVE_UNIFORM_BLOCKS: printf("GL_ACTIVE_UNIFORM_BLOCKS"); break;
        case GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH: printf("GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH"); break;
        case GL_PROGRAM_BINARY_LENGTH: printf("GL_PROGRAM_BINARY_LENGTH"); break;
        case GL_COMPUTE_WORK_GROUP_SIZE: printf("GL_COMPUTE_WORK_GROUP_SIZE"); break;
        case GL_TRANSFORM_FEEDBACK_BUFFER_MODE: printf("GL_TRANSFORM_FEEDBACK_BUFFER_MODE"); break;
        case GL_TRANSFORM_FEEDBACK_VARYINGS: printf("GL_TRANSFORM_FEEDBACK_VARYINGS"); break;
        case GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH: printf("GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH"); break;
        case GL_GEOMETRY_VERTICES_OUT: printf("GL_GEOMETRY_VERTICES_OUT"); break;
        case GL_GEOMETRY_INPUT_TYPE: printf("GL_GEOMETRY_INPUT_TYPE"); break;
        case GL_GEOMETRY_OUTPUT_TYPE: printf("GL_GEOMETRY_OUTPUT_TYPE"); break;
        default: printf("%04x", pname);
    }
}

void print_tex_parameter_pname(GLenum pname) {
    switch (pname) {
        case GL_DEPTH_STENCIL_TEXTURE_MODE: printf("GL_DEPTH_STENCIL_TEXTURE_MODE"); break;
        case GL_TEXTURE_BASE_LEVEL: printf("GL_TEXTURE_BASE_LEVEL"); break;
        case GL_TEXTURE_COMPARE_FUNC: printf("GL_TEXTURE_COMPARE_FUNC"); break;
        case GL_TEXTURE_COMPARE_MODE: printf("GL_TEXTURE_COMPARE_MODE"); break;
        case GL_TEXTURE_LOD_BIAS: printf("GL_TEXTURE_LOD_BIAS"); break;
        case GL_TEXTURE_MIN_FILTER: printf("GL_TEXTURE_MIN_FILTER"); break;
        case GL_TEXTURE_MAG_FILTER: printf("GL_TEXTURE_MAG_FILTER"); break;
        case GL_TEXTURE_MIN_LOD: printf("GL_TEXTURE_MIN_LOD"); break;
        case GL_TEXTURE_MAX_LOD: printf("GL_TEXTURE_MAX_LOD"); break;
        case GL_TEXTURE_MAX_LEVEL: printf("GL_TEXTURE_MAX_LEVEL"); break;
        case GL_TEXTURE_SWIZZLE_R: printf("GL_TEXTURE_SWIZZLE_R"); break;
        case GL_TEXTURE_SWIZZLE_G: printf("GL_TEXTURE_SWIZZLE_G"); break;
        case GL_TEXTURE_SWIZZLE_B: printf("GL_TEXTURE_SWIZZLE_B"); break;
        case GL_TEXTURE_SWIZZLE_A: printf("GL_TEXTURE_SWIZZLE_A"); break;
        case GL_TEXTURE_WRAP_S: printf("GL_TEXTURE_WRAP_S"); break;
        case GL_TEXTURE_WRAP_T: printf("GL_TEXTURE_WRAP_T"); break;
        case GL_TEXTURE_WRAP_R: printf("GL_TEXTURE_WRAP_R"); break;
        default: printf("%04x", pname);
    }
}

void print_tex_level_parameter_pname(GLenum pname) {
    switch (pname) {
        case GL_TEXTURE_WIDTH: printf("GL_TEXTURE_WIDTH"); break;
        case GL_TEXTURE_HEIGHT: printf("GL_TEXTURE_HEIGHT"); break;
        case GL_TEXTURE_DEPTH: printf("GL_TEXTURE_DEPTH"); break;
        case GL_TEXTURE_INTERNAL_FORMAT: printf("GL_TEXTURE_INTERNAL_FORMAT"); break;
        case GL_TEXTURE_RED_SIZE: printf("GL_TEXTURE_RED_SIZE"); break;
        case GL_TEXTURE_GREEN_SIZE: printf("GL_TEXTURE_GREEN_SIZE"); break;
        case GL_TEXTURE_BLUE_SIZE: printf("GL_TEXTURE_BLUE_SIZE"); break;
        case GL_TEXTURE_ALPHA_SIZE: printf("GL_TEXTURE_ALPHA_SIZE"); break;
        case GL_TEXTURE_DEPTH_SIZE: printf("GL_TEXTURE_DEPTH_SIZE"); break;
        case GL_TEXTURE_COMPRESSED: printf("GL_TEXTURE_COMPRESSED"); break;
        case GL_TEXTURE_COMPRESSED_IMAGE_SIZE: printf("GL_TEXTURE_COMPRESSED_IMAGE_SIZE"); break;
        case GL_TEXTURE_BUFFER_OFFSET: printf("GL_TEXTURE_BUFFER_OFFSET"); break;
        default: printf("%04x", pname);
    }
}

void print_sampler_parameter_pname(GLenum pname) {
    switch (pname) {
        case GL_TEXTURE_MAG_FILTER: printf("GL_TEXTURE_MAG_FILTER"); break;
        case GL_TEXTURE_MIN_FILTER: printf("GL_TEXTURE_MIN_FILTER"); break;
        case GL_TEXTURE_MIN_LOD: printf("GL_TEXTURE_MIN_LOD"); break;
        case GL_TEXTURE_MAX_LOD: printf("GL_TEXTURE_MAX_LOD"); break;
        case GL_TEXTURE_LOD_BIAS: printf("GL_TEXTURE_LOD_BIAS"); break;
        case GL_TEXTURE_WRAP_S: printf("GL_TEXTURE_WRAP_S"); break;
        case GL_TEXTURE_WRAP_T: printf("GL_TEXTURE_WRAP_T"); break;
        case GL_TEXTURE_WRAP_R: printf("GL_TEXTURE_WRAP_R"); break;
        case GL_TEXTURE_BORDER_COLOR: printf("GL_TEXTURE_BORDER_COLOR"); break;
        case GL_TEXTURE_COMPARE_MODE: printf("GL_TEXTURE_COMPARE_MODE"); break;
        case GL_TEXTURE_COMPARE_FUNC: printf("GL_TEXTURE_COMPARE_FUNC"); break;
        default: printf("%04x", pname);
    }
}

void print_gl_get_pname(GLenum pname) {
    switch (pname) {
        case GL_ACTIVE_TEXTURE: printf("GL_ACTIVE_TEXTURE"); break;
        case GL_ALIASED_LINE_WIDTH_RANGE: printf("GL_ALIASED_LINE_WIDTH_RANGE"); break;
        case GL_ARRAY_BUFFER_BINDING: printf("GL_ARRAY_BUFFER_BINDING"); break;
        case GL_BLEND: printf("GL_BLEND"); break;
        case GL_BLEND_COLOR: printf("GL_BLEND_COLOR"); break;
        case GL_BLEND_DST_ALPHA: printf("GL_BLEND_DST_ALPHA"); break;
        case GL_BLEND_DST_RGB: printf("GL_BLEND_DST_RGB"); break;
        case GL_BLEND_EQUATION_RGB: printf("GL_BLEND_EQUATION_RGB"); break;
        case GL_BLEND_EQUATION_ALPHA: printf("GL_BLEND_EQUATION_ALPHA"); break;
        case GL_BLEND_SRC_ALPHA: printf("GL_BLEND_SRC_ALPHA"); break;
        case GL_BLEND_SRC_RGB: printf("GL_BLEND_SRC_RGB"); break;
        case GL_COLOR_CLEAR_VALUE: printf("GL_COLOR_CLEAR_VALUE"); break;
        case GL_COLOR_LOGIC_OP: printf("GL_COLOR_LOGIC_OP"); break;
        case GL_COLOR_WRITEMASK: printf("GL_COLOR_WRITEMASK"); break;
        case GL_COMPRESSED_TEXTURE_FORMATS: printf("GL_COMPRESSED_TEXTURE_FORMATS"); break;
        case GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS: printf("GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS"); break;
        case GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS: printf("GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS"); break;
        case GL_MAX_COMPUTE_UNIFORM_BLOCKS: printf("GL_MAX_COMPUTE_UNIFORM_BLOCKS"); break;
        case GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS: printf("GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS"); break;
        case GL_MAX_COMPUTE_IMAGE_UNIFORMS: printf("GL_MAX_COMPUTE_IMAGE_UNIFORMS"); break;
        case GL_MAX_COMPUTE_SHARED_MEMORY_SIZE: printf("GL_MAX_COMPUTE_SHARED_MEMORY_SIZE"); break;
        case GL_MAX_COMPUTE_UNIFORM_COMPONENTS: printf("GL_MAX_COMPUTE_UNIFORM_COMPONENTS"); break;
        case GL_MAX_COMPUTE_ATOMIC_COUNTERS: printf("GL_MAX_COMPUTE_ATOMIC_COUNTERS"); break;
        case GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS: printf("GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS"); break;
        case GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS: printf("GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS"); break;
        case GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS: printf("GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS"); break;
        case GL_MAX_DEBUG_MESSAGE_LENGTH: printf("GL_MAX_DEBUG_MESSAGE_LENGTH"); break;
        case GL_MAX_DEBUG_LOGGED_MESSAGES: printf("GL_MAX_DEBUG_LOGGED_MESSAGES"); break;
        case GL_MAX_COMPUTE_WORK_GROUP_COUNT: printf("GL_MAX_COMPUTE_WORK_GROUP_COUNT"); break;
        case GL_MAX_COMPUTE_WORK_GROUP_SIZE: printf("GL_MAX_COMPUTE_WORK_GROUP_SIZE"); break;
        case GL_DISPATCH_INDIRECT_BUFFER_BINDING: printf("GL_DISPATCH_INDIRECT_BUFFER_BINDING"); break;
        case GL_MAX_DEBUG_GROUP_STACK_DEPTH: printf("GL_MAX_DEBUG_GROUP_STACK_DEPTH"); break;
        case GL_DEBUG_GROUP_STACK_DEPTH: printf("GL_DEBUG_GROUP_STACK_DEPTH"); break;
        case GL_CONTEXT_FLAGS: printf("GL_CONTEXT_FLAGS"); break;
        case GL_CULL_FACE: printf("GL_CULL_FACE"); break;
        case GL_CULL_FACE_MODE: printf("GL_CULL_FACE_MODE"); break;
        case GL_CURRENT_PROGRAM: printf("GL_CURRENT_PROGRAM"); break;
        case GL_DEPTH_CLEAR_VALUE: printf("GL_DEPTH_CLEAR_VALUE"); break;
        case GL_DEPTH_FUNC: printf("GL_DEPTH_FUNC"); break;
        case GL_DEPTH_RANGE: printf("GL_DEPTH_RANGE"); break;
        case GL_DEPTH_TEST: printf("GL_DEPTH_TEST"); break;
        case GL_DEPTH_WRITEMASK: printf("GL_DEPTH_WRITEMASK"); break;
        case GL_DITHER: printf("GL_DITHER"); break;
        case GL_DOUBLEBUFFER: printf("GL_DOUBLEBUFFER"); break;
        case GL_DRAW_BUFFER: printf("GL_DRAW_BUFFER"); break;
        case GL_DRAW_FRAMEBUFFER_BINDING: printf("GL_DRAW_FRAMEBUFFER_BINDING"); break;
        case GL_READ_FRAMEBUFFER_BINDING: printf("GL_READ_FRAMEBUFFER_BINDING"); break;
        case GL_ELEMENT_ARRAY_BUFFER_BINDING: printf("GL_ELEMENT_ARRAY_BUFFER_BINDING"); break;
        case GL_FRAGMENT_SHADER_DERIVATIVE_HINT: printf("GL_FRAGMENT_SHADER_DERIVATIVE_HINT"); break;
        case GL_IMPLEMENTATION_COLOR_READ_FORMAT: printf("GL_IMPLEMENTATION_COLOR_READ_FORMAT"); break;
        case GL_IMPLEMENTATION_COLOR_READ_TYPE: printf("GL_IMPLEMENTATION_COLOR_READ_TYPE"); break;
        case GL_LINE_SMOOTH: printf("GL_LINE_SMOOTH"); break;
        case GL_LINE_SMOOTH_HINT: printf("GL_LINE_SMOOTH_HINT"); break;
        case GL_LINE_WIDTH: printf("GL_LINE_WIDTH"); break;
        case GL_LAYER_PROVOKING_VERTEX: printf("GL_LAYER_PROVOKING_VERTEX"); break;
        case GL_LOGIC_OP_MODE: printf("GL_LOGIC_OP_MODE"); break;
        case GL_MAJOR_VERSION: printf("GL_MAJOR_VERSION"); break;
        case GL_MAX_3D_TEXTURE_SIZE: printf("GL_MAX_3D_TEXTURE_SIZE"); break;
        case GL_MAX_ARRAY_TEXTURE_LAYERS: printf("GL_MAX_ARRAY_TEXTURE_LAYERS"); break;
        case GL_MAX_CLIP_DISTANCES: printf("GL_MAX_CLIP_DISTANCES"); break;
        case GL_MAX_COLOR_TEXTURE_SAMPLES: printf("GL_MAX_COLOR_TEXTURE_SAMPLES"); break;
        case GL_MAX_COMBINED_ATOMIC_COUNTERS: printf("GL_MAX_COMBINED_ATOMIC_COUNTERS"); break;
        case GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE: printf("GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE"); break;
        case GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS: printf("GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS"); break;
        case GL_MAX_IMAGE_UNITS: printf("GL_MAX_IMAGE_UNITS"); break;
        case GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS: printf("GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS"); break;
        case GL_MAX_IMAGE_SAMPLES: printf("GL_MAX_IMAGE_SAMPLES"); break;
        case GL_MAX_VERTEX_IMAGE_UNIFORMS: printf("GL_MAX_VERTEX_IMAGE_UNIFORMS"); break;
        case GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS: printf("GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS"); break;
        case GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS: printf("GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS"); break;
        case GL_MAX_GEOMETRY_IMAGE_UNIFORMS: printf("GL_MAX_GEOMETRY_IMAGE_UNIFORMS"); break;
        case GL_MAX_FRAGMENT_IMAGE_UNIFORMS: printf("GL_MAX_FRAGMENT_IMAGE_UNIFORMS"); break;
        case GL_MAX_COMBINED_IMAGE_UNIFORMS: printf("GL_MAX_COMBINED_IMAGE_UNIFORMS"); break;
        case GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS: printf("GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS"); break;
        case GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS: printf("GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS"); break;
        case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: printf("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS"); break;
        case GL_MAX_COMBINED_UNIFORM_BLOCKS: printf("GL_MAX_COMBINED_UNIFORM_BLOCKS"); break;
        case GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS: printf("GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS"); break;
        case GL_MAX_CUBE_MAP_TEXTURE_SIZE: printf("GL_MAX_CUBE_MAP_TEXTURE_SIZE"); break;
        case GL_MAX_DEPTH_TEXTURE_SAMPLES: printf("GL_MAX_DEPTH_TEXTURE_SAMPLES"); break;
        case GL_MAX_DRAW_BUFFERS: printf("GL_MAX_DRAW_BUFFERS"); break;
        case GL_MAX_DUAL_SOURCE_DRAW_BUFFERS: printf("GL_MAX_DUAL_SOURCE_DRAW_BUFFERS"); break;
        case GL_MAX_ELEMENTS_INDICES: printf("GL_MAX_ELEMENTS_INDICES"); break;
        case GL_MAX_ELEMENTS_VERTICES: printf("GL_MAX_ELEMENTS_VERTICES"); break;
        case GL_MAX_FRAGMENT_ATOMIC_COUNTERS: printf("GL_MAX_FRAGMENT_ATOMIC_COUNTERS"); break;
        case GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS: printf("GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS"); break;
        case GL_MAX_FRAGMENT_INPUT_COMPONENTS: printf("GL_MAX_FRAGMENT_INPUT_COMPONENTS"); break;
        case GL_CONTEXT_PROFILE_MASK: printf("GL_CONTEXT_PROFILE_MASK"); break;
        case GL_MAX_FRAGMENT_UNIFORM_COMPONENTS: printf("GL_MAX_FRAGMENT_UNIFORM_COMPONENTS"); break;
        case GL_MAX_FRAGMENT_UNIFORM_VECTORS: printf("GL_MAX_FRAGMENT_UNIFORM_VECTORS"); break;
        case GL_MAX_FRAGMENT_UNIFORM_BLOCKS: printf("GL_MAX_FRAGMENT_UNIFORM_BLOCKS"); break;
        case GL_MAX_FRAMEBUFFER_WIDTH: printf("GL_MAX_FRAMEBUFFER_WIDTH"); break;
        case GL_MAX_FRAMEBUFFER_HEIGHT: printf("GL_MAX_FRAMEBUFFER_HEIGHT"); break;
        case GL_MAX_FRAMEBUFFER_LAYERS: printf("GL_MAX_FRAMEBUFFER_LAYERS"); break;
        case GL_MAX_FRAMEBUFFER_SAMPLES: printf("GL_MAX_FRAMEBUFFER_SAMPLES"); break;
        case GL_MAX_GEOMETRY_ATOMIC_COUNTERS: printf("GL_MAX_GEOMETRY_ATOMIC_COUNTERS"); break;
        case GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS: printf("GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS"); break;
        case GL_MAX_GEOMETRY_INPUT_COMPONENTS: printf("GL_MAX_GEOMETRY_INPUT_COMPONENTS"); break;
        case GL_MAX_GEOMETRY_OUTPUT_COMPONENTS: printf("GL_MAX_GEOMETRY_OUTPUT_COMPONENTS"); break;
        case GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS: printf("GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS"); break;
        case GL_MAX_GEOMETRY_UNIFORM_BLOCKS: printf("GL_MAX_GEOMETRY_UNIFORM_BLOCKS"); break;
        case GL_MAX_GEOMETRY_UNIFORM_COMPONENTS: printf("GL_MAX_GEOMETRY_UNIFORM_COMPONENTS"); break;
        case GL_MAX_GEOMETRY_OUTPUT_VERTICES: printf("GL_MAX_GEOMETRY_OUTPUT_VERTICES"); break;
        case GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS: printf("GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS"); break;
        case GL_MAX_INTEGER_SAMPLES: printf("GL_MAX_INTEGER_SAMPLES"); break;
        case GL_MIN_MAP_BUFFER_ALIGNMENT: printf("GL_MIN_MAP_BUFFER_ALIGNMENT"); break;
        case GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS: printf("GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS"); break;
        case GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS: printf("GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS"); break;
        case GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS: printf("GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS"); break;
        case GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS: printf("GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS"); break;
        case GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS: printf("GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS"); break;
        case GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS: printf("GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS"); break;
        case GL_MAX_LABEL_LENGTH: printf("GL_MAX_LABEL_LENGTH"); break;
        case GL_MAX_PROGRAM_TEXEL_OFFSET: printf("GL_MAX_PROGRAM_TEXEL_OFFSET"); break;
        case GL_MIN_PROGRAM_TEXEL_OFFSET: printf("GL_MIN_PROGRAM_TEXEL_OFFSET"); break;
        case GL_MAX_RECTANGLE_TEXTURE_SIZE: printf("GL_MAX_RECTANGLE_TEXTURE_SIZE"); break;
        case GL_MAX_RENDERBUFFER_SIZE: printf("GL_MAX_RENDERBUFFER_SIZE"); break;
        case GL_MAX_COLOR_ATTACHMENTS: printf("GL_MAX_COLOR_ATTACHMENTS"); break;
        case GL_MAX_SAMPLES: printf("GL_MAX_SAMPLES"); break;
        case GL_MAX_SAMPLE_MASK_WORDS: printf("GL_MAX_SAMPLE_MASK_WORDS"); break;
        case GL_MAX_SERVER_WAIT_TIMEOUT: printf("GL_MAX_SERVER_WAIT_TIMEOUT"); break;
        case GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS: printf("GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS"); break;
        case GL_MAX_SHADER_STORAGE_BLOCK_SIZE: printf("GL_MAX_SHADER_STORAGE_BLOCK_SIZE"); break;
        case GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS: printf("GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS"); break;
        case GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS: printf("GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS"); break;
        case GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS: printf("GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS"); break;
        case GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS: printf("GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS"); break;
        case GL_MAX_TEXTURE_BUFFER_SIZE: printf("GL_MAX_TEXTURE_BUFFER_SIZE"); break;
        case GL_MAX_TEXTURE_IMAGE_UNITS: printf("GL_MAX_TEXTURE_IMAGE_UNITS"); break;
        case GL_MAX_TEXTURE_LOD_BIAS: printf("GL_MAX_TEXTURE_LOD_BIAS"); break;
        case GL_MAX_TEXTURE_SIZE: printf("GL_MAX_TEXTURE_SIZE"); break;
        case GL_MAX_UNIFORM_BUFFER_BINDINGS: printf("GL_MAX_UNIFORM_BUFFER_BINDINGS"); break;
        case GL_MAX_UNIFORM_BLOCK_SIZE: printf("GL_MAX_UNIFORM_BLOCK_SIZE"); break;
        case GL_MAX_UNIFORM_LOCATIONS: printf("GL_MAX_UNIFORM_LOCATIONS"); break;
        case GL_MAX_VARYING_COMPONENTS: printf("GL_MAX_VARYING_COMPONENTS"); break;
        case GL_MAX_VARYING_VECTORS: printf("GL_MAX_VARYING_VECTORS"); break;
        case GL_MAX_VERTEX_ATOMIC_COUNTERS: printf("GL_MAX_VERTEX_ATOMIC_COUNTERS"); break;
        case GL_MAX_VERTEX_ATTRIBS: printf("GL_MAX_VERTEX_ATTRIBS"); break;
        case GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS: printf("GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS"); break;
        case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS: printf("GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS"); break;
        case GL_MAX_VERTEX_UNIFORM_COMPONENTS: printf("GL_MAX_VERTEX_UNIFORM_COMPONENTS"); break;
        case GL_MAX_VERTEX_UNIFORM_VECTORS: printf("GL_MAX_VERTEX_UNIFORM_VECTORS"); break;
        case GL_MAX_VERTEX_OUTPUT_COMPONENTS: printf("GL_MAX_VERTEX_OUTPUT_COMPONENTS"); break;
        case GL_MAX_VERTEX_UNIFORM_BLOCKS: printf("GL_MAX_VERTEX_UNIFORM_BLOCKS"); break;
        case GL_MAX_VIEWPORT_DIMS: printf("GL_MAX_VIEWPORT_DIMS"); break;
        case GL_MAX_VIEWPORTS: printf("GL_MAX_VIEWPORTS"); break;
        case GL_MINOR_VERSION: printf("GL_MINOR_VERSION"); break;
        case GL_NUM_COMPRESSED_TEXTURE_FORMATS: printf("GL_NUM_COMPRESSED_TEXTURE_FORMATS"); break;
        case GL_NUM_EXTENSIONS: printf("GL_NUM_EXTENSIONS"); break;
        case GL_NUM_PROGRAM_BINARY_FORMATS: printf("GL_NUM_PROGRAM_BINARY_FORMATS"); break;
        case GL_NUM_SHADER_BINARY_FORMATS: printf("GL_NUM_SHADER_BINARY_FORMATS"); break;
        case GL_PACK_ALIGNMENT: printf("GL_PACK_ALIGNMENT"); break;
        case GL_PACK_IMAGE_HEIGHT: printf("GL_PACK_IMAGE_HEIGHT"); break;
        case GL_PACK_LSB_FIRST: printf("GL_PACK_LSB_FIRST"); break;
        case GL_PACK_ROW_LENGTH: printf("GL_PACK_ROW_LENGTH"); break;
        case GL_PACK_SKIP_IMAGES: printf("GL_PACK_SKIP_IMAGES"); break;
        case GL_PACK_SKIP_PIXELS: printf("GL_PACK_SKIP_PIXELS"); break;
        case GL_PACK_SKIP_ROWS: printf("GL_PACK_SKIP_ROWS"); break;
        case GL_PACK_SWAP_BYTES: printf("GL_PACK_SWAP_BYTES"); break;
        case GL_PIXEL_PACK_BUFFER_BINDING: printf("GL_PIXEL_PACK_BUFFER_BINDING"); break;
        case GL_PIXEL_UNPACK_BUFFER_BINDING: printf("GL_PIXEL_UNPACK_BUFFER_BINDING"); break;
        case GL_POINT_FADE_THRESHOLD_SIZE: printf("GL_POINT_FADE_THRESHOLD_SIZE"); break;
        case GL_PRIMITIVE_RESTART_INDEX: printf("GL_PRIMITIVE_RESTART_INDEX"); break;
        case GL_PROGRAM_BINARY_FORMATS: printf("GL_PROGRAM_BINARY_FORMATS"); break;
        case GL_PROGRAM_PIPELINE_BINDING: printf("GL_PROGRAM_PIPELINE_BINDING"); break;
        case GL_PROGRAM_POINT_SIZE: printf("GL_PROGRAM_POINT_SIZE"); break;
        case GL_PROVOKING_VERTEX: printf("GL_PROVOKING_VERTEX"); break;
        case GL_POINT_SIZE: printf("GL_POINT_SIZE"); break;
        case GL_POINT_SIZE_GRANULARITY: printf("GL_POINT_SIZE_GRANULARITY"); break;
        case GL_POINT_SIZE_RANGE: printf("GL_POINT_SIZE_RANGE"); break;
        case GL_POLYGON_OFFSET_FACTOR: printf("GL_POLYGON_OFFSET_FACTOR"); break;
        case GL_POLYGON_OFFSET_UNITS: printf("GL_POLYGON_OFFSET_UNITS"); break;
        case GL_POLYGON_OFFSET_FILL: printf("GL_POLYGON_OFFSET_FILL"); break;
        case GL_POLYGON_OFFSET_LINE: printf("GL_POLYGON_OFFSET_LINE"); break;
        case GL_POLYGON_OFFSET_POINT: printf("GL_POLYGON_OFFSET_POINT"); break;
        case GL_POLYGON_SMOOTH: printf("GL_POLYGON_SMOOTH"); break;
        case GL_POLYGON_SMOOTH_HINT: printf("GL_POLYGON_SMOOTH_HINT"); break;
        case GL_READ_BUFFER: printf("GL_READ_BUFFER"); break;
        case GL_RENDERBUFFER_BINDING: printf("GL_RENDERBUFFER_BINDING"); break;
        case GL_SAMPLE_BUFFERS: printf("GL_SAMPLE_BUFFERS"); break;
        case GL_SAMPLE_COVERAGE_VALUE: printf("GL_SAMPLE_COVERAGE_VALUE"); break;
        case GL_SAMPLE_COVERAGE_INVERT: printf("GL_SAMPLE_COVERAGE_INVERT"); break;
        case GL_SAMPLER_BINDING: printf("GL_SAMPLER_BINDING"); break;
        case GL_SAMPLES: printf("GL_SAMPLES"); break;
        case GL_SCISSOR_BOX: printf("GL_SCISSOR_BOX"); break;
        case GL_SCISSOR_TEST: printf("GL_SCISSOR_TEST"); break;
        case GL_SHADER_COMPILER: printf("GL_SHADER_COMPILER"); break;
        case GL_SHADER_STORAGE_BUFFER_BINDING: printf("GL_SHADER_STORAGE_BUFFER_BINDING"); break;
        case GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT: printf("GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT"); break;
        case GL_SHADER_STORAGE_BUFFER_START: printf("GL_SHADER_STORAGE_BUFFER_START"); break;
        case GL_SHADER_STORAGE_BUFFER_SIZE: printf("GL_SHADER_STORAGE_BUFFER_SIZE"); break;
        case GL_SMOOTH_LINE_WIDTH_RANGE: printf("GL_SMOOTH_LINE_WIDTH_RANGE"); break;
        case GL_SMOOTH_LINE_WIDTH_GRANULARITY: printf("GL_SMOOTH_LINE_WIDTH_GRANULARITY"); break;
        case GL_STENCIL_BACK_FAIL: printf("GL_STENCIL_BACK_FAIL"); break;
        case GL_STENCIL_BACK_FUNC: printf("GL_STENCIL_BACK_FUNC"); break;
        case GL_STENCIL_BACK_PASS_DEPTH_FAIL: printf("GL_STENCIL_BACK_PASS_DEPTH_FAIL"); break;
        case GL_STENCIL_BACK_PASS_DEPTH_PASS: printf("GL_STENCIL_BACK_PASS_DEPTH_PASS"); break;
        case GL_STENCIL_BACK_REF: printf("GL_STENCIL_BACK_REF"); break;
        case GL_STENCIL_BACK_VALUE_MASK: printf("GL_STENCIL_BACK_VALUE_MASK"); break;
        case GL_STENCIL_BACK_WRITEMASK: printf("GL_STENCIL_BACK_WRITEMASK"); break;
        case GL_STENCIL_CLEAR_VALUE: printf("GL_STENCIL_CLEAR_VALUE"); break;
        case GL_STENCIL_FAIL: printf("GL_STENCIL_FAIL"); break;
        case GL_STENCIL_FUNC: printf("GL_STENCIL_FUNC"); break;
        case GL_STENCIL_PASS_DEPTH_FAIL: printf("GL_STENCIL_PASS_DEPTH_FAIL"); break;
        case GL_STENCIL_PASS_DEPTH_PASS: printf("GL_STENCIL_PASS_DEPTH_PASS"); break;
        case GL_STENCIL_REF: printf("GL_STENCIL_REF"); break;
        case GL_STENCIL_TEST: printf("GL_STENCIL_TEST"); break;
        case GL_STENCIL_VALUE_MASK: printf("GL_STENCIL_VALUE_MASK"); break;
        case GL_STENCIL_WRITEMASK: printf("GL_STENCIL_WRITEMASK"); break;
        case GL_STEREO: printf("GL_STEREO"); break;
        case GL_SUBPIXEL_BITS: printf("GL_SUBPIXEL_BITS"); break;
        case GL_TEXTURE_BINDING_1D: printf("GL_TEXTURE_BINDING_1D"); break;
        case GL_TEXTURE_BINDING_1D_ARRAY: printf("GL_TEXTURE_BINDING_1D_ARRAY"); break;
        case GL_TEXTURE_BINDING_2D: printf("GL_TEXTURE_BINDING_2D"); break;
        case GL_TEXTURE_BINDING_2D_ARRAY: printf("GL_TEXTURE_BINDING_2D_ARRAY"); break;
        case GL_TEXTURE_BINDING_2D_MULTISAMPLE: printf("GL_TEXTURE_BINDING_2D_MULTISAMPLE"); break;
        case GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY: printf("GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY"); break;
        case GL_TEXTURE_BINDING_3D: printf("GL_TEXTURE_BINDING_3D"); break;
        case GL_TEXTURE_BINDING_BUFFER: printf("GL_TEXTURE_BINDING_BUFFER"); break;
        case GL_TEXTURE_BINDING_CUBE_MAP: printf("GL_TEXTURE_BINDING_CUBE_MAP"); break;
        case GL_TEXTURE_BINDING_RECTANGLE: printf("GL_TEXTURE_BINDING_RECTANGLE"); break;
        case GL_TEXTURE_COMPRESSION_HINT: printf("GL_TEXTURE_COMPRESSION_HINT"); break;
        case GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT: printf("GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT"); break;
        case GL_TIMESTAMP: printf("GL_TIMESTAMP"); break;
        case GL_TRANSFORM_FEEDBACK_BUFFER_BINDING: printf("GL_TRANSFORM_FEEDBACK_BUFFER_BINDING"); break;
        case GL_TRANSFORM_FEEDBACK_BUFFER_START: printf("GL_TRANSFORM_FEEDBACK_BUFFER_START"); break;
        case GL_TRANSFORM_FEEDBACK_BUFFER_SIZE: printf("GL_TRANSFORM_FEEDBACK_BUFFER_SIZE"); break;
        case GL_UNIFORM_BUFFER_BINDING: printf("GL_UNIFORM_BUFFER_BINDING"); break;
        case GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT: printf("GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT"); break;
        case GL_UNIFORM_BUFFER_SIZE: printf("GL_UNIFORM_BUFFER_SIZE"); break;
        case GL_UNIFORM_BUFFER_START: printf("GL_UNIFORM_BUFFER_START"); break;
        case GL_UNPACK_ALIGNMENT: printf("GL_UNPACK_ALIGNMENT"); break;
        case GL_UNPACK_IMAGE_HEIGHT: printf("GL_UNPACK_IMAGE_HEIGHT"); break;
        case GL_UNPACK_LSB_FIRST: printf("GL_UNPACK_LSB_FIRST"); break;
        case GL_UNPACK_ROW_LENGTH: printf("GL_UNPACK_ROW_LENGTH"); break;
        case GL_UNPACK_SKIP_IMAGES: printf("GL_UNPACK_SKIP_IMAGES"); break;
        case GL_UNPACK_SKIP_PIXELS: printf("GL_UNPACK_SKIP_PIXELS"); break;
        case GL_UNPACK_SKIP_ROWS: printf("GL_UNPACK_SKIP_ROWS"); break;
        case GL_UNPACK_SWAP_BYTES: printf("GL_UNPACK_SWAP_BYTES"); break;
        case GL_VERTEX_ARRAY_BINDING: printf("GL_VERTEX_ARRAY_BINDING"); break;
        case GL_VERTEX_BINDING_DIVISOR: printf("GL_VERTEX_BINDING_DIVISOR"); break;
        case GL_VERTEX_BINDING_OFFSET: printf("GL_VERTEX_BINDING_OFFSET"); break;
        case GL_VERTEX_BINDING_STRIDE: printf("GL_VERTEX_BINDING_STRIDE"); break;
        case GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET: printf("GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET"); break;
        case GL_MAX_VERTEX_ATTRIB_BINDINGS: printf("GL_MAX_VERTEX_ATTRIB_BINDINGS"); break;
        case GL_MAX_VERTEX_ATTRIB_STRIDE: printf("GL_MAX_VERTEX_ATTRIB_STRIDE"); break;
        case GL_MAX_CULL_DISTANCES: printf("GL_MAX_CULL_DISTANCES"); break;
        case GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES: printf("GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES"); break;
        case GL_VIEWPORT: printf("GL_VIEWPORT"); break;
        case GL_VIEWPORT_BOUNDS_RANGE: printf("GL_VIEWPORT_BOUNDS_RANGE"); break;
        case GL_VIEWPORT_INDEX_PROVOKING_VERTEX: printf("GL_VIEWPORT_INDEX_PROVOKING_VERTEX"); break;
        case GL_VIEWPORT_SUBPIXEL_BITS: printf("GL_VIEWPORT_SUBPIXEL_BITS"); break;
        case GL_MAX_ELEMENT_INDEX: printf("GL_MAX_ELEMENT_INDEX"); break;
        default: printf("%04x", pname);
    }
}

extern "C" void GLAPI CullFace(GLenum mode) {
    printf("glCullFace(");
    printf("mode=");
    print_cull_face(mode);
    printf(")");
    gl.CullFace(mode);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI FrontFace(GLenum mode) {
    printf("glFrontFace(");
    printf("mode=");
    printf("0x%04x", mode);
    printf(")");
    gl.FrontFace(mode);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Hint(GLenum target, GLenum mode) {
    printf("glHint(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("mode=");
    printf("0x%04x", mode);
    printf(")");
    gl.Hint(target, mode);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI LineWidth(GLfloat width) {
    printf("glLineWidth(");
    printf("width=");
    printf("%f", width);
    printf(")");
    gl.LineWidth(width);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PointSize(GLfloat size) {
    printf("glPointSize(");
    printf("size=");
    printf("%f", size);
    printf(")");
    gl.PointSize(size);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PolygonMode(GLenum face, GLenum mode) {
    printf("glPolygonMode(");
    printf("face=");
    print_cull_face(face);
    printf(", ");
    printf("mode=");
    printf("0x%04x", mode);
    printf(")");
    gl.PolygonMode(face, mode);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Scissor(GLint x, GLint y, GLsizei width, GLsizei height) {
    printf("glScissor(");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.Scissor(x, y, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexParameterf(GLenum target, GLenum pname, GLfloat param) {
    printf("glTexParameterf(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("pname=");
    print_tex_parameter_pname(pname);
    printf(", ");
    printf("param=");
    printf("%f", param);
    printf(")");
    gl.TexParameterf(target, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexParameterfv(GLenum target, GLenum pname, const GLfloat * params) {
    printf("glTexParameterfv(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("pname=");
    print_tex_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.TexParameterfv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexParameteri(GLenum target, GLenum pname, GLint param) {
    printf("glTexParameteri(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("pname=");
    print_tex_parameter_pname(pname);
    printf(", ");
    printf("param=");
    printf("%d", param);
    printf(")");
    gl.TexParameteri(target, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexParameteriv(GLenum target, GLenum pname, const GLint * params) {
    printf("glTexParameteriv(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("pname=");
    print_tex_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.TexParameteriv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels) {
    printf("glTexImage1D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("internalformat=");
    printf("%d", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("border=");
    printf("%d", border);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.TexImage1D(target, level, internalformat, width, border, format, type, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels) {
    printf("glTexImage2D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("internalformat=");
    printf("%d", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("border=");
    printf("%d", border);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.TexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawBuffer(GLenum buf) {
    printf("glDrawBuffer(");
    printf("buf=");
    printf("0x%04x", buf);
    printf(")");
    gl.DrawBuffer(buf);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Clear(GLbitfield mask) {
    printf("glClear(");
    printf("mask=");
    printf("0x%08x", mask);
    printf(")");
    gl.Clear(mask);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    printf("glClearColor(");
    printf("red=");
    printf("%f", red);
    printf(", ");
    printf("green=");
    printf("%f", green);
    printf(", ");
    printf("blue=");
    printf("%f", blue);
    printf(", ");
    printf("alpha=");
    printf("%f", alpha);
    printf(")");
    gl.ClearColor(red, green, blue, alpha);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearStencil(GLint s) {
    printf("glClearStencil(");
    printf("s=");
    printf("%d", s);
    printf(")");
    gl.ClearStencil(s);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearDepth(GLdouble depth) {
    printf("glClearDepth(");
    printf("depth=");
    printf("%lf", depth);
    printf(")");
    gl.ClearDepth(depth);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI StencilMask(GLuint mask) {
    printf("glStencilMask(");
    printf("mask=");
    printf("%u", mask);
    printf(")");
    gl.StencilMask(mask);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {
    printf("glColorMask(");
    printf("red=");
    printf("%s", red ? "true" : "false");
    printf(", ");
    printf("green=");
    printf("%s", green ? "true" : "false");
    printf(", ");
    printf("blue=");
    printf("%s", blue ? "true" : "false");
    printf(", ");
    printf("alpha=");
    printf("%s", alpha ? "true" : "false");
    printf(")");
    gl.ColorMask(red, green, blue, alpha);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DepthMask(GLboolean flag) {
    printf("glDepthMask(");
    printf("flag=");
    printf("%s", flag ? "true" : "false");
    printf(")");
    gl.DepthMask(flag);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Disable(GLenum cap) {
    printf("glDisable(");
    printf("cap=");
    print_enable_flag(cap);
    printf(")");
    gl.Disable(cap);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Enable(GLenum cap) {
    printf("glEnable(");
    printf("cap=");
    print_enable_flag(cap);
    printf(")");
    gl.Enable(cap);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Finish() {
    printf("glFinish(");
    printf(")");
    gl.Finish();
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Flush() {
    printf("glFlush(");
    printf(")");
    gl.Flush();
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BlendFunc(GLenum sfactor, GLenum dfactor) {
    printf("glBlendFunc(");
    printf("sfactor=");
    printf("0x%04x", sfactor);
    printf(", ");
    printf("dfactor=");
    printf("0x%04x", dfactor);
    printf(")");
    gl.BlendFunc(sfactor, dfactor);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI LogicOp(GLenum opcode) {
    printf("glLogicOp(");
    printf("opcode=");
    printf("0x%04x", opcode);
    printf(")");
    gl.LogicOp(opcode);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI StencilFunc(GLenum func, GLint ref, GLuint mask) {
    printf("glStencilFunc(");
    printf("func=");
    printf("0x%04x", func);
    printf(", ");
    printf("ref=");
    printf("%d", ref);
    printf(", ");
    printf("mask=");
    printf("%u", mask);
    printf(")");
    gl.StencilFunc(func, ref, mask);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI StencilOp(GLenum fail, GLenum zfail, GLenum zpass) {
    printf("glStencilOp(");
    printf("fail=");
    printf("0x%04x", fail);
    printf(", ");
    printf("zfail=");
    printf("0x%04x", zfail);
    printf(", ");
    printf("zpass=");
    printf("0x%04x", zpass);
    printf(")");
    gl.StencilOp(fail, zfail, zpass);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DepthFunc(GLenum func) {
    printf("glDepthFunc(");
    printf("func=");
    printf("0x%04x", func);
    printf(")");
    gl.DepthFunc(func);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PixelStoref(GLenum pname, GLfloat param) {
    printf("glPixelStoref(");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%f", param);
    printf(")");
    gl.PixelStoref(pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PixelStorei(GLenum pname, GLint param) {
    printf("glPixelStorei(");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%d", param);
    printf(")");
    gl.PixelStorei(pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ReadBuffer(GLenum src) {
    printf("glReadBuffer(");
    printf("src=");
    printf("0x%04x", src);
    printf(")");
    gl.ReadBuffer(src);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * pixels) {
    printf("glReadPixels(");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.ReadPixels(x, y, width, height, format, type, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetBooleanv(GLenum pname, GLboolean * data) {
    printf("glGetBooleanv(");
    printf("pname=");
    print_gl_get_pname(pname);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.GetBooleanv(pname, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetDoublev(GLenum pname, GLdouble * data) {
    printf("glGetDoublev(");
    printf("pname=");
    print_gl_get_pname(pname);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.GetDoublev(pname, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLenum GLAPI GetError() {
    printf("glGetError(");
    printf(")");
    printf(" -> ");
    GLenum result = gl.GetError();
    print_gl_error(result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI GetFloatv(GLenum pname, GLfloat * data) {
    printf("glGetFloatv(");
    printf("pname=");
    print_gl_get_pname(pname);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.GetFloatv(pname, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetIntegerv(GLenum pname, GLint * data) {
    printf("glGetIntegerv(");
    printf("pname=");
    print_gl_get_pname(pname);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.GetIntegerv(pname, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" const GLubyte * GLAPI GetString(GLenum name) {
    printf("glGetString(");
    printf("name=");
    print_gl_get_pname(name);
    printf(")");
    printf(" -> ");
    const GLubyte * result = gl.GetString(name);
    printf("%p", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, void * pixels) {
    printf("glGetTexImage(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.GetTexImage(target, level, format, type, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTexParameterfv(GLenum target, GLenum pname, GLfloat * params) {
    printf("glGetTexParameterfv(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("pname=");
    print_tex_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetTexParameterfv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTexParameteriv(GLenum target, GLenum pname, GLint * params) {
    printf("glGetTexParameteriv(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("pname=");
    print_tex_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetTexParameteriv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat * params) {
    printf("glGetTexLevelParameterfv(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("pname=");
    print_tex_level_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetTexLevelParameterfv(target, level, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint * params) {
    printf("glGetTexLevelParameteriv(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("pname=");
    print_tex_level_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetTexLevelParameteriv(target, level, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLboolean GLAPI IsEnabled(GLenum cap) {
    printf("glIsEnabled(");
    printf("cap=");
    print_enable_flag(cap);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsEnabled(cap);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI DepthRange(GLdouble n, GLdouble f) {
    printf("glDepthRange(");
    printf("n=");
    printf("%lf", n);
    printf(", ");
    printf("f=");
    printf("%lf", f);
    printf(")");
    gl.DepthRange(n, f);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    printf("glViewport(");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.Viewport(x, y, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawArrays(GLenum mode, GLint first, GLsizei count) {
    printf("glDrawArrays(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("first=");
    printf("%d", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(")");
    gl.DrawArrays(mode, first, count);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawElements(GLenum mode, GLsizei count, GLenum type, const void * indices) {
    printf("glDrawElements(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indices=");
    printf("%p", indices);
    printf(")");
    gl.DrawElements(mode, count, type, indices);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetPointerv(GLenum pname, void ** params) {
    printf("glGetPointerv(");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetPointerv(pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PolygonOffset(GLfloat factor, GLfloat units) {
    printf("glPolygonOffset(");
    printf("factor=");
    printf("%f", factor);
    printf(", ");
    printf("units=");
    printf("%f", units);
    printf(")");
    gl.PolygonOffset(factor, units);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border) {
    printf("glCopyTexImage1D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("border=");
    printf("%d", border);
    printf(")");
    gl.CopyTexImage1D(target, level, internalformat, x, y, width, border);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
    printf("glCopyTexImage2D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("border=");
    printf("%d", border);
    printf(")");
    gl.CopyTexImage2D(target, level, internalformat, x, y, width, height, border);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {
    printf("glCopyTexSubImage1D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(")");
    gl.CopyTexSubImage1D(target, level, xoffset, x, y, width);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
    printf("glCopyTexSubImage2D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels) {
    printf("glTexSubImage1D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.TexSubImage1D(target, level, xoffset, width, format, type, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels) {
    printf("glTexSubImage2D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.TexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindTexture(GLenum target, GLuint texture) {
    printf("glBindTexture(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("texture=");
    printf("%u", texture);
    printf(")");
    gl.BindTexture(target, texture);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DeleteTextures(GLsizei n, const GLuint * textures) {
    printf("glDeleteTextures(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("textures=");
    printf("%p", textures);
    printf(")");
    gl.DeleteTextures(n, textures);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GenTextures(GLsizei n, GLuint * textures) {
    printf("glGenTextures(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("textures=");
    printf("%p", textures);
    printf(")");
    gl.GenTextures(n, textures);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLboolean GLAPI IsTexture(GLuint texture) {
    printf("glIsTexture(");
    printf("texture=");
    printf("%u", texture);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsTexture(texture);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices) {
    printf("glDrawRangeElements(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("start=");
    printf("%u", start);
    printf(", ");
    printf("end=");
    printf("%u", end);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indices=");
    printf("%p", indices);
    printf(")");
    gl.DrawRangeElements(mode, start, end, count, type, indices);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels) {
    printf("glTexImage3D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("internalformat=");
    printf("%d", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(", ");
    printf("border=");
    printf("%d", border);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.TexImage3D(target, level, internalformat, width, height, depth, border, format, type, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels) {
    printf("glTexSubImage3D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("zoffset=");
    printf("%d", zoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.TexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
    printf("glCopyTexSubImage3D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("zoffset=");
    printf("%d", zoffset);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ActiveTexture(GLenum texture) {
    printf("glActiveTexture(");
    printf("texture=");
    printf("GL_TEXTURE%d", texture - GL_TEXTURE0);
    printf(")");
    gl.ActiveTexture(texture);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI SampleCoverage(GLfloat value, GLboolean invert) {
    printf("glSampleCoverage(");
    printf("value=");
    printf("%f", value);
    printf(", ");
    printf("invert=");
    printf("%s", invert ? "true" : "false");
    printf(")");
    gl.SampleCoverage(value, invert);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * data) {
    printf("glCompressedTexImage3D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(", ");
    printf("border=");
    printf("%d", border);
    printf(", ");
    printf("imageSize=");
    printf("%d", imageSize);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.CompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data) {
    printf("glCompressedTexImage2D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("border=");
    printf("%d", border);
    printf(", ");
    printf("imageSize=");
    printf("%d", imageSize);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.CompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * data) {
    printf("glCompressedTexImage1D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("border=");
    printf("%d", border);
    printf(", ");
    printf("imageSize=");
    printf("%d", imageSize);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.CompressedTexImage1D(target, level, internalformat, width, border, imageSize, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data) {
    printf("glCompressedTexSubImage3D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("zoffset=");
    printf("%d", zoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("imageSize=");
    printf("%d", imageSize);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data) {
    printf("glCompressedTexSubImage2D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("imageSize=");
    printf("%d", imageSize);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.CompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data) {
    printf("glCompressedTexSubImage1D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("imageSize=");
    printf("%d", imageSize);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.CompressedTexSubImage1D(target, level, xoffset, width, format, imageSize, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetCompressedTexImage(GLenum target, GLint level, void * img) {
    printf("glGetCompressedTexImage(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("img=");
    printf("%p", img);
    printf(")");
    gl.GetCompressedTexImage(target, level, img);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) {
    printf("glBlendFuncSeparate(");
    printf("sfactorRGB=");
    printf("0x%04x", sfactorRGB);
    printf(", ");
    printf("dfactorRGB=");
    printf("0x%04x", dfactorRGB);
    printf(", ");
    printf("sfactorAlpha=");
    printf("0x%04x", sfactorAlpha);
    printf(", ");
    printf("dfactorAlpha=");
    printf("0x%04x", dfactorAlpha);
    printf(")");
    gl.BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI MultiDrawArrays(GLenum mode, const GLint * first, const GLsizei * count, GLsizei drawcount) {
    printf("glMultiDrawArrays(");
    printf("mode=");
    printf("0x%04x", mode);
    printf(", ");
    printf("first=");
    printf("%p", first);
    printf(", ");
    printf("count=");
    printf("%p", count);
    printf(", ");
    printf("drawcount=");
    printf("%d", drawcount);
    printf(")");
    gl.MultiDrawArrays(mode, first, count, drawcount);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI MultiDrawElements(GLenum mode, const GLsizei * count, GLenum type, const void * const * indices, GLsizei drawcount) {
    printf("glMultiDrawElements(");
    printf("mode=");
    printf("0x%04x", mode);
    printf(", ");
    printf("count=");
    printf("%p", count);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indices=");
    printf("%p", indices);
    printf(", ");
    printf("drawcount=");
    printf("%d", drawcount);
    printf(")");
    gl.MultiDrawElements(mode, count, type, indices, drawcount);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PointParameterf(GLenum pname, GLfloat param) {
    printf("glPointParameterf(");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%f", param);
    printf(")");
    gl.PointParameterf(pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PointParameterfv(GLenum pname, const GLfloat * params) {
    printf("glPointParameterfv(");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.PointParameterfv(pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PointParameteri(GLenum pname, GLint param) {
    printf("glPointParameteri(");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%d", param);
    printf(")");
    gl.PointParameteri(pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PointParameteriv(GLenum pname, const GLint * params) {
    printf("glPointParameteriv(");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.PointParameteriv(pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    printf("glBlendColor(");
    printf("red=");
    printf("%f", red);
    printf(", ");
    printf("green=");
    printf("%f", green);
    printf(", ");
    printf("blue=");
    printf("%f", blue);
    printf(", ");
    printf("alpha=");
    printf("%f", alpha);
    printf(")");
    gl.BlendColor(red, green, blue, alpha);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BlendEquation(GLenum mode) {
    printf("glBlendEquation(");
    printf("mode=");
    printf("0x%04x", mode);
    printf(")");
    gl.BlendEquation(mode);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GenQueries(GLsizei n, GLuint * ids) {
    printf("glGenQueries(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("ids=");
    printf("%p", ids);
    printf(")");
    gl.GenQueries(n, ids);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DeleteQueries(GLsizei n, const GLuint * ids) {
    printf("glDeleteQueries(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("ids=");
    printf("%p", ids);
    printf(")");
    gl.DeleteQueries(n, ids);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLboolean GLAPI IsQuery(GLuint id) {
    printf("glIsQuery(");
    printf("id=");
    printf("%u", id);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsQuery(id);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI BeginQuery(GLenum target, GLuint id) {
    printf("glBeginQuery(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("id=");
    printf("%u", id);
    printf(")");
    gl.BeginQuery(target, id);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI EndQuery(GLenum target) {
    printf("glEndQuery(");
    printf("target=");
    printf("0x%04x", target);
    printf(")");
    gl.EndQuery(target);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetQueryiv(GLenum target, GLenum pname, GLint * params) {
    printf("glGetQueryiv(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetQueryiv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetQueryObjectiv(GLuint id, GLenum pname, GLint * params) {
    printf("glGetQueryObjectiv(");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetQueryObjectiv(id, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetQueryObjectuiv(GLuint id, GLenum pname, GLuint * params) {
    printf("glGetQueryObjectuiv(");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetQueryObjectuiv(id, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindBuffer(GLenum target, GLuint buffer) {
    printf("glBindBuffer(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(")");
    gl.BindBuffer(target, buffer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DeleteBuffers(GLsizei n, const GLuint * buffers) {
    printf("glDeleteBuffers(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("buffers=");
    printf("%p", buffers);
    printf(")");
    gl.DeleteBuffers(n, buffers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GenBuffers(GLsizei n, GLuint * buffers) {
    printf("glGenBuffers(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("buffers=");
    printf("%p", buffers);
    printf(")");
    gl.GenBuffers(n, buffers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLboolean GLAPI IsBuffer(GLuint buffer) {
    printf("glIsBuffer(");
    printf("buffer=");
    printf("%u", buffer);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsBuffer(buffer);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI BufferData(GLenum target, GLsizeiptr size, const void * data, GLenum usage) {
    printf("glBufferData(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(", ");
    printf("usage=");
    printf("0x%04x", usage);
    printf(")");
    gl.BufferData(target, size, data, usage);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void * data) {
    printf("glBufferSubData(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.BufferSubData(target, offset, size, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, void * data) {
    printf("glGetBufferSubData(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.GetBufferSubData(target, offset, size, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void * GLAPI MapBuffer(GLenum target, GLenum access) {
    printf("glMapBuffer(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("access=");
    print_buffer_access_mode(access);
    printf(")");
    printf(" -> ");
    void * result = gl.MapBuffer(target, access);
    printf("%p", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" GLboolean GLAPI UnmapBuffer(GLenum target) {
    printf("glUnmapBuffer(");
    printf("target=");
    print_buffer_target(target);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.UnmapBuffer(target);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI GetBufferParameteriv(GLenum target, GLenum pname, GLint * params) {
    printf("glGetBufferParameteriv(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetBufferParameteriv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetBufferPointerv(GLenum target, GLenum pname, void ** params) {
    printf("glGetBufferPointerv(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetBufferPointerv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) {
    printf("glBlendEquationSeparate(");
    printf("modeRGB=");
    printf("0x%04x", modeRGB);
    printf(", ");
    printf("modeAlpha=");
    printf("0x%04x", modeAlpha);
    printf(")");
    gl.BlendEquationSeparate(modeRGB, modeAlpha);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawBuffers(GLsizei n, const GLenum * bufs) {
    printf("glDrawBuffers(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("bufs=");
    printf("%p", bufs);
    printf(")");
    gl.DrawBuffers(n, bufs);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) {
    printf("glStencilOpSeparate(");
    printf("face=");
    printf("0x%04x", face);
    printf(", ");
    printf("sfail=");
    printf("0x%04x", sfail);
    printf(", ");
    printf("dpfail=");
    printf("0x%04x", dpfail);
    printf(", ");
    printf("dppass=");
    printf("0x%04x", dppass);
    printf(")");
    gl.StencilOpSeparate(face, sfail, dpfail, dppass);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask) {
    printf("glStencilFuncSeparate(");
    printf("face=");
    printf("0x%04x", face);
    printf(", ");
    printf("func=");
    printf("0x%04x", func);
    printf(", ");
    printf("ref=");
    printf("%d", ref);
    printf(", ");
    printf("mask=");
    printf("%u", mask);
    printf(")");
    gl.StencilFuncSeparate(face, func, ref, mask);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI StencilMaskSeparate(GLenum face, GLuint mask) {
    printf("glStencilMaskSeparate(");
    printf("face=");
    printf("0x%04x", face);
    printf(", ");
    printf("mask=");
    printf("%u", mask);
    printf(")");
    gl.StencilMaskSeparate(face, mask);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI AttachShader(GLuint program, GLuint shader) {
    printf("glAttachShader(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("shader=");
    printf("%u", shader);
    printf(")");
    gl.AttachShader(program, shader);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindAttribLocation(GLuint program, GLuint index, const GLchar * name) {
    printf("glBindAttribLocation(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    gl.BindAttribLocation(program, index, name);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CompileShader(GLuint shader) {
    printf("glCompileShader(");
    printf("shader=");
    printf("%u", shader);
    printf(")");
    gl.CompileShader(shader);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLuint GLAPI CreateProgram() {
    printf("glCreateProgram(");
    printf(")");
    printf(" -> ");
    GLuint result = gl.CreateProgram();
    printf("%u", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" GLuint GLAPI CreateShader(GLenum type) {
    printf("glCreateShader(");
    printf("type=");
    print_shader(type);
    printf(")");
    printf(" -> ");
    GLuint result = gl.CreateShader(type);
    printf("%u", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI DeleteProgram(GLuint program) {
    printf("glDeleteProgram(");
    printf("program=");
    printf("%u", program);
    printf(")");
    gl.DeleteProgram(program);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DeleteShader(GLuint shader) {
    printf("glDeleteShader(");
    printf("shader=");
    printf("%u", shader);
    printf(")");
    gl.DeleteShader(shader);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DetachShader(GLuint program, GLuint shader) {
    printf("glDetachShader(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("shader=");
    printf("%u", shader);
    printf(")");
    gl.DetachShader(program, shader);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DisableVertexAttribArray(GLuint index) {
    printf("glDisableVertexAttribArray(");
    printf("index=");
    printf("%u", index);
    printf(")");
    gl.DisableVertexAttribArray(index);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI EnableVertexAttribArray(GLuint index) {
    printf("glEnableVertexAttribArray(");
    printf("index=");
    printf("%u", index);
    printf(")");
    gl.EnableVertexAttribArray(index);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name) {
    printf("glGetActiveAttrib(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("size=");
    printf("%p", size);
    printf(", ");
    printf("type=");
    printf("%p", type);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    gl.GetActiveAttrib(program, index, bufSize, length, size, type, name);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name) {
    printf("glGetActiveUniform(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("size=");
    printf("%p", size);
    printf(", ");
    printf("type=");
    printf("%p", type);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    gl.GetActiveUniform(program, index, bufSize, length, size, type, name);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei * count, GLuint * shaders) {
    printf("glGetAttachedShaders(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("maxCount=");
    printf("%d", maxCount);
    printf(", ");
    printf("count=");
    printf("%p", count);
    printf(", ");
    printf("shaders=");
    printf("%p", shaders);
    printf(")");
    gl.GetAttachedShaders(program, maxCount, count, shaders);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLint GLAPI GetAttribLocation(GLuint program, const GLchar * name) {
    printf("glGetAttribLocation(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    printf(" -> ");
    GLint result = gl.GetAttribLocation(program, name);
    printf("%d", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI GetProgramiv(GLuint program, GLenum pname, GLint * params) {
    printf("glGetProgramiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("pname=");
    print_program_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetProgramiv(program, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog) {
    printf("glGetProgramInfoLog(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("infoLog=");
    printf("%p", infoLog);
    printf(")");
    gl.GetProgramInfoLog(program, bufSize, length, infoLog);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetShaderiv(GLuint shader, GLenum pname, GLint * params) {
    printf("glGetShaderiv(");
    printf("shader=");
    printf("%u", shader);
    printf(", ");
    printf("pname=");
    print_shader_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetShaderiv(shader, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog) {
    printf("glGetShaderInfoLog(");
    printf("shader=");
    printf("%u", shader);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("infoLog=");
    printf("%p", infoLog);
    printf(")");
    gl.GetShaderInfoLog(shader, bufSize, length, infoLog);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetShaderSource(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source) {
    printf("glGetShaderSource(");
    printf("shader=");
    printf("%u", shader);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("source=");
    printf("%p", source);
    printf(")");
    gl.GetShaderSource(shader, bufSize, length, source);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLint GLAPI GetUniformLocation(GLuint program, const GLchar * name) {
    printf("glGetUniformLocation(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    printf(" -> ");
    GLint result = gl.GetUniformLocation(program, name);
    printf("%d", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI GetUniformfv(GLuint program, GLint location, GLfloat * params) {
    printf("glGetUniformfv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetUniformfv(program, location, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetUniformiv(GLuint program, GLint location, GLint * params) {
    printf("glGetUniformiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetUniformiv(program, location, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetVertexAttribdv(GLuint index, GLenum pname, GLdouble * params) {
    printf("glGetVertexAttribdv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetVertexAttribdv(index, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetVertexAttribfv(GLuint index, GLenum pname, GLfloat * params) {
    printf("glGetVertexAttribfv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetVertexAttribfv(index, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetVertexAttribiv(GLuint index, GLenum pname, GLint * params) {
    printf("glGetVertexAttribiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetVertexAttribiv(index, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetVertexAttribPointerv(GLuint index, GLenum pname, void ** pointer) {
    printf("glGetVertexAttribPointerv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("pointer=");
    printf("%p", pointer);
    printf(")");
    gl.GetVertexAttribPointerv(index, pname, pointer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLboolean GLAPI IsProgram(GLuint program) {
    printf("glIsProgram(");
    printf("program=");
    printf("%u", program);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsProgram(program);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" GLboolean GLAPI IsShader(GLuint shader) {
    printf("glIsShader(");
    printf("shader=");
    printf("%u", shader);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsShader(shader);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI LinkProgram(GLuint program) {
    printf("glLinkProgram(");
    printf("program=");
    printf("%u", program);
    printf(")");
    gl.LinkProgram(program);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ShaderSource(GLuint shader, GLsizei count, const GLchar * const * string, const GLint * length) {
    printf("glShaderSource(");
    printf("shader=");
    printf("%u", shader);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("string=");
    printf("%p", string);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(")");
    gl.ShaderSource(shader, count, string, length);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UseProgram(GLuint program) {
    printf("glUseProgram(");
    printf("program=");
    printf("%u", program);
    printf(")");
    gl.UseProgram(program);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform1f(GLint location, GLfloat v0) {
    printf("glUniform1f(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%f", v0);
    printf(")");
    gl.Uniform1f(location, v0);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform2f(GLint location, GLfloat v0, GLfloat v1) {
    printf("glUniform2f(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%f", v0);
    printf(", ");
    printf("v1=");
    printf("%f", v1);
    printf(")");
    gl.Uniform2f(location, v0, v1);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
    printf("glUniform3f(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%f", v0);
    printf(", ");
    printf("v1=");
    printf("%f", v1);
    printf(", ");
    printf("v2=");
    printf("%f", v2);
    printf(")");
    gl.Uniform3f(location, v0, v1, v2);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    printf("glUniform4f(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%f", v0);
    printf(", ");
    printf("v1=");
    printf("%f", v1);
    printf(", ");
    printf("v2=");
    printf("%f", v2);
    printf(", ");
    printf("v3=");
    printf("%f", v3);
    printf(")");
    gl.Uniform4f(location, v0, v1, v2, v3);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform1i(GLint location, GLint v0) {
    printf("glUniform1i(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%d", v0);
    printf(")");
    gl.Uniform1i(location, v0);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform2i(GLint location, GLint v0, GLint v1) {
    printf("glUniform2i(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%d", v0);
    printf(", ");
    printf("v1=");
    printf("%d", v1);
    printf(")");
    gl.Uniform2i(location, v0, v1);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform3i(GLint location, GLint v0, GLint v1, GLint v2) {
    printf("glUniform3i(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%d", v0);
    printf(", ");
    printf("v1=");
    printf("%d", v1);
    printf(", ");
    printf("v2=");
    printf("%d", v2);
    printf(")");
    gl.Uniform3i(location, v0, v1, v2);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
    printf("glUniform4i(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%d", v0);
    printf(", ");
    printf("v1=");
    printf("%d", v1);
    printf(", ");
    printf("v2=");
    printf("%d", v2);
    printf(", ");
    printf("v3=");
    printf("%d", v3);
    printf(")");
    gl.Uniform4i(location, v0, v1, v2, v3);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform1fv(GLint location, GLsizei count, const GLfloat * value) {
    printf("glUniform1fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform1fv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform2fv(GLint location, GLsizei count, const GLfloat * value) {
    printf("glUniform2fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform2fv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform3fv(GLint location, GLsizei count, const GLfloat * value) {
    printf("glUniform3fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform3fv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform4fv(GLint location, GLsizei count, const GLfloat * value) {
    printf("glUniform4fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform4fv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform1iv(GLint location, GLsizei count, const GLint * value) {
    printf("glUniform1iv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform1iv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform2iv(GLint location, GLsizei count, const GLint * value) {
    printf("glUniform2iv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform2iv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform3iv(GLint location, GLsizei count, const GLint * value) {
    printf("glUniform3iv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform3iv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform4iv(GLint location, GLsizei count, const GLint * value) {
    printf("glUniform4iv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform4iv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glUniformMatrix2fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix2fv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glUniformMatrix3fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix3fv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glUniformMatrix4fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix4fv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ValidateProgram(GLuint program) {
    printf("glValidateProgram(");
    printf("program=");
    printf("%u", program);
    printf(")");
    gl.ValidateProgram(program);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib1d(GLuint index, GLdouble x) {
    printf("glVertexAttrib1d(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%lf", x);
    printf(")");
    gl.VertexAttrib1d(index, x);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib1dv(GLuint index, const GLdouble * v) {
    printf("glVertexAttrib1dv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib1dv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib1f(GLuint index, GLfloat x) {
    printf("glVertexAttrib1f(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%f", x);
    printf(")");
    gl.VertexAttrib1f(index, x);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib1fv(GLuint index, const GLfloat * v) {
    printf("glVertexAttrib1fv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib1fv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib1s(GLuint index, GLshort x) {
    printf("glVertexAttrib1s(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(")");
    gl.VertexAttrib1s(index, x);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib1sv(GLuint index, const GLshort * v) {
    printf("glVertexAttrib1sv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib1sv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib2d(GLuint index, GLdouble x, GLdouble y) {
    printf("glVertexAttrib2d(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%lf", x);
    printf(", ");
    printf("y=");
    printf("%lf", y);
    printf(")");
    gl.VertexAttrib2d(index, x, y);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib2dv(GLuint index, const GLdouble * v) {
    printf("glVertexAttrib2dv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib2dv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib2f(GLuint index, GLfloat x, GLfloat y) {
    printf("glVertexAttrib2f(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%f", x);
    printf(", ");
    printf("y=");
    printf("%f", y);
    printf(")");
    gl.VertexAttrib2f(index, x, y);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib2fv(GLuint index, const GLfloat * v) {
    printf("glVertexAttrib2fv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib2fv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib2s(GLuint index, GLshort x, GLshort y) {
    printf("glVertexAttrib2s(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(")");
    gl.VertexAttrib2s(index, x, y);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib2sv(GLuint index, const GLshort * v) {
    printf("glVertexAttrib2sv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib2sv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z) {
    printf("glVertexAttrib3d(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%lf", x);
    printf(", ");
    printf("y=");
    printf("%lf", y);
    printf(", ");
    printf("z=");
    printf("%lf", z);
    printf(")");
    gl.VertexAttrib3d(index, x, y, z);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib3dv(GLuint index, const GLdouble * v) {
    printf("glVertexAttrib3dv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib3dv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z) {
    printf("glVertexAttrib3f(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%f", x);
    printf(", ");
    printf("y=");
    printf("%f", y);
    printf(", ");
    printf("z=");
    printf("%f", z);
    printf(")");
    gl.VertexAttrib3f(index, x, y, z);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib3fv(GLuint index, const GLfloat * v) {
    printf("glVertexAttrib3fv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib3fv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z) {
    printf("glVertexAttrib3s(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("z=");
    printf("%d", z);
    printf(")");
    gl.VertexAttrib3s(index, x, y, z);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib3sv(GLuint index, const GLshort * v) {
    printf("glVertexAttrib3sv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib3sv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4Nbv(GLuint index, const GLbyte * v) {
    printf("glVertexAttrib4Nbv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4Nbv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4Niv(GLuint index, const GLint * v) {
    printf("glVertexAttrib4Niv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4Niv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4Nsv(GLuint index, const GLshort * v) {
    printf("glVertexAttrib4Nsv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4Nsv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) {
    printf("glVertexAttrib4Nub(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("z=");
    printf("%d", z);
    printf(", ");
    printf("w=");
    printf("%d", w);
    printf(")");
    gl.VertexAttrib4Nub(index, x, y, z, w);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4Nubv(GLuint index, const GLubyte * v) {
    printf("glVertexAttrib4Nubv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4Nubv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4Nuiv(GLuint index, const GLuint * v) {
    printf("glVertexAttrib4Nuiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4Nuiv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4Nusv(GLuint index, const GLushort * v) {
    printf("glVertexAttrib4Nusv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4Nusv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4bv(GLuint index, const GLbyte * v) {
    printf("glVertexAttrib4bv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4bv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
    printf("glVertexAttrib4d(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%lf", x);
    printf(", ");
    printf("y=");
    printf("%lf", y);
    printf(", ");
    printf("z=");
    printf("%lf", z);
    printf(", ");
    printf("w=");
    printf("%lf", w);
    printf(")");
    gl.VertexAttrib4d(index, x, y, z, w);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4dv(GLuint index, const GLdouble * v) {
    printf("glVertexAttrib4dv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4dv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    printf("glVertexAttrib4f(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%f", x);
    printf(", ");
    printf("y=");
    printf("%f", y);
    printf(", ");
    printf("z=");
    printf("%f", z);
    printf(", ");
    printf("w=");
    printf("%f", w);
    printf(")");
    gl.VertexAttrib4f(index, x, y, z, w);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4fv(GLuint index, const GLfloat * v) {
    printf("glVertexAttrib4fv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4fv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4iv(GLuint index, const GLint * v) {
    printf("glVertexAttrib4iv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4iv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) {
    printf("glVertexAttrib4s(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("z=");
    printf("%d", z);
    printf(", ");
    printf("w=");
    printf("%d", w);
    printf(")");
    gl.VertexAttrib4s(index, x, y, z, w);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4sv(GLuint index, const GLshort * v) {
    printf("glVertexAttrib4sv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4sv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4ubv(GLuint index, const GLubyte * v) {
    printf("glVertexAttrib4ubv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4ubv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4uiv(GLuint index, const GLuint * v) {
    printf("glVertexAttrib4uiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4uiv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttrib4usv(GLuint index, const GLushort * v) {
    printf("glVertexAttrib4usv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttrib4usv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer) {
    printf("glVertexAttribPointer(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("size=");
    printf("%d", size);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("normalized=");
    printf("%s", normalized ? "true" : "false");
    printf(", ");
    printf("stride=");
    printf("%d", stride);
    printf(", ");
    printf("pointer=");
    printf("%p", pointer);
    printf(")");
    gl.VertexAttribPointer(index, size, type, normalized, stride, pointer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glUniformMatrix2x3fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix2x3fv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glUniformMatrix3x2fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix3x2fv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glUniformMatrix2x4fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix2x4fv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glUniformMatrix4x2fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix4x2fv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glUniformMatrix3x4fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix3x4fv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glUniformMatrix4x3fv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix4x3fv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) {
    printf("glColorMaski(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("r=");
    printf("%s", r ? "true" : "false");
    printf(", ");
    printf("g=");
    printf("%s", g ? "true" : "false");
    printf(", ");
    printf("b=");
    printf("%s", b ? "true" : "false");
    printf(", ");
    printf("a=");
    printf("%s", a ? "true" : "false");
    printf(")");
    gl.ColorMaski(index, r, g, b, a);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetBooleani_v(GLenum target, GLuint index, GLboolean * data) {
    printf("glGetBooleani_v(");
    printf("target=");
    print_gl_get_pname(target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.GetBooleani_v(target, index, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetIntegeri_v(GLenum target, GLuint index, GLint * data) {
    printf("glGetIntegeri_v(");
    printf("target=");
    print_gl_get_pname(target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.GetIntegeri_v(target, index, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Enablei(GLenum target, GLuint index) {
    printf("glEnablei(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(")");
    gl.Enablei(target, index);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Disablei(GLenum target, GLuint index) {
    printf("glDisablei(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(")");
    gl.Disablei(target, index);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLboolean GLAPI IsEnabledi(GLenum target, GLuint index) {
    printf("glIsEnabledi(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsEnabledi(target, index);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI BeginTransformFeedback(GLenum primitiveMode) {
    printf("glBeginTransformFeedback(");
    printf("primitiveMode=");
    printf("0x%04x", primitiveMode);
    printf(")");
    gl.BeginTransformFeedback(primitiveMode);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI EndTransformFeedback() {
    printf("glEndTransformFeedback(");
    printf(")");
    gl.EndTransformFeedback();
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    printf("glBindBufferRange(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(")");
    gl.BindBufferRange(target, index, buffer, offset, size);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindBufferBase(GLenum target, GLuint index, GLuint buffer) {
    printf("glBindBufferBase(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(")");
    gl.BindBufferBase(target, index, buffer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar * const * varyings, GLenum bufferMode) {
    printf("glTransformFeedbackVaryings(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("varyings=");
    printf("%p", varyings);
    printf(", ");
    printf("bufferMode=");
    print_buffer_mode(bufferMode);
    printf(")");
    gl.TransformFeedbackVaryings(program, count, varyings, bufferMode);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name) {
    printf("glGetTransformFeedbackVarying(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("size=");
    printf("%p", size);
    printf(", ");
    printf("type=");
    printf("%p", type);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    gl.GetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClampColor(GLenum target, GLenum clamp) {
    printf("glClampColor(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("clamp=");
    printf("0x%04x", clamp);
    printf(")");
    gl.ClampColor(target, clamp);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BeginConditionalRender(GLuint id, GLenum mode) {
    printf("glBeginConditionalRender(");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("mode=");
    printf("0x%04x", mode);
    printf(")");
    gl.BeginConditionalRender(id, mode);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI EndConditionalRender() {
    printf("glEndConditionalRender(");
    printf(")");
    gl.EndConditionalRender();
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer) {
    printf("glVertexAttribIPointer(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("size=");
    printf("%d", size);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("stride=");
    printf("%d", stride);
    printf(", ");
    printf("pointer=");
    printf("%p", pointer);
    printf(")");
    gl.VertexAttribIPointer(index, size, type, stride, pointer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetVertexAttribIiv(GLuint index, GLenum pname, GLint * params) {
    printf("glGetVertexAttribIiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetVertexAttribIiv(index, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetVertexAttribIuiv(GLuint index, GLenum pname, GLuint * params) {
    printf("glGetVertexAttribIuiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetVertexAttribIuiv(index, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI1i(GLuint index, GLint x) {
    printf("glVertexAttribI1i(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(")");
    gl.VertexAttribI1i(index, x);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI2i(GLuint index, GLint x, GLint y) {
    printf("glVertexAttribI2i(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(")");
    gl.VertexAttribI2i(index, x, y);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI3i(GLuint index, GLint x, GLint y, GLint z) {
    printf("glVertexAttribI3i(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("z=");
    printf("%d", z);
    printf(")");
    gl.VertexAttribI3i(index, x, y, z);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w) {
    printf("glVertexAttribI4i(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("z=");
    printf("%d", z);
    printf(", ");
    printf("w=");
    printf("%d", w);
    printf(")");
    gl.VertexAttribI4i(index, x, y, z, w);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI1ui(GLuint index, GLuint x) {
    printf("glVertexAttribI1ui(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%u", x);
    printf(")");
    gl.VertexAttribI1ui(index, x);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI2ui(GLuint index, GLuint x, GLuint y) {
    printf("glVertexAttribI2ui(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%u", x);
    printf(", ");
    printf("y=");
    printf("%u", y);
    printf(")");
    gl.VertexAttribI2ui(index, x, y);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI3ui(GLuint index, GLuint x, GLuint y, GLuint z) {
    printf("glVertexAttribI3ui(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%u", x);
    printf(", ");
    printf("y=");
    printf("%u", y);
    printf(", ");
    printf("z=");
    printf("%u", z);
    printf(")");
    gl.VertexAttribI3ui(index, x, y, z);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
    printf("glVertexAttribI4ui(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%u", x);
    printf(", ");
    printf("y=");
    printf("%u", y);
    printf(", ");
    printf("z=");
    printf("%u", z);
    printf(", ");
    printf("w=");
    printf("%u", w);
    printf(")");
    gl.VertexAttribI4ui(index, x, y, z, w);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI1iv(GLuint index, const GLint * v) {
    printf("glVertexAttribI1iv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribI1iv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI2iv(GLuint index, const GLint * v) {
    printf("glVertexAttribI2iv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribI2iv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI3iv(GLuint index, const GLint * v) {
    printf("glVertexAttribI3iv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribI3iv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI4iv(GLuint index, const GLint * v) {
    printf("glVertexAttribI4iv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribI4iv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI1uiv(GLuint index, const GLuint * v) {
    printf("glVertexAttribI1uiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribI1uiv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI2uiv(GLuint index, const GLuint * v) {
    printf("glVertexAttribI2uiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribI2uiv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI3uiv(GLuint index, const GLuint * v) {
    printf("glVertexAttribI3uiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribI3uiv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI4uiv(GLuint index, const GLuint * v) {
    printf("glVertexAttribI4uiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribI4uiv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI4bv(GLuint index, const GLbyte * v) {
    printf("glVertexAttribI4bv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribI4bv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI4sv(GLuint index, const GLshort * v) {
    printf("glVertexAttribI4sv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribI4sv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI4ubv(GLuint index, const GLubyte * v) {
    printf("glVertexAttribI4ubv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribI4ubv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribI4usv(GLuint index, const GLushort * v) {
    printf("glVertexAttribI4usv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribI4usv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetUniformuiv(GLuint program, GLint location, GLuint * params) {
    printf("glGetUniformuiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetUniformuiv(program, location, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindFragDataLocation(GLuint program, GLuint color, const GLchar * name) {
    printf("glBindFragDataLocation(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("color=");
    printf("%u", color);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    gl.BindFragDataLocation(program, color, name);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLint GLAPI GetFragDataLocation(GLuint program, const GLchar * name) {
    printf("glGetFragDataLocation(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    printf(" -> ");
    GLint result = gl.GetFragDataLocation(program, name);
    printf("%d", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI Uniform1ui(GLint location, GLuint v0) {
    printf("glUniform1ui(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%u", v0);
    printf(")");
    gl.Uniform1ui(location, v0);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform2ui(GLint location, GLuint v0, GLuint v1) {
    printf("glUniform2ui(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%u", v0);
    printf(", ");
    printf("v1=");
    printf("%u", v1);
    printf(")");
    gl.Uniform2ui(location, v0, v1);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2) {
    printf("glUniform3ui(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%u", v0);
    printf(", ");
    printf("v1=");
    printf("%u", v1);
    printf(", ");
    printf("v2=");
    printf("%u", v2);
    printf(")");
    gl.Uniform3ui(location, v0, v1, v2);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
    printf("glUniform4ui(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%u", v0);
    printf(", ");
    printf("v1=");
    printf("%u", v1);
    printf(", ");
    printf("v2=");
    printf("%u", v2);
    printf(", ");
    printf("v3=");
    printf("%u", v3);
    printf(")");
    gl.Uniform4ui(location, v0, v1, v2, v3);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform1uiv(GLint location, GLsizei count, const GLuint * value) {
    printf("glUniform1uiv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform1uiv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform2uiv(GLint location, GLsizei count, const GLuint * value) {
    printf("glUniform2uiv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform2uiv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform3uiv(GLint location, GLsizei count, const GLuint * value) {
    printf("glUniform3uiv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform3uiv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform4uiv(GLint location, GLsizei count, const GLuint * value) {
    printf("glUniform4uiv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform4uiv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexParameterIiv(GLenum target, GLenum pname, const GLint * params) {
    printf("glTexParameterIiv(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("pname=");
    print_tex_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.TexParameterIiv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexParameterIuiv(GLenum target, GLenum pname, const GLuint * params) {
    printf("glTexParameterIuiv(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("pname=");
    print_tex_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.TexParameterIuiv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTexParameterIiv(GLenum target, GLenum pname, GLint * params) {
    printf("glGetTexParameterIiv(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("pname=");
    print_tex_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetTexParameterIiv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTexParameterIuiv(GLenum target, GLenum pname, GLuint * params) {
    printf("glGetTexParameterIuiv(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("pname=");
    print_tex_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetTexParameterIuiv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint * value) {
    printf("glClearBufferiv(");
    printf("buffer=");
    printf("0x%04x", buffer);
    printf(", ");
    printf("drawbuffer=");
    printf("%d", drawbuffer);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ClearBufferiv(buffer, drawbuffer, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint * value) {
    printf("glClearBufferuiv(");
    printf("buffer=");
    printf("0x%04x", buffer);
    printf(", ");
    printf("drawbuffer=");
    printf("%d", drawbuffer);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ClearBufferuiv(buffer, drawbuffer, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat * value) {
    printf("glClearBufferfv(");
    printf("buffer=");
    printf("0x%04x", buffer);
    printf(", ");
    printf("drawbuffer=");
    printf("%d", drawbuffer);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ClearBufferfv(buffer, drawbuffer, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) {
    printf("glClearBufferfi(");
    printf("buffer=");
    printf("0x%04x", buffer);
    printf(", ");
    printf("drawbuffer=");
    printf("%d", drawbuffer);
    printf(", ");
    printf("depth=");
    printf("%f", depth);
    printf(", ");
    printf("stencil=");
    printf("%d", stencil);
    printf(")");
    gl.ClearBufferfi(buffer, drawbuffer, depth, stencil);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" const GLubyte * GLAPI GetStringi(GLenum name, GLuint index) {
    printf("glGetStringi(");
    printf("name=");
    printf("0x%04x", name);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(")");
    printf(" -> ");
    const GLubyte * result = gl.GetStringi(name, index);
    printf("%p", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" GLboolean GLAPI IsRenderbuffer(GLuint renderbuffer) {
    printf("glIsRenderbuffer(");
    printf("renderbuffer=");
    printf("%u", renderbuffer);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsRenderbuffer(renderbuffer);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI BindRenderbuffer(GLenum target, GLuint renderbuffer) {
    printf("glBindRenderbuffer(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("renderbuffer=");
    printf("%u", renderbuffer);
    printf(")");
    gl.BindRenderbuffer(target, renderbuffer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers) {
    printf("glDeleteRenderbuffers(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("renderbuffers=");
    printf("%p", renderbuffers);
    printf(")");
    gl.DeleteRenderbuffers(n, renderbuffers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GenRenderbuffers(GLsizei n, GLuint * renderbuffers) {
    printf("glGenRenderbuffers(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("renderbuffers=");
    printf("%p", renderbuffers);
    printf(")");
    gl.GenRenderbuffers(n, renderbuffers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
    printf("glRenderbufferStorage(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.RenderbufferStorage(target, internalformat, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint * params) {
    printf("glGetRenderbufferParameteriv(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetRenderbufferParameteriv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLboolean GLAPI IsFramebuffer(GLuint framebuffer) {
    printf("glIsFramebuffer(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsFramebuffer(framebuffer);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI BindFramebuffer(GLenum target, GLuint framebuffer) {
    printf("glBindFramebuffer(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(")");
    gl.BindFramebuffer(target, framebuffer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DeleteFramebuffers(GLsizei n, const GLuint * framebuffers) {
    printf("glDeleteFramebuffers(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("framebuffers=");
    printf("%p", framebuffers);
    printf(")");
    gl.DeleteFramebuffers(n, framebuffers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GenFramebuffers(GLsizei n, GLuint * framebuffers) {
    printf("glGenFramebuffers(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("framebuffers=");
    printf("%p", framebuffers);
    printf(")");
    gl.GenFramebuffers(n, framebuffers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLenum GLAPI CheckFramebufferStatus(GLenum target) {
    printf("glCheckFramebufferStatus(");
    printf("target=");
    printf("0x%04x", target);
    printf(")");
    printf(" -> ");
    GLenum result = gl.CheckFramebufferStatus(target);
    printf("0x%04x", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
    printf("glFramebufferTexture1D(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("attachment=");
    printf("0x%04x", attachment);
    printf(", ");
    printf("textarget=");
    printf("0x%04x", textarget);
    printf(", ");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(")");
    gl.FramebufferTexture1D(target, attachment, textarget, texture, level);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
    printf("glFramebufferTexture2D(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("attachment=");
    printf("0x%04x", attachment);
    printf(", ");
    printf("textarget=");
    printf("0x%04x", textarget);
    printf(", ");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(")");
    gl.FramebufferTexture2D(target, attachment, textarget, texture, level);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) {
    printf("glFramebufferTexture3D(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("attachment=");
    printf("0x%04x", attachment);
    printf(", ");
    printf("textarget=");
    printf("0x%04x", textarget);
    printf(", ");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("zoffset=");
    printf("%d", zoffset);
    printf(")");
    gl.FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
    printf("glFramebufferRenderbuffer(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("attachment=");
    printf("0x%04x", attachment);
    printf(", ");
    printf("renderbuffertarget=");
    printf("0x%04x", renderbuffertarget);
    printf(", ");
    printf("renderbuffer=");
    printf("%u", renderbuffer);
    printf(")");
    gl.FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint * params) {
    printf("glGetFramebufferAttachmentParameteriv(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("attachment=");
    printf("0x%04x", attachment);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GenerateMipmap(GLenum target) {
    printf("glGenerateMipmap(");
    printf("target=");
    print_texture_target(target);
    printf(")");
    gl.GenerateMipmap(target);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
    printf("glBlitFramebuffer(");
    printf("srcX0=");
    printf("%d", srcX0);
    printf(", ");
    printf("srcY0=");
    printf("%d", srcY0);
    printf(", ");
    printf("srcX1=");
    printf("%d", srcX1);
    printf(", ");
    printf("srcY1=");
    printf("%d", srcY1);
    printf(", ");
    printf("dstX0=");
    printf("%d", dstX0);
    printf(", ");
    printf("dstY0=");
    printf("%d", dstY0);
    printf(", ");
    printf("dstX1=");
    printf("%d", dstX1);
    printf(", ");
    printf("dstY1=");
    printf("%d", dstY1);
    printf(", ");
    printf("mask=");
    printf("0x%08x", mask);
    printf(", ");
    printf("filter=");
    printf("0x%04x", filter);
    printf(")");
    gl.BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
    printf("glRenderbufferStorageMultisample(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("samples=");
    printf("%d", samples);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.RenderbufferStorageMultisample(target, samples, internalformat, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) {
    printf("glFramebufferTextureLayer(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("attachment=");
    printf("0x%04x", attachment);
    printf(", ");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("layer=");
    printf("%d", layer);
    printf(")");
    gl.FramebufferTextureLayer(target, attachment, texture, level, layer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void * GLAPI MapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) {
    printf("glMapBufferRange(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("length=");
    printf("%lld", length);
    printf(", ");
    printf("access=");
    print_buffer_access_bits(access);
    printf(")");
    printf(" -> ");
    void * result = gl.MapBufferRange(target, offset, length, access);
    printf("%p", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI FlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length) {
    printf("glFlushMappedBufferRange(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("length=");
    printf("%lld", length);
    printf(")");
    gl.FlushMappedBufferRange(target, offset, length);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindVertexArray(GLuint array) {
    printf("glBindVertexArray(");
    printf("array=");
    printf("%u", array);
    printf(")");
    gl.BindVertexArray(array);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DeleteVertexArrays(GLsizei n, const GLuint * arrays) {
    printf("glDeleteVertexArrays(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("arrays=");
    printf("%p", arrays);
    printf(")");
    gl.DeleteVertexArrays(n, arrays);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GenVertexArrays(GLsizei n, GLuint * arrays) {
    printf("glGenVertexArrays(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("arrays=");
    printf("%p", arrays);
    printf(")");
    gl.GenVertexArrays(n, arrays);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLboolean GLAPI IsVertexArray(GLuint array) {
    printf("glIsVertexArray(");
    printf("array=");
    printf("%u", array);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsVertexArray(array);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI DrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount) {
    printf("glDrawArraysInstanced(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("first=");
    printf("%d", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("instancecount=");
    printf("%d", instancecount);
    printf(")");
    gl.DrawArraysInstanced(mode, first, count, instancecount);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount) {
    printf("glDrawElementsInstanced(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indices=");
    printf("%p", indices);
    printf(", ");
    printf("instancecount=");
    printf("%d", instancecount);
    printf(")");
    gl.DrawElementsInstanced(mode, count, type, indices, instancecount);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexBuffer(GLenum target, GLenum internalformat, GLuint buffer) {
    printf("glTexBuffer(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(")");
    gl.TexBuffer(target, internalformat, buffer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PrimitiveRestartIndex(GLuint index) {
    printf("glPrimitiveRestartIndex(");
    printf("index=");
    printf("%u", index);
    printf(")");
    gl.PrimitiveRestartIndex(index);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) {
    printf("glCopyBufferSubData(");
    printf("readTarget=");
    printf("0x%04x", readTarget);
    printf(", ");
    printf("writeTarget=");
    printf("0x%04x", writeTarget);
    printf(", ");
    printf("readOffset=");
    printf("%lld", readOffset);
    printf(", ");
    printf("writeOffset=");
    printf("%lld", writeOffset);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(")");
    gl.CopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar * const * uniformNames, GLuint * uniformIndices) {
    printf("glGetUniformIndices(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("uniformCount=");
    printf("%d", uniformCount);
    printf(", ");
    printf("uniformNames=");
    printf("%p", uniformNames);
    printf(", ");
    printf("uniformIndices=");
    printf("%p", uniformIndices);
    printf(")");
    gl.GetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params) {
    printf("glGetActiveUniformsiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("uniformCount=");
    printf("%d", uniformCount);
    printf(", ");
    printf("uniformIndices=");
    printf("%p", uniformIndices);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformName) {
    printf("glGetActiveUniformName(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("uniformIndex=");
    printf("%u", uniformIndex);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("uniformName=");
    printf("%p", uniformName);
    printf(")");
    gl.GetActiveUniformName(program, uniformIndex, bufSize, length, uniformName);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLuint GLAPI GetUniformBlockIndex(GLuint program, const GLchar * uniformBlockName) {
    printf("glGetUniformBlockIndex(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("uniformBlockName=");
    printf("%p", uniformBlockName);
    printf(")");
    printf(" -> ");
    GLuint result = gl.GetUniformBlockIndex(program, uniformBlockName);
    printf("%u", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI GetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint * params) {
    printf("glGetActiveUniformBlockiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("uniformBlockIndex=");
    printf("%u", uniformBlockIndex);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformBlockName) {
    printf("glGetActiveUniformBlockName(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("uniformBlockIndex=");
    printf("%u", uniformBlockIndex);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("uniformBlockName=");
    printf("%p", uniformBlockName);
    printf(")");
    gl.GetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) {
    printf("glUniformBlockBinding(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("uniformBlockIndex=");
    printf("%u", uniformBlockIndex);
    printf(", ");
    printf("uniformBlockBinding=");
    printf("%u", uniformBlockBinding);
    printf(")");
    gl.UniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void * indices, GLint basevertex) {
    printf("glDrawElementsBaseVertex(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indices=");
    printf("%p", indices);
    printf(", ");
    printf("basevertex=");
    printf("%d", basevertex);
    printf(")");
    gl.DrawElementsBaseVertex(mode, count, type, indices, basevertex);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices, GLint basevertex) {
    printf("glDrawRangeElementsBaseVertex(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("start=");
    printf("%u", start);
    printf(", ");
    printf("end=");
    printf("%u", end);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indices=");
    printf("%p", indices);
    printf(", ");
    printf("basevertex=");
    printf("%d", basevertex);
    printf(")");
    gl.DrawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex) {
    printf("glDrawElementsInstancedBaseVertex(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indices=");
    printf("%p", indices);
    printf(", ");
    printf("instancecount=");
    printf("%d", instancecount);
    printf(", ");
    printf("basevertex=");
    printf("%d", basevertex);
    printf(")");
    gl.DrawElementsInstancedBaseVertex(mode, count, type, indices, instancecount, basevertex);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI MultiDrawElementsBaseVertex(GLenum mode, const GLsizei * count, GLenum type, const void * const * indices, GLsizei drawcount, const GLint * basevertex) {
    printf("glMultiDrawElementsBaseVertex(");
    printf("mode=");
    printf("0x%04x", mode);
    printf(", ");
    printf("count=");
    printf("%p", count);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indices=");
    printf("%p", indices);
    printf(", ");
    printf("drawcount=");
    printf("%d", drawcount);
    printf(", ");
    printf("basevertex=");
    printf("%p", basevertex);
    printf(")");
    gl.MultiDrawElementsBaseVertex(mode, count, type, indices, drawcount, basevertex);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProvokingVertex(GLenum mode) {
    printf("glProvokingVertex(");
    printf("mode=");
    printf("0x%04x", mode);
    printf(")");
    gl.ProvokingVertex(mode);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLsync GLAPI FenceSync(GLenum condition, GLbitfield flags) {
    printf("glFenceSync(");
    printf("condition=");
    printf("0x%04x", condition);
    printf(", ");
    printf("flags=");
    printf("0x%08x", flags);
    printf(")");
    printf(" -> ");
    GLsync result = gl.FenceSync(condition, flags);
    printf("%p", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" GLboolean GLAPI IsSync(GLsync sync) {
    printf("glIsSync(");
    printf("sync=");
    printf("%p", sync);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsSync(sync);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI DeleteSync(GLsync sync) {
    printf("glDeleteSync(");
    printf("sync=");
    printf("%p", sync);
    printf(")");
    gl.DeleteSync(sync);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLenum GLAPI ClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout) {
    printf("glClientWaitSync(");
    printf("sync=");
    printf("%p", sync);
    printf(", ");
    printf("flags=");
    printf("0x%08x", flags);
    printf(", ");
    printf("timeout=");
    printf("%llu", timeout);
    printf(")");
    printf(" -> ");
    GLenum result = gl.ClientWaitSync(sync, flags, timeout);
    printf("0x%04x", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI WaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout) {
    printf("glWaitSync(");
    printf("sync=");
    printf("%p", sync);
    printf(", ");
    printf("flags=");
    printf("0x%08x", flags);
    printf(", ");
    printf("timeout=");
    printf("%llu", timeout);
    printf(")");
    gl.WaitSync(sync, flags, timeout);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetInteger64v(GLenum pname, GLint64 * data) {
    printf("glGetInteger64v(");
    printf("pname=");
    print_gl_get_pname(pname);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.GetInteger64v(pname, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei * length, GLint * values) {
    printf("glGetSynciv(");
    printf("sync=");
    printf("%p", sync);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("values=");
    printf("%p", values);
    printf(")");
    gl.GetSynciv(sync, pname, bufSize, length, values);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetInteger64i_v(GLenum target, GLuint index, GLint64 * data) {
    printf("glGetInteger64i_v(");
    printf("target=");
    print_gl_get_pname(target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.GetInteger64i_v(target, index, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetBufferParameteri64v(GLenum target, GLenum pname, GLint64 * params) {
    printf("glGetBufferParameteri64v(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetBufferParameteri64v(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI FramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level) {
    printf("glFramebufferTexture(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("attachment=");
    printf("0x%04x", attachment);
    printf(", ");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(")");
    gl.FramebufferTexture(target, attachment, texture, level);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
    printf("glTexImage2DMultisample(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("samples=");
    printf("%d", samples);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("fixedsamplelocations=");
    printf("%s", fixedsamplelocations ? "true" : "false");
    printf(")");
    gl.TexImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexImage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
    printf("glTexImage3DMultisample(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("samples=");
    printf("%d", samples);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(", ");
    printf("fixedsamplelocations=");
    printf("%s", fixedsamplelocations ? "true" : "false");
    printf(")");
    gl.TexImage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetMultisamplefv(GLenum pname, GLuint index, GLfloat * val) {
    printf("glGetMultisamplefv(");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("val=");
    printf("%p", val);
    printf(")");
    gl.GetMultisamplefv(pname, index, val);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI SampleMaski(GLuint maskNumber, GLbitfield mask) {
    printf("glSampleMaski(");
    printf("maskNumber=");
    printf("%u", maskNumber);
    printf(", ");
    printf("mask=");
    printf("0x%08x", mask);
    printf(")");
    gl.SampleMaski(maskNumber, mask);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindFragDataLocationIndexed(GLuint program, GLuint colorNumber, GLuint index, const GLchar * name) {
    printf("glBindFragDataLocationIndexed(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("colorNumber=");
    printf("%u", colorNumber);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    gl.BindFragDataLocationIndexed(program, colorNumber, index, name);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLint GLAPI GetFragDataIndex(GLuint program, const GLchar * name) {
    printf("glGetFragDataIndex(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    printf(" -> ");
    GLint result = gl.GetFragDataIndex(program, name);
    printf("%d", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI GenSamplers(GLsizei count, GLuint * samplers) {
    printf("glGenSamplers(");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("samplers=");
    printf("%p", samplers);
    printf(")");
    gl.GenSamplers(count, samplers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DeleteSamplers(GLsizei count, const GLuint * samplers) {
    printf("glDeleteSamplers(");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("samplers=");
    printf("%p", samplers);
    printf(")");
    gl.DeleteSamplers(count, samplers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLboolean GLAPI IsSampler(GLuint sampler) {
    printf("glIsSampler(");
    printf("sampler=");
    printf("%u", sampler);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsSampler(sampler);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI BindSampler(GLuint unit, GLuint sampler) {
    printf("glBindSampler(");
    printf("unit=");
    printf("%u", unit);
    printf(", ");
    printf("sampler=");
    printf("%u", sampler);
    printf(")");
    gl.BindSampler(unit, sampler);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI SamplerParameteri(GLuint sampler, GLenum pname, GLint param) {
    printf("glSamplerParameteri(");
    printf("sampler=");
    printf("%u", sampler);
    printf(", ");
    printf("pname=");
    print_sampler_parameter_pname(pname);
    printf(", ");
    printf("param=");
    printf("%d", param);
    printf(")");
    gl.SamplerParameteri(sampler, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI SamplerParameteriv(GLuint sampler, GLenum pname, const GLint * param) {
    printf("glSamplerParameteriv(");
    printf("sampler=");
    printf("%u", sampler);
    printf(", ");
    printf("pname=");
    print_sampler_parameter_pname(pname);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.SamplerParameteriv(sampler, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI SamplerParameterf(GLuint sampler, GLenum pname, GLfloat param) {
    printf("glSamplerParameterf(");
    printf("sampler=");
    printf("%u", sampler);
    printf(", ");
    printf("pname=");
    print_sampler_parameter_pname(pname);
    printf(", ");
    printf("param=");
    printf("%f", param);
    printf(")");
    gl.SamplerParameterf(sampler, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI SamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat * param) {
    printf("glSamplerParameterfv(");
    printf("sampler=");
    printf("%u", sampler);
    printf(", ");
    printf("pname=");
    print_sampler_parameter_pname(pname);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.SamplerParameterfv(sampler, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI SamplerParameterIiv(GLuint sampler, GLenum pname, const GLint * param) {
    printf("glSamplerParameterIiv(");
    printf("sampler=");
    printf("%u", sampler);
    printf(", ");
    printf("pname=");
    print_sampler_parameter_pname(pname);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.SamplerParameterIiv(sampler, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI SamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint * param) {
    printf("glSamplerParameterIuiv(");
    printf("sampler=");
    printf("%u", sampler);
    printf(", ");
    printf("pname=");
    print_sampler_parameter_pname(pname);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.SamplerParameterIuiv(sampler, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetSamplerParameteriv(GLuint sampler, GLenum pname, GLint * params) {
    printf("glGetSamplerParameteriv(");
    printf("sampler=");
    printf("%u", sampler);
    printf(", ");
    printf("pname=");
    print_sampler_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetSamplerParameteriv(sampler, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint * params) {
    printf("glGetSamplerParameterIiv(");
    printf("sampler=");
    printf("%u", sampler);
    printf(", ");
    printf("pname=");
    print_sampler_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetSamplerParameterIiv(sampler, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat * params) {
    printf("glGetSamplerParameterfv(");
    printf("sampler=");
    printf("%u", sampler);
    printf(", ");
    printf("pname=");
    print_sampler_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetSamplerParameterfv(sampler, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint * params) {
    printf("glGetSamplerParameterIuiv(");
    printf("sampler=");
    printf("%u", sampler);
    printf(", ");
    printf("pname=");
    print_sampler_parameter_pname(pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetSamplerParameterIuiv(sampler, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI QueryCounter(GLuint id, GLenum target) {
    printf("glQueryCounter(");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("target=");
    printf("0x%04x", target);
    printf(")");
    gl.QueryCounter(id, target);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetQueryObjecti64v(GLuint id, GLenum pname, GLint64 * params) {
    printf("glGetQueryObjecti64v(");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetQueryObjecti64v(id, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetQueryObjectui64v(GLuint id, GLenum pname, GLuint64 * params) {
    printf("glGetQueryObjectui64v(");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetQueryObjectui64v(id, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribDivisor(GLuint index, GLuint divisor) {
    printf("glVertexAttribDivisor(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("divisor=");
    printf("%u", divisor);
    printf(")");
    gl.VertexAttribDivisor(index, divisor);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    printf("glVertexAttribP1ui(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("normalized=");
    printf("%s", normalized ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%u", value);
    printf(")");
    gl.VertexAttribP1ui(index, type, normalized, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value) {
    printf("glVertexAttribP1uiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("normalized=");
    printf("%s", normalized ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.VertexAttribP1uiv(index, type, normalized, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    printf("glVertexAttribP2ui(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("normalized=");
    printf("%s", normalized ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%u", value);
    printf(")");
    gl.VertexAttribP2ui(index, type, normalized, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value) {
    printf("glVertexAttribP2uiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("normalized=");
    printf("%s", normalized ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.VertexAttribP2uiv(index, type, normalized, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    printf("glVertexAttribP3ui(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("normalized=");
    printf("%s", normalized ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%u", value);
    printf(")");
    gl.VertexAttribP3ui(index, type, normalized, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value) {
    printf("glVertexAttribP3uiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("normalized=");
    printf("%s", normalized ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.VertexAttribP3uiv(index, type, normalized, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    printf("glVertexAttribP4ui(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("normalized=");
    printf("%s", normalized ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%u", value);
    printf(")");
    gl.VertexAttribP4ui(index, type, normalized, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint * value) {
    printf("glVertexAttribP4uiv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("normalized=");
    printf("%s", normalized ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.VertexAttribP4uiv(index, type, normalized, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI MinSampleShading(GLfloat value) {
    printf("glMinSampleShading(");
    printf("value=");
    printf("%f", value);
    printf(")");
    gl.MinSampleShading(value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BlendEquationi(GLuint buf, GLenum mode) {
    printf("glBlendEquationi(");
    printf("buf=");
    printf("%u", buf);
    printf(", ");
    printf("mode=");
    printf("0x%04x", mode);
    printf(")");
    gl.BlendEquationi(buf, mode);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BlendEquationSeparatei(GLuint buf, GLenum modeRGB, GLenum modeAlpha) {
    printf("glBlendEquationSeparatei(");
    printf("buf=");
    printf("%u", buf);
    printf(", ");
    printf("modeRGB=");
    printf("0x%04x", modeRGB);
    printf(", ");
    printf("modeAlpha=");
    printf("0x%04x", modeAlpha);
    printf(")");
    gl.BlendEquationSeparatei(buf, modeRGB, modeAlpha);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BlendFunci(GLuint buf, GLenum src, GLenum dst) {
    printf("glBlendFunci(");
    printf("buf=");
    printf("%u", buf);
    printf(", ");
    printf("src=");
    printf("0x%04x", src);
    printf(", ");
    printf("dst=");
    printf("0x%04x", dst);
    printf(")");
    gl.BlendFunci(buf, src, dst);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BlendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
    printf("glBlendFuncSeparatei(");
    printf("buf=");
    printf("%u", buf);
    printf(", ");
    printf("srcRGB=");
    printf("0x%04x", srcRGB);
    printf(", ");
    printf("dstRGB=");
    printf("0x%04x", dstRGB);
    printf(", ");
    printf("srcAlpha=");
    printf("0x%04x", srcAlpha);
    printf(", ");
    printf("dstAlpha=");
    printf("0x%04x", dstAlpha);
    printf(")");
    gl.BlendFuncSeparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawArraysIndirect(GLenum mode, const void * indirect) {
    printf("glDrawArraysIndirect(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("indirect=");
    printf("%p", indirect);
    printf(")");
    gl.DrawArraysIndirect(mode, indirect);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawElementsIndirect(GLenum mode, GLenum type, const void * indirect) {
    printf("glDrawElementsIndirect(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indirect=");
    printf("%p", indirect);
    printf(")");
    gl.DrawElementsIndirect(mode, type, indirect);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform1d(GLint location, GLdouble x) {
    printf("glUniform1d(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("x=");
    printf("%lf", x);
    printf(")");
    gl.Uniform1d(location, x);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform2d(GLint location, GLdouble x, GLdouble y) {
    printf("glUniform2d(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("x=");
    printf("%lf", x);
    printf(", ");
    printf("y=");
    printf("%lf", y);
    printf(")");
    gl.Uniform2d(location, x, y);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform3d(GLint location, GLdouble x, GLdouble y, GLdouble z) {
    printf("glUniform3d(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("x=");
    printf("%lf", x);
    printf(", ");
    printf("y=");
    printf("%lf", y);
    printf(", ");
    printf("z=");
    printf("%lf", z);
    printf(")");
    gl.Uniform3d(location, x, y, z);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform4d(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
    printf("glUniform4d(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("x=");
    printf("%lf", x);
    printf(", ");
    printf("y=");
    printf("%lf", y);
    printf(", ");
    printf("z=");
    printf("%lf", z);
    printf(", ");
    printf("w=");
    printf("%lf", w);
    printf(")");
    gl.Uniform4d(location, x, y, z, w);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform1dv(GLint location, GLsizei count, const GLdouble * value) {
    printf("glUniform1dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform1dv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform2dv(GLint location, GLsizei count, const GLdouble * value) {
    printf("glUniform2dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform2dv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform3dv(GLint location, GLsizei count, const GLdouble * value) {
    printf("glUniform3dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform3dv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI Uniform4dv(GLint location, GLsizei count, const GLdouble * value) {
    printf("glUniform4dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.Uniform4dv(location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glUniformMatrix2dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix2dv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glUniformMatrix3dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix3dv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glUniformMatrix4dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix4dv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix2x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glUniformMatrix2x3dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix2x3dv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix2x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glUniformMatrix2x4dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix2x4dv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix3x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glUniformMatrix3x2dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix3x2dv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix3x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glUniformMatrix3x4dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix3x4dv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix4x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glUniformMatrix4x2dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix4x2dv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformMatrix4x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glUniformMatrix4x3dv(");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.UniformMatrix4x3dv(location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetUniformdv(GLuint program, GLint location, GLdouble * params) {
    printf("glGetUniformdv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetUniformdv(program, location, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLint GLAPI GetSubroutineUniformLocation(GLuint program, GLenum shadertype, const GLchar * name) {
    printf("glGetSubroutineUniformLocation(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("shadertype=");
    printf("0x%04x", shadertype);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    printf(" -> ");
    GLint result = gl.GetSubroutineUniformLocation(program, shadertype, name);
    printf("%d", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" GLuint GLAPI GetSubroutineIndex(GLuint program, GLenum shadertype, const GLchar * name) {
    printf("glGetSubroutineIndex(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("shadertype=");
    printf("0x%04x", shadertype);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    printf(" -> ");
    GLuint result = gl.GetSubroutineIndex(program, shadertype, name);
    printf("%u", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI GetActiveSubroutineUniformiv(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint * values) {
    printf("glGetActiveSubroutineUniformiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("shadertype=");
    printf("0x%04x", shadertype);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("values=");
    printf("%p", values);
    printf(")");
    gl.GetActiveSubroutineUniformiv(program, shadertype, index, pname, values);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetActiveSubroutineUniformName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei * length, GLchar * name) {
    printf("glGetActiveSubroutineUniformName(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("shadertype=");
    printf("0x%04x", shadertype);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("bufsize=");
    printf("%d", bufsize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    gl.GetActiveSubroutineUniformName(program, shadertype, index, bufsize, length, name);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetActiveSubroutineName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei * length, GLchar * name) {
    printf("glGetActiveSubroutineName(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("shadertype=");
    printf("0x%04x", shadertype);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("bufsize=");
    printf("%d", bufsize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    gl.GetActiveSubroutineName(program, shadertype, index, bufsize, length, name);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UniformSubroutinesuiv(GLenum shadertype, GLsizei count, const GLuint * indices) {
    printf("glUniformSubroutinesuiv(");
    printf("shadertype=");
    printf("0x%04x", shadertype);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("indices=");
    printf("%p", indices);
    printf(")");
    gl.UniformSubroutinesuiv(shadertype, count, indices);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetUniformSubroutineuiv(GLenum shadertype, GLint location, GLuint * params) {
    printf("glGetUniformSubroutineuiv(");
    printf("shadertype=");
    printf("0x%04x", shadertype);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetUniformSubroutineuiv(shadertype, location, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetProgramStageiv(GLuint program, GLenum shadertype, GLenum pname, GLint * values) {
    printf("glGetProgramStageiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("shadertype=");
    printf("0x%04x", shadertype);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("values=");
    printf("%p", values);
    printf(")");
    gl.GetProgramStageiv(program, shadertype, pname, values);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PatchParameteri(GLenum pname, GLint value) {
    printf("glPatchParameteri(");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("value=");
    printf("%d", value);
    printf(")");
    gl.PatchParameteri(pname, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PatchParameterfv(GLenum pname, const GLfloat * values) {
    printf("glPatchParameterfv(");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("values=");
    printf("%p", values);
    printf(")");
    gl.PatchParameterfv(pname, values);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindTransformFeedback(GLenum target, GLuint id) {
    printf("glBindTransformFeedback(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("id=");
    printf("%u", id);
    printf(")");
    gl.BindTransformFeedback(target, id);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DeleteTransformFeedbacks(GLsizei n, const GLuint * ids) {
    printf("glDeleteTransformFeedbacks(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("ids=");
    printf("%p", ids);
    printf(")");
    gl.DeleteTransformFeedbacks(n, ids);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GenTransformFeedbacks(GLsizei n, GLuint * ids) {
    printf("glGenTransformFeedbacks(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("ids=");
    printf("%p", ids);
    printf(")");
    gl.GenTransformFeedbacks(n, ids);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLboolean GLAPI IsTransformFeedback(GLuint id) {
    printf("glIsTransformFeedback(");
    printf("id=");
    printf("%u", id);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsTransformFeedback(id);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI PauseTransformFeedback() {
    printf("glPauseTransformFeedback(");
    printf(")");
    gl.PauseTransformFeedback();
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ResumeTransformFeedback() {
    printf("glResumeTransformFeedback(");
    printf(")");
    gl.ResumeTransformFeedback();
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawTransformFeedback(GLenum mode, GLuint id) {
    printf("glDrawTransformFeedback(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("id=");
    printf("%u", id);
    printf(")");
    gl.DrawTransformFeedback(mode, id);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawTransformFeedbackStream(GLenum mode, GLuint id, GLuint stream) {
    printf("glDrawTransformFeedbackStream(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("stream=");
    printf("%u", stream);
    printf(")");
    gl.DrawTransformFeedbackStream(mode, id, stream);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BeginQueryIndexed(GLenum target, GLuint index, GLuint id) {
    printf("glBeginQueryIndexed(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("id=");
    printf("%u", id);
    printf(")");
    gl.BeginQueryIndexed(target, index, id);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI EndQueryIndexed(GLenum target, GLuint index) {
    printf("glEndQueryIndexed(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(")");
    gl.EndQueryIndexed(target, index);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetQueryIndexediv(GLenum target, GLuint index, GLenum pname, GLint * params) {
    printf("glGetQueryIndexediv(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetQueryIndexediv(target, index, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ReleaseShaderCompiler() {
    printf("glReleaseShaderCompiler(");
    printf(")");
    gl.ReleaseShaderCompiler();
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ShaderBinary(GLsizei count, const GLuint * shaders, GLenum binaryformat, const void * binary, GLsizei length) {
    printf("glShaderBinary(");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("shaders=");
    printf("%p", shaders);
    printf(", ");
    printf("binaryformat=");
    printf("0x%04x", binaryformat);
    printf(", ");
    printf("binary=");
    printf("%p", binary);
    printf(", ");
    printf("length=");
    printf("%d", length);
    printf(")");
    gl.ShaderBinary(count, shaders, binaryformat, binary, length);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint * range, GLint * precision) {
    printf("glGetShaderPrecisionFormat(");
    printf("shadertype=");
    printf("0x%04x", shadertype);
    printf(", ");
    printf("precisiontype=");
    printf("0x%04x", precisiontype);
    printf(", ");
    printf("range=");
    printf("%p", range);
    printf(", ");
    printf("precision=");
    printf("%p", precision);
    printf(")");
    gl.GetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DepthRangef(GLfloat n, GLfloat f) {
    printf("glDepthRangef(");
    printf("n=");
    printf("%f", n);
    printf(", ");
    printf("f=");
    printf("%f", f);
    printf(")");
    gl.DepthRangef(n, f);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearDepthf(GLfloat d) {
    printf("glClearDepthf(");
    printf("d=");
    printf("%f", d);
    printf(")");
    gl.ClearDepthf(d);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetProgramBinary(GLuint program, GLsizei bufSize, GLsizei * length, GLenum * binaryFormat, void * binary) {
    printf("glGetProgramBinary(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("binaryFormat=");
    printf("%p", binaryFormat);
    printf(", ");
    printf("binary=");
    printf("%p", binary);
    printf(")");
    gl.GetProgramBinary(program, bufSize, length, binaryFormat, binary);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramBinary(GLuint program, GLenum binaryFormat, const void * binary, GLsizei length) {
    printf("glProgramBinary(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("binaryFormat=");
    printf("0x%04x", binaryFormat);
    printf(", ");
    printf("binary=");
    printf("%p", binary);
    printf(", ");
    printf("length=");
    printf("%d", length);
    printf(")");
    gl.ProgramBinary(program, binaryFormat, binary, length);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramParameteri(GLuint program, GLenum pname, GLint value) {
    printf("glProgramParameteri(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("value=");
    printf("%d", value);
    printf(")");
    gl.ProgramParameteri(program, pname, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI UseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program) {
    printf("glUseProgramStages(");
    printf("pipeline=");
    printf("%u", pipeline);
    printf(", ");
    printf("stages=");
    printf("0x%08x", stages);
    printf(", ");
    printf("program=");
    printf("%u", program);
    printf(")");
    gl.UseProgramStages(pipeline, stages, program);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ActiveShaderProgram(GLuint pipeline, GLuint program) {
    printf("glActiveShaderProgram(");
    printf("pipeline=");
    printf("%u", pipeline);
    printf(", ");
    printf("program=");
    printf("%u", program);
    printf(")");
    gl.ActiveShaderProgram(pipeline, program);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLuint GLAPI CreateShaderProgramv(GLenum type, GLsizei count, const GLchar * const * strings) {
    printf("glCreateShaderProgramv(");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("strings=");
    printf("%p", strings);
    printf(")");
    printf(" -> ");
    GLuint result = gl.CreateShaderProgramv(type, count, strings);
    printf("%u", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI BindProgramPipeline(GLuint pipeline) {
    printf("glBindProgramPipeline(");
    printf("pipeline=");
    printf("%u", pipeline);
    printf(")");
    gl.BindProgramPipeline(pipeline);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DeleteProgramPipelines(GLsizei n, const GLuint * pipelines) {
    printf("glDeleteProgramPipelines(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("pipelines=");
    printf("%p", pipelines);
    printf(")");
    gl.DeleteProgramPipelines(n, pipelines);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GenProgramPipelines(GLsizei n, GLuint * pipelines) {
    printf("glGenProgramPipelines(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("pipelines=");
    printf("%p", pipelines);
    printf(")");
    gl.GenProgramPipelines(n, pipelines);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLboolean GLAPI IsProgramPipeline(GLuint pipeline) {
    printf("glIsProgramPipeline(");
    printf("pipeline=");
    printf("%u", pipeline);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.IsProgramPipeline(pipeline);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI GetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint * params) {
    printf("glGetProgramPipelineiv(");
    printf("pipeline=");
    printf("%u", pipeline);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetProgramPipelineiv(pipeline, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform1i(GLuint program, GLint location, GLint v0) {
    printf("glProgramUniform1i(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%d", v0);
    printf(")");
    gl.ProgramUniform1i(program, location, v0);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint * value) {
    printf("glProgramUniform1iv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform1iv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform1f(GLuint program, GLint location, GLfloat v0) {
    printf("glProgramUniform1f(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%f", v0);
    printf(")");
    gl.ProgramUniform1f(program, location, v0);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat * value) {
    printf("glProgramUniform1fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform1fv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform1d(GLuint program, GLint location, GLdouble v0) {
    printf("glProgramUniform1d(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%lf", v0);
    printf(")");
    gl.ProgramUniform1d(program, location, v0);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform1dv(GLuint program, GLint location, GLsizei count, const GLdouble * value) {
    printf("glProgramUniform1dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform1dv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform1ui(GLuint program, GLint location, GLuint v0) {
    printf("glProgramUniform1ui(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%u", v0);
    printf(")");
    gl.ProgramUniform1ui(program, location, v0);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform1uiv(GLuint program, GLint location, GLsizei count, const GLuint * value) {
    printf("glProgramUniform1uiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform1uiv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform2i(GLuint program, GLint location, GLint v0, GLint v1) {
    printf("glProgramUniform2i(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%d", v0);
    printf(", ");
    printf("v1=");
    printf("%d", v1);
    printf(")");
    gl.ProgramUniform2i(program, location, v0, v1);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint * value) {
    printf("glProgramUniform2iv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform2iv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform2f(GLuint program, GLint location, GLfloat v0, GLfloat v1) {
    printf("glProgramUniform2f(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%f", v0);
    printf(", ");
    printf("v1=");
    printf("%f", v1);
    printf(")");
    gl.ProgramUniform2f(program, location, v0, v1);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat * value) {
    printf("glProgramUniform2fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform2fv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform2d(GLuint program, GLint location, GLdouble v0, GLdouble v1) {
    printf("glProgramUniform2d(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%lf", v0);
    printf(", ");
    printf("v1=");
    printf("%lf", v1);
    printf(")");
    gl.ProgramUniform2d(program, location, v0, v1);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform2dv(GLuint program, GLint location, GLsizei count, const GLdouble * value) {
    printf("glProgramUniform2dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform2dv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform2ui(GLuint program, GLint location, GLuint v0, GLuint v1) {
    printf("glProgramUniform2ui(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%u", v0);
    printf(", ");
    printf("v1=");
    printf("%u", v1);
    printf(")");
    gl.ProgramUniform2ui(program, location, v0, v1);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform2uiv(GLuint program, GLint location, GLsizei count, const GLuint * value) {
    printf("glProgramUniform2uiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform2uiv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform3i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2) {
    printf("glProgramUniform3i(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%d", v0);
    printf(", ");
    printf("v1=");
    printf("%d", v1);
    printf(", ");
    printf("v2=");
    printf("%d", v2);
    printf(")");
    gl.ProgramUniform3i(program, location, v0, v1, v2);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint * value) {
    printf("glProgramUniform3iv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform3iv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform3f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
    printf("glProgramUniform3f(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%f", v0);
    printf(", ");
    printf("v1=");
    printf("%f", v1);
    printf(", ");
    printf("v2=");
    printf("%f", v2);
    printf(")");
    gl.ProgramUniform3f(program, location, v0, v1, v2);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat * value) {
    printf("glProgramUniform3fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform3fv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform3d(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2) {
    printf("glProgramUniform3d(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%lf", v0);
    printf(", ");
    printf("v1=");
    printf("%lf", v1);
    printf(", ");
    printf("v2=");
    printf("%lf", v2);
    printf(")");
    gl.ProgramUniform3d(program, location, v0, v1, v2);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform3dv(GLuint program, GLint location, GLsizei count, const GLdouble * value) {
    printf("glProgramUniform3dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform3dv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform3ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2) {
    printf("glProgramUniform3ui(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%u", v0);
    printf(", ");
    printf("v1=");
    printf("%u", v1);
    printf(", ");
    printf("v2=");
    printf("%u", v2);
    printf(")");
    gl.ProgramUniform3ui(program, location, v0, v1, v2);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform3uiv(GLuint program, GLint location, GLsizei count, const GLuint * value) {
    printf("glProgramUniform3uiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform3uiv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform4i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
    printf("glProgramUniform4i(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%d", v0);
    printf(", ");
    printf("v1=");
    printf("%d", v1);
    printf(", ");
    printf("v2=");
    printf("%d", v2);
    printf(", ");
    printf("v3=");
    printf("%d", v3);
    printf(")");
    gl.ProgramUniform4i(program, location, v0, v1, v2, v3);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint * value) {
    printf("glProgramUniform4iv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform4iv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform4f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    printf("glProgramUniform4f(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%f", v0);
    printf(", ");
    printf("v1=");
    printf("%f", v1);
    printf(", ");
    printf("v2=");
    printf("%f", v2);
    printf(", ");
    printf("v3=");
    printf("%f", v3);
    printf(")");
    gl.ProgramUniform4f(program, location, v0, v1, v2, v3);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat * value) {
    printf("glProgramUniform4fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform4fv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform4d(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) {
    printf("glProgramUniform4d(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%lf", v0);
    printf(", ");
    printf("v1=");
    printf("%lf", v1);
    printf(", ");
    printf("v2=");
    printf("%lf", v2);
    printf(", ");
    printf("v3=");
    printf("%lf", v3);
    printf(")");
    gl.ProgramUniform4d(program, location, v0, v1, v2, v3);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform4dv(GLuint program, GLint location, GLsizei count, const GLdouble * value) {
    printf("glProgramUniform4dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform4dv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform4ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
    printf("glProgramUniform4ui(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("v0=");
    printf("%u", v0);
    printf(", ");
    printf("v1=");
    printf("%u", v1);
    printf(", ");
    printf("v2=");
    printf("%u", v2);
    printf(", ");
    printf("v3=");
    printf("%u", v3);
    printf(")");
    gl.ProgramUniform4ui(program, location, v0, v1, v2, v3);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniform4uiv(GLuint program, GLint location, GLsizei count, const GLuint * value) {
    printf("glProgramUniform4uiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniform4uiv(program, location, count, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glProgramUniformMatrix2fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix2fv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glProgramUniformMatrix3fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix3fv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glProgramUniformMatrix4fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix4fv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glProgramUniformMatrix2dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix2dv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glProgramUniformMatrix3dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix3dv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glProgramUniformMatrix4dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix4dv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix2x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glProgramUniformMatrix2x3fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix2x3fv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix3x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glProgramUniformMatrix3x2fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix3x2fv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix2x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glProgramUniformMatrix2x4fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix2x4fv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix4x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glProgramUniformMatrix4x2fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix4x2fv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix3x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glProgramUniformMatrix3x4fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix3x4fv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix4x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value) {
    printf("glProgramUniformMatrix4x3fv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix4x3fv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix2x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glProgramUniformMatrix2x3dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix2x3dv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix3x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glProgramUniformMatrix3x2dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix3x2dv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix2x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glProgramUniformMatrix2x4dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix2x4dv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix4x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glProgramUniformMatrix4x2dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix4x2dv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix3x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glProgramUniformMatrix3x4dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix3x4dv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ProgramUniformMatrix4x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value) {
    printf("glProgramUniformMatrix4x3dv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("transpose=");
    printf("%s", transpose ? "true" : "false");
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ProgramUniformMatrix4x3dv(program, location, count, transpose, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ValidateProgramPipeline(GLuint pipeline) {
    printf("glValidateProgramPipeline(");
    printf("pipeline=");
    printf("%u", pipeline);
    printf(")");
    gl.ValidateProgramPipeline(pipeline);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog) {
    printf("glGetProgramPipelineInfoLog(");
    printf("pipeline=");
    printf("%u", pipeline);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("infoLog=");
    printf("%p", infoLog);
    printf(")");
    gl.GetProgramPipelineInfoLog(pipeline, bufSize, length, infoLog);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribL1d(GLuint index, GLdouble x) {
    printf("glVertexAttribL1d(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%lf", x);
    printf(")");
    gl.VertexAttribL1d(index, x);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribL2d(GLuint index, GLdouble x, GLdouble y) {
    printf("glVertexAttribL2d(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%lf", x);
    printf(", ");
    printf("y=");
    printf("%lf", y);
    printf(")");
    gl.VertexAttribL2d(index, x, y);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribL3d(GLuint index, GLdouble x, GLdouble y, GLdouble z) {
    printf("glVertexAttribL3d(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%lf", x);
    printf(", ");
    printf("y=");
    printf("%lf", y);
    printf(", ");
    printf("z=");
    printf("%lf", z);
    printf(")");
    gl.VertexAttribL3d(index, x, y, z);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribL4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
    printf("glVertexAttribL4d(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%lf", x);
    printf(", ");
    printf("y=");
    printf("%lf", y);
    printf(", ");
    printf("z=");
    printf("%lf", z);
    printf(", ");
    printf("w=");
    printf("%lf", w);
    printf(")");
    gl.VertexAttribL4d(index, x, y, z, w);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribL1dv(GLuint index, const GLdouble * v) {
    printf("glVertexAttribL1dv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribL1dv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribL2dv(GLuint index, const GLdouble * v) {
    printf("glVertexAttribL2dv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribL2dv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribL3dv(GLuint index, const GLdouble * v) {
    printf("glVertexAttribL3dv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribL3dv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribL4dv(GLuint index, const GLdouble * v) {
    printf("glVertexAttribL4dv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.VertexAttribL4dv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer) {
    printf("glVertexAttribLPointer(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("size=");
    printf("%d", size);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("stride=");
    printf("%d", stride);
    printf(", ");
    printf("pointer=");
    printf("%p", pointer);
    printf(")");
    gl.VertexAttribLPointer(index, size, type, stride, pointer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetVertexAttribLdv(GLuint index, GLenum pname, GLdouble * params) {
    printf("glGetVertexAttribLdv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetVertexAttribLdv(index, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ViewportArrayv(GLuint first, GLsizei count, const GLfloat * v) {
    printf("glViewportArrayv(");
    printf("first=");
    printf("%u", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.ViewportArrayv(first, count, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ViewportIndexedf(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
    printf("glViewportIndexedf(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("x=");
    printf("%f", x);
    printf(", ");
    printf("y=");
    printf("%f", y);
    printf(", ");
    printf("w=");
    printf("%f", w);
    printf(", ");
    printf("h=");
    printf("%f", h);
    printf(")");
    gl.ViewportIndexedf(index, x, y, w, h);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ViewportIndexedfv(GLuint index, const GLfloat * v) {
    printf("glViewportIndexedfv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.ViewportIndexedfv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ScissorArrayv(GLuint first, GLsizei count, const GLint * v) {
    printf("glScissorArrayv(");
    printf("first=");
    printf("%u", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.ScissorArrayv(first, count, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ScissorIndexed(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height) {
    printf("glScissorIndexed(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("left=");
    printf("%d", left);
    printf(", ");
    printf("bottom=");
    printf("%d", bottom);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.ScissorIndexed(index, left, bottom, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ScissorIndexedv(GLuint index, const GLint * v) {
    printf("glScissorIndexedv(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.ScissorIndexedv(index, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DepthRangeArrayv(GLuint first, GLsizei count, const GLdouble * v) {
    printf("glDepthRangeArrayv(");
    printf("first=");
    printf("%u", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("v=");
    printf("%p", v);
    printf(")");
    gl.DepthRangeArrayv(first, count, v);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DepthRangeIndexed(GLuint index, GLdouble n, GLdouble f) {
    printf("glDepthRangeIndexed(");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("n=");
    printf("%lf", n);
    printf(", ");
    printf("f=");
    printf("%lf", f);
    printf(")");
    gl.DepthRangeIndexed(index, n, f);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetFloati_v(GLenum target, GLuint index, GLfloat * data) {
    printf("glGetFloati_v(");
    printf("target=");
    print_gl_get_pname(target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.GetFloati_v(target, index, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetDoublei_v(GLenum target, GLuint index, GLdouble * data) {
    printf("glGetDoublei_v(");
    printf("target=");
    print_gl_get_pname(target);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.GetDoublei_v(target, index, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance) {
    printf("glDrawArraysInstancedBaseInstance(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("first=");
    printf("%d", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("instancecount=");
    printf("%d", instancecount);
    printf(", ");
    printf("baseinstance=");
    printf("%u", baseinstance);
    printf(")");
    gl.DrawArraysInstancedBaseInstance(mode, first, count, instancecount, baseinstance);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawElementsInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLuint baseinstance) {
    printf("glDrawElementsInstancedBaseInstance(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indices=");
    printf("%p", indices);
    printf(", ");
    printf("instancecount=");
    printf("%d", instancecount);
    printf(", ");
    printf("baseinstance=");
    printf("%u", baseinstance);
    printf(")");
    gl.DrawElementsInstancedBaseInstance(mode, count, type, indices, instancecount, baseinstance);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance) {
    printf("glDrawElementsInstancedBaseVertexBaseInstance(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indices=");
    printf("%p", indices);
    printf(", ");
    printf("instancecount=");
    printf("%d", instancecount);
    printf(", ");
    printf("basevertex=");
    printf("%d", basevertex);
    printf(", ");
    printf("baseinstance=");
    printf("%u", baseinstance);
    printf(")");
    gl.DrawElementsInstancedBaseVertexBaseInstance(mode, count, type, indices, instancecount, basevertex, baseinstance);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint * params) {
    printf("glGetInternalformativ(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetInternalformativ(target, internalformat, pname, bufSize, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetActiveAtomicCounterBufferiv(GLuint program, GLuint bufferIndex, GLenum pname, GLint * params) {
    printf("glGetActiveAtomicCounterBufferiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("bufferIndex=");
    printf("%u", bufferIndex);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetActiveAtomicCounterBufferiv(program, bufferIndex, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format) {
    printf("glBindImageTexture(");
    printf("unit=");
    printf("%u", unit);
    printf(", ");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("layered=");
    printf("%s", layered ? "true" : "false");
    printf(", ");
    printf("layer=");
    printf("%d", layer);
    printf(", ");
    printf("access=");
    printf("0x%04x", access);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(")");
    gl.BindImageTexture(unit, texture, level, layered, layer, access, format);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI MemoryBarrier(GLbitfield barriers) {
    printf("glMemoryBarrier(");
    printf("barriers=");
    printf("0x%08x", barriers);
    printf(")");
    gl.MemoryBarrier(barriers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexStorage1D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width) {
    printf("glTexStorage1D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("levels=");
    printf("%d", levels);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(")");
    gl.TexStorage1D(target, levels, internalformat, width);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) {
    printf("glTexStorage2D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("levels=");
    printf("%d", levels);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.TexStorage2D(target, levels, internalformat, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) {
    printf("glTexStorage3D(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("levels=");
    printf("%d", levels);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(")");
    gl.TexStorage3D(target, levels, internalformat, width, height, depth);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawTransformFeedbackInstanced(GLenum mode, GLuint id, GLsizei instancecount) {
    printf("glDrawTransformFeedbackInstanced(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("instancecount=");
    printf("%d", instancecount);
    printf(")");
    gl.DrawTransformFeedbackInstanced(mode, id, instancecount);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DrawTransformFeedbackStreamInstanced(GLenum mode, GLuint id, GLuint stream, GLsizei instancecount) {
    printf("glDrawTransformFeedbackStreamInstanced(");
    printf("mode=");
    print_render_mode(mode);
    printf(", ");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("stream=");
    printf("%u", stream);
    printf(", ");
    printf("instancecount=");
    printf("%d", instancecount);
    printf(")");
    gl.DrawTransformFeedbackStreamInstanced(mode, id, stream, instancecount);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearBufferData(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void * data) {
    printf("glClearBufferData(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("internalformat=");
    print_clear_buffer_internalformat(internalformat);
    printf(", ");
    printf("format=");
    print_clear_buffer_format(format);
    printf(", ");
    printf("type=");
    print_clear_buffer_type(type);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.ClearBufferData(target, internalformat, format, type, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearBufferSubData(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) {
    printf("glClearBufferSubData(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("internalformat=");
    print_clear_buffer_internalformat(internalformat);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(", ");
    printf("format=");
    print_clear_buffer_format(format);
    printf(", ");
    printf("type=");
    print_clear_buffer_type(type);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.ClearBufferSubData(target, internalformat, offset, size, format, type, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z) {
    printf("glDispatchCompute(");
    printf("num_groups_x=");
    printf("%u", num_groups_x);
    printf(", ");
    printf("num_groups_y=");
    printf("%u", num_groups_y);
    printf(", ");
    printf("num_groups_z=");
    printf("%u", num_groups_z);
    printf(")");
    gl.DispatchCompute(num_groups_x, num_groups_y, num_groups_z);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DispatchComputeIndirect(GLintptr indirect) {
    printf("glDispatchComputeIndirect(");
    printf("indirect=");
    printf("%lld", indirect);
    printf(")");
    gl.DispatchComputeIndirect(indirect);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CopyImageSubData(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth) {
    printf("glCopyImageSubData(");
    printf("srcName=");
    printf("%u", srcName);
    printf(", ");
    printf("srcTarget=");
    print_texture_target(srcTarget);
    printf(", ");
    printf("srcLevel=");
    printf("%d", srcLevel);
    printf(", ");
    printf("srcX=");
    printf("%d", srcX);
    printf(", ");
    printf("srcY=");
    printf("%d", srcY);
    printf(", ");
    printf("srcZ=");
    printf("%d", srcZ);
    printf(", ");
    printf("dstName=");
    printf("%u", dstName);
    printf(", ");
    printf("dstTarget=");
    print_texture_target(dstTarget);
    printf(", ");
    printf("dstLevel=");
    printf("%d", dstLevel);
    printf(", ");
    printf("dstX=");
    printf("%d", dstX);
    printf(", ");
    printf("dstY=");
    printf("%d", dstY);
    printf(", ");
    printf("dstZ=");
    printf("%d", dstZ);
    printf(", ");
    printf("srcWidth=");
    printf("%d", srcWidth);
    printf(", ");
    printf("srcHeight=");
    printf("%d", srcHeight);
    printf(", ");
    printf("srcDepth=");
    printf("%d", srcDepth);
    printf(")");
    gl.CopyImageSubData(srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI FramebufferParameteri(GLenum target, GLenum pname, GLint param) {
    printf("glFramebufferParameteri(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%d", param);
    printf(")");
    gl.FramebufferParameteri(target, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetFramebufferParameteriv(GLenum target, GLenum pname, GLint * params) {
    printf("glGetFramebufferParameteriv(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetFramebufferParameteriv(target, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetInternalformati64v(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 * params) {
    printf("glGetInternalformati64v(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetInternalformati64v(target, internalformat, pname, bufSize, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI InvalidateTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth) {
    printf("glInvalidateTexSubImage(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("zoffset=");
    printf("%d", zoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(")");
    gl.InvalidateTexSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI InvalidateTexImage(GLuint texture, GLint level) {
    printf("glInvalidateTexImage(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(")");
    gl.InvalidateTexImage(texture, level);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI InvalidateBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr length) {
    printf("glInvalidateBufferSubData(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("length=");
    printf("%lld", length);
    printf(")");
    gl.InvalidateBufferSubData(buffer, offset, length);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI InvalidateBufferData(GLuint buffer) {
    printf("glInvalidateBufferData(");
    printf("buffer=");
    printf("%u", buffer);
    printf(")");
    gl.InvalidateBufferData(buffer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI InvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum * attachments) {
    printf("glInvalidateFramebuffer(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("numAttachments=");
    printf("%d", numAttachments);
    printf(", ");
    printf("attachments=");
    printf("%p", attachments);
    printf(")");
    gl.InvalidateFramebuffer(target, numAttachments, attachments);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI InvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height) {
    printf("glInvalidateSubFramebuffer(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("numAttachments=");
    printf("%d", numAttachments);
    printf(", ");
    printf("attachments=");
    printf("%p", attachments);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.InvalidateSubFramebuffer(target, numAttachments, attachments, x, y, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI MultiDrawArraysIndirect(GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride) {
    printf("glMultiDrawArraysIndirect(");
    printf("mode=");
    printf("0x%04x", mode);
    printf(", ");
    printf("indirect=");
    printf("%p", indirect);
    printf(", ");
    printf("drawcount=");
    printf("%d", drawcount);
    printf(", ");
    printf("stride=");
    printf("%d", stride);
    printf(")");
    gl.MultiDrawArraysIndirect(mode, indirect, drawcount, stride);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI MultiDrawElementsIndirect(GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride) {
    printf("glMultiDrawElementsIndirect(");
    printf("mode=");
    printf("0x%04x", mode);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indirect=");
    printf("%p", indirect);
    printf(", ");
    printf("drawcount=");
    printf("%d", drawcount);
    printf(", ");
    printf("stride=");
    printf("%d", stride);
    printf(")");
    gl.MultiDrawElementsIndirect(mode, type, indirect, drawcount, stride);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetProgramInterfaceiv(GLuint program, GLenum programInterface, GLenum pname, GLint * params) {
    printf("glGetProgramInterfaceiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("programInterface=");
    printf("0x%04x", programInterface);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetProgramInterfaceiv(program, programInterface, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLuint GLAPI GetProgramResourceIndex(GLuint program, GLenum programInterface, const GLchar * name) {
    printf("glGetProgramResourceIndex(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("programInterface=");
    printf("0x%04x", programInterface);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    printf(" -> ");
    GLuint result = gl.GetProgramResourceIndex(program, programInterface, name);
    printf("%u", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI GetProgramResourceName(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name) {
    printf("glGetProgramResourceName(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("programInterface=");
    printf("0x%04x", programInterface);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    gl.GetProgramResourceName(program, programInterface, index, bufSize, length, name);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetProgramResourceiv(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum * props, GLsizei bufSize, GLsizei * length, GLint * params) {
    printf("glGetProgramResourceiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("programInterface=");
    printf("0x%04x", programInterface);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("propCount=");
    printf("%d", propCount);
    printf(", ");
    printf("props=");
    printf("%p", props);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetProgramResourceiv(program, programInterface, index, propCount, props, bufSize, length, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLint GLAPI GetProgramResourceLocation(GLuint program, GLenum programInterface, const GLchar * name) {
    printf("glGetProgramResourceLocation(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("programInterface=");
    printf("0x%04x", programInterface);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    printf(" -> ");
    GLint result = gl.GetProgramResourceLocation(program, programInterface, name);
    printf("%d", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" GLint GLAPI GetProgramResourceLocationIndex(GLuint program, GLenum programInterface, const GLchar * name) {
    printf("glGetProgramResourceLocationIndex(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("programInterface=");
    printf("0x%04x", programInterface);
    printf(", ");
    printf("name=");
    printf("%p", name);
    printf(")");
    printf(" -> ");
    GLint result = gl.GetProgramResourceLocationIndex(program, programInterface, name);
    printf("%d", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI ShaderStorageBlockBinding(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding) {
    printf("glShaderStorageBlockBinding(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("storageBlockIndex=");
    printf("%u", storageBlockIndex);
    printf(", ");
    printf("storageBlockBinding=");
    printf("%u", storageBlockBinding);
    printf(")");
    gl.ShaderStorageBlockBinding(program, storageBlockIndex, storageBlockBinding);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexBufferRange(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    printf("glTexBufferRange(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(")");
    gl.TexBufferRange(target, internalformat, buffer, offset, size);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
    printf("glTexStorage2DMultisample(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("samples=");
    printf("%d", samples);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("fixedsamplelocations=");
    printf("%s", fixedsamplelocations ? "true" : "false");
    printf(")");
    gl.TexStorage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TexStorage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
    printf("glTexStorage3DMultisample(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("samples=");
    printf("%d", samples);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(", ");
    printf("fixedsamplelocations=");
    printf("%s", fixedsamplelocations ? "true" : "false");
    printf(")");
    gl.TexStorage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureView(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers) {
    printf("glTextureView(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("origtexture=");
    printf("%u", origtexture);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("minlevel=");
    printf("%u", minlevel);
    printf(", ");
    printf("numlevels=");
    printf("%u", numlevels);
    printf(", ");
    printf("minlayer=");
    printf("%u", minlayer);
    printf(", ");
    printf("numlayers=");
    printf("%u", numlayers);
    printf(")");
    gl.TextureView(texture, target, origtexture, internalformat, minlevel, numlevels, minlayer, numlayers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) {
    printf("glBindVertexBuffer(");
    printf("bindingindex=");
    printf("%u", bindingindex);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("stride=");
    printf("%d", stride);
    printf(")");
    gl.BindVertexBuffer(bindingindex, buffer, offset, stride);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
    printf("glVertexAttribFormat(");
    printf("attribindex=");
    printf("%u", attribindex);
    printf(", ");
    printf("size=");
    printf("%d", size);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("normalized=");
    printf("%s", normalized ? "true" : "false");
    printf(", ");
    printf("relativeoffset=");
    printf("%u", relativeoffset);
    printf(")");
    gl.VertexAttribFormat(attribindex, size, type, normalized, relativeoffset);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
    printf("glVertexAttribIFormat(");
    printf("attribindex=");
    printf("%u", attribindex);
    printf(", ");
    printf("size=");
    printf("%d", size);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("relativeoffset=");
    printf("%u", relativeoffset);
    printf(")");
    gl.VertexAttribIFormat(attribindex, size, type, relativeoffset);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribLFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
    printf("glVertexAttribLFormat(");
    printf("attribindex=");
    printf("%u", attribindex);
    printf(", ");
    printf("size=");
    printf("%d", size);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("relativeoffset=");
    printf("%u", relativeoffset);
    printf(")");
    gl.VertexAttribLFormat(attribindex, size, type, relativeoffset);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexAttribBinding(GLuint attribindex, GLuint bindingindex) {
    printf("glVertexAttribBinding(");
    printf("attribindex=");
    printf("%u", attribindex);
    printf(", ");
    printf("bindingindex=");
    printf("%u", bindingindex);
    printf(")");
    gl.VertexAttribBinding(attribindex, bindingindex);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexBindingDivisor(GLuint bindingindex, GLuint divisor) {
    printf("glVertexBindingDivisor(");
    printf("bindingindex=");
    printf("%u", bindingindex);
    printf(", ");
    printf("divisor=");
    printf("%u", divisor);
    printf(")");
    gl.VertexBindingDivisor(bindingindex, divisor);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled) {
    printf("glDebugMessageControl(");
    printf("source=");
    printf("0x%04x", source);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("severity=");
    printf("0x%04x", severity);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("ids=");
    printf("%p", ids);
    printf(", ");
    printf("enabled=");
    printf("%s", enabled ? "true" : "false");
    printf(")");
    gl.DebugMessageControl(source, type, severity, count, ids, enabled);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf) {
    printf("glDebugMessageInsert(");
    printf("source=");
    printf("0x%04x", source);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("severity=");
    printf("0x%04x", severity);
    printf(", ");
    printf("length=");
    printf("%d", length);
    printf(", ");
    printf("buf=");
    printf("%p", buf);
    printf(")");
    gl.DebugMessageInsert(source, type, id, severity, length, buf);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DebugMessageCallback(GLDEBUGPROC callback, const void * userParam) {
    printf("glDebugMessageCallback(");
    printf("callback=");
    printf("%p", callback);
    printf(", ");
    printf("userParam=");
    printf("%p", userParam);
    printf(")");
    gl.DebugMessageCallback(callback, userParam);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLuint GLAPI GetDebugMessageLog(GLuint count, GLsizei bufSize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog) {
    printf("glGetDebugMessageLog(");
    printf("count=");
    printf("%u", count);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("sources=");
    printf("%p", sources);
    printf(", ");
    printf("types=");
    printf("%p", types);
    printf(", ");
    printf("ids=");
    printf("%p", ids);
    printf(", ");
    printf("severities=");
    printf("%p", severities);
    printf(", ");
    printf("lengths=");
    printf("%p", lengths);
    printf(", ");
    printf("messageLog=");
    printf("%p", messageLog);
    printf(")");
    printf(" -> ");
    GLuint result = gl.GetDebugMessageLog(count, bufSize, sources, types, ids, severities, lengths, messageLog);
    printf("%u", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI PushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar * message) {
    printf("glPushDebugGroup(");
    printf("source=");
    printf("0x%04x", source);
    printf(", ");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("length=");
    printf("%d", length);
    printf(", ");
    printf("message=");
    printf("%p", message);
    printf(")");
    gl.PushDebugGroup(source, id, length, message);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PopDebugGroup() {
    printf("glPopDebugGroup(");
    printf(")");
    gl.PopDebugGroup();
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar * label) {
    printf("glObjectLabel(");
    printf("identifier=");
    printf("0x%04x", identifier);
    printf(", ");
    printf("name=");
    printf("%u", name);
    printf(", ");
    printf("length=");
    printf("%d", length);
    printf(", ");
    printf("label=");
    printf("%p", label);
    printf(")");
    gl.ObjectLabel(identifier, name, length, label);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei * length, GLchar * label) {
    printf("glGetObjectLabel(");
    printf("identifier=");
    printf("0x%04x", identifier);
    printf(", ");
    printf("name=");
    printf("%u", name);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("label=");
    printf("%p", label);
    printf(")");
    gl.GetObjectLabel(identifier, name, bufSize, length, label);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ObjectPtrLabel(const void * ptr, GLsizei length, const GLchar * label) {
    printf("glObjectPtrLabel(");
    printf("ptr=");
    printf("%p", ptr);
    printf(", ");
    printf("length=");
    printf("%d", length);
    printf(", ");
    printf("label=");
    printf("%p", label);
    printf(")");
    gl.ObjectPtrLabel(ptr, length, label);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetObjectPtrLabel(const void * ptr, GLsizei bufSize, GLsizei * length, GLchar * label) {
    printf("glGetObjectPtrLabel(");
    printf("ptr=");
    printf("%p", ptr);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("length=");
    printf("%p", length);
    printf(", ");
    printf("label=");
    printf("%p", label);
    printf(")");
    gl.GetObjectPtrLabel(ptr, bufSize, length, label);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BufferStorage(GLenum target, GLsizeiptr size, const void * data, GLbitfield flags) {
    printf("glBufferStorage(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(", ");
    printf("flags=");
    print_buffer_access_bits(flags);
    printf(")");
    gl.BufferStorage(target, size, data, flags);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearTexImage(GLuint texture, GLint level, GLenum format, GLenum type, const void * data) {
    printf("glClearTexImage(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.ClearTexImage(texture, level, format, type, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * data) {
    printf("glClearTexSubImage(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("zoffset=");
    printf("%d", zoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.ClearTexSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindBuffersBase(GLenum target, GLuint first, GLsizei count, const GLuint * buffers) {
    printf("glBindBuffersBase(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("first=");
    printf("%u", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("buffers=");
    printf("%p", buffers);
    printf(")");
    gl.BindBuffersBase(target, first, count, buffers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindBuffersRange(GLenum target, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizeiptr * sizes) {
    printf("glBindBuffersRange(");
    printf("target=");
    print_buffer_target(target);
    printf(", ");
    printf("first=");
    printf("%u", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("buffers=");
    printf("%p", buffers);
    printf(", ");
    printf("offsets=");
    printf("%p", offsets);
    printf(", ");
    printf("sizes=");
    printf("%p", sizes);
    printf(")");
    gl.BindBuffersRange(target, first, count, buffers, offsets, sizes);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindTextures(GLuint first, GLsizei count, const GLuint * textures) {
    printf("glBindTextures(");
    printf("first=");
    printf("%u", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("textures=");
    printf("%p", textures);
    printf(")");
    gl.BindTextures(first, count, textures);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindSamplers(GLuint first, GLsizei count, const GLuint * samplers) {
    printf("glBindSamplers(");
    printf("first=");
    printf("%u", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("samplers=");
    printf("%p", samplers);
    printf(")");
    gl.BindSamplers(first, count, samplers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindImageTextures(GLuint first, GLsizei count, const GLuint * textures) {
    printf("glBindImageTextures(");
    printf("first=");
    printf("%u", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("textures=");
    printf("%p", textures);
    printf(")");
    gl.BindImageTextures(first, count, textures);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindVertexBuffers(GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides) {
    printf("glBindVertexBuffers(");
    printf("first=");
    printf("%u", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("buffers=");
    printf("%p", buffers);
    printf(", ");
    printf("offsets=");
    printf("%p", offsets);
    printf(", ");
    printf("strides=");
    printf("%p", strides);
    printf(")");
    gl.BindVertexBuffers(first, count, buffers, offsets, strides);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClipControl(GLenum origin, GLenum depth) {
    printf("glClipControl(");
    printf("origin=");
    printf("0x%04x", origin);
    printf(", ");
    printf("depth=");
    printf("0x%04x", depth);
    printf(")");
    gl.ClipControl(origin, depth);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CreateTransformFeedbacks(GLsizei n, GLuint * ids) {
    printf("glCreateTransformFeedbacks(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("ids=");
    printf("%p", ids);
    printf(")");
    gl.CreateTransformFeedbacks(n, ids);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TransformFeedbackBufferBase(GLuint xfb, GLuint index, GLuint buffer) {
    printf("glTransformFeedbackBufferBase(");
    printf("xfb=");
    printf("%u", xfb);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(")");
    gl.TransformFeedbackBufferBase(xfb, index, buffer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TransformFeedbackBufferRange(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    printf("glTransformFeedbackBufferRange(");
    printf("xfb=");
    printf("%u", xfb);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(")");
    gl.TransformFeedbackBufferRange(xfb, index, buffer, offset, size);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTransformFeedbackiv(GLuint xfb, GLenum pname, GLint * param) {
    printf("glGetTransformFeedbackiv(");
    printf("xfb=");
    printf("%u", xfb);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.GetTransformFeedbackiv(xfb, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTransformFeedbacki_v(GLuint xfb, GLenum pname, GLuint index, GLint * param) {
    printf("glGetTransformFeedbacki_v(");
    printf("xfb=");
    printf("%u", xfb);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.GetTransformFeedbacki_v(xfb, pname, index, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTransformFeedbacki64_v(GLuint xfb, GLenum pname, GLuint index, GLint64 * param) {
    printf("glGetTransformFeedbacki64_v(");
    printf("xfb=");
    printf("%u", xfb);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.GetTransformFeedbacki64_v(xfb, pname, index, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CreateBuffers(GLsizei n, GLuint * buffers) {
    printf("glCreateBuffers(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("buffers=");
    printf("%p", buffers);
    printf(")");
    gl.CreateBuffers(n, buffers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI NamedBufferStorage(GLuint buffer, GLsizeiptr size, const void * data, GLbitfield flags) {
    printf("glNamedBufferStorage(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(", ");
    printf("flags=");
    printf("0x%08x", flags);
    printf(")");
    gl.NamedBufferStorage(buffer, size, data, flags);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI NamedBufferData(GLuint buffer, GLsizeiptr size, const void * data, GLenum usage) {
    printf("glNamedBufferData(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(", ");
    printf("usage=");
    printf("0x%04x", usage);
    printf(")");
    gl.NamedBufferData(buffer, size, data, usage);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI NamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const void * data) {
    printf("glNamedBufferSubData(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.NamedBufferSubData(buffer, offset, size, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CopyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) {
    printf("glCopyNamedBufferSubData(");
    printf("readBuffer=");
    printf("%u", readBuffer);
    printf(", ");
    printf("writeBuffer=");
    printf("%u", writeBuffer);
    printf(", ");
    printf("readOffset=");
    printf("%lld", readOffset);
    printf(", ");
    printf("writeOffset=");
    printf("%lld", writeOffset);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(")");
    gl.CopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset, size);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearNamedBufferData(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void * data) {
    printf("glClearNamedBufferData(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.ClearNamedBufferData(buffer, internalformat, format, type, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearNamedBufferSubData(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data) {
    printf("glClearNamedBufferSubData(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.ClearNamedBufferSubData(buffer, internalformat, offset, size, format, type, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void * GLAPI MapNamedBuffer(GLuint buffer, GLenum access) {
    printf("glMapNamedBuffer(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("access=");
    print_buffer_access_mode(access);
    printf(")");
    printf(" -> ");
    void * result = gl.MapNamedBuffer(buffer, access);
    printf("%p", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void * GLAPI MapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access) {
    printf("glMapNamedBufferRange(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("length=");
    printf("%lld", length);
    printf(", ");
    printf("access=");
    print_buffer_access_bits(access);
    printf(")");
    printf(" -> ");
    void * result = gl.MapNamedBufferRange(buffer, offset, length, access);
    printf("%p", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" GLboolean GLAPI UnmapNamedBuffer(GLuint buffer) {
    printf("glUnmapNamedBuffer(");
    printf("buffer=");
    printf("%u", buffer);
    printf(")");
    printf(" -> ");
    GLboolean result = gl.UnmapNamedBuffer(buffer);
    printf("%s", result ? "true" : "false");
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI FlushMappedNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length) {
    printf("glFlushMappedNamedBufferRange(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("length=");
    printf("%lld", length);
    printf(")");
    gl.FlushMappedNamedBufferRange(buffer, offset, length);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint * params) {
    printf("glGetNamedBufferParameteriv(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetNamedBufferParameteriv(buffer, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetNamedBufferParameteri64v(GLuint buffer, GLenum pname, GLint64 * params) {
    printf("glGetNamedBufferParameteri64v(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetNamedBufferParameteri64v(buffer, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetNamedBufferPointerv(GLuint buffer, GLenum pname, void ** params) {
    printf("glGetNamedBufferPointerv(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetNamedBufferPointerv(buffer, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, void * data) {
    printf("glGetNamedBufferSubData(");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.GetNamedBufferSubData(buffer, offset, size, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CreateFramebuffers(GLsizei n, GLuint * framebuffers) {
    printf("glCreateFramebuffers(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("framebuffers=");
    printf("%p", framebuffers);
    printf(")");
    gl.CreateFramebuffers(n, framebuffers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI NamedFramebufferRenderbuffer(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
    printf("glNamedFramebufferRenderbuffer(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("attachment=");
    printf("0x%04x", attachment);
    printf(", ");
    printf("renderbuffertarget=");
    printf("0x%04x", renderbuffertarget);
    printf(", ");
    printf("renderbuffer=");
    printf("%u", renderbuffer);
    printf(")");
    gl.NamedFramebufferRenderbuffer(framebuffer, attachment, renderbuffertarget, renderbuffer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI NamedFramebufferParameteri(GLuint framebuffer, GLenum pname, GLint param) {
    printf("glNamedFramebufferParameteri(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%d", param);
    printf(")");
    gl.NamedFramebufferParameteri(framebuffer, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI NamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level) {
    printf("glNamedFramebufferTexture(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("attachment=");
    printf("0x%04x", attachment);
    printf(", ");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(")");
    gl.NamedFramebufferTexture(framebuffer, attachment, texture, level);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI NamedFramebufferTextureLayer(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer) {
    printf("glNamedFramebufferTextureLayer(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("attachment=");
    printf("0x%04x", attachment);
    printf(", ");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("layer=");
    printf("%d", layer);
    printf(")");
    gl.NamedFramebufferTextureLayer(framebuffer, attachment, texture, level, layer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI NamedFramebufferDrawBuffer(GLuint framebuffer, GLenum buf) {
    printf("glNamedFramebufferDrawBuffer(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("buf=");
    printf("0x%04x", buf);
    printf(")");
    gl.NamedFramebufferDrawBuffer(framebuffer, buf);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI NamedFramebufferDrawBuffers(GLuint framebuffer, GLsizei n, const GLenum * bufs) {
    printf("glNamedFramebufferDrawBuffers(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("bufs=");
    printf("%p", bufs);
    printf(")");
    gl.NamedFramebufferDrawBuffers(framebuffer, n, bufs);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI NamedFramebufferReadBuffer(GLuint framebuffer, GLenum src) {
    printf("glNamedFramebufferReadBuffer(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("src=");
    printf("0x%04x", src);
    printf(")");
    gl.NamedFramebufferReadBuffer(framebuffer, src);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI InvalidateNamedFramebufferData(GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments) {
    printf("glInvalidateNamedFramebufferData(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("numAttachments=");
    printf("%d", numAttachments);
    printf(", ");
    printf("attachments=");
    printf("%p", attachments);
    printf(")");
    gl.InvalidateNamedFramebufferData(framebuffer, numAttachments, attachments);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI InvalidateNamedFramebufferSubData(GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height) {
    printf("glInvalidateNamedFramebufferSubData(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("numAttachments=");
    printf("%d", numAttachments);
    printf(", ");
    printf("attachments=");
    printf("%p", attachments);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.InvalidateNamedFramebufferSubData(framebuffer, numAttachments, attachments, x, y, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearNamedFramebufferiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint * value) {
    printf("glClearNamedFramebufferiv(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("buffer=");
    printf("0x%04x", buffer);
    printf(", ");
    printf("drawbuffer=");
    printf("%d", drawbuffer);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ClearNamedFramebufferiv(framebuffer, buffer, drawbuffer, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearNamedFramebufferuiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint * value) {
    printf("glClearNamedFramebufferuiv(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("buffer=");
    printf("0x%04x", buffer);
    printf(", ");
    printf("drawbuffer=");
    printf("%d", drawbuffer);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ClearNamedFramebufferuiv(framebuffer, buffer, drawbuffer, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearNamedFramebufferfv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat * value) {
    printf("glClearNamedFramebufferfv(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("buffer=");
    printf("0x%04x", buffer);
    printf(", ");
    printf("drawbuffer=");
    printf("%d", drawbuffer);
    printf(", ");
    printf("value=");
    printf("%p", value);
    printf(")");
    gl.ClearNamedFramebufferfv(framebuffer, buffer, drawbuffer, value);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ClearNamedFramebufferfi(GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) {
    printf("glClearNamedFramebufferfi(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("buffer=");
    printf("0x%04x", buffer);
    printf(", ");
    printf("drawbuffer=");
    printf("%d", drawbuffer);
    printf(", ");
    printf("depth=");
    printf("%f", depth);
    printf(", ");
    printf("stencil=");
    printf("%d", stencil);
    printf(")");
    gl.ClearNamedFramebufferfi(framebuffer, buffer, drawbuffer, depth, stencil);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BlitNamedFramebuffer(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
    printf("glBlitNamedFramebuffer(");
    printf("readFramebuffer=");
    printf("%u", readFramebuffer);
    printf(", ");
    printf("drawFramebuffer=");
    printf("%u", drawFramebuffer);
    printf(", ");
    printf("srcX0=");
    printf("%d", srcX0);
    printf(", ");
    printf("srcY0=");
    printf("%d", srcY0);
    printf(", ");
    printf("srcX1=");
    printf("%d", srcX1);
    printf(", ");
    printf("srcY1=");
    printf("%d", srcY1);
    printf(", ");
    printf("dstX0=");
    printf("%d", dstX0);
    printf(", ");
    printf("dstY0=");
    printf("%d", dstY0);
    printf(", ");
    printf("dstX1=");
    printf("%d", dstX1);
    printf(", ");
    printf("dstY1=");
    printf("%d", dstY1);
    printf(", ");
    printf("mask=");
    printf("0x%08x", mask);
    printf(", ");
    printf("filter=");
    printf("0x%04x", filter);
    printf(")");
    gl.BlitNamedFramebuffer(readFramebuffer, drawFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLenum GLAPI CheckNamedFramebufferStatus(GLuint framebuffer, GLenum target) {
    printf("glCheckNamedFramebufferStatus(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("target=");
    printf("0x%04x", target);
    printf(")");
    printf(" -> ");
    GLenum result = gl.CheckNamedFramebufferStatus(framebuffer, target);
    printf("0x%04x", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI GetNamedFramebufferParameteriv(GLuint framebuffer, GLenum pname, GLint * param) {
    printf("glGetNamedFramebufferParameteriv(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.GetNamedFramebufferParameteriv(framebuffer, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetNamedFramebufferAttachmentParameteriv(GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params) {
    printf("glGetNamedFramebufferAttachmentParameteriv(");
    printf("framebuffer=");
    printf("%u", framebuffer);
    printf(", ");
    printf("attachment=");
    printf("0x%04x", attachment);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetNamedFramebufferAttachmentParameteriv(framebuffer, attachment, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CreateRenderbuffers(GLsizei n, GLuint * renderbuffers) {
    printf("glCreateRenderbuffers(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("renderbuffers=");
    printf("%p", renderbuffers);
    printf(")");
    gl.CreateRenderbuffers(n, renderbuffers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI NamedRenderbufferStorage(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height) {
    printf("glNamedRenderbufferStorage(");
    printf("renderbuffer=");
    printf("%u", renderbuffer);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.NamedRenderbufferStorage(renderbuffer, internalformat, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI NamedRenderbufferStorageMultisample(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
    printf("glNamedRenderbufferStorageMultisample(");
    printf("renderbuffer=");
    printf("%u", renderbuffer);
    printf(", ");
    printf("samples=");
    printf("%d", samples);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.NamedRenderbufferStorageMultisample(renderbuffer, samples, internalformat, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetNamedRenderbufferParameteriv(GLuint renderbuffer, GLenum pname, GLint * params) {
    printf("glGetNamedRenderbufferParameteriv(");
    printf("renderbuffer=");
    printf("%u", renderbuffer);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetNamedRenderbufferParameteriv(renderbuffer, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CreateTextures(GLenum target, GLsizei n, GLuint * textures) {
    printf("glCreateTextures(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("textures=");
    printf("%p", textures);
    printf(")");
    gl.CreateTextures(target, n, textures);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureBuffer(GLuint texture, GLenum internalformat, GLuint buffer) {
    printf("glTextureBuffer(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(")");
    gl.TextureBuffer(texture, internalformat, buffer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureBufferRange(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    printf("glTextureBufferRange(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("size=");
    printf("%lld", size);
    printf(")");
    gl.TextureBufferRange(texture, internalformat, buffer, offset, size);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureStorage1D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width) {
    printf("glTextureStorage1D(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("levels=");
    printf("%d", levels);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(")");
    gl.TextureStorage1D(texture, levels, internalformat, width);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) {
    printf("glTextureStorage2D(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("levels=");
    printf("%d", levels);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.TextureStorage2D(texture, levels, internalformat, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureStorage3D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) {
    printf("glTextureStorage3D(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("levels=");
    printf("%d", levels);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(")");
    gl.TextureStorage3D(texture, levels, internalformat, width, height, depth);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureStorage2DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
    printf("glTextureStorage2DMultisample(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("samples=");
    printf("%d", samples);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("fixedsamplelocations=");
    printf("%s", fixedsamplelocations ? "true" : "false");
    printf(")");
    gl.TextureStorage2DMultisample(texture, samples, internalformat, width, height, fixedsamplelocations);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureStorage3DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
    printf("glTextureStorage3DMultisample(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("samples=");
    printf("%d", samples);
    printf(", ");
    printf("internalformat=");
    printf("0x%04x", internalformat);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(", ");
    printf("fixedsamplelocations=");
    printf("%s", fixedsamplelocations ? "true" : "false");
    printf(")");
    gl.TextureStorage3DMultisample(texture, samples, internalformat, width, height, depth, fixedsamplelocations);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels) {
    printf("glTextureSubImage1D(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.TextureSubImage1D(texture, level, xoffset, width, format, type, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels) {
    printf("glTextureSubImage2D(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.TextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, type, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels) {
    printf("glTextureSubImage3D(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("zoffset=");
    printf("%d", zoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.TextureSubImage3D(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CompressedTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data) {
    printf("glCompressedTextureSubImage1D(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("imageSize=");
    printf("%d", imageSize);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.CompressedTextureSubImage1D(texture, level, xoffset, width, format, imageSize, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CompressedTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data) {
    printf("glCompressedTextureSubImage2D(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("imageSize=");
    printf("%d", imageSize);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.CompressedTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, imageSize, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CompressedTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data) {
    printf("glCompressedTextureSubImage3D(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("zoffset=");
    printf("%d", zoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("imageSize=");
    printf("%d", imageSize);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.CompressedTextureSubImage3D(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CopyTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {
    printf("glCopyTextureSubImage1D(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(")");
    gl.CopyTextureSubImage1D(texture, level, xoffset, x, y, width);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CopyTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
    printf("glCopyTextureSubImage2D(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.CopyTextureSubImage2D(texture, level, xoffset, yoffset, x, y, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CopyTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
    printf("glCopyTextureSubImage3D(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("zoffset=");
    printf("%d", zoffset);
    printf(", ");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(")");
    gl.CopyTextureSubImage3D(texture, level, xoffset, yoffset, zoffset, x, y, width, height);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureParameterf(GLuint texture, GLenum pname, GLfloat param) {
    printf("glTextureParameterf(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%f", param);
    printf(")");
    gl.TextureParameterf(texture, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureParameterfv(GLuint texture, GLenum pname, const GLfloat * param) {
    printf("glTextureParameterfv(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.TextureParameterfv(texture, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureParameteri(GLuint texture, GLenum pname, GLint param) {
    printf("glTextureParameteri(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%d", param);
    printf(")");
    gl.TextureParameteri(texture, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureParameterIiv(GLuint texture, GLenum pname, const GLint * params) {
    printf("glTextureParameterIiv(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.TextureParameterIiv(texture, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureParameterIuiv(GLuint texture, GLenum pname, const GLuint * params) {
    printf("glTextureParameterIuiv(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.TextureParameterIuiv(texture, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureParameteriv(GLuint texture, GLenum pname, const GLint * param) {
    printf("glTextureParameteriv(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.TextureParameteriv(texture, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GenerateTextureMipmap(GLuint texture) {
    printf("glGenerateTextureMipmap(");
    printf("texture=");
    printf("%u", texture);
    printf(")");
    gl.GenerateTextureMipmap(texture);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI BindTextureUnit(GLuint unit, GLuint texture) {
    printf("glBindTextureUnit(");
    printf("unit=");
    printf("%u", unit);
    printf(", ");
    printf("texture=");
    printf("%u", texture);
    printf(")");
    gl.BindTextureUnit(unit, texture);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTextureImage(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void * pixels) {
    printf("glGetTextureImage(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.GetTextureImage(texture, level, format, type, bufSize, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetCompressedTextureImage(GLuint texture, GLint level, GLsizei bufSize, void * pixels) {
    printf("glGetCompressedTextureImage(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.GetCompressedTextureImage(texture, level, bufSize, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTextureLevelParameterfv(GLuint texture, GLint level, GLenum pname, GLfloat * params) {
    printf("glGetTextureLevelParameterfv(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetTextureLevelParameterfv(texture, level, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTextureLevelParameteriv(GLuint texture, GLint level, GLenum pname, GLint * params) {
    printf("glGetTextureLevelParameteriv(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetTextureLevelParameteriv(texture, level, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTextureParameterfv(GLuint texture, GLenum pname, GLfloat * params) {
    printf("glGetTextureParameterfv(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetTextureParameterfv(texture, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTextureParameterIiv(GLuint texture, GLenum pname, GLint * params) {
    printf("glGetTextureParameterIiv(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetTextureParameterIiv(texture, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTextureParameterIuiv(GLuint texture, GLenum pname, GLuint * params) {
    printf("glGetTextureParameterIuiv(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetTextureParameterIuiv(texture, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTextureParameteriv(GLuint texture, GLenum pname, GLint * params) {
    printf("glGetTextureParameteriv(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetTextureParameteriv(texture, pname, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CreateVertexArrays(GLsizei n, GLuint * arrays) {
    printf("glCreateVertexArrays(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("arrays=");
    printf("%p", arrays);
    printf(")");
    gl.CreateVertexArrays(n, arrays);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI DisableVertexArrayAttrib(GLuint vaobj, GLuint index) {
    printf("glDisableVertexArrayAttrib(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(")");
    gl.DisableVertexArrayAttrib(vaobj, index);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI EnableVertexArrayAttrib(GLuint vaobj, GLuint index) {
    printf("glEnableVertexArrayAttrib(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(")");
    gl.EnableVertexArrayAttrib(vaobj, index);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexArrayElementBuffer(GLuint vaobj, GLuint buffer) {
    printf("glVertexArrayElementBuffer(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(")");
    gl.VertexArrayElementBuffer(vaobj, buffer);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) {
    printf("glVertexArrayVertexBuffer(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("bindingindex=");
    printf("%u", bindingindex);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(", ");
    printf("stride=");
    printf("%d", stride);
    printf(")");
    gl.VertexArrayVertexBuffer(vaobj, bindingindex, buffer, offset, stride);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexArrayVertexBuffers(GLuint vaobj, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides) {
    printf("glVertexArrayVertexBuffers(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("first=");
    printf("%u", first);
    printf(", ");
    printf("count=");
    printf("%d", count);
    printf(", ");
    printf("buffers=");
    printf("%p", buffers);
    printf(", ");
    printf("offsets=");
    printf("%p", offsets);
    printf(", ");
    printf("strides=");
    printf("%p", strides);
    printf(")");
    gl.VertexArrayVertexBuffers(vaobj, first, count, buffers, offsets, strides);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexArrayAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex) {
    printf("glVertexArrayAttribBinding(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("attribindex=");
    printf("%u", attribindex);
    printf(", ");
    printf("bindingindex=");
    printf("%u", bindingindex);
    printf(")");
    gl.VertexArrayAttribBinding(vaobj, attribindex, bindingindex);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexArrayAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
    printf("glVertexArrayAttribFormat(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("attribindex=");
    printf("%u", attribindex);
    printf(", ");
    printf("size=");
    printf("%d", size);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("normalized=");
    printf("%s", normalized ? "true" : "false");
    printf(", ");
    printf("relativeoffset=");
    printf("%u", relativeoffset);
    printf(")");
    gl.VertexArrayAttribFormat(vaobj, attribindex, size, type, normalized, relativeoffset);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexArrayAttribIFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
    printf("glVertexArrayAttribIFormat(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("attribindex=");
    printf("%u", attribindex);
    printf(", ");
    printf("size=");
    printf("%d", size);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("relativeoffset=");
    printf("%u", relativeoffset);
    printf(")");
    gl.VertexArrayAttribIFormat(vaobj, attribindex, size, type, relativeoffset);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexArrayAttribLFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
    printf("glVertexArrayAttribLFormat(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("attribindex=");
    printf("%u", attribindex);
    printf(", ");
    printf("size=");
    printf("%d", size);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("relativeoffset=");
    printf("%u", relativeoffset);
    printf(")");
    gl.VertexArrayAttribLFormat(vaobj, attribindex, size, type, relativeoffset);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI VertexArrayBindingDivisor(GLuint vaobj, GLuint bindingindex, GLuint divisor) {
    printf("glVertexArrayBindingDivisor(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("bindingindex=");
    printf("%u", bindingindex);
    printf(", ");
    printf("divisor=");
    printf("%u", divisor);
    printf(")");
    gl.VertexArrayBindingDivisor(vaobj, bindingindex, divisor);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetVertexArrayiv(GLuint vaobj, GLenum pname, GLint * param) {
    printf("glGetVertexArrayiv(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.GetVertexArrayiv(vaobj, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetVertexArrayIndexediv(GLuint vaobj, GLuint index, GLenum pname, GLint * param) {
    printf("glGetVertexArrayIndexediv(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.GetVertexArrayIndexediv(vaobj, index, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetVertexArrayIndexed64iv(GLuint vaobj, GLuint index, GLenum pname, GLint64 * param) {
    printf("glGetVertexArrayIndexed64iv(");
    printf("vaobj=");
    printf("%u", vaobj);
    printf(", ");
    printf("index=");
    printf("%u", index);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("param=");
    printf("%p", param);
    printf(")");
    gl.GetVertexArrayIndexed64iv(vaobj, index, pname, param);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CreateSamplers(GLsizei n, GLuint * samplers) {
    printf("glCreateSamplers(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("samplers=");
    printf("%p", samplers);
    printf(")");
    gl.CreateSamplers(n, samplers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CreateProgramPipelines(GLsizei n, GLuint * pipelines) {
    printf("glCreateProgramPipelines(");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("pipelines=");
    printf("%p", pipelines);
    printf(")");
    gl.CreateProgramPipelines(n, pipelines);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI CreateQueries(GLenum target, GLsizei n, GLuint * ids) {
    printf("glCreateQueries(");
    printf("target=");
    printf("0x%04x", target);
    printf(", ");
    printf("n=");
    printf("%d", n);
    printf(", ");
    printf("ids=");
    printf("%p", ids);
    printf(")");
    gl.CreateQueries(target, n, ids);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetQueryBufferObjecti64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset) {
    printf("glGetQueryBufferObjecti64v(");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(")");
    gl.GetQueryBufferObjecti64v(id, buffer, pname, offset);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetQueryBufferObjectiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset) {
    printf("glGetQueryBufferObjectiv(");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(")");
    gl.GetQueryBufferObjectiv(id, buffer, pname, offset);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetQueryBufferObjectui64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset) {
    printf("glGetQueryBufferObjectui64v(");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(")");
    gl.GetQueryBufferObjectui64v(id, buffer, pname, offset);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetQueryBufferObjectuiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset) {
    printf("glGetQueryBufferObjectuiv(");
    printf("id=");
    printf("%u", id);
    printf(", ");
    printf("buffer=");
    printf("%u", buffer);
    printf(", ");
    printf("pname=");
    printf("0x%04x", pname);
    printf(", ");
    printf("offset=");
    printf("%lld", offset);
    printf(")");
    gl.GetQueryBufferObjectuiv(id, buffer, pname, offset);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI MemoryBarrierByRegion(GLbitfield barriers) {
    printf("glMemoryBarrierByRegion(");
    printf("barriers=");
    printf("0x%08x", barriers);
    printf(")");
    gl.MemoryBarrierByRegion(barriers);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void * pixels) {
    printf("glGetTextureSubImage(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("zoffset=");
    printf("%d", zoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.GetTextureSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, bufSize, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetCompressedTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void * pixels) {
    printf("glGetCompressedTextureSubImage(");
    printf("texture=");
    printf("%u", texture);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("xoffset=");
    printf("%d", xoffset);
    printf(", ");
    printf("yoffset=");
    printf("%d", yoffset);
    printf(", ");
    printf("zoffset=");
    printf("%d", zoffset);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("depth=");
    printf("%d", depth);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.GetCompressedTextureSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, bufSize, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" GLenum GLAPI GetGraphicsResetStatus() {
    printf("glGetGraphicsResetStatus(");
    printf(")");
    printf(" -> ");
    GLenum result = gl.GetGraphicsResetStatus();
    printf("0x%04x", result);
    print_error();
    printf("\n");
    fflush(stdout);
    return result;
}

extern "C" void GLAPI GetnCompressedTexImage(GLenum target, GLint lod, GLsizei bufSize, void * pixels) {
    printf("glGetnCompressedTexImage(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("lod=");
    printf("%d", lod);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.GetnCompressedTexImage(target, lod, bufSize, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetnTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void * pixels) {
    printf("glGetnTexImage(");
    printf("target=");
    print_texture_target(target);
    printf(", ");
    printf("level=");
    printf("%d", level);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("pixels=");
    printf("%p", pixels);
    printf(")");
    gl.GetnTexImage(target, level, format, type, bufSize, pixels);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetnUniformdv(GLuint program, GLint location, GLsizei bufSize, GLdouble * params) {
    printf("glGetnUniformdv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetnUniformdv(program, location, bufSize, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetnUniformfv(GLuint program, GLint location, GLsizei bufSize, GLfloat * params) {
    printf("glGetnUniformfv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetnUniformfv(program, location, bufSize, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint * params) {
    printf("glGetnUniformiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetnUniformiv(program, location, bufSize, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI GetnUniformuiv(GLuint program, GLint location, GLsizei bufSize, GLuint * params) {
    printf("glGetnUniformuiv(");
    printf("program=");
    printf("%u", program);
    printf(", ");
    printf("location=");
    printf("%d", location);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("params=");
    printf("%p", params);
    printf(")");
    gl.GetnUniformuiv(program, location, bufSize, params);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI ReadnPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void * data) {
    printf("glReadnPixels(");
    printf("x=");
    printf("%d", x);
    printf(", ");
    printf("y=");
    printf("%d", y);
    printf(", ");
    printf("width=");
    printf("%d", width);
    printf(", ");
    printf("height=");
    printf("%d", height);
    printf(", ");
    printf("format=");
    printf("0x%04x", format);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("bufSize=");
    printf("%d", bufSize);
    printf(", ");
    printf("data=");
    printf("%p", data);
    printf(")");
    gl.ReadnPixels(x, y, width, height, format, type, bufSize, data);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI TextureBarrier() {
    printf("glTextureBarrier(");
    printf(")");
    gl.TextureBarrier();
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI SpecializeShader(GLuint shader, const GLchar * pEntryPoint, GLuint numSpecializationConstants, const GLuint * pConstantIndex, const GLuint * pConstantValue) {
    printf("glSpecializeShader(");
    printf("shader=");
    printf("%u", shader);
    printf(", ");
    printf("pEntryPoint=");
    printf("%p", pEntryPoint);
    printf(", ");
    printf("numSpecializationConstants=");
    printf("%u", numSpecializationConstants);
    printf(", ");
    printf("pConstantIndex=");
    printf("%p", pConstantIndex);
    printf(", ");
    printf("pConstantValue=");
    printf("%p", pConstantValue);
    printf(")");
    gl.SpecializeShader(shader, pEntryPoint, numSpecializationConstants, pConstantIndex, pConstantValue);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI MultiDrawArraysIndirectCount(GLenum mode, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride) {
    printf("glMultiDrawArraysIndirectCount(");
    printf("mode=");
    printf("0x%04x", mode);
    printf(", ");
    printf("indirect=");
    printf("%p", indirect);
    printf(", ");
    printf("drawcount=");
    printf("%lld", drawcount);
    printf(", ");
    printf("maxdrawcount=");
    printf("%d", maxdrawcount);
    printf(", ");
    printf("stride=");
    printf("%d", stride);
    printf(")");
    gl.MultiDrawArraysIndirectCount(mode, indirect, drawcount, maxdrawcount, stride);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI MultiDrawElementsIndirectCount(GLenum mode, GLenum type, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride) {
    printf("glMultiDrawElementsIndirectCount(");
    printf("mode=");
    printf("0x%04x", mode);
    printf(", ");
    printf("type=");
    printf("0x%04x", type);
    printf(", ");
    printf("indirect=");
    printf("%p", indirect);
    printf(", ");
    printf("drawcount=");
    printf("%lld", drawcount);
    printf(", ");
    printf("maxdrawcount=");
    printf("%d", maxdrawcount);
    printf(", ");
    printf("stride=");
    printf("%d", stride);
    printf(")");
    gl.MultiDrawElementsIndirectCount(mode, type, indirect, drawcount, maxdrawcount, stride);
    print_error();
    printf("\n");
    fflush(stdout);
}

extern "C" void GLAPI PolygonOffsetClamp(GLfloat factor, GLfloat units, GLfloat clamp) {
    printf("glPolygonOffsetClamp(");
    printf("factor=");
    printf("%f", factor);
    printf(", ");
    printf("units=");
    printf("%f", units);
    printf(", ");
    printf("clamp=");
    printf("%f", clamp);
    printf(")");
    gl.PolygonOffsetClamp(factor, units, clamp);
    print_error();
    printf("\n");
    fflush(stdout);
}

GLMethods traces = {
    (PFNGLCULLFACEPROC)CullFace,
    (PFNGLFRONTFACEPROC)FrontFace,
    (PFNGLHINTPROC)Hint,
    (PFNGLLINEWIDTHPROC)LineWidth,
    (PFNGLPOINTSIZEPROC)PointSize,
    (PFNGLPOLYGONMODEPROC)PolygonMode,
    (PFNGLSCISSORPROC)Scissor,
    (PFNGLTEXPARAMETERFPROC)TexParameterf,
    (PFNGLTEXPARAMETERFVPROC)TexParameterfv,
    (PFNGLTEXPARAMETERIPROC)TexParameteri,
    (PFNGLTEXPARAMETERIVPROC)TexParameteriv,
    (PFNGLTEXIMAGE1DPROC)TexImage1D,
    (PFNGLTEXIMAGE2DPROC)TexImage2D,
    (PFNGLDRAWBUFFERPROC)DrawBuffer,
    (PFNGLCLEARPROC)Clear,
    (PFNGLCLEARCOLORPROC)ClearColor,
    (PFNGLCLEARSTENCILPROC)ClearStencil,
    (PFNGLCLEARDEPTHPROC)ClearDepth,
    (PFNGLSTENCILMASKPROC)StencilMask,
    (PFNGLCOLORMASKPROC)ColorMask,
    (PFNGLDEPTHMASKPROC)DepthMask,
    (PFNGLDISABLEPROC)Disable,
    (PFNGLENABLEPROC)Enable,
    (PFNGLFINISHPROC)Finish,
    (PFNGLFLUSHPROC)Flush,
    (PFNGLBLENDFUNCPROC)BlendFunc,
    (PFNGLLOGICOPPROC)LogicOp,
    (PFNGLSTENCILFUNCPROC)StencilFunc,
    (PFNGLSTENCILOPPROC)StencilOp,
    (PFNGLDEPTHFUNCPROC)DepthFunc,
    (PFNGLPIXELSTOREFPROC)PixelStoref,
    (PFNGLPIXELSTOREIPROC)PixelStorei,
    (PFNGLREADBUFFERPROC)ReadBuffer,
    (PFNGLREADPIXELSPROC)ReadPixels,
    (PFNGLGETBOOLEANVPROC)GetBooleanv,
    (PFNGLGETDOUBLEVPROC)GetDoublev,
    (PFNGLGETERRORPROC)GetError,
    (PFNGLGETFLOATVPROC)GetFloatv,
    (PFNGLGETINTEGERVPROC)GetIntegerv,
    (PFNGLGETSTRINGPROC)GetString,
    (PFNGLGETTEXIMAGEPROC)GetTexImage,
    (PFNGLGETTEXPARAMETERFVPROC)GetTexParameterfv,
    (PFNGLGETTEXPARAMETERIVPROC)GetTexParameteriv,
    (PFNGLGETTEXLEVELPARAMETERFVPROC)GetTexLevelParameterfv,
    (PFNGLGETTEXLEVELPARAMETERIVPROC)GetTexLevelParameteriv,
    (PFNGLISENABLEDPROC)IsEnabled,
    (PFNGLDEPTHRANGEPROC)DepthRange,
    (PFNGLVIEWPORTPROC)Viewport,
    (PFNGLDRAWARRAYSPROC)DrawArrays,
    (PFNGLDRAWELEMENTSPROC)DrawElements,
    (PFNGLGETPOINTERVPROC)GetPointerv,
    (PFNGLPOLYGONOFFSETPROC)PolygonOffset,
    (PFNGLCOPYTEXIMAGE1DPROC)CopyTexImage1D,
    (PFNGLCOPYTEXIMAGE2DPROC)CopyTexImage2D,
    (PFNGLCOPYTEXSUBIMAGE1DPROC)CopyTexSubImage1D,
    (PFNGLCOPYTEXSUBIMAGE2DPROC)CopyTexSubImage2D,
    (PFNGLTEXSUBIMAGE1DPROC)TexSubImage1D,
    (PFNGLTEXSUBIMAGE2DPROC)TexSubImage2D,
    (PFNGLBINDTEXTUREPROC)BindTexture,
    (PFNGLDELETETEXTURESPROC)DeleteTextures,
    (PFNGLGENTEXTURESPROC)GenTextures,
    (PFNGLISTEXTUREPROC)IsTexture,
    (PFNGLDRAWRANGEELEMENTSPROC)DrawRangeElements,
    (PFNGLTEXIMAGE3DPROC)TexImage3D,
    (PFNGLTEXSUBIMAGE3DPROC)TexSubImage3D,
    (PFNGLCOPYTEXSUBIMAGE3DPROC)CopyTexSubImage3D,
    (PFNGLACTIVETEXTUREPROC)ActiveTexture,
    (PFNGLSAMPLECOVERAGEPROC)SampleCoverage,
    (PFNGLCOMPRESSEDTEXIMAGE3DPROC)CompressedTexImage3D,
    (PFNGLCOMPRESSEDTEXIMAGE2DPROC)CompressedTexImage2D,
    (PFNGLCOMPRESSEDTEXIMAGE1DPROC)CompressedTexImage1D,
    (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)CompressedTexSubImage3D,
    (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)CompressedTexSubImage2D,
    (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)CompressedTexSubImage1D,
    (PFNGLGETCOMPRESSEDTEXIMAGEPROC)GetCompressedTexImage,
    (PFNGLBLENDFUNCSEPARATEPROC)BlendFuncSeparate,
    (PFNGLMULTIDRAWARRAYSPROC)MultiDrawArrays,
    (PFNGLMULTIDRAWELEMENTSPROC)MultiDrawElements,
    (PFNGLPOINTPARAMETERFPROC)PointParameterf,
    (PFNGLPOINTPARAMETERFVPROC)PointParameterfv,
    (PFNGLPOINTPARAMETERIPROC)PointParameteri,
    (PFNGLPOINTPARAMETERIVPROC)PointParameteriv,
    (PFNGLBLENDCOLORPROC)BlendColor,
    (PFNGLBLENDEQUATIONPROC)BlendEquation,
    (PFNGLGENQUERIESPROC)GenQueries,
    (PFNGLDELETEQUERIESPROC)DeleteQueries,
    (PFNGLISQUERYPROC)IsQuery,
    (PFNGLBEGINQUERYPROC)BeginQuery,
    (PFNGLENDQUERYPROC)EndQuery,
    (PFNGLGETQUERYIVPROC)GetQueryiv,
    (PFNGLGETQUERYOBJECTIVPROC)GetQueryObjectiv,
    (PFNGLGETQUERYOBJECTUIVPROC)GetQueryObjectuiv,
    (PFNGLBINDBUFFERPROC)BindBuffer,
    (PFNGLDELETEBUFFERSPROC)DeleteBuffers,
    (PFNGLGENBUFFERSPROC)GenBuffers,
    (PFNGLISBUFFERPROC)IsBuffer,
    (PFNGLBUFFERDATAPROC)BufferData,
    (PFNGLBUFFERSUBDATAPROC)BufferSubData,
    (PFNGLGETBUFFERSUBDATAPROC)GetBufferSubData,
    (PFNGLMAPBUFFERPROC)MapBuffer,
    (PFNGLUNMAPBUFFERPROC)UnmapBuffer,
    (PFNGLGETBUFFERPARAMETERIVPROC)GetBufferParameteriv,
    (PFNGLGETBUFFERPOINTERVPROC)GetBufferPointerv,
    (PFNGLBLENDEQUATIONSEPARATEPROC)BlendEquationSeparate,
    (PFNGLDRAWBUFFERSPROC)DrawBuffers,
    (PFNGLSTENCILOPSEPARATEPROC)StencilOpSeparate,
    (PFNGLSTENCILFUNCSEPARATEPROC)StencilFuncSeparate,
    (PFNGLSTENCILMASKSEPARATEPROC)StencilMaskSeparate,
    (PFNGLATTACHSHADERPROC)AttachShader,
    (PFNGLBINDATTRIBLOCATIONPROC)BindAttribLocation,
    (PFNGLCOMPILESHADERPROC)CompileShader,
    (PFNGLCREATEPROGRAMPROC)CreateProgram,
    (PFNGLCREATESHADERPROC)CreateShader,
    (PFNGLDELETEPROGRAMPROC)DeleteProgram,
    (PFNGLDELETESHADERPROC)DeleteShader,
    (PFNGLDETACHSHADERPROC)DetachShader,
    (PFNGLDISABLEVERTEXATTRIBARRAYPROC)DisableVertexAttribArray,
    (PFNGLENABLEVERTEXATTRIBARRAYPROC)EnableVertexAttribArray,
    (PFNGLGETACTIVEATTRIBPROC)GetActiveAttrib,
    (PFNGLGETACTIVEUNIFORMPROC)GetActiveUniform,
    (PFNGLGETATTACHEDSHADERSPROC)GetAttachedShaders,
    (PFNGLGETATTRIBLOCATIONPROC)GetAttribLocation,
    (PFNGLGETPROGRAMIVPROC)GetProgramiv,
    (PFNGLGETPROGRAMINFOLOGPROC)GetProgramInfoLog,
    (PFNGLGETSHADERIVPROC)GetShaderiv,
    (PFNGLGETSHADERINFOLOGPROC)GetShaderInfoLog,
    (PFNGLGETSHADERSOURCEPROC)GetShaderSource,
    (PFNGLGETUNIFORMLOCATIONPROC)GetUniformLocation,
    (PFNGLGETUNIFORMFVPROC)GetUniformfv,
    (PFNGLGETUNIFORMIVPROC)GetUniformiv,
    (PFNGLGETVERTEXATTRIBDVPROC)GetVertexAttribdv,
    (PFNGLGETVERTEXATTRIBFVPROC)GetVertexAttribfv,
    (PFNGLGETVERTEXATTRIBIVPROC)GetVertexAttribiv,
    (PFNGLGETVERTEXATTRIBPOINTERVPROC)GetVertexAttribPointerv,
    (PFNGLISPROGRAMPROC)IsProgram,
    (PFNGLISSHADERPROC)IsShader,
    (PFNGLLINKPROGRAMPROC)LinkProgram,
    (PFNGLSHADERSOURCEPROC)ShaderSource,
    (PFNGLUSEPROGRAMPROC)UseProgram,
    (PFNGLUNIFORM1FPROC)Uniform1f,
    (PFNGLUNIFORM2FPROC)Uniform2f,
    (PFNGLUNIFORM3FPROC)Uniform3f,
    (PFNGLUNIFORM4FPROC)Uniform4f,
    (PFNGLUNIFORM1IPROC)Uniform1i,
    (PFNGLUNIFORM2IPROC)Uniform2i,
    (PFNGLUNIFORM3IPROC)Uniform3i,
    (PFNGLUNIFORM4IPROC)Uniform4i,
    (PFNGLUNIFORM1FVPROC)Uniform1fv,
    (PFNGLUNIFORM2FVPROC)Uniform2fv,
    (PFNGLUNIFORM3FVPROC)Uniform3fv,
    (PFNGLUNIFORM4FVPROC)Uniform4fv,
    (PFNGLUNIFORM1IVPROC)Uniform1iv,
    (PFNGLUNIFORM2IVPROC)Uniform2iv,
    (PFNGLUNIFORM3IVPROC)Uniform3iv,
    (PFNGLUNIFORM4IVPROC)Uniform4iv,
    (PFNGLUNIFORMMATRIX2FVPROC)UniformMatrix2fv,
    (PFNGLUNIFORMMATRIX3FVPROC)UniformMatrix3fv,
    (PFNGLUNIFORMMATRIX4FVPROC)UniformMatrix4fv,
    (PFNGLVALIDATEPROGRAMPROC)ValidateProgram,
    (PFNGLVERTEXATTRIB1DPROC)VertexAttrib1d,
    (PFNGLVERTEXATTRIB1DVPROC)VertexAttrib1dv,
    (PFNGLVERTEXATTRIB1FPROC)VertexAttrib1f,
    (PFNGLVERTEXATTRIB1FVPROC)VertexAttrib1fv,
    (PFNGLVERTEXATTRIB1SPROC)VertexAttrib1s,
    (PFNGLVERTEXATTRIB1SVPROC)VertexAttrib1sv,
    (PFNGLVERTEXATTRIB2DPROC)VertexAttrib2d,
    (PFNGLVERTEXATTRIB2DVPROC)VertexAttrib2dv,
    (PFNGLVERTEXATTRIB2FPROC)VertexAttrib2f,
    (PFNGLVERTEXATTRIB2FVPROC)VertexAttrib2fv,
    (PFNGLVERTEXATTRIB2SPROC)VertexAttrib2s,
    (PFNGLVERTEXATTRIB2SVPROC)VertexAttrib2sv,
    (PFNGLVERTEXATTRIB3DPROC)VertexAttrib3d,
    (PFNGLVERTEXATTRIB3DVPROC)VertexAttrib3dv,
    (PFNGLVERTEXATTRIB3FPROC)VertexAttrib3f,
    (PFNGLVERTEXATTRIB3FVPROC)VertexAttrib3fv,
    (PFNGLVERTEXATTRIB3SPROC)VertexAttrib3s,
    (PFNGLVERTEXATTRIB3SVPROC)VertexAttrib3sv,
    (PFNGLVERTEXATTRIB4NBVPROC)VertexAttrib4Nbv,
    (PFNGLVERTEXATTRIB4NIVPROC)VertexAttrib4Niv,
    (PFNGLVERTEXATTRIB4NSVPROC)VertexAttrib4Nsv,
    (PFNGLVERTEXATTRIB4NUBPROC)VertexAttrib4Nub,
    (PFNGLVERTEXATTRIB4NUBVPROC)VertexAttrib4Nubv,
    (PFNGLVERTEXATTRIB4NUIVPROC)VertexAttrib4Nuiv,
    (PFNGLVERTEXATTRIB4NUSVPROC)VertexAttrib4Nusv,
    (PFNGLVERTEXATTRIB4BVPROC)VertexAttrib4bv,
    (PFNGLVERTEXATTRIB4DPROC)VertexAttrib4d,
    (PFNGLVERTEXATTRIB4DVPROC)VertexAttrib4dv,
    (PFNGLVERTEXATTRIB4FPROC)VertexAttrib4f,
    (PFNGLVERTEXATTRIB4FVPROC)VertexAttrib4fv,
    (PFNGLVERTEXATTRIB4IVPROC)VertexAttrib4iv,
    (PFNGLVERTEXATTRIB4SPROC)VertexAttrib4s,
    (PFNGLVERTEXATTRIB4SVPROC)VertexAttrib4sv,
    (PFNGLVERTEXATTRIB4UBVPROC)VertexAttrib4ubv,
    (PFNGLVERTEXATTRIB4UIVPROC)VertexAttrib4uiv,
    (PFNGLVERTEXATTRIB4USVPROC)VertexAttrib4usv,
    (PFNGLVERTEXATTRIBPOINTERPROC)VertexAttribPointer,
    (PFNGLUNIFORMMATRIX2X3FVPROC)UniformMatrix2x3fv,
    (PFNGLUNIFORMMATRIX3X2FVPROC)UniformMatrix3x2fv,
    (PFNGLUNIFORMMATRIX2X4FVPROC)UniformMatrix2x4fv,
    (PFNGLUNIFORMMATRIX4X2FVPROC)UniformMatrix4x2fv,
    (PFNGLUNIFORMMATRIX3X4FVPROC)UniformMatrix3x4fv,
    (PFNGLUNIFORMMATRIX4X3FVPROC)UniformMatrix4x3fv,
    (PFNGLCOLORMASKIPROC)ColorMaski,
    (PFNGLGETBOOLEANI_VPROC)GetBooleani_v,
    (PFNGLGETINTEGERI_VPROC)GetIntegeri_v,
    (PFNGLENABLEIPROC)Enablei,
    (PFNGLDISABLEIPROC)Disablei,
    (PFNGLISENABLEDIPROC)IsEnabledi,
    (PFNGLBEGINTRANSFORMFEEDBACKPROC)BeginTransformFeedback,
    (PFNGLENDTRANSFORMFEEDBACKPROC)EndTransformFeedback,
    (PFNGLBINDBUFFERRANGEPROC)BindBufferRange,
    (PFNGLBINDBUFFERBASEPROC)BindBufferBase,
    (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)TransformFeedbackVaryings,
    (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)GetTransformFeedbackVarying,
    (PFNGLCLAMPCOLORPROC)ClampColor,
    (PFNGLBEGINCONDITIONALRENDERPROC)BeginConditionalRender,
    (PFNGLENDCONDITIONALRENDERPROC)EndConditionalRender,
    (PFNGLVERTEXATTRIBIPOINTERPROC)VertexAttribIPointer,
    (PFNGLGETVERTEXATTRIBIIVPROC)GetVertexAttribIiv,
    (PFNGLGETVERTEXATTRIBIUIVPROC)GetVertexAttribIuiv,
    (PFNGLVERTEXATTRIBI1IPROC)VertexAttribI1i,
    (PFNGLVERTEXATTRIBI2IPROC)VertexAttribI2i,
    (PFNGLVERTEXATTRIBI3IPROC)VertexAttribI3i,
    (PFNGLVERTEXATTRIBI4IPROC)VertexAttribI4i,
    (PFNGLVERTEXATTRIBI1UIPROC)VertexAttribI1ui,
    (PFNGLVERTEXATTRIBI2UIPROC)VertexAttribI2ui,
    (PFNGLVERTEXATTRIBI3UIPROC)VertexAttribI3ui,
    (PFNGLVERTEXATTRIBI4UIPROC)VertexAttribI4ui,
    (PFNGLVERTEXATTRIBI1IVPROC)VertexAttribI1iv,
    (PFNGLVERTEXATTRIBI2IVPROC)VertexAttribI2iv,
    (PFNGLVERTEXATTRIBI3IVPROC)VertexAttribI3iv,
    (PFNGLVERTEXATTRIBI4IVPROC)VertexAttribI4iv,
    (PFNGLVERTEXATTRIBI1UIVPROC)VertexAttribI1uiv,
    (PFNGLVERTEXATTRIBI2UIVPROC)VertexAttribI2uiv,
    (PFNGLVERTEXATTRIBI3UIVPROC)VertexAttribI3uiv,
    (PFNGLVERTEXATTRIBI4UIVPROC)VertexAttribI4uiv,
    (PFNGLVERTEXATTRIBI4BVPROC)VertexAttribI4bv,
    (PFNGLVERTEXATTRIBI4SVPROC)VertexAttribI4sv,
    (PFNGLVERTEXATTRIBI4UBVPROC)VertexAttribI4ubv,
    (PFNGLVERTEXATTRIBI4USVPROC)VertexAttribI4usv,
    (PFNGLGETUNIFORMUIVPROC)GetUniformuiv,
    (PFNGLBINDFRAGDATALOCATIONPROC)BindFragDataLocation,
    (PFNGLGETFRAGDATALOCATIONPROC)GetFragDataLocation,
    (PFNGLUNIFORM1UIPROC)Uniform1ui,
    (PFNGLUNIFORM2UIPROC)Uniform2ui,
    (PFNGLUNIFORM3UIPROC)Uniform3ui,
    (PFNGLUNIFORM4UIPROC)Uniform4ui,
    (PFNGLUNIFORM1UIVPROC)Uniform1uiv,
    (PFNGLUNIFORM2UIVPROC)Uniform2uiv,
    (PFNGLUNIFORM3UIVPROC)Uniform3uiv,
    (PFNGLUNIFORM4UIVPROC)Uniform4uiv,
    (PFNGLTEXPARAMETERIIVPROC)TexParameterIiv,
    (PFNGLTEXPARAMETERIUIVPROC)TexParameterIuiv,
    (PFNGLGETTEXPARAMETERIIVPROC)GetTexParameterIiv,
    (PFNGLGETTEXPARAMETERIUIVPROC)GetTexParameterIuiv,
    (PFNGLCLEARBUFFERIVPROC)ClearBufferiv,
    (PFNGLCLEARBUFFERUIVPROC)ClearBufferuiv,
    (PFNGLCLEARBUFFERFVPROC)ClearBufferfv,
    (PFNGLCLEARBUFFERFIPROC)ClearBufferfi,
    (PFNGLGETSTRINGIPROC)GetStringi,
    (PFNGLISRENDERBUFFERPROC)IsRenderbuffer,
    (PFNGLBINDRENDERBUFFERPROC)BindRenderbuffer,
    (PFNGLDELETERENDERBUFFERSPROC)DeleteRenderbuffers,
    (PFNGLGENRENDERBUFFERSPROC)GenRenderbuffers,
    (PFNGLRENDERBUFFERSTORAGEPROC)RenderbufferStorage,
    (PFNGLGETRENDERBUFFERPARAMETERIVPROC)GetRenderbufferParameteriv,
    (PFNGLISFRAMEBUFFERPROC)IsFramebuffer,
    (PFNGLBINDFRAMEBUFFERPROC)BindFramebuffer,
    (PFNGLDELETEFRAMEBUFFERSPROC)DeleteFramebuffers,
    (PFNGLGENFRAMEBUFFERSPROC)GenFramebuffers,
    (PFNGLCHECKFRAMEBUFFERSTATUSPROC)CheckFramebufferStatus,
    (PFNGLFRAMEBUFFERTEXTURE1DPROC)FramebufferTexture1D,
    (PFNGLFRAMEBUFFERTEXTURE2DPROC)FramebufferTexture2D,
    (PFNGLFRAMEBUFFERTEXTURE3DPROC)FramebufferTexture3D,
    (PFNGLFRAMEBUFFERRENDERBUFFERPROC)FramebufferRenderbuffer,
    (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)GetFramebufferAttachmentParameteriv,
    (PFNGLGENERATEMIPMAPPROC)GenerateMipmap,
    (PFNGLBLITFRAMEBUFFERPROC)BlitFramebuffer,
    (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)RenderbufferStorageMultisample,
    (PFNGLFRAMEBUFFERTEXTURELAYERPROC)FramebufferTextureLayer,
    (PFNGLMAPBUFFERRANGEPROC)MapBufferRange,
    (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)FlushMappedBufferRange,
    (PFNGLBINDVERTEXARRAYPROC)BindVertexArray,
    (PFNGLDELETEVERTEXARRAYSPROC)DeleteVertexArrays,
    (PFNGLGENVERTEXARRAYSPROC)GenVertexArrays,
    (PFNGLISVERTEXARRAYPROC)IsVertexArray,
    (PFNGLDRAWARRAYSINSTANCEDPROC)DrawArraysInstanced,
    (PFNGLDRAWELEMENTSINSTANCEDPROC)DrawElementsInstanced,
    (PFNGLTEXBUFFERPROC)TexBuffer,
    (PFNGLPRIMITIVERESTARTINDEXPROC)PrimitiveRestartIndex,
    (PFNGLCOPYBUFFERSUBDATAPROC)CopyBufferSubData,
    (PFNGLGETUNIFORMINDICESPROC)GetUniformIndices,
    (PFNGLGETACTIVEUNIFORMSIVPROC)GetActiveUniformsiv,
    (PFNGLGETACTIVEUNIFORMNAMEPROC)GetActiveUniformName,
    (PFNGLGETUNIFORMBLOCKINDEXPROC)GetUniformBlockIndex,
    (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)GetActiveUniformBlockiv,
    (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)GetActiveUniformBlockName,
    (PFNGLUNIFORMBLOCKBINDINGPROC)UniformBlockBinding,
    (PFNGLDRAWELEMENTSBASEVERTEXPROC)DrawElementsBaseVertex,
    (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)DrawRangeElementsBaseVertex,
    (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)DrawElementsInstancedBaseVertex,
    (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)MultiDrawElementsBaseVertex,
    (PFNGLPROVOKINGVERTEXPROC)ProvokingVertex,
    (PFNGLFENCESYNCPROC)FenceSync,
    (PFNGLISSYNCPROC)IsSync,
    (PFNGLDELETESYNCPROC)DeleteSync,
    (PFNGLCLIENTWAITSYNCPROC)ClientWaitSync,
    (PFNGLWAITSYNCPROC)WaitSync,
    (PFNGLGETINTEGER64VPROC)GetInteger64v,
    (PFNGLGETSYNCIVPROC)GetSynciv,
    (PFNGLGETINTEGER64I_VPROC)GetInteger64i_v,
    (PFNGLGETBUFFERPARAMETERI64VPROC)GetBufferParameteri64v,
    (PFNGLFRAMEBUFFERTEXTUREPROC)FramebufferTexture,
    (PFNGLTEXIMAGE2DMULTISAMPLEPROC)TexImage2DMultisample,
    (PFNGLTEXIMAGE3DMULTISAMPLEPROC)TexImage3DMultisample,
    (PFNGLGETMULTISAMPLEFVPROC)GetMultisamplefv,
    (PFNGLSAMPLEMASKIPROC)SampleMaski,
    (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)BindFragDataLocationIndexed,
    (PFNGLGETFRAGDATAINDEXPROC)GetFragDataIndex,
    (PFNGLGENSAMPLERSPROC)GenSamplers,
    (PFNGLDELETESAMPLERSPROC)DeleteSamplers,
    (PFNGLISSAMPLERPROC)IsSampler,
    (PFNGLBINDSAMPLERPROC)BindSampler,
    (PFNGLSAMPLERPARAMETERIPROC)SamplerParameteri,
    (PFNGLSAMPLERPARAMETERIVPROC)SamplerParameteriv,
    (PFNGLSAMPLERPARAMETERFPROC)SamplerParameterf,
    (PFNGLSAMPLERPARAMETERFVPROC)SamplerParameterfv,
    (PFNGLSAMPLERPARAMETERIIVPROC)SamplerParameterIiv,
    (PFNGLSAMPLERPARAMETERIUIVPROC)SamplerParameterIuiv,
    (PFNGLGETSAMPLERPARAMETERIVPROC)GetSamplerParameteriv,
    (PFNGLGETSAMPLERPARAMETERIIVPROC)GetSamplerParameterIiv,
    (PFNGLGETSAMPLERPARAMETERFVPROC)GetSamplerParameterfv,
    (PFNGLGETSAMPLERPARAMETERIUIVPROC)GetSamplerParameterIuiv,
    (PFNGLQUERYCOUNTERPROC)QueryCounter,
    (PFNGLGETQUERYOBJECTI64VPROC)GetQueryObjecti64v,
    (PFNGLGETQUERYOBJECTUI64VPROC)GetQueryObjectui64v,
    (PFNGLVERTEXATTRIBDIVISORPROC)VertexAttribDivisor,
    (PFNGLVERTEXATTRIBP1UIPROC)VertexAttribP1ui,
    (PFNGLVERTEXATTRIBP1UIVPROC)VertexAttribP1uiv,
    (PFNGLVERTEXATTRIBP2UIPROC)VertexAttribP2ui,
    (PFNGLVERTEXATTRIBP2UIVPROC)VertexAttribP2uiv,
    (PFNGLVERTEXATTRIBP3UIPROC)VertexAttribP3ui,
    (PFNGLVERTEXATTRIBP3UIVPROC)VertexAttribP3uiv,
    (PFNGLVERTEXATTRIBP4UIPROC)VertexAttribP4ui,
    (PFNGLVERTEXATTRIBP4UIVPROC)VertexAttribP4uiv,
    (PFNGLMINSAMPLESHADINGPROC)MinSampleShading,
    (PFNGLBLENDEQUATIONIPROC)BlendEquationi,
    (PFNGLBLENDEQUATIONSEPARATEIPROC)BlendEquationSeparatei,
    (PFNGLBLENDFUNCIPROC)BlendFunci,
    (PFNGLBLENDFUNCSEPARATEIPROC)BlendFuncSeparatei,
    (PFNGLDRAWARRAYSINDIRECTPROC)DrawArraysIndirect,
    (PFNGLDRAWELEMENTSINDIRECTPROC)DrawElementsIndirect,
    (PFNGLUNIFORM1DPROC)Uniform1d,
    (PFNGLUNIFORM2DPROC)Uniform2d,
    (PFNGLUNIFORM3DPROC)Uniform3d,
    (PFNGLUNIFORM4DPROC)Uniform4d,
    (PFNGLUNIFORM1DVPROC)Uniform1dv,
    (PFNGLUNIFORM2DVPROC)Uniform2dv,
    (PFNGLUNIFORM3DVPROC)Uniform3dv,
    (PFNGLUNIFORM4DVPROC)Uniform4dv,
    (PFNGLUNIFORMMATRIX2DVPROC)UniformMatrix2dv,
    (PFNGLUNIFORMMATRIX3DVPROC)UniformMatrix3dv,
    (PFNGLUNIFORMMATRIX4DVPROC)UniformMatrix4dv,
    (PFNGLUNIFORMMATRIX2X3DVPROC)UniformMatrix2x3dv,
    (PFNGLUNIFORMMATRIX2X4DVPROC)UniformMatrix2x4dv,
    (PFNGLUNIFORMMATRIX3X2DVPROC)UniformMatrix3x2dv,
    (PFNGLUNIFORMMATRIX3X4DVPROC)UniformMatrix3x4dv,
    (PFNGLUNIFORMMATRIX4X2DVPROC)UniformMatrix4x2dv,
    (PFNGLUNIFORMMATRIX4X3DVPROC)UniformMatrix4x3dv,
    (PFNGLGETUNIFORMDVPROC)GetUniformdv,
    (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC)GetSubroutineUniformLocation,
    (PFNGLGETSUBROUTINEINDEXPROC)GetSubroutineIndex,
    (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC)GetActiveSubroutineUniformiv,
    (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC)GetActiveSubroutineUniformName,
    (PFNGLGETACTIVESUBROUTINENAMEPROC)GetActiveSubroutineName,
    (PFNGLUNIFORMSUBROUTINESUIVPROC)UniformSubroutinesuiv,
    (PFNGLGETUNIFORMSUBROUTINEUIVPROC)GetUniformSubroutineuiv,
    (PFNGLGETPROGRAMSTAGEIVPROC)GetProgramStageiv,
    (PFNGLPATCHPARAMETERIPROC)PatchParameteri,
    (PFNGLPATCHPARAMETERFVPROC)PatchParameterfv,
    (PFNGLBINDTRANSFORMFEEDBACKPROC)BindTransformFeedback,
    (PFNGLDELETETRANSFORMFEEDBACKSPROC)DeleteTransformFeedbacks,
    (PFNGLGENTRANSFORMFEEDBACKSPROC)GenTransformFeedbacks,
    (PFNGLISTRANSFORMFEEDBACKPROC)IsTransformFeedback,
    (PFNGLPAUSETRANSFORMFEEDBACKPROC)PauseTransformFeedback,
    (PFNGLRESUMETRANSFORMFEEDBACKPROC)ResumeTransformFeedback,
    (PFNGLDRAWTRANSFORMFEEDBACKPROC)DrawTransformFeedback,
    (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC)DrawTransformFeedbackStream,
    (PFNGLBEGINQUERYINDEXEDPROC)BeginQueryIndexed,
    (PFNGLENDQUERYINDEXEDPROC)EndQueryIndexed,
    (PFNGLGETQUERYINDEXEDIVPROC)GetQueryIndexediv,
    (PFNGLRELEASESHADERCOMPILERPROC)ReleaseShaderCompiler,
    (PFNGLSHADERBINARYPROC)ShaderBinary,
    (PFNGLGETSHADERPRECISIONFORMATPROC)GetShaderPrecisionFormat,
    (PFNGLDEPTHRANGEFPROC)DepthRangef,
    (PFNGLCLEARDEPTHFPROC)ClearDepthf,
    (PFNGLGETPROGRAMBINARYPROC)GetProgramBinary,
    (PFNGLPROGRAMBINARYPROC)ProgramBinary,
    (PFNGLPROGRAMPARAMETERIPROC)ProgramParameteri,
    (PFNGLUSEPROGRAMSTAGESPROC)UseProgramStages,
    (PFNGLACTIVESHADERPROGRAMPROC)ActiveShaderProgram,
    (PFNGLCREATESHADERPROGRAMVPROC)CreateShaderProgramv,
    (PFNGLBINDPROGRAMPIPELINEPROC)BindProgramPipeline,
    (PFNGLDELETEPROGRAMPIPELINESPROC)DeleteProgramPipelines,
    (PFNGLGENPROGRAMPIPELINESPROC)GenProgramPipelines,
    (PFNGLISPROGRAMPIPELINEPROC)IsProgramPipeline,
    (PFNGLGETPROGRAMPIPELINEIVPROC)GetProgramPipelineiv,
    (PFNGLPROGRAMUNIFORM1IPROC)ProgramUniform1i,
    (PFNGLPROGRAMUNIFORM1IVPROC)ProgramUniform1iv,
    (PFNGLPROGRAMUNIFORM1FPROC)ProgramUniform1f,
    (PFNGLPROGRAMUNIFORM1FVPROC)ProgramUniform1fv,
    (PFNGLPROGRAMUNIFORM1DPROC)ProgramUniform1d,
    (PFNGLPROGRAMUNIFORM1DVPROC)ProgramUniform1dv,
    (PFNGLPROGRAMUNIFORM1UIPROC)ProgramUniform1ui,
    (PFNGLPROGRAMUNIFORM1UIVPROC)ProgramUniform1uiv,
    (PFNGLPROGRAMUNIFORM2IPROC)ProgramUniform2i,
    (PFNGLPROGRAMUNIFORM2IVPROC)ProgramUniform2iv,
    (PFNGLPROGRAMUNIFORM2FPROC)ProgramUniform2f,
    (PFNGLPROGRAMUNIFORM2FVPROC)ProgramUniform2fv,
    (PFNGLPROGRAMUNIFORM2DPROC)ProgramUniform2d,
    (PFNGLPROGRAMUNIFORM2DVPROC)ProgramUniform2dv,
    (PFNGLPROGRAMUNIFORM2UIPROC)ProgramUniform2ui,
    (PFNGLPROGRAMUNIFORM2UIVPROC)ProgramUniform2uiv,
    (PFNGLPROGRAMUNIFORM3IPROC)ProgramUniform3i,
    (PFNGLPROGRAMUNIFORM3IVPROC)ProgramUniform3iv,
    (PFNGLPROGRAMUNIFORM3FPROC)ProgramUniform3f,
    (PFNGLPROGRAMUNIFORM3FVPROC)ProgramUniform3fv,
    (PFNGLPROGRAMUNIFORM3DPROC)ProgramUniform3d,
    (PFNGLPROGRAMUNIFORM3DVPROC)ProgramUniform3dv,
    (PFNGLPROGRAMUNIFORM3UIPROC)ProgramUniform3ui,
    (PFNGLPROGRAMUNIFORM3UIVPROC)ProgramUniform3uiv,
    (PFNGLPROGRAMUNIFORM4IPROC)ProgramUniform4i,
    (PFNGLPROGRAMUNIFORM4IVPROC)ProgramUniform4iv,
    (PFNGLPROGRAMUNIFORM4FPROC)ProgramUniform4f,
    (PFNGLPROGRAMUNIFORM4FVPROC)ProgramUniform4fv,
    (PFNGLPROGRAMUNIFORM4DPROC)ProgramUniform4d,
    (PFNGLPROGRAMUNIFORM4DVPROC)ProgramUniform4dv,
    (PFNGLPROGRAMUNIFORM4UIPROC)ProgramUniform4ui,
    (PFNGLPROGRAMUNIFORM4UIVPROC)ProgramUniform4uiv,
    (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)ProgramUniformMatrix2fv,
    (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)ProgramUniformMatrix3fv,
    (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)ProgramUniformMatrix4fv,
    (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)ProgramUniformMatrix2dv,
    (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)ProgramUniformMatrix3dv,
    (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)ProgramUniformMatrix4dv,
    (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC)ProgramUniformMatrix2x3fv,
    (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC)ProgramUniformMatrix3x2fv,
    (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC)ProgramUniformMatrix2x4fv,
    (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC)ProgramUniformMatrix4x2fv,
    (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC)ProgramUniformMatrix3x4fv,
    (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC)ProgramUniformMatrix4x3fv,
    (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC)ProgramUniformMatrix2x3dv,
    (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC)ProgramUniformMatrix3x2dv,
    (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC)ProgramUniformMatrix2x4dv,
    (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC)ProgramUniformMatrix4x2dv,
    (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC)ProgramUniformMatrix3x4dv,
    (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC)ProgramUniformMatrix4x3dv,
    (PFNGLVALIDATEPROGRAMPIPELINEPROC)ValidateProgramPipeline,
    (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)GetProgramPipelineInfoLog,
    (PFNGLVERTEXATTRIBL1DPROC)VertexAttribL1d,
    (PFNGLVERTEXATTRIBL2DPROC)VertexAttribL2d,
    (PFNGLVERTEXATTRIBL3DPROC)VertexAttribL3d,
    (PFNGLVERTEXATTRIBL4DPROC)VertexAttribL4d,
    (PFNGLVERTEXATTRIBL1DVPROC)VertexAttribL1dv,
    (PFNGLVERTEXATTRIBL2DVPROC)VertexAttribL2dv,
    (PFNGLVERTEXATTRIBL3DVPROC)VertexAttribL3dv,
    (PFNGLVERTEXATTRIBL4DVPROC)VertexAttribL4dv,
    (PFNGLVERTEXATTRIBLPOINTERPROC)VertexAttribLPointer,
    (PFNGLGETVERTEXATTRIBLDVPROC)GetVertexAttribLdv,
    (PFNGLVIEWPORTARRAYVPROC)ViewportArrayv,
    (PFNGLVIEWPORTINDEXEDFPROC)ViewportIndexedf,
    (PFNGLVIEWPORTINDEXEDFVPROC)ViewportIndexedfv,
    (PFNGLSCISSORARRAYVPROC)ScissorArrayv,
    (PFNGLSCISSORINDEXEDPROC)ScissorIndexed,
    (PFNGLSCISSORINDEXEDVPROC)ScissorIndexedv,
    (PFNGLDEPTHRANGEARRAYVPROC)DepthRangeArrayv,
    (PFNGLDEPTHRANGEINDEXEDPROC)DepthRangeIndexed,
    (PFNGLGETFLOATI_VPROC)GetFloati_v,
    (PFNGLGETDOUBLEI_VPROC)GetDoublei_v,
    (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC)DrawArraysInstancedBaseInstance,
    (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC)DrawElementsInstancedBaseInstance,
    (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC)DrawElementsInstancedBaseVertexBaseInstance,
    (PFNGLGETINTERNALFORMATIVPROC)GetInternalformativ,
    (PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC)GetActiveAtomicCounterBufferiv,
    (PFNGLBINDIMAGETEXTUREPROC)BindImageTexture,
    (PFNGLMEMORYBARRIERPROC)MemoryBarrier,
    (PFNGLTEXSTORAGE1DPROC)TexStorage1D,
    (PFNGLTEXSTORAGE2DPROC)TexStorage2D,
    (PFNGLTEXSTORAGE3DPROC)TexStorage3D,
    (PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC)DrawTransformFeedbackInstanced,
    (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC)DrawTransformFeedbackStreamInstanced,
    (PFNGLCLEARBUFFERDATAPROC)ClearBufferData,
    (PFNGLCLEARBUFFERSUBDATAPROC)ClearBufferSubData,
    (PFNGLDISPATCHCOMPUTEPROC)DispatchCompute,
    (PFNGLDISPATCHCOMPUTEINDIRECTPROC)DispatchComputeIndirect,
    (PFNGLCOPYIMAGESUBDATAPROC)CopyImageSubData,
    (PFNGLFRAMEBUFFERPARAMETERIPROC)FramebufferParameteri,
    (PFNGLGETFRAMEBUFFERPARAMETERIVPROC)GetFramebufferParameteriv,
    (PFNGLGETINTERNALFORMATI64VPROC)GetInternalformati64v,
    (PFNGLINVALIDATETEXSUBIMAGEPROC)InvalidateTexSubImage,
    (PFNGLINVALIDATETEXIMAGEPROC)InvalidateTexImage,
    (PFNGLINVALIDATEBUFFERSUBDATAPROC)InvalidateBufferSubData,
    (PFNGLINVALIDATEBUFFERDATAPROC)InvalidateBufferData,
    (PFNGLINVALIDATEFRAMEBUFFERPROC)InvalidateFramebuffer,
    (PFNGLINVALIDATESUBFRAMEBUFFERPROC)InvalidateSubFramebuffer,
    (PFNGLMULTIDRAWARRAYSINDIRECTPROC)MultiDrawArraysIndirect,
    (PFNGLMULTIDRAWELEMENTSINDIRECTPROC)MultiDrawElementsIndirect,
    (PFNGLGETPROGRAMINTERFACEIVPROC)GetProgramInterfaceiv,
    (PFNGLGETPROGRAMRESOURCEINDEXPROC)GetProgramResourceIndex,
    (PFNGLGETPROGRAMRESOURCENAMEPROC)GetProgramResourceName,
    (PFNGLGETPROGRAMRESOURCEIVPROC)GetProgramResourceiv,
    (PFNGLGETPROGRAMRESOURCELOCATIONPROC)GetProgramResourceLocation,
    (PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC)GetProgramResourceLocationIndex,
    (PFNGLSHADERSTORAGEBLOCKBINDINGPROC)ShaderStorageBlockBinding,
    (PFNGLTEXBUFFERRANGEPROC)TexBufferRange,
    (PFNGLTEXSTORAGE2DMULTISAMPLEPROC)TexStorage2DMultisample,
    (PFNGLTEXSTORAGE3DMULTISAMPLEPROC)TexStorage3DMultisample,
    (PFNGLTEXTUREVIEWPROC)TextureView,
    (PFNGLBINDVERTEXBUFFERPROC)BindVertexBuffer,
    (PFNGLVERTEXATTRIBFORMATPROC)VertexAttribFormat,
    (PFNGLVERTEXATTRIBIFORMATPROC)VertexAttribIFormat,
    (PFNGLVERTEXATTRIBLFORMATPROC)VertexAttribLFormat,
    (PFNGLVERTEXATTRIBBINDINGPROC)VertexAttribBinding,
    (PFNGLVERTEXBINDINGDIVISORPROC)VertexBindingDivisor,
    (PFNGLDEBUGMESSAGECONTROLPROC)DebugMessageControl,
    (PFNGLDEBUGMESSAGEINSERTPROC)DebugMessageInsert,
    (PFNGLDEBUGMESSAGECALLBACKPROC)DebugMessageCallback,
    (PFNGLGETDEBUGMESSAGELOGPROC)GetDebugMessageLog,
    (PFNGLPUSHDEBUGGROUPPROC)PushDebugGroup,
    (PFNGLPOPDEBUGGROUPPROC)PopDebugGroup,
    (PFNGLOBJECTLABELPROC)ObjectLabel,
    (PFNGLGETOBJECTLABELPROC)GetObjectLabel,
    (PFNGLOBJECTPTRLABELPROC)ObjectPtrLabel,
    (PFNGLGETOBJECTPTRLABELPROC)GetObjectPtrLabel,
    (PFNGLBUFFERSTORAGEPROC)BufferStorage,
    (PFNGLCLEARTEXIMAGEPROC)ClearTexImage,
    (PFNGLCLEARTEXSUBIMAGEPROC)ClearTexSubImage,
    (PFNGLBINDBUFFERSBASEPROC)BindBuffersBase,
    (PFNGLBINDBUFFERSRANGEPROC)BindBuffersRange,
    (PFNGLBINDTEXTURESPROC)BindTextures,
    (PFNGLBINDSAMPLERSPROC)BindSamplers,
    (PFNGLBINDIMAGETEXTURESPROC)BindImageTextures,
    (PFNGLBINDVERTEXBUFFERSPROC)BindVertexBuffers,
    (PFNGLCLIPCONTROLPROC)ClipControl,
    (PFNGLCREATETRANSFORMFEEDBACKSPROC)CreateTransformFeedbacks,
    (PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC)TransformFeedbackBufferBase,
    (PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC)TransformFeedbackBufferRange,
    (PFNGLGETTRANSFORMFEEDBACKIVPROC)GetTransformFeedbackiv,
    (PFNGLGETTRANSFORMFEEDBACKI_VPROC)GetTransformFeedbacki_v,
    (PFNGLGETTRANSFORMFEEDBACKI64_VPROC)GetTransformFeedbacki64_v,
    (PFNGLCREATEBUFFERSPROC)CreateBuffers,
    (PFNGLNAMEDBUFFERSTORAGEPROC)NamedBufferStorage,
    (PFNGLNAMEDBUFFERDATAPROC)NamedBufferData,
    (PFNGLNAMEDBUFFERSUBDATAPROC)NamedBufferSubData,
    (PFNGLCOPYNAMEDBUFFERSUBDATAPROC)CopyNamedBufferSubData,
    (PFNGLCLEARNAMEDBUFFERDATAPROC)ClearNamedBufferData,
    (PFNGLCLEARNAMEDBUFFERSUBDATAPROC)ClearNamedBufferSubData,
    (PFNGLMAPNAMEDBUFFERPROC)MapNamedBuffer,
    (PFNGLMAPNAMEDBUFFERRANGEPROC)MapNamedBufferRange,
    (PFNGLUNMAPNAMEDBUFFERPROC)UnmapNamedBuffer,
    (PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC)FlushMappedNamedBufferRange,
    (PFNGLGETNAMEDBUFFERPARAMETERIVPROC)GetNamedBufferParameteriv,
    (PFNGLGETNAMEDBUFFERPARAMETERI64VPROC)GetNamedBufferParameteri64v,
    (PFNGLGETNAMEDBUFFERPOINTERVPROC)GetNamedBufferPointerv,
    (PFNGLGETNAMEDBUFFERSUBDATAPROC)GetNamedBufferSubData,
    (PFNGLCREATEFRAMEBUFFERSPROC)CreateFramebuffers,
    (PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC)NamedFramebufferRenderbuffer,
    (PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC)NamedFramebufferParameteri,
    (PFNGLNAMEDFRAMEBUFFERTEXTUREPROC)NamedFramebufferTexture,
    (PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC)NamedFramebufferTextureLayer,
    (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC)NamedFramebufferDrawBuffer,
    (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC)NamedFramebufferDrawBuffers,
    (PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC)NamedFramebufferReadBuffer,
    (PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC)InvalidateNamedFramebufferData,
    (PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC)InvalidateNamedFramebufferSubData,
    (PFNGLCLEARNAMEDFRAMEBUFFERIVPROC)ClearNamedFramebufferiv,
    (PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC)ClearNamedFramebufferuiv,
    (PFNGLCLEARNAMEDFRAMEBUFFERFVPROC)ClearNamedFramebufferfv,
    (PFNGLCLEARNAMEDFRAMEBUFFERFIPROC)ClearNamedFramebufferfi,
    (PFNGLBLITNAMEDFRAMEBUFFERPROC)BlitNamedFramebuffer,
    (PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC)CheckNamedFramebufferStatus,
    (PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC)GetNamedFramebufferParameteriv,
    (PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC)GetNamedFramebufferAttachmentParameteriv,
    (PFNGLCREATERENDERBUFFERSPROC)CreateRenderbuffers,
    (PFNGLNAMEDRENDERBUFFERSTORAGEPROC)NamedRenderbufferStorage,
    (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC)NamedRenderbufferStorageMultisample,
    (PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC)GetNamedRenderbufferParameteriv,
    (PFNGLCREATETEXTURESPROC)CreateTextures,
    (PFNGLTEXTUREBUFFERPROC)TextureBuffer,
    (PFNGLTEXTUREBUFFERRANGEPROC)TextureBufferRange,
    (PFNGLTEXTURESTORAGE1DPROC)TextureStorage1D,
    (PFNGLTEXTURESTORAGE2DPROC)TextureStorage2D,
    (PFNGLTEXTURESTORAGE3DPROC)TextureStorage3D,
    (PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC)TextureStorage2DMultisample,
    (PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC)TextureStorage3DMultisample,
    (PFNGLTEXTURESUBIMAGE1DPROC)TextureSubImage1D,
    (PFNGLTEXTURESUBIMAGE2DPROC)TextureSubImage2D,
    (PFNGLTEXTURESUBIMAGE3DPROC)TextureSubImage3D,
    (PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC)CompressedTextureSubImage1D,
    (PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC)CompressedTextureSubImage2D,
    (PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC)CompressedTextureSubImage3D,
    (PFNGLCOPYTEXTURESUBIMAGE1DPROC)CopyTextureSubImage1D,
    (PFNGLCOPYTEXTURESUBIMAGE2DPROC)CopyTextureSubImage2D,
    (PFNGLCOPYTEXTURESUBIMAGE3DPROC)CopyTextureSubImage3D,
    (PFNGLTEXTUREPARAMETERFPROC)TextureParameterf,
    (PFNGLTEXTUREPARAMETERFVPROC)TextureParameterfv,
    (PFNGLTEXTUREPARAMETERIPROC)TextureParameteri,
    (PFNGLTEXTUREPARAMETERIIVPROC)TextureParameterIiv,
    (PFNGLTEXTUREPARAMETERIUIVPROC)TextureParameterIuiv,
    (PFNGLTEXTUREPARAMETERIVPROC)TextureParameteriv,
    (PFNGLGENERATETEXTUREMIPMAPPROC)GenerateTextureMipmap,
    (PFNGLBINDTEXTUREUNITPROC)BindTextureUnit,
    (PFNGLGETTEXTUREIMAGEPROC)GetTextureImage,
    (PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC)GetCompressedTextureImage,
    (PFNGLGETTEXTURELEVELPARAMETERFVPROC)GetTextureLevelParameterfv,
    (PFNGLGETTEXTURELEVELPARAMETERIVPROC)GetTextureLevelParameteriv,
    (PFNGLGETTEXTUREPARAMETERFVPROC)GetTextureParameterfv,
    (PFNGLGETTEXTUREPARAMETERIIVPROC)GetTextureParameterIiv,
    (PFNGLGETTEXTUREPARAMETERIUIVPROC)GetTextureParameterIuiv,
    (PFNGLGETTEXTUREPARAMETERIVPROC)GetTextureParameteriv,
    (PFNGLCREATEVERTEXARRAYSPROC)CreateVertexArrays,
    (PFNGLDISABLEVERTEXARRAYATTRIBPROC)DisableVertexArrayAttrib,
    (PFNGLENABLEVERTEXARRAYATTRIBPROC)EnableVertexArrayAttrib,
    (PFNGLVERTEXARRAYELEMENTBUFFERPROC)VertexArrayElementBuffer,
    (PFNGLVERTEXARRAYVERTEXBUFFERPROC)VertexArrayVertexBuffer,
    (PFNGLVERTEXARRAYVERTEXBUFFERSPROC)VertexArrayVertexBuffers,
    (PFNGLVERTEXARRAYATTRIBBINDINGPROC)VertexArrayAttribBinding,
    (PFNGLVERTEXARRAYATTRIBFORMATPROC)VertexArrayAttribFormat,
    (PFNGLVERTEXARRAYATTRIBIFORMATPROC)VertexArrayAttribIFormat,
    (PFNGLVERTEXARRAYATTRIBLFORMATPROC)VertexArrayAttribLFormat,
    (PFNGLVERTEXARRAYBINDINGDIVISORPROC)VertexArrayBindingDivisor,
    (PFNGLGETVERTEXARRAYIVPROC)GetVertexArrayiv,
    (PFNGLGETVERTEXARRAYINDEXEDIVPROC)GetVertexArrayIndexediv,
    (PFNGLGETVERTEXARRAYINDEXED64IVPROC)GetVertexArrayIndexed64iv,
    (PFNGLCREATESAMPLERSPROC)CreateSamplers,
    (PFNGLCREATEPROGRAMPIPELINESPROC)CreateProgramPipelines,
    (PFNGLCREATEQUERIESPROC)CreateQueries,
    (PFNGLGETQUERYBUFFEROBJECTI64VPROC)GetQueryBufferObjecti64v,
    (PFNGLGETQUERYBUFFEROBJECTIVPROC)GetQueryBufferObjectiv,
    (PFNGLGETQUERYBUFFEROBJECTUI64VPROC)GetQueryBufferObjectui64v,
    (PFNGLGETQUERYBUFFEROBJECTUIVPROC)GetQueryBufferObjectuiv,
    (PFNGLMEMORYBARRIERBYREGIONPROC)MemoryBarrierByRegion,
    (PFNGLGETTEXTURESUBIMAGEPROC)GetTextureSubImage,
    (PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC)GetCompressedTextureSubImage,
    (PFNGLGETGRAPHICSRESETSTATUSPROC)GetGraphicsResetStatus,
    (PFNGLGETNCOMPRESSEDTEXIMAGEPROC)GetnCompressedTexImage,
    (PFNGLGETNTEXIMAGEPROC)GetnTexImage,
    (PFNGLGETNUNIFORMDVPROC)GetnUniformdv,
    (PFNGLGETNUNIFORMFVPROC)GetnUniformfv,
    (PFNGLGETNUNIFORMIVPROC)GetnUniformiv,
    (PFNGLGETNUNIFORMUIVPROC)GetnUniformuiv,
    (PFNGLREADNPIXELSPROC)ReadnPixels,
    (PFNGLTEXTUREBARRIERPROC)TextureBarrier,
    (PFNGLSPECIALIZESHADERPROC)SpecializeShader,
    (PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC)MultiDrawArraysIndirectCount,
    (PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC)MultiDrawElementsIndirectCount,
    (PFNGLPOLYGONOFFSETCLAMPPROC)PolygonOffsetClamp,
};

const char * names[] = {
    "glCullFace",
    "glFrontFace",
    "glHint",
    "glLineWidth",
    "glPointSize",
    "glPolygonMode",
    "glScissor",
    "glTexParameterf",
    "glTexParameterfv",
    "glTexParameteri",
    "glTexParameteriv",
    "glTexImage1D",
    "glTexImage2D",
    "glDrawBuffer",
    "glClear",
    "glClearColor",
    "glClearStencil",
    "glClearDepth",
    "glStencilMask",
    "glColorMask",
    "glDepthMask",
    "glDisable",
    "glEnable",
    "glFinish",
    "glFlush",
    "glBlendFunc",
    "glLogicOp",
    "glStencilFunc",
    "glStencilOp",
    "glDepthFunc",
    "glPixelStoref",
    "glPixelStorei",
    "glReadBuffer",
    "glReadPixels",
    "glGetBooleanv",
    "glGetDoublev",
    "glGetError",
    "glGetFloatv",
    "glGetIntegerv",
    "glGetString",
    "glGetTexImage",
    "glGetTexParameterfv",
    "glGetTexParameteriv",
    "glGetTexLevelParameterfv",
    "glGetTexLevelParameteriv",
    "glIsEnabled",
    "glDepthRange",
    "glViewport",
    "glDrawArrays",
    "glDrawElements",
    "glGetPointerv",
    "glPolygonOffset",
    "glCopyTexImage1D",
    "glCopyTexImage2D",
    "glCopyTexSubImage1D",
    "glCopyTexSubImage2D",
    "glTexSubImage1D",
    "glTexSubImage2D",
    "glBindTexture",
    "glDeleteTextures",
    "glGenTextures",
    "glIsTexture",
    "glDrawRangeElements",
    "glTexImage3D",
    "glTexSubImage3D",
    "glCopyTexSubImage3D",
    "glActiveTexture",
    "glSampleCoverage",
    "glCompressedTexImage3D",
    "glCompressedTexImage2D",
    "glCompressedTexImage1D",
    "glCompressedTexSubImage3D",
    "glCompressedTexSubImage2D",
    "glCompressedTexSubImage1D",
    "glGetCompressedTexImage",
    "glBlendFuncSeparate",
    "glMultiDrawArrays",
    "glMultiDrawElements",
    "glPointParameterf",
    "glPointParameterfv",
    "glPointParameteri",
    "glPointParameteriv",
    "glBlendColor",
    "glBlendEquation",
    "glGenQueries",
    "glDeleteQueries",
    "glIsQuery",
    "glBeginQuery",
    "glEndQuery",
    "glGetQueryiv",
    "glGetQueryObjectiv",
    "glGetQueryObjectuiv",
    "glBindBuffer",
    "glDeleteBuffers",
    "glGenBuffers",
    "glIsBuffer",
    "glBufferData",
    "glBufferSubData",
    "glGetBufferSubData",
    "glMapBuffer",
    "glUnmapBuffer",
    "glGetBufferParameteriv",
    "glGetBufferPointerv",
    "glBlendEquationSeparate",
    "glDrawBuffers",
    "glStencilOpSeparate",
    "glStencilFuncSeparate",
    "glStencilMaskSeparate",
    "glAttachShader",
    "glBindAttribLocation",
    "glCompileShader",
    "glCreateProgram",
    "glCreateShader",
    "glDeleteProgram",
    "glDeleteShader",
    "glDetachShader",
    "glDisableVertexAttribArray",
    "glEnableVertexAttribArray",
    "glGetActiveAttrib",
    "glGetActiveUniform",
    "glGetAttachedShaders",
    "glGetAttribLocation",
    "glGetProgramiv",
    "glGetProgramInfoLog",
    "glGetShaderiv",
    "glGetShaderInfoLog",
    "glGetShaderSource",
    "glGetUniformLocation",
    "glGetUniformfv",
    "glGetUniformiv",
    "glGetVertexAttribdv",
    "glGetVertexAttribfv",
    "glGetVertexAttribiv",
    "glGetVertexAttribPointerv",
    "glIsProgram",
    "glIsShader",
    "glLinkProgram",
    "glShaderSource",
    "glUseProgram",
    "glUniform1f",
    "glUniform2f",
    "glUniform3f",
    "glUniform4f",
    "glUniform1i",
    "glUniform2i",
    "glUniform3i",
    "glUniform4i",
    "glUniform1fv",
    "glUniform2fv",
    "glUniform3fv",
    "glUniform4fv",
    "glUniform1iv",
    "glUniform2iv",
    "glUniform3iv",
    "glUniform4iv",
    "glUniformMatrix2fv",
    "glUniformMatrix3fv",
    "glUniformMatrix4fv",
    "glValidateProgram",
    "glVertexAttrib1d",
    "glVertexAttrib1dv",
    "glVertexAttrib1f",
    "glVertexAttrib1fv",
    "glVertexAttrib1s",
    "glVertexAttrib1sv",
    "glVertexAttrib2d",
    "glVertexAttrib2dv",
    "glVertexAttrib2f",
    "glVertexAttrib2fv",
    "glVertexAttrib2s",
    "glVertexAttrib2sv",
    "glVertexAttrib3d",
    "glVertexAttrib3dv",
    "glVertexAttrib3f",
    "glVertexAttrib3fv",
    "glVertexAttrib3s",
    "glVertexAttrib3sv",
    "glVertexAttrib4Nbv",
    "glVertexAttrib4Niv",
    "glVertexAttrib4Nsv",
    "glVertexAttrib4Nub",
    "glVertexAttrib4Nubv",
    "glVertexAttrib4Nuiv",
    "glVertexAttrib4Nusv",
    "glVertexAttrib4bv",
    "glVertexAttrib4d",
    "glVertexAttrib4dv",
    "glVertexAttrib4f",
    "glVertexAttrib4fv",
    "glVertexAttrib4iv",
    "glVertexAttrib4s",
    "glVertexAttrib4sv",
    "glVertexAttrib4ubv",
    "glVertexAttrib4uiv",
    "glVertexAttrib4usv",
    "glVertexAttribPointer",
    "glUniformMatrix2x3fv",
    "glUniformMatrix3x2fv",
    "glUniformMatrix2x4fv",
    "glUniformMatrix4x2fv",
    "glUniformMatrix3x4fv",
    "glUniformMatrix4x3fv",
    "glColorMaski",
    "glGetBooleani_v",
    "glGetIntegeri_v",
    "glEnablei",
    "glDisablei",
    "glIsEnabledi",
    "glBeginTransformFeedback",
    "glEndTransformFeedback",
    "glBindBufferRange",
    "glBindBufferBase",
    "glTransformFeedbackVaryings",
    "glGetTransformFeedbackVarying",
    "glClampColor",
    "glBeginConditionalRender",
    "glEndConditionalRender",
    "glVertexAttribIPointer",
    "glGetVertexAttribIiv",
    "glGetVertexAttribIuiv",
    "glVertexAttribI1i",
    "glVertexAttribI2i",
    "glVertexAttribI3i",
    "glVertexAttribI4i",
    "glVertexAttribI1ui",
    "glVertexAttribI2ui",
    "glVertexAttribI3ui",
    "glVertexAttribI4ui",
    "glVertexAttribI1iv",
    "glVertexAttribI2iv",
    "glVertexAttribI3iv",
    "glVertexAttribI4iv",
    "glVertexAttribI1uiv",
    "glVertexAttribI2uiv",
    "glVertexAttribI3uiv",
    "glVertexAttribI4uiv",
    "glVertexAttribI4bv",
    "glVertexAttribI4sv",
    "glVertexAttribI4ubv",
    "glVertexAttribI4usv",
    "glGetUniformuiv",
    "glBindFragDataLocation",
    "glGetFragDataLocation",
    "glUniform1ui",
    "glUniform2ui",
    "glUniform3ui",
    "glUniform4ui",
    "glUniform1uiv",
    "glUniform2uiv",
    "glUniform3uiv",
    "glUniform4uiv",
    "glTexParameterIiv",
    "glTexParameterIuiv",
    "glGetTexParameterIiv",
    "glGetTexParameterIuiv",
    "glClearBufferiv",
    "glClearBufferuiv",
    "glClearBufferfv",
    "glClearBufferfi",
    "glGetStringi",
    "glIsRenderbuffer",
    "glBindRenderbuffer",
    "glDeleteRenderbuffers",
    "glGenRenderbuffers",
    "glRenderbufferStorage",
    "glGetRenderbufferParameteriv",
    "glIsFramebuffer",
    "glBindFramebuffer",
    "glDeleteFramebuffers",
    "glGenFramebuffers",
    "glCheckFramebufferStatus",
    "glFramebufferTexture1D",
    "glFramebufferTexture2D",
    "glFramebufferTexture3D",
    "glFramebufferRenderbuffer",
    "glGetFramebufferAttachmentParameteriv",
    "glGenerateMipmap",
    "glBlitFramebuffer",
    "glRenderbufferStorageMultisample",
    "glFramebufferTextureLayer",
    "glMapBufferRange",
    "glFlushMappedBufferRange",
    "glBindVertexArray",
    "glDeleteVertexArrays",
    "glGenVertexArrays",
    "glIsVertexArray",
    "glDrawArraysInstanced",
    "glDrawElementsInstanced",
    "glTexBuffer",
    "glPrimitiveRestartIndex",
    "glCopyBufferSubData",
    "glGetUniformIndices",
    "glGetActiveUniformsiv",
    "glGetActiveUniformName",
    "glGetUniformBlockIndex",
    "glGetActiveUniformBlockiv",
    "glGetActiveUniformBlockName",
    "glUniformBlockBinding",
    "glDrawElementsBaseVertex",
    "glDrawRangeElementsBaseVertex",
    "glDrawElementsInstancedBaseVertex",
    "glMultiDrawElementsBaseVertex",
    "glProvokingVertex",
    "glFenceSync",
    "glIsSync",
    "glDeleteSync",
    "glClientWaitSync",
    "glWaitSync",
    "glGetInteger64v",
    "glGetSynciv",
    "glGetInteger64i_v",
    "glGetBufferParameteri64v",
    "glFramebufferTexture",
    "glTexImage2DMultisample",
    "glTexImage3DMultisample",
    "glGetMultisamplefv",
    "glSampleMaski",
    "glBindFragDataLocationIndexed",
    "glGetFragDataIndex",
    "glGenSamplers",
    "glDeleteSamplers",
    "glIsSampler",
    "glBindSampler",
    "glSamplerParameteri",
    "glSamplerParameteriv",
    "glSamplerParameterf",
    "glSamplerParameterfv",
    "glSamplerParameterIiv",
    "glSamplerParameterIuiv",
    "glGetSamplerParameteriv",
    "glGetSamplerParameterIiv",
    "glGetSamplerParameterfv",
    "glGetSamplerParameterIuiv",
    "glQueryCounter",
    "glGetQueryObjecti64v",
    "glGetQueryObjectui64v",
    "glVertexAttribDivisor",
    "glVertexAttribP1ui",
    "glVertexAttribP1uiv",
    "glVertexAttribP2ui",
    "glVertexAttribP2uiv",
    "glVertexAttribP3ui",
    "glVertexAttribP3uiv",
    "glVertexAttribP4ui",
    "glVertexAttribP4uiv",
    "glMinSampleShading",
    "glBlendEquationi",
    "glBlendEquationSeparatei",
    "glBlendFunci",
    "glBlendFuncSeparatei",
    "glDrawArraysIndirect",
    "glDrawElementsIndirect",
    "glUniform1d",
    "glUniform2d",
    "glUniform3d",
    "glUniform4d",
    "glUniform1dv",
    "glUniform2dv",
    "glUniform3dv",
    "glUniform4dv",
    "glUniformMatrix2dv",
    "glUniformMatrix3dv",
    "glUniformMatrix4dv",
    "glUniformMatrix2x3dv",
    "glUniformMatrix2x4dv",
    "glUniformMatrix3x2dv",
    "glUniformMatrix3x4dv",
    "glUniformMatrix4x2dv",
    "glUniformMatrix4x3dv",
    "glGetUniformdv",
    "glGetSubroutineUniformLocation",
    "glGetSubroutineIndex",
    "glGetActiveSubroutineUniformiv",
    "glGetActiveSubroutineUniformName",
    "glGetActiveSubroutineName",
    "glUniformSubroutinesuiv",
    "glGetUniformSubroutineuiv",
    "glGetProgramStageiv",
    "glPatchParameteri",
    "glPatchParameterfv",
    "glBindTransformFeedback",
    "glDeleteTransformFeedbacks",
    "glGenTransformFeedbacks",
    "glIsTransformFeedback",
    "glPauseTransformFeedback",
    "glResumeTransformFeedback",
    "glDrawTransformFeedback",
    "glDrawTransformFeedbackStream",
    "glBeginQueryIndexed",
    "glEndQueryIndexed",
    "glGetQueryIndexediv",
    "glReleaseShaderCompiler",
    "glShaderBinary",
    "glGetShaderPrecisionFormat",
    "glDepthRangef",
    "glClearDepthf",
    "glGetProgramBinary",
    "glProgramBinary",
    "glProgramParameteri",
    "glUseProgramStages",
    "glActiveShaderProgram",
    "glCreateShaderProgramv",
    "glBindProgramPipeline",
    "glDeleteProgramPipelines",
    "glGenProgramPipelines",
    "glIsProgramPipeline",
    "glGetProgramPipelineiv",
    "glProgramUniform1i",
    "glProgramUniform1iv",
    "glProgramUniform1f",
    "glProgramUniform1fv",
    "glProgramUniform1d",
    "glProgramUniform1dv",
    "glProgramUniform1ui",
    "glProgramUniform1uiv",
    "glProgramUniform2i",
    "glProgramUniform2iv",
    "glProgramUniform2f",
    "glProgramUniform2fv",
    "glProgramUniform2d",
    "glProgramUniform2dv",
    "glProgramUniform2ui",
    "glProgramUniform2uiv",
    "glProgramUniform3i",
    "glProgramUniform3iv",
    "glProgramUniform3f",
    "glProgramUniform3fv",
    "glProgramUniform3d",
    "glProgramUniform3dv",
    "glProgramUniform3ui",
    "glProgramUniform3uiv",
    "glProgramUniform4i",
    "glProgramUniform4iv",
    "glProgramUniform4f",
    "glProgramUniform4fv",
    "glProgramUniform4d",
    "glProgramUniform4dv",
    "glProgramUniform4ui",
    "glProgramUniform4uiv",
    "glProgramUniformMatrix2fv",
    "glProgramUniformMatrix3fv",
    "glProgramUniformMatrix4fv",
    "glProgramUniformMatrix2dv",
    "glProgramUniformMatrix3dv",
    "glProgramUniformMatrix4dv",
    "glProgramUniformMatrix2x3fv",
    "glProgramUniformMatrix3x2fv",
    "glProgramUniformMatrix2x4fv",
    "glProgramUniformMatrix4x2fv",
    "glProgramUniformMatrix3x4fv",
    "glProgramUniformMatrix4x3fv",
    "glProgramUniformMatrix2x3dv",
    "glProgramUniformMatrix3x2dv",
    "glProgramUniformMatrix2x4dv",
    "glProgramUniformMatrix4x2dv",
    "glProgramUniformMatrix3x4dv",
    "glProgramUniformMatrix4x3dv",
    "glValidateProgramPipeline",
    "glGetProgramPipelineInfoLog",
    "glVertexAttribL1d",
    "glVertexAttribL2d",
    "glVertexAttribL3d",
    "glVertexAttribL4d",
    "glVertexAttribL1dv",
    "glVertexAttribL2dv",
    "glVertexAttribL3dv",
    "glVertexAttribL4dv",
    "glVertexAttribLPointer",
    "glGetVertexAttribLdv",
    "glViewportArrayv",
    "glViewportIndexedf",
    "glViewportIndexedfv",
    "glScissorArrayv",
    "glScissorIndexed",
    "glScissorIndexedv",
    "glDepthRangeArrayv",
    "glDepthRangeIndexed",
    "glGetFloati_v",
    "glGetDoublei_v",
    "glDrawArraysInstancedBaseInstance",
    "glDrawElementsInstancedBaseInstance",
    "glDrawElementsInstancedBaseVertexBaseInstance",
    "glGetInternalformativ",
    "glGetActiveAtomicCounterBufferiv",
    "glBindImageTexture",
    "glMemoryBarrier",
    "glTexStorage1D",
    "glTexStorage2D",
    "glTexStorage3D",
    "glDrawTransformFeedbackInstanced",
    "glDrawTransformFeedbackStreamInstanced",
    "glClearBufferData",
    "glClearBufferSubData",
    "glDispatchCompute",
    "glDispatchComputeIndirect",
    "glCopyImageSubData",
    "glFramebufferParameteri",
    "glGetFramebufferParameteriv",
    "glGetInternalformati64v",
    "glInvalidateTexSubImage",
    "glInvalidateTexImage",
    "glInvalidateBufferSubData",
    "glInvalidateBufferData",
    "glInvalidateFramebuffer",
    "glInvalidateSubFramebuffer",
    "glMultiDrawArraysIndirect",
    "glMultiDrawElementsIndirect",
    "glGetProgramInterfaceiv",
    "glGetProgramResourceIndex",
    "glGetProgramResourceName",
    "glGetProgramResourceiv",
    "glGetProgramResourceLocation",
    "glGetProgramResourceLocationIndex",
    "glShaderStorageBlockBinding",
    "glTexBufferRange",
    "glTexStorage2DMultisample",
    "glTexStorage3DMultisample",
    "glTextureView",
    "glBindVertexBuffer",
    "glVertexAttribFormat",
    "glVertexAttribIFormat",
    "glVertexAttribLFormat",
    "glVertexAttribBinding",
    "glVertexBindingDivisor",
    "glDebugMessageControl",
    "glDebugMessageInsert",
    "glDebugMessageCallback",
    "glGetDebugMessageLog",
    "glPushDebugGroup",
    "glPopDebugGroup",
    "glObjectLabel",
    "glGetObjectLabel",
    "glObjectPtrLabel",
    "glGetObjectPtrLabel",
    "glBufferStorage",
    "glClearTexImage",
    "glClearTexSubImage",
    "glBindBuffersBase",
    "glBindBuffersRange",
    "glBindTextures",
    "glBindSamplers",
    "glBindImageTextures",
    "glBindVertexBuffers",
    "glClipControl",
    "glCreateTransformFeedbacks",
    "glTransformFeedbackBufferBase",
    "glTransformFeedbackBufferRange",
    "glGetTransformFeedbackiv",
    "glGetTransformFeedbacki_v",
    "glGetTransformFeedbacki64_v",
    "glCreateBuffers",
    "glNamedBufferStorage",
    "glNamedBufferData",
    "glNamedBufferSubData",
    "glCopyNamedBufferSubData",
    "glClearNamedBufferData",
    "glClearNamedBufferSubData",
    "glMapNamedBuffer",
    "glMapNamedBufferRange",
    "glUnmapNamedBuffer",
    "glFlushMappedNamedBufferRange",
    "glGetNamedBufferParameteriv",
    "glGetNamedBufferParameteri64v",
    "glGetNamedBufferPointerv",
    "glGetNamedBufferSubData",
    "glCreateFramebuffers",
    "glNamedFramebufferRenderbuffer",
    "glNamedFramebufferParameteri",
    "glNamedFramebufferTexture",
    "glNamedFramebufferTextureLayer",
    "glNamedFramebufferDrawBuffer",
    "glNamedFramebufferDrawBuffers",
    "glNamedFramebufferReadBuffer",
    "glInvalidateNamedFramebufferData",
    "glInvalidateNamedFramebufferSubData",
    "glClearNamedFramebufferiv",
    "glClearNamedFramebufferuiv",
    "glClearNamedFramebufferfv",
    "glClearNamedFramebufferfi",
    "glBlitNamedFramebuffer",
    "glCheckNamedFramebufferStatus",
    "glGetNamedFramebufferParameteriv",
    "glGetNamedFramebufferAttachmentParameteriv",
    "glCreateRenderbuffers",
    "glNamedRenderbufferStorage",
    "glNamedRenderbufferStorageMultisample",
    "glGetNamedRenderbufferParameteriv",
    "glCreateTextures",
    "glTextureBuffer",
    "glTextureBufferRange",
    "glTextureStorage1D",
    "glTextureStorage2D",
    "glTextureStorage3D",
    "glTextureStorage2DMultisample",
    "glTextureStorage3DMultisample",
    "glTextureSubImage1D",
    "glTextureSubImage2D",
    "glTextureSubImage3D",
    "glCompressedTextureSubImage1D",
    "glCompressedTextureSubImage2D",
    "glCompressedTextureSubImage3D",
    "glCopyTextureSubImage1D",
    "glCopyTextureSubImage2D",
    "glCopyTextureSubImage3D",
    "glTextureParameterf",
    "glTextureParameterfv",
    "glTextureParameteri",
    "glTextureParameterIiv",
    "glTextureParameterIuiv",
    "glTextureParameteriv",
    "glGenerateTextureMipmap",
    "glBindTextureUnit",
    "glGetTextureImage",
    "glGetCompressedTextureImage",
    "glGetTextureLevelParameterfv",
    "glGetTextureLevelParameteriv",
    "glGetTextureParameterfv",
    "glGetTextureParameterIiv",
    "glGetTextureParameterIuiv",
    "glGetTextureParameteriv",
    "glCreateVertexArrays",
    "glDisableVertexArrayAttrib",
    "glEnableVertexArrayAttrib",
    "glVertexArrayElementBuffer",
    "glVertexArrayVertexBuffer",
    "glVertexArrayVertexBuffers",
    "glVertexArrayAttribBinding",
    "glVertexArrayAttribFormat",
    "glVertexArrayAttribIFormat",
    "glVertexArrayAttribLFormat",
    "glVertexArrayBindingDivisor",
    "glGetVertexArrayiv",
    "glGetVertexArrayIndexediv",
    "glGetVertexArrayIndexed64iv",
    "glCreateSamplers",
    "glCreateProgramPipelines",
    "glCreateQueries",
    "glGetQueryBufferObjecti64v",
    "glGetQueryBufferObjectiv",
    "glGetQueryBufferObjectui64v",
    "glGetQueryBufferObjectuiv",
    "glMemoryBarrierByRegion",
    "glGetTextureSubImage",
    "glGetCompressedTextureSubImage",
    "glGetGraphicsResetStatus",
    "glGetnCompressedTexImage",
    "glGetnTexImage",
    "glGetnUniformdv",
    "glGetnUniformfv",
    "glGetnUniformiv",
    "glGetnUniformuiv",
    "glReadnPixels",
    "glTextureBarrier",
    "glSpecializeShader",
    "glMultiDrawArraysIndirectCount",
    "glMultiDrawElementsIndirectCount",
    "glPolygonOffsetClamp",
    0,
};

PyModuleDef module_def = {PyModuleDef_HEAD_INIT, "moderngl.gltraces", 0, -1, 0, 0, 0, 0, 0};

extern "C" PyObject * PyInit_gltraces() {
    PyObject * module = PyModule_Create(&module_def);

    PyObject * py_glprocs = PyMemoryView_FromMemory((char *)&gl, sizeof(gl), PyBUF_WRITE);
    PyObject * py_gltraces = PyMemoryView_FromMemory((char *)&traces, sizeof(traces), PyBUF_WRITE);
    PyObject * py_lookup = PyDict_New();

    for (int i = 0; names[i]; ++i) {
        PyDict_SetItemString(py_lookup, names[i], PyLong_FromLong(i));
    }

    PyModule_AddObject(module, "glprocs", py_glprocs);
    PyModule_AddObject(module, "gltraces", py_gltraces);
    PyModule_AddObject(module, "lookup", py_lookup);

    return module;
}
