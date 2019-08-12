#pragma once
#include "common.hpp"
#include "gl_methods.hpp"

extern "C" unsigned glGetError();
#define ERR printf("%s:%d %s (error=%x, before=%x)\n", __FILE__, __LINE__, __FUNCTION__, glGetError(), glGetError())

inline PyObject * _type_check(PyObject * obj, PyTypeObject * typ, const char * file, int line) {
    if (!obj) {
        fprintf(stderr, "null object %s:%d", file, line);
        exit(1);
    }
    if (Py_TYPE(obj) != typ) {
        fprintf(stderr, "type error at %s:%d", file, line);
        exit(1);
    }
    return obj;
}

#define cast(T, o) (T *)_type_check((PyObject *)o, T ## _type, __FILE__, __LINE__)

template <typename T>
inline T * _new_ref(T * x, const char * file, int line) {
    if (!x) {
        fprintf(stderr, "new_ref NULL %s:%d", file, line);
        exit(1);
    }
    Py_INCREF(x);
    return x;
}

#define new_ref(obj) _new_ref(obj, __FILE__, __LINE__)

PyObject * new_ref_or_none(void * obj) {
    if (obj) {
        Py_INCREF(obj);
        return (PyObject *)obj;
    }
    Py_RETURN_NONE;
}

inline bool is_buffer(PyObject * obj) {
    return obj && Py_TYPE(obj)->tp_as_buffer && Py_TYPE(obj)->tp_as_buffer->bf_getbuffer;
}

inline int py_floats(float * ptr, int min, int max, PyObject * tup) {
    PyObject * seq = PySequence_Fast(tup, "not iterable");
    if (!seq) {
        return -1;
    }
    int size = (int)PySequence_Fast_GET_SIZE(seq);
    if (size < min || size > max) {
        return -1;
    }
    for (int i = 0; i < size; ++i) {
        ptr[i] = (float)PyFloat_AsDouble(PySequence_Fast_GET_ITEM(seq, i));
    }
    Py_DECREF(seq);
    if (PyErr_Occurred()) {
        return -1;
    }
    return size;
}

inline int py_ints(int * ptr, int min, int max, PyObject * tup) {
    PyObject * seq = PySequence_Fast(tup, "not iterable");
    if (!seq) {
        return -1;
    }
    int size = (int)PySequence_Fast_GET_SIZE(seq);
    if (size < min || size > max) {
        return -1;
    }
    for (int i = 0; i < size; ++i) {
        ptr[i] = PyLong_AsLong(PySequence_Fast_GET_ITEM(seq, i));
    }
    Py_DECREF(seq);
    if (PyErr_Occurred()) {
        return -1;
    }
    return size;
}

inline int py_uints(unsigned * ptr, int min, int max, PyObject * tup) {
    PyObject * seq = PySequence_Fast(tup, "not iterable");
    if (!seq) {
        return -1;
    }
    int size = (int)PySequence_Fast_GET_SIZE(seq);
    if (size < min || size > max) {
        return -1;
    }
    for (int i = 0; i < size; ++i) {
        ptr[i] = PyLong_AsUnsignedLong(PySequence_Fast_GET_ITEM(seq, i));
    }
    Py_DECREF(seq);
    if (PyErr_Occurred()) {
        return -1;
    }
    return size;
}

inline void clean_glsl_name(char * name, int & name_len) {
    if (name_len && name[name_len - 1] == ']') {
        name_len -= 1;
        while (name_len && name[name_len] != '[') {
            name_len -= 1;
        }
    }
    name[name_len] = 0;
}

inline int swizzle_from_chr(char c) {
	switch (c) {
		case 'R':
		case 'r':
			return GL_RED;

		case 'G':
		case 'g':
			return GL_GREEN;

		case 'B':
		case 'b':
			return GL_BLUE;

		case 'A':
		case 'a':
			return GL_ALPHA;

		case '0':
			return GL_ZERO;

		case '1':
			return GL_ONE;
	}

	return -1;
}

