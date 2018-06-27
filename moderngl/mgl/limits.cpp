#include "limits.hpp"

#include "tools.hpp"
#include "mgl.hpp"

template <typename T>
PyObject * to_pyobject(const T & value) {
	Py_RETURN_NONE;
}

template <>
PyObject * to_pyobject(const int & value) {
	return PyLong_FromLong(value);
}

template <>
PyObject * to_pyobject(const long long & value) {
	return PyLong_FromLongLong(value);
}

template <>
PyObject * to_pyobject(const float & value) {
	return PyFloat_FromDouble(value);
}

template <typename T>
void gl_get(const GLMethods & gl, int param, T * value) {
}

template <>
void gl_get(const GLMethods & gl, int param, int * value) {
	gl.GetIntegerv(param, value);
}

template <>
void gl_get(const GLMethods & gl, int param, long long * value) {
	if (gl.GetInteger64v) {
		gl.GetInteger64v(param, value);
	}
}

template <>
void gl_get(const GLMethods & gl, int param, float * value) {
	gl.GetFloatv(param, value);
}

template <typename T, int size>
PyObject * query_limit(const GLMethods & gl, int param) {
	T value[size] = {};
	gl_get(gl, param, value);
	if (size == 1) {
		return to_pyobject<T>(value[0]);
	}
	PyObject * tuple = PyTuple_New(size);
	for (int i = 0; i < size; ++i) {
		PyTuple_SET_ITEM(tuple, i, to_pyobject<T>(value[i]));
	}
	return tuple;
}

