#include "texture.hpp"
#include "buffer.hpp"
#include "context.hpp"

#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"

#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"
#include "internal/data_type.hpp"

/* MGLContext.texture(size, components, data, levels, samples, aligment, dtype)
 */
PyObject * MGLContext_meth_texture(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 7) {
        // TODO: error
        return 0;
    }

    PyObject * size = args[0];
    int components = PyLong_AsLong(args[1]);
    PyObject * data = args[2];
    int levels = PyLong_AsLong(args[3]);
    int samples = PyLong_AsLong(args[4]);
    int alignment = PyLong_AsLong(args[5]);
    MGLDataType * data_type = from_dtype(args[6]);

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
    if (dims != 2 && dims != 3) {
        return 0;
    }

    int width = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 0));
    int height = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 1));
    int depth = dims == 3 ? PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 2)) : 1;

    Py_DECREF(size);

    if (!data_type) {
        PyErr_Format(moderngl_error, "invalid dtype");
        return 0;
    }

    int max_levels = -1;
    {
        int size = width > height ? width : height;
        size = size > depth ? size : depth;
        while (size) {
            max_levels += 1;
            size >>= 1;
        }
    }

    if (levels < 0 || levels > max_levels) {
        levels = max_levels;
    }

    int expected_size = width * components * data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height * depth;

    int texture_target = dims == 3 ? GL_TEXTURE_3D : (samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);
    int pixel_type = data_type->gl_type;
    int base_format = data_type->base_format[components];
    int internal_format = data_type->internal_format[components];

    MGLTexture * texture = MGLContext_new_object(self, Texture);

    texture->width = width;
    texture->height = height;
    texture->depth = depth;
    texture->components = components;
    texture->levels = levels;
    texture->samples = samples;
    texture->data_type = data_type;
    texture->texture_target = texture_target;

    const GLMethods & gl = self->gl;
    gl.GenTextures(1, (GLuint *)&texture->texture_obj);

    if (!texture->texture_obj) {
        // TODO: error
        return 0;
    }

    self->bind_temp_texture(texture_target, texture->texture_obj);

    if (data != Py_None) {
        Py_buffer view = {};
        if (prepare_buffer(data, &view) < 0) {
            return 0;
        }
        void * buf = view.buf;
        bool contiguos = PyBuffer_IsContiguous(&view, 'C');
        self->set_alignment(alignment);
        if (!contiguos) {
            buf = malloc(expected_size);
            PyBuffer_ToContiguous(buf, &view, view.len, 'C');
        }
        if (dims == 3) {
            if (gl.TexStorage3D) {
                gl.TexStorage3D(texture_target, levels, internal_format, width, height, depth);
                gl.TexSubImage3D(texture_target, 0, 0, 0, 0, width, height, depth, base_format, pixel_type, buf);
            } else {
                gl.TexImage3D(texture_target, 0, internal_format, width, height, depth, 0, base_format, pixel_type, buf);
            }
        } else {
            if (gl.TexStorage2D) {
                gl.TexStorage2D(texture_target, levels, internal_format, width, height);
                gl.TexSubImage2D(texture_target, 0, 0, 0, width, height, base_format, pixel_type, buf);
            } else {
                gl.TexImage2D(texture_target, 0, internal_format, width, height, 0, base_format, pixel_type, buf);
            }
        }
        if (!contiguos) {
            free(buf);
        }
    } else if (samples) {
        gl.TexImage2DMultisample(texture_target, samples, internal_format, width, height, true);
    } else {
        if (dims == 3) {
            gl.TexImage3D(texture_target, 0, internal_format, width, height, depth, 0, base_format, pixel_type, 0);
        } else {
            gl.TexImage2D(texture_target, 0, internal_format, width, height, 0, base_format, pixel_type, 0);
        }
    }

    SLOT(texture->wrapper, PyObject, Texture_class_level) = PyLong_FromLong(0);
    SLOT(texture->wrapper, PyObject, Texture_class_layer) = PyLong_FromLong(-1);
    SLOT(texture->wrapper, PyObject, Texture_class_size) = dims == 3 ? int_tuple(width, height, depth) : int_tuple(width, height);
    return NEW_REF(texture->wrapper);
}

/* MGLTexture.write(data, viewport, alignment, level)
 */
PyObject * MGLTexture_meth_write(MGLTexture * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 4) {
        // TODO: error
        return 0;
    }

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

    if (data->ob_type == Buffer_class) {
        MGLBuffer * buffer = SLOT(data, MGLBuffer, Buffer_class_mglo);
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
