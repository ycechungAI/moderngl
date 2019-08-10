#include "extras.hpp"

void pack_f4(void ** ptr, PyObject *** it) {
    *(float *)(*ptr) = (float)PyFloat_AsDouble(*((*it)++));
    *ptr = (float *)(*ptr) + 1;
}

void pack_f2(void ** ptr, PyObject *** it) {
    if (float f = (float)PyFloat_AsDouble(*((*it)++))) {
    *(short *)(*ptr) = (((*(int *)&f & 0x7fffffff) >> 13) - 0x1c000) | ((*(int *)&f & 0x80000000) >> 16);
    } else {
        *(short *)(*ptr) = 0;
    }
    *ptr = (short *)(*ptr) + 1;
}

void pack_f1(void ** ptr, PyObject *** it) {
    *(unsigned char *)(*ptr) = (unsigned char)round(PyFloat_AsDouble(*((*it)++)) * 255.0);
    *ptr = (unsigned char *)(*ptr) + 1;
}

void pack_i4(void ** ptr, PyObject *** it) {
    *(int *)(*ptr) = PyLong_AsLong(*((*it)++));
    *ptr = (int *)(*ptr) + 1;
}

void pack_i2(void ** ptr, PyObject *** it) {
    *(short *)(*ptr) = (short)PyLong_AsLong(*((*it)++));
    *ptr = (short *)(*ptr) + 1;
}

void pack_i1(void ** ptr, PyObject *** it) {
    *(char *)(*ptr) = (char )PyLong_AsLong(*((*it)++));
    *ptr = (char *)(*ptr) + 1;
}

void pack_u4(void ** ptr, PyObject *** it) {
    *(unsigned *)(*ptr) = PyLong_AsUnsignedLong(*((*it)++));
    *ptr = (unsigned *)(*ptr) + 1;
}

void pack_u2(void ** ptr, PyObject *** it) {
    *(unsigned short *)(*ptr) = (unsigned short)PyLong_AsUnsignedLong(*((*it)++));
    *ptr = (unsigned short *)(*ptr) + 1;
}

void pack_u1(void ** ptr, PyObject *** it) {
    *(unsigned char *)(*ptr) = (unsigned char )PyLong_AsUnsignedLong(*((*it)++));
    *ptr = (unsigned char *)(*ptr) + 1;
}

void pack_x4(void ** ptr, PyObject *** it) {
    *(int *)(*ptr) = 0;
    *ptr = (int *)(*ptr) + 1;
}

void pack_x2(void ** ptr, PyObject *** it) {
    *(short *)(*ptr) = 0;
    *ptr = (short *)(*ptr) + 1;
}

void pack_x1(void ** ptr, PyObject *** it) {
    *(char *)(*ptr) = 0;
    *ptr = (char *)(*ptr) + 1;
}

typedef void (* pack_t)(void ** ptr, PyObject *** it);
const pack_t pack_f[5] = {NULL, pack_f1, pack_f2, NULL, pack_f4};
const pack_t pack_i[5] = {NULL, pack_i1, pack_i2, NULL, pack_i4};
const pack_t pack_u[5] = {NULL, pack_u1, pack_u2, NULL, pack_u4};
const pack_t pack_x[5] = {NULL, pack_x1, pack_x2, NULL, pack_x4};

PyObject * moderngl_meth_pack(PyObject * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"array", "layout", "stride", NULL};

    PyObject * array;
    const char * layout = "f4";
    int stride = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "O|si", kw, &array, &layout, &stride)) {
        return NULL;
    }

    int total_size = 0;
    pack_t packer[1024];
    int packers = 0;
    int readers = 0;
    int idx = 0;

    while (layout[idx]) {
        int count = 0;
        while ('0' <= layout[idx] && layout[idx] <= '9') {
            count = count * 10 + layout[idx] - '0';
            idx += 1;
        }
        const pack_t * proc = pack_f;
        switch (layout[idx]) {
            case 'f': proc = pack_f; idx += 1; break;
            case 'i': proc = pack_i; idx += 1; break;
            case 'u': proc = pack_u; idx += 1; break;
            case 'x': proc = pack_x; idx += 1; break;
        }
        int size = 4;
        if (layout[idx] == '1' || layout[idx] == '2' || layout[idx] == '4') {
            size = layout[idx] - '0';
            idx += 1;
        }
        count = count ? count : 1;
        total_size += size * count;
        for (int i = 0; i < count; ++i) {
            if (packers == 1024) {
                return NULL;
            }
            packer[packers++] = proc[size];
            if (proc != pack_x) {
                readers += 1;
            }
        }
        if (layout[idx] && layout[idx] != ' ') {
            return NULL;
        }
        if (layout[idx] == ' ') {
            idx += 1;
        }
    }

    const int skip = total_size < stride ? stride - total_size : 0;
    if (total_size < stride) {
        total_size = stride;
    }

    array = PySequence_Fast(array, "not iterable");
    PyObject ** it = PySequence_Fast_ITEMS(array);
    int count = (int)PySequence_Fast_GET_SIZE(array);
    if (count % readers) {
        PyErr_BadInternalCall();
        return NULL;
    }
    int rows = count / readers;
    PyObject * res = PyBytes_FromStringAndSize(NULL, rows * total_size);
    void * ptr = PyBytes_AS_STRING(res);
    memset(ptr, 0, rows * total_size);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < packers; ++j) {
            packer[j](&ptr, &it);
        }
        ptr = (char *)ptr + skip;
    }
    return res;
}

