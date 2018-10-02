#include "texture.hpp"
#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"
#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"
#include "internal/data_type.hpp"

/* MGLContext.texture(...)
 */
PyObject * MGLContext_meth_texture(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    // size, components, data, levels, samples, aligment, dtype
    if (nargs != 7) {
        // TODO: error
        return 0;
    }

    PyObject * size = PySequence_Fast(args[0], "size is not iterable");
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

    int components = PyLong_AsLong(args[1]);
    PyObject * data = args[2];
    int levels = PyLong_AsLong(args[3]);
    int samples = PyLong_AsLong(args[4]);
    int alignment = PyLong_AsLong(args[5]);
    MGLDataType * data_type = from_dtype(args[6]);

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

    gl.ActiveTexture(GL_TEXTURE0 + self->default_texture_unit);
    gl.BindTexture(texture_target, texture->texture_obj);
    gl.TexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl.TexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data != Py_None) {
        Py_buffer view = {};
        if (prepare_buffer(data, &view) < 0) {
            return 0;
        }
        void * buf = view.buf;
        bool contiguos = PyBuffer_IsContiguous(&view, 'C');
        gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
        gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
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

    return NEW_REF(texture->wrapper);
}

PyObject * MGLTexture_meth_write(MGLTexture * self, PyObject * const * args, Py_ssize_t nargs) {
    Py_RETURN_NONE;
}
