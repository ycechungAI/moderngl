#include "sampler.hpp"
#include "context.hpp"
#include "texture.hpp"

#include "internal/wrapper.hpp"

#include "internal/compare_func.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

int MGLSampler_set_filter2(MGLSampler * self, PyObject * value);
int MGLSampler_set_wrap2(MGLSampler * self, PyObject * value);
int MGLSampler_set_anisotropy2(MGLSampler * self, PyObject * value);
int MGLSampler_set_compare_func2(MGLSampler * self, PyObject * value);
int MGLSampler_set_lod_range2(MGLSampler * self, PyObject * value);
int MGLSampler_set_lod_bias2(MGLSampler * self, PyObject * value);
int MGLSampler_set_border2(MGLSampler * self, PyObject * value);
int MGLSampler_set_texture2(MGLSampler * self, PyObject * value);

/* MGLContext.sampler(texture)
 */
PyObject * MGLContext_meth_sampler(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(8);

    MGLSampler * sampler = PyObject_New(MGLSampler, MGLSampler_class);
    chain_objects(self, sampler);
    sampler->context = self;

    const GLMethods & gl = self->gl;
    gl.GenSamplers(1, (GLuint *)&sampler->sampler_obj);

    if (!sampler->sampler_obj) {
        PyErr_Format(moderngl_error, "cannot create sampler");
        Py_DECREF(sampler);
        return 0;
    }

    sampler->slots.anisotropy = new_ref(Py_None);
    sampler->slots.border = new_ref(Py_None);
    sampler->slots.compare_func = new_ref(Py_None);
    sampler->slots.filter = new_ref(Py_None);
    sampler->slots.lod_bias = new_ref(Py_None);
    sampler->slots.lod_range = new_ref(Py_None);
    sampler->slots.texture = new_ref(Py_None);
    sampler->slots.wrap = new_ref(Py_None);

    if (MGLSampler_set_texture2(sampler, args[0])) {
        Py_DECREF(sampler);
        return 0;
    }

    if (MGLSampler_set_filter2(sampler, args[1])) {
        Py_DECREF(sampler);
        return 0;
    }

    if (MGLSampler_set_wrap2(sampler, args[2])) {
        Py_DECREF(sampler);
        return 0;
    }

    if (MGLSampler_set_compare_func2(sampler, args[3])) {
        Py_DECREF(sampler);
        return 0;
    }

    if (args[4] != Py_None) {
        if (MGLSampler_set_anisotropy2(sampler, args[4])) {
            Py_DECREF(sampler);
            return 0;
        }
    } else {
        sampler->slots.anisotropy = PyFloat_FromDouble(1.0);
    }

    if (args[5] != Py_None) {
        if (MGLSampler_set_lod_range2(sampler, args[5])) {
            Py_DECREF(sampler);
            return 0;
        }
    } else {
        sampler->slots.lod_range = Py_BuildValue("ii", -1000, 1000);
    }

    if (args[6] != Py_None) {
        if (MGLSampler_set_lod_bias2(sampler, args[6])) {
            Py_DECREF(sampler);
            return 0;
        }
    } else {
        sampler->slots.lod_bias = PyFloat_FromDouble(0.0);
    }

    if (args[7] != Py_None) {
        if (MGLSampler_set_border2(sampler, args[7])) {
            Py_DECREF(sampler);
            return 0;
        }
    } else {
        sampler->slots.border = Py_BuildValue("ffff", 0.0, 0.0, 0.0, 0.0);
    }

    sampler->wrapper = Sampler_New("O", sampler);
    return sampler->wrapper;
}

/* MGLSampler.use(location)
 */
PyObject * MGLSampler_meth_use(MGLSampler * self, PyObject * arg) {
    int location = PyLong_AsLong(arg);
    if (PyErr_Occurred()) {
        PyErr_Format(moderngl_error, "invalid location");
        return NULL;
    }
    self->context->bind_sampler(location, self->texture->texture_target, self->texture->texture_obj, self->sampler_obj);
    Py_RETURN_NONE;
}

PyObject * MGLSampler_get_filter2(MGLSampler * self) {
    return new_ref(self->slots.filter);
}

int MGLSampler_set_filter2(MGLSampler * self, PyObject * value) {
    int min_filter;
    int mag_filter;
    if (PyLong_Check(value)) {
        min_filter = PyLong_AsLong(value);
        mag_filter = min_filter;
        if (PyErr_Occurred()) {
            PyErr_Format(moderngl_error, "invalid filter");
            return -1;
        }
        Py_DECREF(self->slots.lod_range);
        self->slots.lod_range = Py_BuildValue("ii", min_filter, mag_filter);
    } else {
        PyObject * filter = PySequence_Tuple(value);
        if (!filter || PyTuple_GET_SIZE(filter) != 2) {
            Py_XDECREF(filter);
    		PyErr_Format(moderngl_error, "invalid filter");
            return -1;
        }

        min_filter = PyLong_AsLong(PyTuple_GET_ITEM(filter, 0));
        mag_filter = PyLong_AsLong(PyTuple_GET_ITEM(filter, 1));
        Py_DECREF(self->slots.lod_range);
        self->slots.lod_range = filter;
    }

 	const GLMethods & gl = self->context->gl;
	gl.SamplerParameteri(self->sampler_obj, GL_TEXTURE_MIN_FILTER, min_filter);
	gl.SamplerParameteri(self->sampler_obj, GL_TEXTURE_MAG_FILTER, mag_filter);
    return 0;
}

