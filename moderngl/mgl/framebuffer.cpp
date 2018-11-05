#include "framebuffer.hpp"
#include "context.hpp"
#include "renderbuffer.hpp"
#include "texture.hpp"

#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"

#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"
#include "internal/data_type.hpp"

/* MGLContext.framebuffer(...)
 */
PyObject * MGLContext_meth_framebuffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 2) {
        // TODO: error
        return 0;
    }

    PyObject * color_attachments = args[0];
    PyObject * depth_attachment = args[1];

    if (!PySequence_Check(color_attachments)) {
        PyObject * tuple = PyTuple_New(1);
        PyTuple_SET_ITEM(tuple, 0, color_attachments);
        color_attachments = tuple;
    } else {
        color_attachments = PySequence_Fast(color_attachments, "not iterable");
    }

    const GLMethods & gl = self->gl;

    MGLFramebuffer * framebuffer = MGLContext_new_object(self, Framebuffer);

    framebuffer->framebuffer_obj = 0;
    gl.GenFramebuffers(1, (GLuint *)&framebuffer->framebuffer_obj);

    if (!framebuffer->framebuffer_obj) {
        return 0;
    }

    self->bind_framebuffer(framebuffer->framebuffer_obj);

    int color_attachments_len = (int)PySequence_Fast_GET_SIZE(color_attachments);
    for (int i = 0; i < color_attachments_len; ++i) {
        PyObject * attachment = PySequence_Fast_GET_ITEM(color_attachments, i);
        if (attachment->ob_type == Renderbuffer_class) {
            MGLRenderbuffer * renderbuffer = SLOT(attachment, MGLRenderbuffer, Renderbuffer_class_mglo);
            gl.FramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + i,
                GL_RENDERBUFFER,
                renderbuffer->renderbuffer_obj
            );
        } else if (attachment->ob_type == Texture_class) {
            MGLTexture * texture = SLOT(attachment, MGLTexture, Texture_class_mglo);
            gl.FramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + i,
                texture->samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
                texture->texture_obj,
                0
            );
        } else {
            return 0;
        }
    }

    Py_DECREF(color_attachments);

    int status = gl.CheckFramebufferStatus(GL_FRAMEBUFFER);
    self->bind_framebuffer(self->bound_framebuffer->framebuffer_obj);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        const char * message = "the framebuffer is not complete";

        // switch (status) {
        // 	case GL_FRAMEBUFFER_UNDEFINED:
        // 		message = "the framebuffer is not complete (UNDEFINED)";
        // 		break;

        // 	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        // 		message = "the framebuffer is not complete (INCOMPLETE_ATTACHMENT)";
        // 		break;

        // 	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        // 		message = "the framebuffer is not complete (INCOMPLETE_MISSING_ATTACHMENT)";
        // 		break;

        // 	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        // 		message = "the framebuffer is not complete (INCOMPLETE_DRAW_BUFFER)";
        // 		break;

        // 	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        // 		message = "the framebuffer is not complete (INCOMPLETE_READ_BUFFER)";
        // 		break;

        // 	case GL_FRAMEBUFFER_UNSUPPORTED:
        // 		message = "the framebuffer is not complete (UNSUPPORTED)";
        // 		break;

        // 	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        // 		message = "the framebuffer is not complete (INCOMPLETE_MULTISAMPLE)";
        // 		break;

        // 	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        // 		message = "the framebuffer is not complete (INCOMPLETE_LAYER_TARGETS)";
        // 		break;
        // }

        return 0;
    }

    return NEW_REF(framebuffer->wrapper);
}

/* MGLFramebuffer.read(viewport, components, alignment, attachment, data_type, np)
 */
