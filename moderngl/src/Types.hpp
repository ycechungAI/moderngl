#pragma once

#include <Python.h>
#include "gl_methods.hpp"

typedef void (* MGLProc)();

enum MGLEnableFlag {
	MGL_NOTHING = 0,
	MGL_BLEND = 1,
	MGL_DEPTH_TEST = 2,
	MGL_CULL_FACE = 4,
	MGL_RASTERIZER_DISCARD = 8,
	MGL_PROGRAM_POINT_SIZE = 16,
	MGL_INVALID = 0x40000000,
};

enum SHADER_SLOT_ENUM {
	VERTEX_SHADER_SLOT,
	FRAGMENT_SHADER_SLOT,
	GEOMETRY_SHADER_SLOT,
	TESS_EVALUATION_SHADER_SLOT,
	TESS_CONTROL_SHADER_SLOT,
	NUM_SHADER_SLOTS,
};

static const int SHADER_TYPE[] = {
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_GEOMETRY_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER,
};

struct MGLBuffer;
struct MGLComputeShader;
struct MGLContext;
struct MGLFramebuffer;
struct MGLProgram;
struct MGLRenderbuffer;
struct MGLTexture;
struct MGLTexture3D;
struct MGLTextureArray;
struct MGLTextureCube;
struct MGLVertexArray;
struct MGLSampler;

struct MGLDataType {
	int * base_format;
	int * internal_format;
	int gl_type;
	int size;
	bool float_type;
};

struct MGLBuffer {
	PyObject_HEAD

	MGLContext * context;

	int buffer_obj;

	Py_ssize_t size;
	bool dynamic;

    bool released;
};

struct MGLComputeShader {
	PyObject_HEAD

	MGLContext * context;

	int program_obj;
	int shader_obj;

    bool released;
};

struct MGLContext {
	PyObject_HEAD

	PyObject * ctx;
	PyObject * enter_func;
	PyObject * exit_func;
	PyObject * release_func;
	PyObject * extensions;

	MGLFramebuffer * default_framebuffer;
	MGLFramebuffer * bound_framebuffer;

	int version_code;

	int max_samples;
	int max_integer_samples;
	int max_color_attachments;

	int max_texture_units;
	int default_texture_unit;
	float max_anisotropy;

	int enable_flags;
	int front_face;
	int cull_face;

	int depth_func;
	int blend_func_src;
	int blend_func_dst;

	bool wireframe;
	bool multisample;

	int provoking_vertex;

	float polygon_offset_factor;
	float polygon_offset_units;

	GLMethods gl;

    bool released;
};

struct MGLFramebuffer {
	PyObject_HEAD

	MGLContext * context;

	bool * color_mask;

	unsigned * draw_buffers;
	int draw_buffers_len;

	int framebuffer_obj;

	int viewport_x;
	int viewport_y;
	int viewport_width;
	int viewport_height;

	bool scissor_enabled;
	int scissor_x;
	int scissor_y;
	int scissor_width;
	int scissor_height;

	// Flags this as a detected framebuffer we don't control the size of
	bool dynamic;
	int width;
	int height;
	int samples;

	bool depth_mask;

    bool released;
};

struct MGLProgram {
	PyObject_HEAD

	MGLContext * context;

	int geometry_input;
	int geometry_output;

	int program_obj;

	int num_vertex_shader_subroutines;
	int num_fragment_shader_subroutines;
	int num_geometry_shader_subroutines;
	int num_tess_evaluation_shader_subroutines;
	int num_tess_control_shader_subroutines;

	int geometry_vertices;
	int num_varyings;

    bool released;
};

enum MGLQueryKeys {
	SAMPLES_PASSED,
	ANY_SAMPLES_PASSED,
	TIME_ELAPSED,
	PRIMITIVES_GENERATED,
};

struct MGLQuery {
	PyObject_HEAD

	MGLContext * context;

	int query_obj[4];

    bool released;
};

struct MGLRenderbuffer {
	PyObject_HEAD

	MGLContext * context;
	MGLDataType * data_type;

	union {
		int renderbuffer_obj;
		int texture_obj;
	};

	int width;
	int height;
	int components;

	int samples;
	bool depth;

    bool released;
};

struct MGLScope {
	PyObject_HEAD

	MGLContext * context;
	MGLFramebuffer * framebuffer;
	MGLFramebuffer * old_framebuffer;