enum MGLSamplerWrapModes {
    MGL_CLAMP_TO_EDGE = 0x01,
    MGL_REPEAT = 0x02,
    MGL_MIRRORED_REPEAT = 0x04,
    MGL_MIRROR_CLAMP_TO_EDGE = 0x08,
    MGL_CLAMP_TO_BORDER = 0x10,
};

PyObject * MGLSampler_get_wrap2(MGLSampler * self) {
    return new_ref(self->slots.wrap);
}

int MGLSampler_set_wrap2(MGLSampler * self, PyObject * value) {
    static const int pnames[] = {GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R};

    int wrap = PyLong_AsLong(value);

    if (PyErr_Occurred() || (wrap >> (self->texture->dimensions * 8))) {
        PyErr_Format(moderngl_error, "invalid wrap");
        return -1;
    }

    Py_DECREF(self->slots.wrap);
    self->slots.wrap = new_ref(value);

 	const GLMethods & gl = self->context->gl;

    for (int i = 0; i < self->texture->dimensions; ++i) {
        switch (((unsigned char *)&wrap)[i]) {
            case 0:
            case MGL_CLAMP_TO_EDGE:
                gl.SamplerParameteri(self->sampler_obj, pnames[i], GL_CLAMP_TO_EDGE);
                break;
            case MGL_REPEAT:
                gl.SamplerParameteri(self->sampler_obj, pnames[i], GL_REPEAT);
                break;
            case MGL_MIRRORED_REPEAT:
                gl.SamplerParameteri(self->sampler_obj, pnames[i], GL_MIRRORED_REPEAT);
                break;
            case MGL_MIRROR_CLAMP_TO_EDGE:
                gl.SamplerParameteri(self->sampler_obj, pnames[i], GL_MIRROR_CLAMP_TO_EDGE);
                break;
            case MGL_CLAMP_TO_BORDER:
                gl.SamplerParameteri(self->sampler_obj, pnames[i], GL_CLAMP_TO_BORDER);
                break;
            default:
                PyErr_Format(moderngl_error, "invalid wrap");
                return -1;
        }
    }
    return 0;
}

PyObject * MGLSampler_get_border2(MGLSampler * self) {
    return new_ref(self->slots.border);
}

int MGLSampler_set_border2(MGLSampler * self, PyObject * value) {
    float color[4] = {};

    if (PyNumber_Check(value)) {
        color[0] = color[1] = color[2] = color[3] = (float)PyFloat_AsDouble(value);
        if (PyErr_Occurred()) {
            PyErr_Format(moderngl_error, "invalid border");
            return -1;
        }

    } else {
        PyObject * border = PySequence_Tuple(value);
        if (!border) {
            return -1;
        }

        PyObject ** border_items = PySequence_Fast_ITEMS(border);

        switch (PyTuple_GET_SIZE(border)) {
            case 4:
                color[3] = (float)PyFloat_AsDouble(border_items[3]);
            case 3:
                color[2] = (float)PyFloat_AsDouble(border_items[2]);
            case 2:
                color[1] = (float)PyFloat_AsDouble(border_items[1]);
            case 1:
                color[0] = (float)PyFloat_AsDouble(border_items[0]);
                break;
            default:
                Py_DECREF(border);
                PyErr_Format(moderngl_error, "invalid border");
                return -1;
        }

        if (PyErr_Occurred()) {
            Py_DECREF(border);
            PyErr_Format(moderngl_error, "invalid border");
            return -1;
        }

        Py_DECREF(self->slots.border);
        self->slots.border = border;
    }

	self->context->gl.SamplerParameterfv(self->sampler_obj, GL_TEXTURE_BORDER_COLOR, color);
    return 0;
}

PyObject * MGLSampler_get_anisotropy2(MGLSampler * self) {
    return new_ref(self->slots.anisotropy);
}

int MGLSampler_set_anisotropy2(MGLSampler * self, PyObject * value) {
	float anisotropy = (float)PyFloat_AsDouble(value);
    if (PyErr_Occurred()) {
        PyErr_Format(moderngl_error, "invalid anisotropy");
        return -1;
    }

    if (anisotropy < 1.0) {
        anisotropy = 1.0;
    }
    // TODO: clamp with max value
	self->context->gl.SamplerParameterf(self->sampler_obj, GL_TEXTURE_MAX_ANISOTROPY, anisotropy);
    Py_DECREF(self->slots.anisotropy);
    self->slots.anisotropy = new_ref(value);
    return 0;
}

PyObject * MGLSampler_get_compare_func2(MGLSampler * self) {
    return new_ref(self->slots.compare_func);
}

