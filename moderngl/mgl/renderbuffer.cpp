#include "renderbuffer.hpp"
#include "context.hpp"

#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"

#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"
#include "internal/data_type.hpp"

/* MGLContext.renderbuffer(size, components, samples, dtype)
 */
PyObject * MGLContext_meth_renderbuffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 4) {
        // TODO: error
        return 0;
    }

    PyObject * size = args[0];
    int components = PyLong_AsLong(args[1]);
    int samples = PyLong_AsLong(args[2]);
    MGLDataType * dtype = from_dtype(args[3]);

    int dims = (int)PySequence_Fast_GET_SIZE(size);
    if (dims != 2) {
        return 0;
    }

    int width = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 0));
    int height = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 1));
    int format = dtype->internal_format[components];

    MGLRenderbuffer * renderbuffer = MGLContext_new_object(self, Renderbuffer);

    const GLMethods & gl = self->gl;
    gl.GenRenderbuffers(1, (GLuint *)&renderbuffer->renderbuffer_obj);

    if (!renderbuffer->renderbuffer_obj) {
        // TODO: error
        return 0;
    }

    gl.BindRenderbuffer(GL_RENDERBUFFER, renderbuffer->renderbuffer_obj);

    if (samples == 0) {
        gl.RenderbufferStorage(GL_RENDERBUFFER, format, width, height);
    } else {
        gl.RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
    }

    renderbuffer->width = width;
    renderbuffer->height = height;
    renderbuffer->components = components;
    renderbuffer->samples = samples;
    renderbuffer->data_type = dtype;
    renderbuffer->depth = false;

    SLOT(renderbuffer->wrapper, PyObject, Renderbuffer_class_size) = int_tuple(width, height);
    return NEW_REF(renderbuffer->wrapper);
}

PyObject * MGLRenderbuffer_meth_write(MGLRenderbuffer * self, PyObject * const * args, Py_ssize_t nargs) {
    Py_RETURN_NONE;
}