PyObject * get_limits(const GLMethods & gl, int version_code) {
	PyObject * limits = new_object(PyObject, Limits_class);

	SLOT(limits, PyObject, Limits_class_point_size_range) = query_limit<float, 2>(gl, GL_POINT_SIZE_RANGE);
	SLOT(limits, PyObject, Limits_class_smooth_line_width_range) = query_limit<float, 2>(gl, GL_SMOOTH_LINE_WIDTH_RANGE);
	SLOT(limits, PyObject, Limits_class_aliased_line_width_range) = query_limit<float, 2>(gl, GL_ALIASED_LINE_WIDTH_RANGE);
	SLOT(limits, PyObject, Limits_class_point_fade_threshold_size) = query_limit<float, 1>(gl, GL_POINT_FADE_THRESHOLD_SIZE);
	SLOT(limits, PyObject, Limits_class_point_size_granularity) = query_limit<float, 1>(gl, GL_POINT_SIZE_GRANULARITY);
	SLOT(limits, PyObject, Limits_class_smooth_line_width_granularity) = query_limit<float, 1>(gl, GL_SMOOTH_LINE_WIDTH_GRANULARITY);
	SLOT(limits, PyObject, Limits_class_min_program_texel_offset) = query_limit<float, 1>(gl, GL_MIN_PROGRAM_TEXEL_OFFSET);
	SLOT(limits, PyObject, Limits_class_max_program_texel_offset) = query_limit<float, 1>(gl, GL_MAX_PROGRAM_TEXEL_OFFSET);

	SLOT(limits, PyObject, Limits_class_minor_version) = query_limit<int, 1>(gl, GL_MINOR_VERSION);
	SLOT(limits, PyObject, Limits_class_major_version) = query_limit<int, 1>(gl, GL_MAJOR_VERSION);
	SLOT(limits, PyObject, Limits_class_sample_buffers) = query_limit<int, 1>(gl, GL_SAMPLE_BUFFERS);
	SLOT(limits, PyObject, Limits_class_subpixel_bits) = query_limit<int, 1>(gl, GL_SUBPIXEL_BITS);
	SLOT(limits, PyObject, Limits_class_context_profile_mask) = query_limit<int, 1>(gl, GL_CONTEXT_PROFILE_MASK);
	SLOT(limits, PyObject, Limits_class_uniform_buffer_offset_alignment) = query_limit<int, 1>(gl, GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT);
	SLOT(limits, PyObject, Limits_class_max_viewport_dims) = query_limit<int, 2>(gl, GL_MAX_VIEWPORT_DIMS);
	SLOT(limits, PyObject, Limits_class_max_3d_texture_size) = query_limit<int, 1>(gl, GL_MAX_3D_TEXTURE_SIZE);
	SLOT(limits, PyObject, Limits_class_max_array_texture_layers) = query_limit<int, 1>(gl, GL_MAX_ARRAY_TEXTURE_LAYERS);
	SLOT(limits, PyObject, Limits_class_max_clip_distances) = query_limit<int, 1>(gl, GL_MAX_CLIP_DISTANCES);
	SLOT(limits, PyObject, Limits_class_max_color_attachments) = query_limit<int, 1>(gl, GL_MAX_COLOR_ATTACHMENTS);
	SLOT(limits, PyObject, Limits_class_max_color_texture_samples) = query_limit<int, 1>(gl, GL_MAX_COLOR_TEXTURE_SAMPLES);
	SLOT(limits, PyObject, Limits_class_max_combined_fragment_uniform_components) = query_limit<int, 1>(gl, GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS);
	SLOT(limits, PyObject, Limits_class_max_combined_geometry_uniform_components) = query_limit<int, 1>(gl, GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS);
	SLOT(limits, PyObject, Limits_class_max_combined_texture_image_units) = query_limit<int, 1>(gl, GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
	SLOT(limits, PyObject, Limits_class_max_combined_uniform_blocks) = query_limit<int, 1>(gl, GL_MAX_COMBINED_UNIFORM_BLOCKS);
	SLOT(limits, PyObject, Limits_class_max_combined_vertex_uniform_components) = query_limit<int, 1>(gl, GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS);
	SLOT(limits, PyObject, Limits_class_max_cube_map_texture_size) = query_limit<int, 1>(gl, GL_MAX_CUBE_MAP_TEXTURE_SIZE);
	SLOT(limits, PyObject, Limits_class_max_depth_texture_samples) = query_limit<int, 1>(gl, GL_MAX_DEPTH_TEXTURE_SAMPLES);
	SLOT(limits, PyObject, Limits_class_max_draw_buffers) = query_limit<int, 1>(gl, GL_MAX_DRAW_BUFFERS);
	SLOT(limits, PyObject, Limits_class_max_dual_source_draw_buffers) = query_limit<int, 1>(gl, GL_MAX_DUAL_SOURCE_DRAW_BUFFERS);
	SLOT(limits, PyObject, Limits_class_max_elements_indices) = query_limit<int, 1>(gl, GL_MAX_ELEMENTS_INDICES);
	SLOT(limits, PyObject, Limits_class_max_elements_vertices) = query_limit<int, 1>(gl, GL_MAX_ELEMENTS_VERTICES);
	SLOT(limits, PyObject, Limits_class_max_fragment_input_components) = query_limit<int, 1>(gl, GL_MAX_FRAGMENT_INPUT_COMPONENTS);
	SLOT(limits, PyObject, Limits_class_max_fragment_uniform_components) = query_limit<int, 1>(gl, GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);
	SLOT(limits, PyObject, Limits_class_max_fragment_uniform_vectors) = query_limit<int, 1>(gl, GL_MAX_FRAGMENT_UNIFORM_VECTORS);
	SLOT(limits, PyObject, Limits_class_max_fragment_uniform_blocks) = query_limit<int, 1>(gl, GL_MAX_FRAGMENT_UNIFORM_BLOCKS);
	SLOT(limits, PyObject, Limits_class_max_geometry_input_components) = query_limit<int, 1>(gl, GL_MAX_GEOMETRY_INPUT_COMPONENTS);
	SLOT(limits, PyObject, Limits_class_max_geometry_output_components) = query_limit<int, 1>(gl, GL_MAX_GEOMETRY_OUTPUT_COMPONENTS);
	SLOT(limits, PyObject, Limits_class_max_geometry_texture_image_units) = query_limit<int, 1>(gl, GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS);
	SLOT(limits, PyObject, Limits_class_max_geometry_uniform_blocks) = query_limit<int, 1>(gl, GL_MAX_GEOMETRY_UNIFORM_BLOCKS);
	SLOT(limits, PyObject, Limits_class_max_geometry_uniform_components) = query_limit<int, 1>(gl, GL_MAX_GEOMETRY_UNIFORM_COMPONENTS);
	SLOT(limits, PyObject, Limits_class_max_integer_samples) = query_limit<int, 1>(gl, GL_MAX_INTEGER_SAMPLES);
	SLOT(limits, PyObject, Limits_class_max_samples) = query_limit<int, 1>(gl, GL_MAX_SAMPLES);
	SLOT(limits, PyObject, Limits_class_max_rectangle_texture_size) = query_limit<int, 1>(gl, GL_MAX_RECTANGLE_TEXTURE_SIZE);
	SLOT(limits, PyObject, Limits_class_max_renderbuffer_size) = query_limit<int, 1>(gl, GL_MAX_RENDERBUFFER_SIZE);
	SLOT(limits, PyObject, Limits_class_max_sample_mask_words) = query_limit<int, 1>(gl, GL_MAX_SAMPLE_MASK_WORDS);
	SLOT(limits, PyObject, Limits_class_max_texture_buffer_size) = query_limit<int, 1>(gl, GL_MAX_TEXTURE_BUFFER_SIZE);
	SLOT(limits, PyObject, Limits_class_max_texture_image_units) = query_limit<int, 1>(gl, GL_MAX_TEXTURE_IMAGE_UNITS);
	SLOT(limits, PyObject, Limits_class_max_texture_lod_bias) = query_limit<int, 1>(gl, GL_MAX_TEXTURE_LOD_BIAS);
	SLOT(limits, PyObject, Limits_class_max_texture_size) = query_limit<int, 1>(gl, GL_MAX_TEXTURE_SIZE);
	SLOT(limits, PyObject, Limits_class_max_uniform_buffer_bindings) = query_limit<int, 1>(gl, GL_MAX_UNIFORM_BUFFER_BINDINGS);
	SLOT(limits, PyObject, Limits_class_max_uniform_block_size) = query_limit<int, 1>(gl, GL_MAX_UNIFORM_BLOCK_SIZE);
	SLOT(limits, PyObject, Limits_class_max_varying_components) = query_limit<int, 1>(gl, GL_MAX_VARYING_COMPONENTS);
	SLOT(limits, PyObject, Limits_class_max_varying_vectors) = query_limit<int, 1>(gl, GL_MAX_VARYING_VECTORS);
	SLOT(limits, PyObject, Limits_class_max_varying_floats) = query_limit<int, 1>(gl, GL_MAX_VARYING_FLOATS);
	SLOT(limits, PyObject, Limits_class_max_vertex_attribs) = query_limit<int, 1>(gl, GL_MAX_VERTEX_ATTRIBS);
	SLOT(limits, PyObject, Limits_class_max_vertex_texture_image_units) = query_limit<int, 1>(gl, GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
	SLOT(limits, PyObject, Limits_class_max_vertex_uniform_components) = query_limit<int, 1>(gl, GL_MAX_VERTEX_UNIFORM_COMPONENTS);
	SLOT(limits, PyObject, Limits_class_max_vertex_uniform_vectors) = query_limit<int, 1>(gl, GL_MAX_VERTEX_UNIFORM_VECTORS);
	SLOT(limits, PyObject, Limits_class_max_vertex_output_components) = query_limit<int, 1>(gl, GL_MAX_VERTEX_OUTPUT_COMPONENTS);
	SLOT(limits, PyObject, Limits_class_max_vertex_uniform_blocks) = query_limit<int, 1>(gl, GL_MAX_VERTEX_UNIFORM_BLOCKS);
	SLOT(limits, PyObject, Limits_class_max_vertex_attrib_relative_offset) = query_limit<int, 1>(gl, GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET);
	SLOT(limits, PyObject, Limits_class_max_vertex_attrib_bindings) = query_limit<int, 1>(gl, GL_MAX_VERTEX_ATTRIB_BINDINGS);

	SLOT(limits, PyObject, Limits_class_max_server_wait_timeout) = query_limit<long long, 1>(gl, GL_MAX_SERVER_WAIT_TIMEOUT);

	if (version_code >= 410) {
		SLOT(limits, PyObject, Limits_class_viewport_bounds_range) = query_limit<int, 2>(gl, GL_VIEWPORT_BOUNDS_RANGE);
		SLOT(limits, PyObject, Limits_class_viewport_subpixel_bits) = query_limit<int, 1>(gl, GL_VIEWPORT_SUBPIXEL_BITS);
		SLOT(limits, PyObject, Limits_class_max_viewports) = query_limit<int, 1>(gl, GL_MAX_VIEWPORTS);
	}

	if (version_code >= 420) {
		SLOT(limits, PyObject, Limits_class_min_map_buffer_alignment) = query_limit<int, 1>(gl, GL_MIN_MAP_BUFFER_ALIGNMENT);
		SLOT(limits, PyObject, Limits_class_max_combined_atomic_counters) = query_limit<int, 1>(gl, GL_MAX_COMBINED_ATOMIC_COUNTERS);
		SLOT(limits, PyObject, Limits_class_max_fragment_atomic_counters) = query_limit<int, 1>(gl, GL_MAX_FRAGMENT_ATOMIC_COUNTERS);
		SLOT(limits, PyObject, Limits_class_max_geometry_atomic_counters) = query_limit<int, 1>(gl, GL_MAX_GEOMETRY_ATOMIC_COUNTERS);
		SLOT(limits, PyObject, Limits_class_max_tess_control_atomic_counters) = query_limit<int, 1>(gl, GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS);
		SLOT(limits, PyObject, Limits_class_max_tess_evaluation_atomic_counters) = query_limit<int, 1>(gl, GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS);
		SLOT(limits, PyObject, Limits_class_max_vertex_atomic_counters) = query_limit<int, 1>(gl, GL_MAX_VERTEX_ATOMIC_COUNTERS);
	}

	if (version_code >= 430) {
		// int gl_max_compute_work_group_count[3] = {};
		// gl.GetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &gl_max_compute_work_group_count[0]);
		// gl.GetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &gl_max_compute_work_group_count[1]);
		// gl.GetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &gl_max_compute_work_group_count[2]);

		// int gl_max_compute_work_group_size[3] = {};
		// gl.GetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &gl_max_compute_work_group_size[0]);
		// gl.GetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &gl_max_compute_work_group_size[1]);
		// gl.GetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &gl_max_compute_work_group_size[2]);

		SLOT(limits, PyObject, Limits_class_max_shader_storage_buffer_bindings) = query_limit<int, 1>(gl, GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS);
		SLOT(limits, PyObject, Limits_class_max_combined_shader_storage_blocks) = query_limit<int, 1>(gl, GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS);
		SLOT(limits, PyObject, Limits_class_max_vertex_shader_storage_blocks) = query_limit<int, 1>(gl, GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS);
		SLOT(limits, PyObject, Limits_class_max_fragment_shader_storage_blocks) = query_limit<int, 1>(gl, GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS);
		SLOT(limits, PyObject, Limits_class_max_geometry_shader_storage_blocks) = query_limit<int, 1>(gl, GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS);
		SLOT(limits, PyObject, Limits_class_max_tess_evaluation_shader_storage_blocks) = query_limit<int, 1>(gl, GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS);
		SLOT(limits, PyObject, Limits_class_max_tess_control_shader_storage_blocks) = query_limit<int, 1>(gl, GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS);
		SLOT(limits, PyObject, Limits_class_max_compute_shader_storage_blocks) = query_limit<int, 1>(gl, GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS);
		SLOT(limits, PyObject, Limits_class_max_compute_uniform_components) = query_limit<int, 1>(gl, GL_MAX_COMPUTE_UNIFORM_COMPONENTS);
		SLOT(limits, PyObject, Limits_class_max_compute_atomic_counters) = query_limit<int, 1>(gl, GL_MAX_COMPUTE_ATOMIC_COUNTERS);
		SLOT(limits, PyObject, Limits_class_max_compute_atomic_counter_buffers) = query_limit<int, 1>(gl, GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS);
		SLOT(limits, PyObject, Limits_class_max_compute_work_group_invocations) = query_limit<int, 1>(gl, GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS);
		SLOT(limits, PyObject, Limits_class_max_compute_uniform_blocks) = query_limit<int, 1>(gl, GL_MAX_COMPUTE_UNIFORM_BLOCKS);
		SLOT(limits, PyObject, Limits_class_max_compute_texture_image_units) = query_limit<int, 1>(gl, GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS);
		SLOT(limits, PyObject, Limits_class_max_combined_compute_uniform_components) = query_limit<int, 1>(gl, GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS);
		SLOT(limits, PyObject, Limits_class_max_framebuffer_width) = query_limit<int, 1>(gl, GL_MAX_FRAMEBUFFER_WIDTH);
		SLOT(limits, PyObject, Limits_class_max_framebuffer_height) = query_limit<int, 1>(gl, GL_MAX_FRAMEBUFFER_HEIGHT);
		SLOT(limits, PyObject, Limits_class_max_framebuffer_layers) = query_limit<int, 1>(gl, GL_MAX_FRAMEBUFFER_LAYERS);
		SLOT(limits, PyObject, Limits_class_max_framebuffer_samples) = query_limit<int, 1>(gl, GL_MAX_FRAMEBUFFER_SAMPLES);
		SLOT(limits, PyObject, Limits_class_max_uniform_locations) = query_limit<int, 1>(gl, GL_MAX_UNIFORM_LOCATIONS);

		SLOT(limits, PyObject, Limits_class_max_element_index) = query_limit<long long, 1>(gl, GL_MAX_ELEMENT_INDEX);
		SLOT(limits, PyObject, Limits_class_max_shader_storage_block_size) = query_limit<long long, 1>(gl, GL_MAX_SHADER_STORAGE_BLOCK_SIZE);
	}

	return limits;
}