	int * textures;
	int * buffers;
	PyObject * samplers;

	int num_textures;
	int num_buffers;

	int enable_flags;
	int old_enable_flags;

    bool released;
};

struct MGLTexture {
	PyObject_HEAD

	MGLContext * context;
	MGLDataType * data_type;

	union {
		int renderbuffer_obj;
		int texture_obj;
	};

	int width;
	int height;
	int components;

	int samples;

	int min_filter;
	int mag_filter;
	int max_level;

	int compare_func;
	float anisotropy;

	bool depth;

	bool repeat_x;
	bool repeat_y;

    bool external;

    bool released;
};

struct MGLTexture3D {
	PyObject_HEAD

	MGLContext * context;
	MGLDataType * data_type;

	int texture_obj;

	int width;
	int height;
	int depth;

	int components;

	int min_filter;
	int mag_filter;
	int max_level;

	bool repeat_x;
	bool repeat_y;
	bool repeat_z;

    bool released;
};

struct MGLTextureArray {
	PyObject_HEAD

	MGLContext * context;
	MGLDataType * data_type;

	union {
		int renderbuffer_obj;
		int texture_obj;
	};

	int width;
	int height;
	int layers;
	int components;

	int min_filter;
	int mag_filter;
	int max_level;

	bool repeat_x;
	bool repeat_y;
	float anisotropy;

    bool released;
};

struct MGLTextureCube {
	PyObject_HEAD

	MGLContext * context;
	MGLDataType * data_type;

	int texture_obj;

	int width;
	int height;
	int depth;

	int components;

	int min_filter;
	int mag_filter;
	int max_level;
	float anisotropy;

    bool released;
};

struct MGLVertexArray {
	PyObject_HEAD

	MGLContext * context;

	MGLProgram * program;
	MGLBuffer * index_buffer;
	int index_element_size;
	int index_element_type;

	unsigned * subroutines;
	int num_subroutines;

	int vertex_array_obj;
	int num_vertices;
	int num_instances;

    bool released;
};

struct MGLSampler {
	PyObject_HEAD

	MGLContext * context;
	int sampler_obj;

	int min_filter;
	int mag_filter;
	float anisotropy;

	int compare_func;

	bool repeat_x;
	bool repeat_y;
	bool repeat_z;

	float border_color[4];

	float min_lod;
	float max_lod;

    bool released;
};

MGLDataType * from_dtype(const char * dtype, Py_ssize_t size);

void MGLBuffer_Invalidate(MGLBuffer * buffer);
void MGLComputeShader_Invalidate(MGLComputeShader * program);
void MGLContext_Invalidate(MGLContext * context);
void MGLFramebuffer_Invalidate(MGLFramebuffer * framebuffer);
void MGLProgram_Invalidate(MGLProgram * program);
void MGLRenderbuffer_Invalidate(MGLRenderbuffer * renderbuffer);
void MGLTexture3D_Invalidate(MGLTexture3D * texture);
void MGLTextureCube_Invalidate(MGLTextureCube * texture);
void MGLTexture_Invalidate(MGLTexture * texture);
void MGLTextureArray_Invalidate(MGLTextureArray * texture);
void MGLVertexArray_Invalidate(MGLVertexArray * vertex_array);
void MGLSampler_Invalidate(MGLSampler * sampler);
void MGLScope_Invalidate(MGLScope * scope);

void MGLVertexArray_Complete(MGLVertexArray * vertex_array);

void MGLContext_Initialize(MGLContext * self);

extern PyTypeObject * MGLBuffer_type;
extern PyTypeObject * MGLComputeShader_type;
extern PyTypeObject * MGLContext_type;
extern PyTypeObject * MGLFramebuffer_type;
extern PyTypeObject * MGLProgram_type;
extern PyTypeObject * MGLQuery_type;
extern PyTypeObject * MGLRenderbuffer_type;
extern PyTypeObject * MGLScope_type;
extern PyTypeObject * MGLTexture3D_type;
extern PyTypeObject * MGLTextureCube_type;
extern PyTypeObject * MGLTexture_type;
extern PyTypeObject * MGLTextureArray_type;
extern PyTypeObject * MGLVertexArray_type;
extern PyTypeObject * MGLSampler_type;

extern PyObject * helper;
extern PyObject * moderngl_error;

#define MGLError_Set(...) PyErr_Format(moderngl_error, __VA_ARGS__)