inline char chr_from_swizzle(int c) {
	switch (c) {
		case GL_RED:
			return 'R';

		case GL_GREEN:
			return 'G';

		case GL_BLUE:
			return 'B';

		case GL_ALPHA:
			return 'A';

		case GL_ZERO:
			return '0';

		case GL_ONE:
			return '1';
	}

	return '?';
}

inline void set_uniform(const GLMethods & gl, int gltype, int location, int size, void * buffer) {
    switch (gltype) {
        case GL_BOOL: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_BOOL_VEC2: gl.Uniform2iv(location, size, (int *)buffer); break;
        case GL_BOOL_VEC3: gl.Uniform3iv(location, size, (int *)buffer); break;
        case GL_BOOL_VEC4: gl.Uniform4iv(location, size, (int *)buffer); break;
        case GL_INT: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_INT_VEC2: gl.Uniform2iv(location, size, (int *)buffer); break;
        case GL_INT_VEC3: gl.Uniform3iv(location, size, (int *)buffer); break;
        case GL_INT_VEC4: gl.Uniform4iv(location, size, (int *)buffer); break;
        case GL_UNSIGNED_INT: gl.Uniform1uiv(location, size, (unsigned *)buffer); break;
        case GL_UNSIGNED_INT_VEC2: gl.Uniform2uiv(location, size, (unsigned *)buffer); break;
        case GL_UNSIGNED_INT_VEC3: gl.Uniform3uiv(location, size, (unsigned *)buffer); break;
        case GL_UNSIGNED_INT_VEC4: gl.Uniform4uiv(location, size, (unsigned *)buffer); break;
        case GL_FLOAT: gl.Uniform1fv(location, size, (float *)buffer); break;
        case GL_FLOAT_VEC2: gl.Uniform2fv(location, size, (float *)buffer); break;
        case GL_FLOAT_VEC3: gl.Uniform3fv(location, size, (float *)buffer); break;
        case GL_FLOAT_VEC4: gl.Uniform4fv(location, size, (float *)buffer); break;
        case GL_DOUBLE: gl.Uniform1dv(location, size, (double *)buffer); break;
        case GL_DOUBLE_VEC2: gl.Uniform2dv(location, size, (double *)buffer); break;
        case GL_DOUBLE_VEC3: gl.Uniform3dv(location, size, (double *)buffer); break;
        case GL_DOUBLE_VEC4: gl.Uniform4dv(location, size, (double *)buffer); break;
        case GL_FLOAT_MAT2: gl.UniformMatrix2fv(location, size, false, (float *)buffer); break;
        case GL_FLOAT_MAT2x3: gl.UniformMatrix2x3fv(location, size, false, (float *)buffer); break;
        case GL_FLOAT_MAT2x4: gl.UniformMatrix2x4fv(location, size, false, (float *)buffer); break;
        case GL_FLOAT_MAT3x2: gl.UniformMatrix3x2fv(location, size, false, (float *)buffer); break;
        case GL_FLOAT_MAT3: gl.UniformMatrix3fv(location, size, false, (float *)buffer); break;
        case GL_FLOAT_MAT3x4: gl.UniformMatrix3x4fv(location, size, false, (float *)buffer); break;
        case GL_FLOAT_MAT4x2: gl.UniformMatrix4x2fv(location, size, false, (float *)buffer); break;
        case GL_FLOAT_MAT4x3: gl.UniformMatrix4x3fv(location, size, false, (float *)buffer); break;
        case GL_FLOAT_MAT4: gl.UniformMatrix4fv(location, size, false, (float *)buffer); break;
        case GL_DOUBLE_MAT2: gl.UniformMatrix2dv(location, size, false, (double *)buffer); break;
        case GL_DOUBLE_MAT2x3: gl.UniformMatrix2x3dv(location, size, false, (double *)buffer); break;
        case GL_DOUBLE_MAT2x4: gl.UniformMatrix2x4dv(location, size, false, (double *)buffer); break;
        case GL_DOUBLE_MAT3x2: gl.UniformMatrix3x2dv(location, size, false, (double *)buffer); break;
        case GL_DOUBLE_MAT3: gl.UniformMatrix3dv(location, size, false, (double *)buffer); break;
        case GL_DOUBLE_MAT3x4: gl.UniformMatrix3x4dv(location, size, false, (double *)buffer); break;
        case GL_DOUBLE_MAT4x2: gl.UniformMatrix4x2dv(location, size, false, (double *)buffer); break;
        case GL_DOUBLE_MAT4x3: gl.UniformMatrix4x3dv(location, size, false, (double *)buffer); break;
        case GL_DOUBLE_MAT4: gl.UniformMatrix4dv(location, size, false, (double *)buffer); break;
        case GL_SAMPLER_2D: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_SAMPLER_3D: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_SAMPLER_2D_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_SAMPLER_2D_SHADOW: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_SAMPLER_2D_MULTISAMPLE: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_SAMPLER_CUBE: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_SAMPLER_CUBE_SHADOW: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_SAMPLER_CUBE_MAP_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_IMAGE_1D: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_IMAGE_2D: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_IMAGE_3D: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_IMAGE_2D_RECT: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_IMAGE_CUBE: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_IMAGE_BUFFER: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_IMAGE_1D_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_IMAGE_2D_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_IMAGE_CUBE_MAP_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_IMAGE_2D_MULTISAMPLE: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_IMAGE_2D_MULTISAMPLE_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_INT_IMAGE_1D: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_INT_IMAGE_2D: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_INT_IMAGE_3D: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_INT_IMAGE_2D_RECT: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_INT_IMAGE_CUBE: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_INT_IMAGE_BUFFER: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_INT_IMAGE_1D_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_INT_IMAGE_2D_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_INT_IMAGE_CUBE_MAP_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_INT_IMAGE_2D_MULTISAMPLE: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_UNSIGNED_INT_IMAGE_1D: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_UNSIGNED_INT_IMAGE_2D: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_UNSIGNED_INT_IMAGE_3D: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_UNSIGNED_INT_IMAGE_2D_RECT: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_UNSIGNED_INT_IMAGE_CUBE: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_UNSIGNED_INT_IMAGE_BUFFER: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_UNSIGNED_INT_IMAGE_1D_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_UNSIGNED_INT_IMAGE_2D_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: gl.Uniform1iv(location, size, (int *)buffer); break;
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: gl.Uniform1iv(location, size, (int *)buffer); break;
    }
}

