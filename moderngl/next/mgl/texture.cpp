#include "texture.hpp"
#include "buffer.hpp"
#include "context.hpp"

#include "internal/wrapper.hpp"

#include "internal/tools.hpp"
#include "internal/glsl.hpp"
#include "internal/data_type.hpp"

enum MGLTextureTypes {
    MGL_TEXTURE_2D,
    MGL_TEXTURE_3D,
    MGL_TEXTURE_2D_ARRAY,
    MGL_TEXTURE_CUBE,
    MGL_TEXTURE_CUBE_ARRAY,
};

/* MGLContext.texture(size, components, data, levels, samples, aligment, dtype)
 */
PyObject * MGLContext_meth_texture(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(8);

    int texture_type = PyLong_AsLong(args[0]);
    PyObject * size = args[1];
    int components = PyLong_AsLong(args[2]);
    PyObject * data = args[3];
    int levels = PyLong_AsLong(args[4]);
    int samples = PyLong_AsLong(args[5]);
    int alignment = PyLong_AsLong(args[6]);
    MGLDataType * data_type = from_dtype(args[7]);

    if ((PyObject *)args[0]->ob_type == pillow_image) {
        PyObject * image = size;
        size = PyObject_GetAttr(image, size_str);

        if (!size) {
            return 0;
        }

        PyObject * mode = PyObject_GetAttr(image, mode_str);
        if (!mode) {
            return 0;
        }

        const char * img_mode = PyUnicode_AsUTF8(mode);

        if (!strcmp(img_mode, "RGBA")) {
            components = 4;
        } else if (!strcmp(img_mode, "RGB")) {
            components = 3;
        } else if (!strcmp(img_mode, "L")) {
            components = 1;
        } else {
        }

        data = image;
        alignment = 1;
    }

    size = PySequence_Fast(size, "size is not iterable");
    if (!size) {
        return 0;
    }

    int dims = (int)PySequence_Fast_GET_SIZE(size);

    if (texture_type == MGL_TEXTURE_2D && dims == 3) {
        texture_type = MGL_TEXTURE_3D;
    }

    MGLTexture * texture = PyObject_New(MGLTexture, MGLTexture_class);
    chain_objects(self, texture);
    texture->context = self;

    switch (texture_type) {
        case MGL_TEXTURE_2D:
            texture->texture_target = samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
            texture->width = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 0));
            texture->height = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 1));
            texture->depth = 1;
            texture->dimensions = 2;
            break;

        case MGL_TEXTURE_3D:
            texture->texture_target = GL_TEXTURE_3D;
            texture->width = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 0));
            texture->height = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 1));
            texture->depth = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 2));
            texture->dimensions = 3;
            break;

        case MGL_TEXTURE_2D_ARRAY:
            texture->texture_target = samples ? GL_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_2D_ARRAY;
            texture->width = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 0));
            texture->height = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 1));
            texture->depth = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 2));
            texture->dimensions = 2;
            break;

        case MGL_TEXTURE_CUBE:
            texture->texture_target = GL_TEXTURE_CUBE_MAP;
            texture->width = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 0));
            texture->height = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 1));
            texture->depth = 6;
            texture->dimensions = 3;
            break;

        case MGL_TEXTURE_CUBE_ARRAY:
            texture->texture_target = GL_TEXTURE_CUBE_MAP_ARRAY;
            texture->width = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 0));
            texture->height = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 1));
            texture->depth = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 2)) * 6;
            texture->dimensions = 3;
            break;

        default:
            // TODO: error
            return 0;
    }

    Py_DECREF(size);

    if (!data_type) {
        PyErr_Format(moderngl_error, "invalid dtype");
        return 0;
    }

    if (samples & (samples - 1)) {
        PyErr_Format(moderngl_error, "invalid samples");
        return 0;
    }

    if (levels > 0 && samples) {
        PyErr_Format(moderngl_error, "invalid levels");
        return 0;
    }

    int max_levels = -1;
    {
        int size = texture->width > texture->height ? texture->width : texture->height;
        size = size > texture->depth ? size : texture->depth;
        while (size) {
            max_levels += 1;
            size >>= 1;
        }
    }

    if (levels < 0 || levels > max_levels) {
        levels = max_levels;
    }

    texture->expected_size = (texture->width * components * data_type->size + alignment - 1) & -alignment;
    texture->expected_size *= texture->height * texture->depth;

    texture->data_type = data_type;
    texture->components = components;
    // texture->depth_component = false;
    texture->levels = levels;
    texture->samples = samples;

    int pixel_type = data_type->gl_type;
    int base_format = data_type->base_format[components];
    int internal_format = data_type->internal_format[components];

    // if (texture->components < 0) {
    //     texture->components = 1;
    //     texture->depth_component = true;
    //     internal_format = GL_DEPTH_COMPONENT24;
    //     base_format = GL_DEPTH_COMPONENT;
    //     pixel_type = GL_FLOAT;
    // }

    const GLMethods & gl = self->gl;
    gl.GenTextures(1, (GLuint *)&texture->texture_obj);

    if (!texture->texture_obj) {
        // TODO: error
        return 0;
    }

    self->bind_temp_texture(texture->texture_target, texture->texture_obj);

    void * buf = 0;
    bool allocated = false;

    if (data != Py_None) {
        Py_buffer view = {};
        if (prepare_buffer(data, &view) < 0) {
            return 0;
        }
        allocated = !PyBuffer_IsContiguous(&view, 'C');
        if (allocated) {
            buf = malloc(texture->expected_size);
            PyBuffer_ToContiguous(buf, &view, view.len, 'C');
        } else {
            buf = view.buf;
        }
    }

    self->set_alignment(alignment);

    int tx = texture->texture_target;
    int width = texture->width;
    int height = texture->height;
    int depth = texture->depth;

    switch (texture_type) {
        case MGL_TEXTURE_2D:
            if (samples) {
                if (gl.TexStorage2DMultisample) {
                    gl.TexStorage2DMultisample(tx, samples, internal_format, width, height, true);
                } else {
                    gl.TexImage2DMultisample(tx, samples, internal_format, width, height, true);
                }
            } else {
                if (gl.TexStorage2D) {
                    gl.TexStorage2D(tx, levels, internal_format, width, height);
                    gl.TexSubImage2D(tx, 0, 0, 0, width, height, base_format, pixel_type, buf);
                } else {
                    gl.TexImage2D(tx, 0, internal_format, width, height, 0, base_format, pixel_type, buf);
                }
            }
            break;

        case MGL_TEXTURE_3D:
        case MGL_TEXTURE_2D_ARRAY:
        case MGL_TEXTURE_CUBE_ARRAY:
            // if (samples) GL_TEXTURE_2D_MULTISAMPLE_ARRAY
            if (gl.TexStorage3D) {
                gl.TexStorage3D(tx, levels, internal_format, width, height, depth);
                gl.TexSubImage3D(tx, 0, 0, 0, 0, width, height, depth, base_format, pixel_type, buf);
            } else {
                gl.TexImage3D(tx, 0, internal_format, width, height, depth, 0, base_format, pixel_type, buf);
            }
            break;

        case MGL_TEXTURE_CUBE:
            if (samples) {
                if (gl.TexStorage2DMultisample) {
                    gl.TexStorage2DMultisample(GL_TEXTURE_CUBE_MAP, samples, internal_format, width, height, true);
                } else {
                    for (int i = 0; i < 6; ++i) {
                        gl.TexImage2DMultisample(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, samples, internal_format, width, height, true);
                    }
                }
            } else {
                if (gl.TexStorage2D) {
                    gl.TexStorage2D(GL_TEXTURE_CUBE_MAP, levels, internal_format, width, height);
                    for (int i = 0; i < 6; ++i) {
                        const char * ptr = (const char *)buf + texture->expected_size * i / 6;
                        gl.TexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height, base_format, pixel_type, buf ? ptr : 0);
                    }
                } else {
                    for (int i = 0; i < 6; ++i) {
                        const char * ptr = (const char *)buf + texture->expected_size * i / 6;
                        gl.TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format, width, height, 0, base_format, pixel_type, buf ? ptr : 0);
                    }
                }
            }
            break;
    }

    if (allocated) {
        free(buf);
    }

    PyObject * size_arg = dims == 3 ? int_tuple(width, height, depth) : int_tuple(width, height);
    texture->wrapper = Texture_New("OiiO", texture, 0, -1, size_arg);
    return texture->wrapper;
}

