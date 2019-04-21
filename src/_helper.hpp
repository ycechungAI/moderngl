// Temporary helper functions. TODO: organized later

#include "Python.hpp"

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