PyObject * math_meth_camera(PyObject * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"projection", "eye", "target", "up", "relative", NULL};

    float fov, ratio, znear, zfar;
    float e[3];
    float f[3] = {0.0f, 0.0f, 0.0f};
    float u[3] = {0.0f, 0.0f, 1.0f};
    int relative = false;

    int args_ok = PyArg_ParseTupleAndKeywords(
        args,
        kwa,
        "(ffff)(fff)|(fff)(fff)p",
        kw,
        &fov,
        &ratio,
        &znear,
        &zfar,
        &e[0], &e[1], &e[2],
        &f[0], &f[1], &f[2],
        &u[0], &u[1], &u[2],
        &relative
    );

    if (!args_ok) {
        return NULL;
    }

    if (!relative) {
        f[0] -= e[0];
        f[1] -= e[1];
        f[2] -= e[2];
    }

    float t0 = tanf(fov * 3.14159265f / 180.0f / 2.0f);
    float t1 = 1.0f / (ratio * t0);
    float t2 = 1.0f / t0;
    float t3 = -(zfar + znear) / (zfar - znear);
    float t4 = -1.0f;
    float t5 = -(2.0f * zfar * znear) / (zfar - znear);

    float l1 = sqrtf(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]);

    f[0] /= l1;
    f[1] /= l1;
    f[2] /= l1;

    float s[3] = {
        f[1] * u[2] - u[1] * f[2],
        f[2] * u[0] - u[2] * f[0],
        f[0] * u[1] - u[0] * f[1],
    };

    float l2 = sqrtf(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);

    s[0] /= l2;
    s[1] /= l2;
    s[2] /= l2;

    u[0] = s[1] * f[2] - f[1] * s[2];
    u[1] = s[2] * f[0] - f[2] * s[0];
    u[2] = s[0] * f[1] - f[0] * s[1];

    float dx = -(s[0] * e[0] + s[1] * e[1] + s[2] * e[2]);
    float dy = -(u[0] * e[0] + u[1] * e[1] + u[2] * e[2]);
    float dz = f[0] * e[0] + f[1] * e[1] + f[2] * e[2];

    PyObject * res = PyBytes_FromStringAndSize(NULL, 64);
    float * ptr = (float *)PyBytes_AS_STRING(res);
    ptr[0] = t1 * s[0];
    ptr[1] = t2 * u[0];
    ptr[2] = -t3 * f[0];
    ptr[3] = -t4 * f[0];
    ptr[4] = t1 * s[1];
    ptr[5] = t2 * u[1];
    ptr[6] = -t3 * f[1];
    ptr[7] = -t4 * f[1];
    ptr[8] = t1 * s[2];
    ptr[9] = t2 * u[2];
    ptr[10] = -t3 * f[2];
    ptr[11] = -t4 * f[2];
    ptr[12] = t1 * dx;
    ptr[13] = t2 * dy;
    ptr[14] = t3 * dz + t5;
    ptr[15] = t4 * dz;
    return res;
}

PyObject * math_meth_transform(PyObject * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"vector", "position", "rotation", NULL};

    float v[3];
    float p[3] = {0.0f, 0.0f, 0.0f};
    float q[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    int args_ok = PyArg_ParseTupleAndKeywords(
        args,
        kwa,
        "(fff)|(fff)(ffff)",
        kw,
        &v[0], &v[1], &v[2],
        &p[0], &p[1], &p[2],
        &q[0], &q[1], &q[2], &q[3]
    );

    if (!args_ok) {
        return NULL;
    }

    float a[3] = {
        v[1] * q[2] - q[1] * v[2] + q[3] * v[0],
        v[2] * q[0] - q[2] * v[0] + q[3] * v[1],
        v[0] * q[1] - q[0] * v[1] + q[3] * v[2],
    };

    float r[3] = {
        p[0] + v[0] + (a[1] * q[2] - q[1] * a[2]) * 2.0f,
        p[1] + v[1] + (a[2] * q[0] - q[2] * a[0]) * 2.0f,
        p[2] + v[2] + (a[0] * q[1] - q[0] * a[1]) * 2.0f,
    };

    return Py_BuildValue("fff", r[0], r[1], r[2]);
}