/* MGLTexture.write(data, viewport, alignment, level)
 */
PyObject * MGLTexture_meth_write(MGLTexture * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(4);

    PyObject * data = args[0];
    PyObject * viewport = args[1];
    int alignment = PyLong_AsLong(args[2]);
    int level = PyLong_AsLong(args[3]);

    if (alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8) {
        PyErr_Format(moderngl_error, "the alignment must be 1, 2, 4 or 8");
        return 0;
    }

    if (level > self->levels) {
        PyErr_Format(moderngl_error, "invalid level");
        return 0;
    }

    if (self->samples) {
        PyErr_Format(moderngl_error, "multisample textures cannot be read directly");
        return 0;
    }

    int max_width = self->width / (1 << level);
    int max_height = self->height / (1 << level);
    int max_depth = self->depth / (1 << level);

    max_width = max_width > 1 ? max_width : 1;
    max_height = max_height > 1 ? max_height : 1;
    max_depth = max_depth > 1 ? max_depth : 1;

    int x = 0;
    int y = 0;
    int z = 0;
    int width = max_width;
    int height = max_height;
    int depth = max_depth;

    if (self->texture_target == GL_TEXTURE_3D) {
        if (!unpack_viewport(viewport, x, y, z, width, height, depth)) {
            return 0;
        }
    } else {
        if (!unpack_viewport(viewport, x, y, width, height)) {
            return 0;
        }
    }

    int expected_size = width * self->components * self->data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height * depth;

    int pixel_type = self->data_type->gl_type;
    int format = self->data_type->base_format[self->components];

    const GLMethods & gl = self->context->gl;

    self->context->bind_temp_texture(self->texture_target, self->texture_obj);
    self->context->set_alignment(alignment);

    if (Buffer_Check(data)) {
        MGLBuffer * buffer = (MGLBuffer *)get_slot(data, "mglo");
        gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer->buffer_obj);
        if (self->texture_target == GL_TEXTURE_3D) {
            gl.TexSubImage3D(self->texture_target, level, x, y, z, width, height, depth, format, pixel_type, 0);
        } else {
            gl.TexSubImage2D(self->texture_target, level, x, y, width, height, format, pixel_type, 0);
        }
        gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    } else {
        Py_buffer view = {};
        if (prepare_buffer(data, &view) < 0) {
            return 0;
        }
        void * buf = view.buf;
        bool contiguos = PyBuffer_IsContiguous(&view, 'C');
        if (!contiguos) {
            buf = malloc(expected_size);
            PyBuffer_ToContiguous(buf, &view, view.len, 'C');
        }
        if (self->texture_target == GL_TEXTURE_3D) {
            gl.TexSubImage3D(self->texture_target, level, x, y, z, width, height, depth, format, pixel_type, buf);
        } else {
            gl.TexSubImage2D(self->texture_target, level, x, y, width, height, format, pixel_type, buf);
        }
        if (!contiguos) {
            free(buf);
        }
        PyBuffer_Release(&view);
    }

    Py_RETURN_NONE;
}

