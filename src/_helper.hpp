// Temporary helper functions. TODO: organized later

#include "Python.hpp"

typedef void (* read_value)(void *& ptr, PyObject * value);

static PyObject * get_wrapper_class(const char * name) {
    static PyObject * sys = PyImport_ImportModule("sys");
    if (!sys) {
        printf("no sys\n");
        exit(0);
    }
    static PyObject * sys_modules = PyObject_GetAttrString(sys, "modules");
    if (!sys_modules) {
        printf("no sys_modules\n");
        exit(0);
    }
    static PyObject * moderngl = PyDict_GetItemString(sys_modules, "moderngl");
    if (!moderngl) {
        printf("no moderngl\n");
        exit(0);
    }
    PyObject * wrapper = PyObject_GetAttrString(moderngl, name);
    if (!wrapper) {
        printf("no wrapper\n");
        exit(0);
    }
    if (!PyType_HasFeature(Py_TYPE(wrapper), Py_TPFLAGS_TYPE_SUBCLASS)) {
        printf("not a class\n");
        exit(0);
    }
    return wrapper;
}

static PyObject * create_wrapper(const char * name) {
    return PyObject_CallFunction(get_wrapper_class(name), NULL);
}

static void set_slot(PyObject * obj, const char * slot, PyObject * value) {
    PyObject_SetAttrString(obj, slot, value);
    Py_DECREF(value);
}

static PyObject * get_slot(PyObject * obj, const char * slot) {
    PyObject * value = PyObject_GetAttrString(obj, slot);
    if (!value) {
        printf("missing slot %s.%s\n", Py_TYPE(obj)->tp_name, slot);
        exit(0);
    }
    Py_DECREF(value);
    return value;
}

static void set_item(PyObject * dict, const char * slot, PyObject * value) {
    PyDict_SetItemString(dict, slot, value);
    Py_DECREF(value);
}

struct GLTypeInfo {
    int cols;
    int rows;
    int shape;
};