PyObject * MGLFramebuffer_meth_read(MGLFramebuffer * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 4) {
        // TODO: error
        return 0;
    }

    PyObject * viewport = args[0];
    int components = PyLong_AsLong(args[1]);
    int alignment = PyLong_AsLong(args[2]);
    int attachment = PyLong_AsLong(args[3]);
    MGLDataType * data_type = from_dtype(args[4]);
    int np = PyObject_IsTrue(args[5]);

    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;

    if (!unpack_viewport(viewport, x, y, width, height)) {
        return 0;
    }

    bool read_depth = attachment == -1;

    int expected_size = width * self->components * data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height;

    int pixel_type = data_type->gl_type;
    int base_format = read_depth ? GL_DEPTH_COMPONENT : data_type->base_format[components];

    PyObject * result = PyBytes_FromStringAndSize(0, expected_size);
    char * data = PyBytes_AS_STRING(result);

    const GLMethods & gl = self->context->gl;

    self->context->set_alignment(alignment);
    self->context->bind_framebuffer(self->framebuffer_obj);
    gl.ReadBuffer(read_depth ? GL_NONE : (GL_COLOR_ATTACHMENT0 + attachment));
    gl.ReadPixels(x, y, width, height, base_format, pixel_type, data);
    self->context->bind_framebuffer(self->context->bound_framebuffer->framebuffer_obj);

    return result;
}

/* MGLFramebuffer.use()
 */
PyObject * MGLFramebuffer_meth_use(MGLFramebuffer * self) {
    const GLMethods & gl = self->context->gl;

    self->context->bind_framebuffer(self->framebuffer_obj);

    Py_RETURN_NONE;
}

/* MGLFramebuffer.clear(attachment, value, viewport, color_mask)
 */
PyObject * MGLFramebuffer_meth_clear(MGLFramebuffer * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 4) {
        // TODO: error
        return 0;
    }

    int attachment = PyLong_AsLong(args[0]);
    PyObject * value = args[1];
    PyObject * viewport = args[2];
    int color_mask = PyLong_AsLong(args[3]);

    char color_bytes[32] = {};
    bool scissor = false;

    const GLMethods & gl = self->context->gl;

    self->context->bind_framebuffer(self->framebuffer_obj);

    if (viewport != Py_None) {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
        unpack_viewport(viewport, x, y, width, height);
        gl.Enable(GL_SCISSOR_TEST);
        gl.Scissor(x, y, width, height);
        scissor = true;
    } else if (self->viewport[0] || self->viewport[1] || self->viewport[2] != self->width || self->viewport[3] != self->height) {
        gl.Enable(GL_SCISSOR_TEST);
        gl.Scissor(self->viewport[0], self->viewport[1], self->viewport[2], self->viewport[3]);
        scissor = true;
    }

    if (attachment < 0) {
        float depth = (float)PyFloat_AsDouble(value);
        if (!self->context->current_depth_mask) {
            gl.DepthMask(true);
        }
        gl.ClearBufferfv(GL_DEPTH, 0, &depth);
        if (!self->context->current_depth_mask) {
            gl.DepthMask(false);
        }
    } else if (attachment < self->attachments) {
        value = PySequence_Fast(value, "value is not iterable");
        if (!value) {
            return 0;
        }
        read_value func;
        void * ptr = color_bytes;
        const char shape = self->attachment_type[attachment];
        switch (shape) {
            case 'f': func = (read_value)read_float; break;
            case 'i': func = (read_value)read_int; break;
            case 'u': func = (read_value)read_unsigned; break;
        }
        int size = (int)PySequence_Fast_GET_SIZE(value);
        for (int i = 0; i < size; ++i) {
            func(ptr, PySequence_Fast_GET_ITEM(value, i));
        }
        Py_DECREF(value);
        if (PyErr_Occurred()) {
            if (scissor) {
                gl.Disable(GL_SCISSOR_TEST);
            }
            return 0;
        }
        int old_mask = self->context->current_color_mask >> (attachment * 4) & 0xF;
        if (old_mask != color_mask) {
            gl.ColorMaski(attachment, color_mask & 1, color_mask & 2, color_mask & 4, color_mask & 8);
        }
        switch (shape) {
            case 'f': gl.ClearBufferfv(GL_COLOR, attachment, (float *)color_bytes); break;
            case 'i': gl.ClearBufferiv(GL_COLOR, attachment, (int *)color_bytes); break;
            case 'u': gl.ClearBufferuiv(GL_COLOR, attachment, (unsigned *)color_bytes); break;
        }
        if (old_mask != color_mask) {
            gl.ColorMaski(attachment, old_mask & 1, old_mask & 2, old_mask & 4, old_mask & 8);
        }
    } else {
        return 0;
    }

    if (scissor) {
        gl.Disable(GL_SCISSOR_TEST);
    }

    // return 0;
    Py_RETURN_NONE;
}