/* MGLTexture.bind(...)
 */
PyObject * MGLTexture_meth_bind(MGLTexture * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(3);

	int binding = PyLong_AsLong(args[0]);
	int access = PyLong_AsLong(args[1]);
	int format = PyLong_AsLong(args[2]);

    const GLMethods & gl = self->context->gl;
	gl.BindImageTexture(binding, self->texture_obj, 0, 0, 0, access, format);
    Py_RETURN_NONE;
}

int MGLTexture_set_swizzle(MGLTexture * self, PyObject * value) {
	const char * swizzle = PyUnicode_AsUTF8(value);

    if (PyUnicode_GetSize(value) > 4) {
        PyErr_Format(PyExc_Exception, "error -- %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
        return -1;
    }

	int tex_swizzle[4] = {GL_ZERO, GL_ZERO, GL_ZERO, GL_ONE};

	for (int i = 0; i < swizzle[i]; ++i) {
		tex_swizzle[i] = swizzle_from_chr(swizzle[i]);
		if (tex_swizzle[i] == -1) {
            PyErr_Format(PyExc_Exception, "error -- %s (%s:%d)", __FUNCTION__, __FILE__, __LINE__);
			return -1;
		}
	}

	int texture_target = self->samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

	const GLMethods & gl = self->context->gl;

	gl.ActiveTexture(GL_TEXTURE0 + self->context->default_texture_unit);
	gl.BindTexture(texture_target, self->texture_obj);
	gl.TexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_RGBA, tex_swizzle);

	char swizzle_str[4] = {
		chr_from_swizzle(tex_swizzle[0]),
		chr_from_swizzle(tex_swizzle[1]),
		chr_from_swizzle(tex_swizzle[2]),
		chr_from_swizzle(tex_swizzle[3]),
	};

    // PyObject *& swizzle_slot = get_slot(self->wrapper, "swizzle");
    // Py_DECREF(swizzle_slot);
	// swizzle_slot = PyUnicode_FromStringAndSize(swizzle_str, 4);
    return 0;
}

void MGLTexture_dealloc(MGLTexture * self) {
    printf("MGLTexture_dealloc\n");
    Py_TYPE(self)->tp_free(self);
}

fastcallable(MGLTexture_meth_write)
fastcallable(MGLTexture_meth_bind)

PyMethodDef MGLTexture_methods[] = {
    {"write", fastcall(MGLTexture_meth_write), fastcall_flags, NULL},
    {"bind", fastcall(MGLTexture_meth_bind), fastcall_flags, NULL},
    {0},
};

PyGetSetDef MGLTexture_getset[] = {
    {"swizzle", 0, (setter)MGLTexture_set_swizzle, 0, 0},
    {0},
};

PyType_Slot MGLTexture_slots[] = {
    {Py_tp_methods, MGLTexture_methods},
    {Py_tp_getset, MGLTexture_getset},
    {Py_tp_dealloc, (void *)MGLTexture_dealloc},
    {0},
};

PyType_Spec MGLTexture_spec = {
    "moderngl.mgl.new.MGLTexture",
    sizeof(MGLTexture),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLTexture_slots,
};

PyTypeObject * MGLTexture_class;
