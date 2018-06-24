#include "mgl.hpp"

#include "context.hpp"
#include "tools.hpp"

PyObject * module_error;

PyObject * compiler_error;
PyObject * linker_error;
PyObject * split_format;

PyObject * tobytes_str;
PyObject * size_str;
PyObject * mode_str;
PyObject * detect_framebuffer_str;

PyObject * triangles_long;
PyObject * points_long;
PyObject * zero_long;

#ifdef MGL_DEBUG
PyObject * moderngl_debugger;
#endif
PyObject * moderngl;
PyObject * numpy;
PyObject * pillow;

PyObject * numpy_frombuffer;
PyObject * pillow_image;

PyTypeObject * Context_class;
int Context_class_mglo;
int Context_class_framebuffers;
int Context_class_limits;
int Context_class_screen;
int Context_class_fbo;
int Context_class_extra;

PyTypeObject * Attribute_class;
int Attribute_class_type;
int Attribute_class_location;
int Attribute_class_cols;
int Attribute_class_rows;
int Attribute_class_size;
int Attribute_class_shape;

PyTypeObject * Uniform_class;
int Uniform_class_type;
int Uniform_class_location;
int Uniform_class_cols;
int Uniform_class_rows;
int Uniform_class_size;
int Uniform_class_shape;

PyTypeObject * Buffer_class;
int Buffer_class_mglo;
int Buffer_class_size;
int Buffer_class_extra;

PyTypeObject * Framebuffer_class;
int Framebuffer_class_mglo;
int Framebuffer_class_viewport;
int Framebuffer_class_size;
int Framebuffer_class_extra;

PyTypeObject * Program_class;
int Program_class_mglo;
int Program_class_uniforms;
int Program_class_attributes;
int Program_class_extra;

PyTypeObject * Renderbuffer_class;
int Renderbuffer_class_mglo;
int Renderbuffer_class_size;
int Renderbuffer_class_extra;

PyTypeObject * Texture_class;
int Texture_class_mglo;
int Texture_class_level;
int Texture_class_layer;
int Texture_class_size;
int Texture_class_extra;

PyTypeObject * TextureArray_class;
int TextureArray_class_mglo;
int TextureArray_class_level;
int TextureArray_class_index;
int TextureArray_class_size;
int TextureArray_class_length;
int TextureArray_class_extra;

PyTypeObject * TextureCube_class;
int TextureCube_class_mglo;
int TextureCube_class_size;
int TextureCube_class_extra;

PyTypeObject * VertexArray_class;
int VertexArray_class_mglo;
int VertexArray_class_ibo;
int VertexArray_class_program;
int VertexArray_class_mode;
int VertexArray_class_vertices;
int VertexArray_class_extra;

PyTypeObject * Query_class;
int Query_class_mglo;
int Query_class_extra;

PyTypeObject * Scope_class;
int Scope_class_mglo;
int Scope_class_extra;

PyTypeObject * Limits_class;
int Limits_class_max_samples;
int Limits_class_max_integer_samples;

PyTypeObject * MGLContext_type;

PyMethodDef mgl_methods[] = {
	{"create_context", (PyCFunction)meth_create_context, METH_VARARGS, 0},
	{0},
};

PyModuleDef mgl_def = {PyModuleDef_HEAD_INIT, "moderngl.mgl", 0, -1, mgl_methods, 0, 0, 0, 0};