PyObject * math_meth_quat(PyObject * self, PyObject * args) {
    if (PyTuple_Size(args) == 1) {
        return math_meth_quat(self, PyTuple_GetItem(args, 0));
    }

    if (PyTuple_Size(args) == 4) {
        float q[4];
        if (!PyArg_ParseTuple(args, "ffff", &q[0], &q[1], &q[2], &q[3])) {
            return NULL;
        }
        return PyBytes_FromStringAndSize((char *)q, 16);
    }

    float m[9];
    if (!PyArg_ParseTuple(args, "fffffffff", &m[0], &m[1], &m[2], &m[3], &m[4], &m[5], &m[6], &m[7], &m[8])) {
        return NULL;
    }

    int i = 0;
    float b = m[0] + m[4] + m[8];
    if (m[0] - m[4] - m[8] > b) {
        b = m[0] - m[4] - m[8];
        i = 1;
    }
    if (m[4] - m[0] - m[8] > b) {
        b = m[4] - m[0] - m[8];
        i = 2;
    }
    if (m[8] - m[0] - m[4] > b) {
        b = m[8] - m[0] - m[4];
        i = 3;
    }

    b = sqrtf(b + 1.0f) * 0.5f;
    float k = 0.25f / b;

    PyObject * res = PyBytes_FromStringAndSize(NULL, 16);
    float * r = (float *)PyBytes_AS_STRING(res);
    switch (i) {
        case 0:
            r[0] = b;
            r[1] = (m[5] - m[7]) * k;
            r[2] = (m[6] - m[2]) * k;
            r[3] = (m[1] - m[3]) * k;
            break;
        case 1:
            r[0] = (m[5] - m[7]) * k;
            r[1] = b;
            r[2] = (m[1] + m[3]) * k;
            r[3] = (m[6] + m[2]) * k;
            break;
        case 2:
            r[0] = (m[6] - m[2]) * k;
            r[1] = (m[1] + m[3]) * k;
            r[2] = b;
            r[3] = (m[5] + m[7]) * k;
            break;
        case 3:
            r[0] = (m[1] - m[3]) * k;
            r[1] = (m[6] + m[2]) * k;
            r[2] = (m[5] + m[7]) * k;
            r[3] = b;
            break;
    }
    return res;
}

PyObject * math_meth_mat(PyObject * self, PyObject * args) {
    if (PyTuple_Size(args) == 1) {
        return math_meth_mat(self, PyTuple_GetItem(args, 0));
    }

    if (PyTuple_Size(args) == 4) {
        float q[4];
        if (!PyArg_ParseTuple(args, "ffff", &q[0], &q[1], &q[2], &q[3])) {
            return NULL;
        }

        PyObject * res = PyBytes_FromStringAndSize(NULL, 36);
        float * r = (float *)PyBytes_AS_STRING(res);
        r[0] = 1.0f - 2.0f * (q[1] * q[1] + q[2] * q[2]);
        r[1] = 2.0f * (q[0] * q[1] + q[3] * q[2]);
        r[2] = 2.0f * (q[0] * q[2] - q[3] * q[1]);
        r[3] = 2.0f * (q[0] * q[1] - q[3] * q[2]);
        r[4] = 1.0f - 2.0f * (q[0] * q[0] + q[2] * q[2]);
        r[5] = 2.0f * (q[1] * q[2] + q[3] * q[0]);
        r[6] = 2.0f * (q[0] * q[2] + q[3] * q[1]);
        r[7] = 2.0f * (q[1] * q[2] - q[3] * q[0]);
        r[8] = 1.0f - 2.0f * (q[0] * q[0] + q[1] * q[1]);
        return res;
    }

    float m[9];
    if (!PyArg_ParseTuple(args, "fffffffff", &m[0], &m[1], &m[2], &m[3], &m[4], &m[5], &m[6], &m[7], &m[8])) {
        return NULL;
    }

    return PyBytes_FromStringAndSize((char *)m, 36);
}

PyMethodDef math_module_methods[] = {
    {"camera", (PyCFunction)math_meth_camera, METH_VARARGS | METH_KEYWORDS, NULL},
    {"transform", (PyCFunction)math_meth_transform, METH_VARARGS | METH_KEYWORDS, NULL},
    {"quat", (PyCFunction)math_meth_quat, METH_VARARGS | METH_KEYWORDS, NULL},
    {"mat", (PyCFunction)math_meth_mat, METH_VARARGS | METH_KEYWORDS, NULL},
    {},
};

PyModuleDef math_module_def = {PyModuleDef_HEAD_INIT, "moderngl.math", NULL, -1, math_module_methods};

extern "C" PyObject * PyInit_moderngl_math() {
    PyObject * module = PyModule_Create(&math_module_def);
    return module;
}