static GLTypeInfo type_info(int type) {
    GLTypeInfo info = {};

    int & cols = info.cols;
    int & rows = info.rows;
    int & shape = info.shape;

    switch (type) {
        case GL_FLOAT: cols = 1; rows = 1; shape = 'f'; break;
        case GL_FLOAT_VEC2: cols = 2; rows = 1; shape = 'f'; break;
        case GL_FLOAT_VEC3: cols = 3; rows = 1; shape = 'f'; break;
        case GL_FLOAT_VEC4: cols = 4; rows = 1; shape = 'f'; break;
        case GL_DOUBLE: cols = 1; rows = 1; shape = 'd'; break;
        case GL_DOUBLE_VEC2: cols = 2; rows = 1; shape = 'd'; break;
        case GL_DOUBLE_VEC3: cols = 3; rows = 1; shape = 'd'; break;
        case GL_DOUBLE_VEC4: cols = 4; rows = 1; shape = 'd'; break;
        case GL_INT: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_VEC2: cols = 2; rows = 1; shape = 'i'; break;
        case GL_INT_VEC3: cols = 3; rows = 1; shape = 'i'; break;
        case GL_INT_VEC4: cols = 4; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT: cols = 1; rows = 1; shape = 'u'; break;
        case GL_UNSIGNED_INT_VEC2: cols = 2; rows = 1; shape = 'u'; break;
        case GL_UNSIGNED_INT_VEC3: cols = 3; rows = 1; shape = 'u'; break;
        case GL_UNSIGNED_INT_VEC4: cols = 4; rows = 1; shape = 'u'; break;
        case GL_BOOL: cols = 1; rows = 1; shape = 'p'; break;
        case GL_BOOL_VEC2: cols = 2; rows = 1; shape = 'p'; break;
        case GL_BOOL_VEC3: cols = 3; rows = 1; shape = 'p'; break;
        case GL_BOOL_VEC4: cols = 4; rows = 1; shape = 'p'; break;
        case GL_FLOAT_MAT2: cols = 2; rows = 2; shape = 'f'; break;
        case GL_FLOAT_MAT3: cols = 3; rows = 3; shape = 'f'; break;
        case GL_FLOAT_MAT4: cols = 4; rows = 4; shape = 'f'; break;
        case GL_FLOAT_MAT2x3: cols = 3; rows = 2; shape = 'f'; break;
        case GL_FLOAT_MAT2x4: cols = 4; rows = 2; shape = 'f'; break;
        case GL_FLOAT_MAT3x2: cols = 2; rows = 3; shape = 'f'; break;
        case GL_FLOAT_MAT3x4: cols = 4; rows = 3; shape = 'f'; break;
        case GL_FLOAT_MAT4x2: cols = 2; rows = 4; shape = 'f'; break;
        case GL_FLOAT_MAT4x3: cols = 3; rows = 4; shape = 'f'; break;
        case GL_DOUBLE_MAT2: cols = 2; rows = 2; shape = 'd'; break;
        case GL_DOUBLE_MAT3: cols = 3; rows = 3; shape = 'd'; break;
        case GL_DOUBLE_MAT4: cols = 4; rows = 4; shape = 'd'; break;
        case GL_DOUBLE_MAT2x3: cols = 3; rows = 2; shape = 'd'; break;
        case GL_DOUBLE_MAT2x4: cols = 4; rows = 2; shape = 'd'; break;
        case GL_DOUBLE_MAT3x2: cols = 2; rows = 3; shape = 'd'; break;
        case GL_DOUBLE_MAT3x4: cols = 4; rows = 3; shape = 'd'; break;
        case GL_DOUBLE_MAT4x2: cols = 2; rows = 4; shape = 'd'; break;
        case GL_DOUBLE_MAT4x3: cols = 3; rows = 4; shape = 'd'; break;
        case GL_SAMPLER_1D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_2D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_3D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_CUBE: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_1D_SHADOW: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_2D_SHADOW: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_1D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_2D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_1D_ARRAY_SHADOW: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_2D_ARRAY_SHADOW: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_2D_MULTISAMPLE: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_CUBE_SHADOW: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_BUFFER: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_2D_RECT: cols = 1; rows = 1; shape = 'i'; break;
        case GL_SAMPLER_2D_RECT_SHADOW: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_SAMPLER_1D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_SAMPLER_2D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_SAMPLER_3D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_SAMPLER_CUBE: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_SAMPLER_1D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_SAMPLER_2D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_SAMPLER_2D_MULTISAMPLE: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_SAMPLER_BUFFER: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_SAMPLER_2D_RECT: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_SAMPLER_1D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_SAMPLER_2D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_SAMPLER_3D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_SAMPLER_CUBE: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_SAMPLER_BUFFER: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_SAMPLER_2D_RECT: cols = 1; rows = 1; shape = 'i'; break;
        case GL_IMAGE_1D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_IMAGE_2D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_IMAGE_3D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_IMAGE_2D_RECT: cols = 1; rows = 1; shape = 'i'; break;
        case GL_IMAGE_CUBE: cols = 1; rows = 1; shape = 'i'; break;
        case GL_IMAGE_BUFFER: cols = 1; rows = 1; shape = 'i'; break;
        case GL_IMAGE_1D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_IMAGE_2D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_IMAGE_2D_MULTISAMPLE: cols = 1; rows = 1; shape = 'i'; break;
        case GL_IMAGE_2D_MULTISAMPLE_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_IMAGE_1D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_IMAGE_2D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_IMAGE_3D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_IMAGE_2D_RECT: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_IMAGE_CUBE: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_IMAGE_BUFFER: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_IMAGE_1D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_IMAGE_2D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_IMAGE_2D_MULTISAMPLE: cols = 1; rows = 1; shape = 'i'; break;
        case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_IMAGE_1D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_IMAGE_2D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_IMAGE_3D: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_IMAGE_2D_RECT: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_IMAGE_CUBE: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_IMAGE_BUFFER: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_IMAGE_1D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_IMAGE_2D_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: cols = 1; rows = 1; shape = 'i'; break;
        case GL_UNSIGNED_INT_ATOMIC_COUNTER: cols = 1; rows = 1; shape = 'i'; break;
    }

    return info;
}