void initialize_module() {
	static bool initialized = false;
	if (initialized) {
		return;
	}

	moderngl = PyImport_ImportModule("moderngl");
	if (!moderngl) {
		return;
	}

	#ifdef MGL_DEBUG
	moderngl_debugger = PyImport_ImportModule("moderngl_debugger");
	if (!moderngl_debugger) {
		PyErr_Clear();
	}
	#endif

	numpy = PyImport_ImportModule("numpy");
	if (!numpy) {
		PyErr_Clear();
	}

	pillow = PyImport_ImportModule("PIL.Image");
	if (!pillow) {
		PyErr_Clear();
	}

	MGLContext_type = MGLContext_define();

	module_error = PyObject_GetAttrString(moderngl, "Error");
	compiler_error = PyObject_GetAttrString(moderngl, "_compiler_error");
	linker_error = PyObject_GetAttrString(moderngl, "_linker_error");
	split_format = PyObject_GetAttrString(moderngl, "_split_format");

	tobytes_str = PyUnicode_FromString("tobytes");
	size_str = PyUnicode_FromString("size");
	mode_str = PyUnicode_FromString("mode");
	detect_framebuffer_str = PyUnicode_FromString("detect_framebuffer");

	triangles_long = PyLong_FromLong(GL_TRIANGLES);
	points_long = PyLong_FromLong(GL_POINTS);
	zero_long = PyLong_FromLong(0);

	if (numpy) {
		numpy_frombuffer = PyObject_GetAttrString(numpy, "frombuffer");
	}

	if (pillow) {
		pillow_image = PyObject_GetAttrString(pillow, "Image");
	}

	int Context_slots = 0;
	Context_class = detect_class(moderngl, "Context", Context_slots);
	Context_class_mglo = slot_offset(Context_class, "_Context__mglo", Context_slots);
	Context_class_framebuffers = slot_offset(Context_class, "_Context__framebuffers", Context_slots);
	Context_class_limits = slot_offset(Context_class, "limits", Context_slots);
	Context_class_screen = slot_offset(Context_class, "screen", Context_slots);
	Context_class_fbo = slot_offset(Context_class, "fbo", Context_slots);
	Context_class_extra = slot_offset(Context_class, "extra", Context_slots);
	assert_slots_len(Context_class, Context_slots);
	protect_slot(Context_class, "limits");
	protect_slot(Context_class, "screen");
	protect_slot(Context_class, "fbo");
	remove_init(Context_class);

	int Attribute_slots = 0;
	Attribute_class = detect_class(moderngl, "Attribute", Attribute_slots);
	Attribute_class_type = slot_offset(Attribute_class, "_Attribute__type", Attribute_slots);
	Attribute_class_location = slot_offset(Attribute_class, "location", Attribute_slots);
	Attribute_class_cols = slot_offset(Attribute_class, "cols", Attribute_slots);
	Attribute_class_rows = slot_offset(Attribute_class, "rows", Attribute_slots);
	Attribute_class_size = slot_offset(Attribute_class, "size", Attribute_slots);
	Attribute_class_shape = slot_offset(Attribute_class, "shape", Attribute_slots);
	assert_slots_len(Attribute_class, Attribute_slots);
	protect_slot(Attribute_class, "location");
	protect_slot(Attribute_class, "cols");
	protect_slot(Attribute_class, "rows");
	protect_slot(Attribute_class, "size");
	protect_slot(Attribute_class, "shape");
	remove_init(Attribute_class);

	int Uniform_slots = 0;
	Uniform_class = detect_class(moderngl, "Uniform", Uniform_slots);
	Uniform_class_type = slot_offset(Uniform_class, "_Uniform__type", Uniform_slots);
	Uniform_class_location = slot_offset(Uniform_class, "location", Uniform_slots);
	Uniform_class_cols = slot_offset(Uniform_class, "cols", Uniform_slots);
	Uniform_class_rows = slot_offset(Uniform_class, "rows", Uniform_slots);
	Uniform_class_size = slot_offset(Uniform_class, "size", Uniform_slots);
	Uniform_class_shape = slot_offset(Uniform_class, "shape", Uniform_slots);
	assert_slots_len(Uniform_class, Uniform_slots);
	protect_slot(Uniform_class, "location");
	protect_slot(Uniform_class, "cols");
	protect_slot(Uniform_class, "rows");
	protect_slot(Uniform_class, "size");
	protect_slot(Uniform_class, "shape");
	remove_init(Uniform_class);

	int Buffer_slots = 0;
	Buffer_class = detect_class(moderngl, "Buffer", Buffer_slots);
	Buffer_class_mglo = slot_offset(Buffer_class, "_Buffer__mglo", Buffer_slots);
	Buffer_class_size = slot_offset(Buffer_class, "size", Buffer_slots);
	Buffer_class_extra = slot_offset(Buffer_class, "extra", Buffer_slots);
	assert_slots_len(Buffer_class, Buffer_slots);
	protect_slot(Buffer_class, "size");
	remove_init(Buffer_class);

	int Framebuffer_slots = 0;
	Framebuffer_class = detect_class(moderngl, "Framebuffer", Framebuffer_slots);
	Framebuffer_class_mglo = slot_offset(Framebuffer_class, "_Framebuffer__mglo", Framebuffer_slots);
	Framebuffer_class_viewport = slot_offset(Framebuffer_class, "_Framebuffer__viewport", Framebuffer_slots);
	Framebuffer_class_size = slot_offset(Framebuffer_class, "size", Framebuffer_slots);
	Framebuffer_class_extra = slot_offset(Framebuffer_class, "extra", Framebuffer_slots);
	assert_slots_len(Framebuffer_class, Framebuffer_slots);
	protect_slot(Framebuffer_class, "size");
	remove_init(Framebuffer_class);

	int Program_slots = 0;
	Program_class = detect_class(moderngl, "Program", Program_slots);
	Program_class_mglo = slot_offset(Program_class, "_Program__mglo", Program_slots);
	Program_class_uniforms = slot_offset(Program_class, "uniforms", Program_slots);
	Program_class_attributes = slot_offset(Program_class, "attributes", Program_slots);
	Program_class_extra = slot_offset(Program_class, "extra", Program_slots);
	assert_slots_len(Program_class, Program_slots);
	protect_slot(Program_class, "uniforms");
	protect_slot(Program_class, "attributes");
	remove_init(Program_class);

	int Query_slots = 0;
	Query_class = detect_class(moderngl, "Query", Query_slots);
	Query_class_mglo = slot_offset(Query_class, "_Query__mglo", Query_slots);
	Query_class_extra = slot_offset(Query_class, "extra", Query_slots);
	assert_slots_len(Query_class, Query_slots);
	remove_init(Query_class);

	int Renderbuffer_slots = 0;
	Renderbuffer_class = detect_class(moderngl, "Renderbuffer", Renderbuffer_slots);
	Renderbuffer_class_mglo = slot_offset(Renderbuffer_class, "_Renderbuffer__mglo", Renderbuffer_slots);
	Renderbuffer_class_size = slot_offset(Renderbuffer_class, "size", Renderbuffer_slots);
	Renderbuffer_class_extra = slot_offset(Renderbuffer_class, "extra", Renderbuffer_slots);
	assert_slots_len(Renderbuffer_class, Renderbuffer_slots);
	protect_slot(Renderbuffer_class, "size");
	remove_init(Renderbuffer_class);

	int Scope_slots = 0;
	Scope_class = detect_class(moderngl, "Scope", Scope_slots);
	Scope_class_mglo = slot_offset(Scope_class, "_Scope__mglo", Scope_slots);
	Scope_class_extra = slot_offset(Scope_class, "extra", Scope_slots);
	assert_slots_len(Scope_class, Scope_slots);
	remove_init(Scope_class);

	int TextureArray_slots = 0;
	TextureArray_class = detect_class(moderngl, "TextureArray", TextureArray_slots);
	TextureArray_class_mglo = slot_offset(TextureArray_class, "_TextureArray__mglo", TextureArray_slots);
	TextureArray_class_level = slot_offset(TextureArray_class, "_TextureArray__level", TextureArray_slots);
	TextureArray_class_index = slot_offset(TextureArray_class, "_TextureArray__index", TextureArray_slots);
	TextureArray_class_size = slot_offset(TextureArray_class, "size", TextureArray_slots);
	TextureArray_class_length = slot_offset(TextureArray_class, "length", TextureArray_slots);
	TextureArray_class_extra = slot_offset(TextureArray_class, "extra", TextureArray_slots);
	assert_slots_len(TextureArray_class, TextureArray_slots);
	protect_slot(TextureArray_class, "size");
	protect_slot(TextureArray_class, "length");
	remove_init(TextureArray_class);

	int TextureCube_slots = 0;
	TextureCube_class = detect_class(moderngl, "TextureCube", TextureCube_slots);
	TextureCube_class_mglo = slot_offset(TextureCube_class, "_TextureCube__mglo", TextureCube_slots);
	TextureCube_class_size = slot_offset(TextureCube_class, "size", TextureCube_slots);
	TextureCube_class_extra = slot_offset(TextureCube_class, "extra", TextureCube_slots);
	assert_slots_len(TextureCube_class, TextureCube_slots);
	protect_slot(TextureCube_class, "size");
	remove_init(TextureCube_class);

	int Texture_slots = 0;
	Texture_class = detect_class(moderngl, "Texture", Texture_slots);
	Texture_class_mglo = slot_offset(Texture_class, "_Texture__mglo", Texture_slots);
	Texture_class_level = slot_offset(Texture_class, "_Texture__level", Texture_slots);
	Texture_class_layer = slot_offset(Texture_class, "_Texture__layer", Texture_slots);
	Texture_class_size = slot_offset(Texture_class, "size", Texture_slots);
	Texture_class_extra = slot_offset(Texture_class, "extra", Texture_slots);
	assert_slots_len(Texture_class, Texture_slots);
	protect_slot(Texture_class, "size");
	remove_init(Texture_class);

	int VertexArray_slots = 0;
	VertexArray_class = detect_class(moderngl, "VertexArray", VertexArray_slots);
	VertexArray_class_mglo = slot_offset(VertexArray_class, "_VertexArray__mglo", VertexArray_slots);
	VertexArray_class_ibo = slot_offset(VertexArray_class, "_VertexArray__ibo", VertexArray_slots);
	VertexArray_class_program = slot_offset(VertexArray_class, "program", VertexArray_slots);
	VertexArray_class_mode = slot_offset(VertexArray_class, "mode", VertexArray_slots);
	VertexArray_class_vertices = slot_offset(VertexArray_class, "vertices", VertexArray_slots);
	VertexArray_class_extra = slot_offset(VertexArray_class, "extra", VertexArray_slots);
	assert_slots_len(VertexArray_class, VertexArray_slots);
	protect_slot(VertexArray_class, "program");
	remove_init(VertexArray_class);

	int Limits_slots = 0;
	Limits_class = detect_class(moderngl, "Limits", Limits_slots);
	Limits_class_max_samples = slot_offset(Limits_class, "max_samples", Limits_slots);
	Limits_class_max_integer_samples = slot_offset(Limits_class, "max_integer_samples", Limits_slots);
	assert_slots_len(Limits_class, Limits_slots);
	protect_all_slots(Limits_class);
	remove_init(Limits_class);

	initialized = true;
}

extern "C" PyObject * PyInit_mgl() {
	PyObject * module = PyModule_Create(&mgl_def);
	return module;
}