int get_int(const GLMethods & gl, int param) {
    int value = 0;
    gl.GetIntegerv(param, &value);
    return value;
}

float get_float(const GLMethods & gl, int param) {
    float value = 0;
    gl.GetFloatv(param, &value);
    return value;
}

PyObject * get_limits(const GLMethods & gl, int glversion) {
    PyObject * res = PyList_New(0);

    #define store(name, obj) PyList_Append(res, Py_BuildValue("sN", name, obj))
    #define store_int_limit(name) PyList_Append(res, Py_BuildValue("si", #name, get_int(gl, GL_ ## name)))
    #define store_float_limit(name) PyList_Append(res, Py_BuildValue("sf", #name, get_float(gl, GL_ ## name)))

    if (glversion >= 100) {
        store("VERSION", Py_BuildValue("s", gl.GetString(GL_VERSION)));
        store("RENDERER", Py_BuildValue("s", gl.GetString(GL_RENDERER)));
        store("VENDOR", Py_BuildValue("s", gl.GetString(GL_VENDOR)));

        float point_size[2] = {};
        gl.GetFloatv(GL_POINT_SIZE_RANGE, point_size);
        store("POINT_SIZE_RANGE", Py_BuildValue("ff", point_size[0], point_size[1]));

        int max_viewport[2] = {};
        gl.GetIntegerv(GL_MAX_VIEWPORT_DIMS, max_viewport);
        store("MAX_VIEWPORT_DIMS", Py_BuildValue("ii", max_viewport[0], max_viewport[1]));

        store_float_limit(POINT_SIZE_GRANULARITY);
        store_int_limit(MAX_TEXTURE_SIZE);
        store_int_limit(SUBPIXEL_BITS);
    }

    if (glversion >= 120) {
        float smooth_point_size[2] = {};
        gl.GetFloatv(GL_SMOOTH_POINT_SIZE_RANGE, smooth_point_size);
        store("SMOOTH_POINT_SIZE_RANGE", Py_BuildValue("ff", smooth_point_size[0], smooth_point_size[1]));

        float smooth_line_width[2] = {};
        gl.GetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, smooth_line_width);
        store("SMOOTH_LINE_WIDTH_RANGE", Py_BuildValue("ff", smooth_line_width[0], smooth_line_width[1]));

        float aliased_line_width[2] = {};
        gl.GetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, aliased_line_width);
        store("ALIASED_LINE_WIDTH_RANGE", Py_BuildValue("ff", aliased_line_width[0], aliased_line_width[1]));

        store_int_limit(MAX_3D_TEXTURE_SIZE);
        store_int_limit(MAX_ELEMENTS_VERTICES);
        store_int_limit(MAX_ELEMENTS_INDICES);
        store_float_limit(SMOOTH_POINT_SIZE_GRANULARITY);
        store_float_limit(SMOOTH_LINE_WIDTH_GRANULARITY);
    }

    if (glversion >= 130) {
        store_int_limit(MAX_CUBE_MAP_TEXTURE_SIZE);
    }

    if (glversion >= 140) {
        store_float_limit(POINT_FADE_THRESHOLD_SIZE);
        store_int_limit(MAX_TEXTURE_LOD_BIAS);
    }

    if (glversion >= 200) {
        store_int_limit(MAX_DRAW_BUFFERS);
        store_int_limit(MAX_VERTEX_ATTRIBS);
        store_int_limit(MAX_TEXTURE_IMAGE_UNITS);
        store_int_limit(MAX_FRAGMENT_UNIFORM_COMPONENTS);
        store_int_limit(MAX_VERTEX_UNIFORM_COMPONENTS);
        store_int_limit(MAX_COMBINED_TEXTURE_IMAGE_UNITS);
        store_int_limit(MAX_VERTEX_TEXTURE_IMAGE_UNITS);
    }

    if (glversion >= 200 && glversion < 440) {
        store_int_limit(MAX_VARYING_FLOATS);
    }

    if (glversion >= 300) {
        store_int_limit(MAX_CLIP_DISTANCES);
        store_int_limit(MAJOR_VERSION);
        store_int_limit(MINOR_VERSION);
        store_int_limit(MAX_ARRAY_TEXTURE_LAYERS);
        store_float_limit(MIN_PROGRAM_TEXEL_OFFSET);
        store_float_limit(MAX_PROGRAM_TEXEL_OFFSET);
        store_int_limit(MAX_RENDERBUFFER_SIZE);
        store_int_limit(MAX_COLOR_ATTACHMENTS);
        store_int_limit(MAX_SAMPLES);
    }

    if (glversion >= 300 && glversion < 440) {
        store_int_limit(MAX_VARYING_COMPONENTS);
    }

    if (glversion >= 310) {
        store_int_limit(MAX_TEXTURE_BUFFER_SIZE);
        store_int_limit(MAX_RECTANGLE_TEXTURE_SIZE);
        store_int_limit(MAX_VERTEX_UNIFORM_BLOCKS);
        store_int_limit(MAX_GEOMETRY_UNIFORM_BLOCKS);
        store_int_limit(MAX_FRAGMENT_UNIFORM_BLOCKS);
        store_int_limit(MAX_COMBINED_UNIFORM_BLOCKS);
        store_int_limit(MAX_UNIFORM_BUFFER_BINDINGS);
        store_int_limit(MAX_UNIFORM_BLOCK_SIZE);
        store_int_limit(MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS);
        store_int_limit(MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS);
        store_int_limit(MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS);
        store_int_limit(UNIFORM_BUFFER_OFFSET_ALIGNMENT);
    }

    if (glversion >= 320) {
        long long max_server_wait_timeout = 0;
        gl.GetInteger64v(GL_MAX_SERVER_WAIT_TIMEOUT, &max_server_wait_timeout);
        store("MAX_SERVER_WAIT_TIMEOUT", PyLong_FromLongLong(max_server_wait_timeout));

        store_int_limit(MAX_GEOMETRY_TEXTURE_IMAGE_UNITS);
        store_int_limit(MAX_GEOMETRY_UNIFORM_COMPONENTS);
        store_int_limit(MAX_GEOMETRY_OUTPUT_VERTICES);
        store_int_limit(MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS);
        store_int_limit(MAX_VERTEX_OUTPUT_COMPONENTS);
        store_int_limit(MAX_GEOMETRY_INPUT_COMPONENTS);
        store_int_limit(MAX_GEOMETRY_OUTPUT_COMPONENTS);
        store_int_limit(MAX_FRAGMENT_INPUT_COMPONENTS);
        store_int_limit(CONTEXT_PROFILE_MASK);
        store_int_limit(MAX_SAMPLE_MASK_WORDS);
        store_int_limit(MAX_COLOR_TEXTURE_SAMPLES);
        store_int_limit(MAX_DEPTH_TEXTURE_SAMPLES);
        store_int_limit(MAX_INTEGER_SAMPLES);
    }

    if (glversion >= 330) {
        store_int_limit(MAX_DUAL_SOURCE_DRAW_BUFFERS);
    }

    if (glversion >= 410) {
        int viewport_bounds[2] = {};
        gl.GetIntegerv(GL_VIEWPORT_BOUNDS_RANGE, viewport_bounds);
        store("VIEWPORT_BOUNDS_RANGE", Py_BuildValue("ii", viewport_bounds[0], viewport_bounds[1]));

        store_int_limit(MAX_VERTEX_UNIFORM_VECTORS);
        store_int_limit(MAX_VARYING_VECTORS);
        store_int_limit(MAX_FRAGMENT_UNIFORM_VECTORS);
        store_int_limit(MAX_VIEWPORTS);
        store_int_limit(VIEWPORT_SUBPIXEL_BITS);
    }

    if (glversion >= 420) {
        store_int_limit(MIN_MAP_BUFFER_ALIGNMENT);
        store_int_limit(MAX_VERTEX_ATOMIC_COUNTER_BUFFERS);
        store_int_limit(MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS);
        store_int_limit(MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS);
        store_int_limit(MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS);
        store_int_limit(MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS);
        store_int_limit(MAX_COMBINED_ATOMIC_COUNTER_BUFFERS);
        store_int_limit(MAX_VERTEX_ATOMIC_COUNTERS);
        store_int_limit(MAX_TESS_CONTROL_ATOMIC_COUNTERS);
        store_int_limit(MAX_TESS_EVALUATION_ATOMIC_COUNTERS);
        store_int_limit(MAX_GEOMETRY_ATOMIC_COUNTERS);
        store_int_limit(MAX_FRAGMENT_ATOMIC_COUNTERS);
        store_int_limit(MAX_COMBINED_ATOMIC_COUNTERS);
        store_int_limit(MAX_ATOMIC_COUNTER_BUFFER_SIZE);
        store_int_limit(MAX_ATOMIC_COUNTER_BUFFER_BINDINGS);
        store_int_limit(MAX_IMAGE_UNITS);
        store_int_limit(MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS);
        store_int_limit(MAX_IMAGE_SAMPLES);
        store_int_limit(MAX_VERTEX_IMAGE_UNIFORMS);
        store_int_limit(MAX_TESS_CONTROL_IMAGE_UNIFORMS);
        store_int_limit(MAX_TESS_EVALUATION_IMAGE_UNIFORMS);
        store_int_limit(MAX_GEOMETRY_IMAGE_UNIFORMS);
        store_int_limit(MAX_FRAGMENT_IMAGE_UNIFORMS);
        store_int_limit(MAX_COMBINED_IMAGE_UNIFORMS);
    }

    if (glversion >= 430) {
        int group_count[3] = {};
        gl.GetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &group_count[0]);
        gl.GetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &group_count[1]);
        gl.GetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &group_count[2]);
        store("MAX_COMPUTE_WORK_GROUP_COUNT", Py_BuildValue("iii", group_count[0], group_count[1], group_count[2]));

        int group_size[3] = {};
        gl.GetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &group_size[0]);
        gl.GetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &group_size[1]);
        gl.GetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &group_size[2]);
        store("MAX_COMPUTE_WORK_GROUP_SIZE", Py_BuildValue("iii", group_size[0], group_size[1], group_size[2]));

        long long max_element_index = 0;
        gl.GetInteger64v(GL_MAX_ELEMENT_INDEX, &max_element_index);
        store("MAX_ELEMENT_INDEX", PyLong_FromLongLong(max_element_index));

        long long max_shader_storage_block_size = 0;
        gl.GetInteger64v(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &max_shader_storage_block_size);
        store("MAX_SHADER_STORAGE_BLOCK_SIZE", PyLong_FromLongLong(max_shader_storage_block_size));

        store_int_limit(MAX_COMPUTE_UNIFORM_BLOCKS);
        store_int_limit(MAX_COMPUTE_TEXTURE_IMAGE_UNITS);
        store_int_limit(MAX_COMPUTE_IMAGE_UNIFORMS);
        store_int_limit(MAX_COMPUTE_SHARED_MEMORY_SIZE);
        store_int_limit(MAX_COMPUTE_UNIFORM_COMPONENTS);
        store_int_limit(MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS);
        store_int_limit(MAX_COMPUTE_ATOMIC_COUNTERS);
        store_int_limit(MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS);
        store_int_limit(MAX_COMPUTE_WORK_GROUP_INVOCATIONS);
        store_int_limit(MAX_DEBUG_MESSAGE_LENGTH);
        store_int_limit(MAX_DEBUG_LOGGED_MESSAGES);
        store_int_limit(MAX_DEBUG_GROUP_STACK_DEPTH);
        store_int_limit(MAX_LABEL_LENGTH);
        store_int_limit(MAX_UNIFORM_LOCATIONS);
        store_int_limit(MAX_FRAMEBUFFER_WIDTH);
        store_int_limit(MAX_FRAMEBUFFER_HEIGHT);
        store_int_limit(MAX_FRAMEBUFFER_LAYERS);
        store_int_limit(MAX_FRAMEBUFFER_SAMPLES);
        store_int_limit(MAX_VERTEX_SHADER_STORAGE_BLOCKS);
        store_int_limit(MAX_GEOMETRY_SHADER_STORAGE_BLOCKS);
        store_int_limit(MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS);
        store_int_limit(MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS);
        store_int_limit(MAX_FRAGMENT_SHADER_STORAGE_BLOCKS);
        store_int_limit(MAX_COMPUTE_SHADER_STORAGE_BLOCKS);
        store_int_limit(MAX_COMBINED_SHADER_STORAGE_BLOCKS);
        store_int_limit(MAX_SHADER_STORAGE_BUFFER_BINDINGS);
        store_int_limit(MAX_COMBINED_SHADER_OUTPUT_RESOURCES);
        store_int_limit(MAX_VERTEX_ATTRIB_RELATIVE_OFFSET);
        store_int_limit(MAX_VERTEX_ATTRIB_BINDINGS);
    }

    if (glversion >= 440) {
        store_int_limit(MAX_VERTEX_ATTRIB_STRIDE);
    }

    if (glversion >= 450) {
        store_float_limit(MAX_CULL_DISTANCES);
        store_float_limit(MAX_COMBINED_CLIP_AND_CULL_DISTANCES);
    }

    if (glversion >= 460) {
        store_float_limit(MAX_TEXTURE_MAX_ANISOTROPY);
    }

    #undef store_float_limit
    #undef store_int_limit
    #undef store

    for (int i = 0; i < PyList_GET_SIZE(res); ++i) {
        Py_DECREF(PyList_GET_ITEM(res, i));
    }
    return res;
}