static void write_uni(const GLMethods & gl, int location, int type, int size, void * ptr) {
    switch (type) {
        case GL_BOOL: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_BOOL_VEC2: gl.Uniform2iv(location, size, (int *)ptr); break;
        case GL_BOOL_VEC3: gl.Uniform3iv(location, size, (int *)ptr); break;
        case GL_BOOL_VEC4: gl.Uniform4iv(location, size, (int *)ptr); break;
        case GL_INT: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_INT_VEC2: gl.Uniform2iv(location, size, (int *)ptr); break;
        case GL_INT_VEC3: gl.Uniform3iv(location, size, (int *)ptr); break;
        case GL_INT_VEC4: gl.Uniform4iv(location, size, (int *)ptr); break;
        case GL_UNSIGNED_INT: gl.Uniform1uiv(location, size, (unsigned *)ptr); break;
        case GL_UNSIGNED_INT_VEC2: gl.Uniform2uiv(location, size, (unsigned *)ptr); break;
        case GL_UNSIGNED_INT_VEC3: gl.Uniform3uiv(location, size, (unsigned *)ptr); break;
        case GL_UNSIGNED_INT_VEC4: gl.Uniform4uiv(location, size, (unsigned *)ptr); break;
        case GL_FLOAT: gl.Uniform1fv(location, size, (float *)ptr); break;
        case GL_FLOAT_VEC2: gl.Uniform2fv(location, size, (float *)ptr); break;
        case GL_FLOAT_VEC3: gl.Uniform3fv(location, size, (float *)ptr); break;
        case GL_FLOAT_VEC4: gl.Uniform4fv(location, size, (float *)ptr); break;
        case GL_DOUBLE: gl.Uniform1dv(location, size, (double *)ptr); break;
        case GL_DOUBLE_VEC2: gl.Uniform2dv(location, size, (double *)ptr); break;
        case GL_DOUBLE_VEC3: gl.Uniform3dv(location, size, (double *)ptr); break;
        case GL_DOUBLE_VEC4: gl.Uniform4dv(location, size, (double *)ptr); break;
        case GL_FLOAT_MAT2: gl.UniformMatrix2fv(location, size, false, (float *)ptr); break;
        case GL_FLOAT_MAT2x3: gl.UniformMatrix2x3fv(location, size, false, (float *)ptr); break;
        case GL_FLOAT_MAT2x4: gl.UniformMatrix2x4fv(location, size, false, (float *)ptr); break;
        case GL_FLOAT_MAT3x2: gl.UniformMatrix3x2fv(location, size, false, (float *)ptr); break;
        case GL_FLOAT_MAT3: gl.UniformMatrix3fv(location, size, false, (float *)ptr); break;
        case GL_FLOAT_MAT3x4: gl.UniformMatrix3x4fv(location, size, false, (float *)ptr); break;
        case GL_FLOAT_MAT4x2: gl.UniformMatrix4x2fv(location, size, false, (float *)ptr); break;
        case GL_FLOAT_MAT4x3: gl.UniformMatrix4x3fv(location, size, false, (float *)ptr); break;
        case GL_FLOAT_MAT4: gl.UniformMatrix4fv(location, size, false, (float *)ptr); break;
        case GL_DOUBLE_MAT2: gl.UniformMatrix2dv(location, size, false, (double *)ptr); break;
        case GL_DOUBLE_MAT2x3: gl.UniformMatrix2x3dv(location, size, false, (double *)ptr); break;
        case GL_DOUBLE_MAT2x4: gl.UniformMatrix2x4dv(location, size, false, (double *)ptr); break;
        case GL_DOUBLE_MAT3x2: gl.UniformMatrix3x2dv(location, size, false, (double *)ptr); break;
        case GL_DOUBLE_MAT3: gl.UniformMatrix3dv(location, size, false, (double *)ptr); break;
        case GL_DOUBLE_MAT3x4: gl.UniformMatrix3x4dv(location, size, false, (double *)ptr); break;
        case GL_DOUBLE_MAT4x2: gl.UniformMatrix4x2dv(location, size, false, (double *)ptr); break;
        case GL_DOUBLE_MAT4x3: gl.UniformMatrix4x3dv(location, size, false, (double *)ptr); break;
        case GL_DOUBLE_MAT4: gl.UniformMatrix4dv(location, size, false, (double *)ptr); break;
        case GL_SAMPLER_2D: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_SAMPLER_3D: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_SAMPLER_2D_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_SAMPLER_2D_SHADOW: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_SAMPLER_2D_MULTISAMPLE: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_SAMPLER_CUBE: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_SAMPLER_CUBE_SHADOW: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_SAMPLER_CUBE_MAP_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_IMAGE_1D: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_IMAGE_2D: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_IMAGE_3D: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_IMAGE_2D_RECT: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_IMAGE_CUBE: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_IMAGE_BUFFER: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_IMAGE_1D_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_IMAGE_2D_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_IMAGE_CUBE_MAP_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_IMAGE_2D_MULTISAMPLE: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_IMAGE_2D_MULTISAMPLE_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_INT_IMAGE_1D: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_INT_IMAGE_2D: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_INT_IMAGE_3D: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_INT_IMAGE_2D_RECT: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_INT_IMAGE_CUBE: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_INT_IMAGE_BUFFER: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_INT_IMAGE_1D_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_INT_IMAGE_2D_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_INT_IMAGE_CUBE_MAP_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_INT_IMAGE_2D_MULTISAMPLE: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_UNSIGNED_INT_IMAGE_1D: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_UNSIGNED_INT_IMAGE_2D: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_UNSIGNED_INT_IMAGE_3D: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_UNSIGNED_INT_IMAGE_2D_RECT: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_UNSIGNED_INT_IMAGE_CUBE: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_UNSIGNED_INT_IMAGE_BUFFER: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_UNSIGNED_INT_IMAGE_1D_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_UNSIGNED_INT_IMAGE_2D_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: gl.Uniform1iv(location, size, (int *)ptr); break;
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: gl.Uniform1iv(location, size, (int *)ptr); break;
    }
}