int MGLSampler_set_compare_func2(MGLSampler * self, PyObject * value) {
	const GLMethods & gl = self->context->gl;
    if (value == Py_None) {
		gl.SamplerParameteri(self->sampler_obj, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        Py_DECREF(self->slots.compare_func);
        self->slots.compare_func = new_ref(Py_None);
        return 0;
    }

    const char * compare_func_str = PyUnicode_AsUTF8(value);
	int compare_func = compare_func_from_str(compare_func_str);
	if (!compare_func) {
        PyErr_Format(moderngl_error, "invalid compare_func");
        return -1;
	}

    Py_DECREF(self->slots.compare_func);
    self->slots.compare_func = new_ref(value);

    gl.SamplerParameteri(self->sampler_obj, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    gl.SamplerParameteri(self->sampler_obj, GL_TEXTURE_COMPARE_FUNC, compare_func);
    return 0;
}

PyObject * MGLSampler_get_lod_range2(MGLSampler * self) {
    return new_ref(self->slots.lod_range);
}

int MGLSampler_set_lod_range2(MGLSampler * self, PyObject * value) {
    PyObject * lod_range = PySequence_Tuple(value);
    if (!lod_range) {
        PyErr_Format(moderngl_error, "invalid lod_range");
        return -1;
    }

    if (PyTuple_GET_SIZE(lod_range) != 2) {
        Py_DECREF(lod_range);
        PyErr_Format(moderngl_error, "invalid lod_range");
        return -1;
    }

    int min_lod = PyLong_AsLong(PyTuple_GET_ITEM(lod_range, 0));
    int max_lod = PyLong_AsLong(PyTuple_GET_ITEM(lod_range, 1));

    if (PyErr_Occurred()) {
        Py_DECREF(lod_range);
        PyErr_Format(moderngl_error, "invalid lod_range");
        return -1;
    }

    Py_DECREF(self->slots.lod_range);
    self->slots.lod_range = new_ref(value);

	self->context->gl.SamplerParameteri(self->sampler_obj, GL_TEXTURE_MIN_LOD, min_lod);
	self->context->gl.SamplerParameteri(self->sampler_obj, GL_TEXTURE_MAX_LOD, max_lod);
    return 0;
}

PyObject * MGLSampler_get_lod_bias2(MGLSampler * self) {
    return new_ref(self->slots.lod_bias);
}

int MGLSampler_set_lod_bias2(MGLSampler * self, PyObject * value) {
	float lod_bias = (float)PyFloat_AsDouble(value);
    if (PyErr_Occurred()) {
        PyErr_Format(moderngl_error, "invalid lod_bias");
        return -1;
    }

    Py_DECREF(self->slots.lod_bias);
    self->slots.lod_bias = new_ref(value);

	self->context->gl.SamplerParameterf(self->sampler_obj, GL_TEXTURE_LOD_BIAS, lod_bias);
    return 0;
}

PyObject * MGLSampler_get_texture2(MGLSampler * self) {
    return new_ref(self->slots.texture);
}

int MGLSampler_set_texture2(MGLSampler * self, PyObject * value) {
    if (!Texture_Check(value)) {
        PyErr_Format(moderngl_error, "invalid texture");
        return -1;
    }

    value = get_new_wrapper(value);

    Py_DECREF(self->slots.texture);
    self->slots.texture = new_ref(value);

    self->texture = (MGLTexture *)get_slot(value, "mglo");
    return 0;
}

void MGLSampler_dealloc(MGLSampler * self) {
    printf("MGLSampler_dealloc\n");
    Py_TYPE(self)->tp_free(self);
}

PyMethodDef MGLSampler_methods[] = {
    {"use", (PyCFunction)MGLSampler_meth_use, METH_O, 0},
    {0},
};

PyGetSetDef MGLSampler_getset[] = {
    {"filter", (getter)MGLSampler_get_filter2, (setter)MGLSampler_set_filter2, 0, 0},
    {"wrap", (getter)MGLSampler_get_wrap2, (setter)MGLSampler_set_wrap2, 0, 0},
    {"anisotropy", (getter)MGLSampler_get_anisotropy2, (setter)MGLSampler_set_anisotropy2, 0, 0},
    {"compare_func", (getter)MGLSampler_get_compare_func2, (setter)MGLSampler_set_compare_func2, 0, 0},
    {"border", (getter)MGLSampler_get_border2, (setter)MGLSampler_set_border2, 0, 0},
    {"lod_range", (getter)MGLSampler_get_lod_range2, (setter)MGLSampler_set_lod_range2, 0, 0},
    {"lod_bias", (getter)MGLSampler_get_lod_bias2, (setter)MGLSampler_set_lod_bias2, 0, 0},
    {"texture", (getter)MGLSampler_get_texture2, (setter)MGLSampler_set_texture2, 0, 0},
    {0},
};

PyType_Slot MGLSampler_slots[] = {
    {Py_tp_methods, MGLSampler_methods},
    {Py_tp_getset, MGLSampler_getset},
    {Py_tp_dealloc, (void *)MGLSampler_dealloc},
    {0},
};

PyType_Spec MGLSampler_spec = {
    "moderngl.mgl.new.MGLSampler",
    sizeof(MGLSampler),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLSampler_slots,
};

PyTypeObject * MGLSampler_class;