static void read_float(float *& ptr, PyObject * value) {
    *ptr++ = (float)PyFloat_AsDouble(value);
}

static void read_int(int *& ptr, PyObject * value) {
    *ptr++ = PyLong_AsLong(value);
}

static void read_unsigned(unsigned *& ptr, PyObject * value) {
    *ptr++ = PyLong_AsUnsignedLong(value);
}

static void read_double(double *& ptr, PyObject * value) {
    *ptr++ = PyFloat_AsDouble(value);
}

static void read_bool(int *& ptr, PyObject * value) {
    *ptr++ = PyObject_IsTrue(value);
}

static PyObject * getset_uniform(const GLMethods & gl, int program_obj, PyObject * uniform, PyObject * value) {
    if (!value) {
        int location = PyLong_AsLong(get_slot(uniform, "new_location"));
        int shape = PyLong_AsLong(get_slot(uniform, "new_shape"));

        if (shape) {
            int cols = PyLong_AsLong(get_slot(uniform, "new_cols"));
            int rows = PyLong_AsLong(get_slot(uniform, "new_rows"));
            int size = PyLong_AsLong(get_slot(uniform, "new_size"));

            PyObject * result = PyBytes_FromStringAndSize(0, cols * rows * size * (shape == 'd' ? 8 : 4));
            char * data = PyBytes_AS_STRING(result);

            switch (shape) {
                case 'f': gl.GetUniformfv(program_obj, location, (float *)data); break;
                case 'i': gl.GetUniformiv(program_obj, location, (int *)data); break;
                case 'u': gl.GetUniformuiv(program_obj, location, (unsigned *)data); break;
                case 'd': gl.GetUniformdv(program_obj, location, (double *)data); break;
                case 'p': gl.GetUniformiv(program_obj, location, (int *)data); break;
                default:
                    return 0;
            }

            return result;
        } else {
            int binding = -1;
            gl.GetActiveUniformBlockiv(program_obj, location, GL_UNIFORM_BLOCK_BINDING, &binding);
            return PyLong_FromLong(binding);
        }
    }

    if (uniform == Py_None) {
        Py_RETURN_NONE;
    }

    int location = PyLong_AsLong(get_slot(uniform, "new_location"));
    int shape = PyLong_AsLong(get_slot(uniform, "new_shape"));
    int type = PyLong_AsLong(get_slot(uniform, "new_type"));

    if (shape) {
        int cols = PyLong_AsLong(get_slot(uniform, "new_cols"));
        int rows = PyLong_AsLong(get_slot(uniform, "new_rows"));
        int size = PyLong_AsLong(get_slot(uniform, "new_size"));

        if (value->ob_type->tp_as_buffer && value->ob_type->tp_as_buffer->bf_getbuffer) {
            Py_buffer view = {};
            if (value->ob_type->tp_as_buffer->bf_getbuffer(value, &view, PyBUF_STRIDED_RO) < 0) {
                return 0;
            }
            if (PyBuffer_IsContiguous(&view, 'C')) {
                write_uni(gl, location, type, size, view.buf);
            } else {
                void * cache = malloc(view.len);
                PyBuffer_ToContiguous(cache, &view, view.len, 'C');
                write_uni(gl, location, type, size, cache);
                free(cache);
            }
            PyBuffer_Release(&view);
        } else {
            read_value func;
            switch (shape) {
                case 'f': func = (read_value)read_float; break;
                case 'i': func = (read_value)read_int; break;
                case 'u': func = (read_value)read_unsigned; break;
                case 'd': func = (read_value)read_double; break;
                case 'p': func = (read_value)read_bool; break;
                default:
                    return 0;
            }
            if (size == 1 && rows == 1 && cols == 1) {
                char cache[8];
                void * ptr = cache;
                func(ptr, value);
                if (PyErr_Occurred()) {
                    return 0;
                }
                write_uni(gl, location, type, size, cache);
            } else {
                PyObject * seq = PySequence_Fast(value, "fail");
                if (!seq) {
                    return 0;
                }
                int seq_len = (int)PySequence_Fast_GET_SIZE(seq);
                void * cache = malloc(seq_len * (shape == 'd' ? 8 : 4));
                void * ptr = cache;
                for (int i = 0; i < seq_len; ++i) {
                    func(ptr, PySequence_Fast_GET_ITEM(seq, i));
                }
                if (PyErr_Occurred()) {
                    Py_DECREF(seq);
                    free(cache);
                    return 0;
                }
                write_uni(gl, location, type, size, cache);
                Py_DECREF(seq);
                free(cache);
            }
        }
    } else {
        int binding = PyLong_AsLong(value);

        if (PyErr_Occurred()) {
            return 0;
        }

        gl.UniformBlockBinding(program_obj, location, binding);
    }

    Py_RETURN_NONE;
}
