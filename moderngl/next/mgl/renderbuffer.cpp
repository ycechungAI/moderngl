#include "renderbuffer.hpp"
#include "context.hpp"

#include "internal/wrapper.hpp"

#include "internal/tools.hpp"
#include "internal/glsl.hpp"
#include "internal/data_type.hpp"

/* MGLContext.renderbuffer(size, components, samples, dtype)
 */
PyObject * MGLContext_meth_renderbuffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(4);

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

    MGLRenderbuffer * renderbuffer = PyObject_New(MGLRenderbuffer, MGLRenderbuffer_class);
    chain_objects(self, renderbuffer);
    renderbuffer->context = self;

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

    renderbuffer->wrapper = Renderbuffer_New("O(ii)", renderbuffer, width, height);
    return renderbuffer->wrapper;
}

/* MGLContext.depth_renderbuffer(size, samples)
 */
PyObject * MGLContext_meth_depth_renderbuffer(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(3);

    PyObject * size = args[0];
    int samples = PyLong_AsLong(args[1]);
    int bits = PyLong_AsLong(args[2]);

    int dims = (int)PySequence_Fast_GET_SIZE(size);
    if (dims != 2) {
        return 0;
    }

    int width = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 0));
    int height = PyLong_AsLong(PySequence_Fast_GET_ITEM(size, 1));

    int internal_format = GL_DEPTH_COMPONENT24;
    if (bits == 32) {
        internal_format = GL_DEPTH_COMPONENT32;
    } else if (bits == 16) {
        internal_format = GL_DEPTH_COMPONENT16;
    }

    MGLRenderbuffer * renderbuffer = PyObject_New(MGLRenderbuffer, MGLRenderbuffer_class);
    chain_objects(self, renderbuffer);
    renderbuffer->context = self;

    const GLMethods & gl = self->gl;
    gl.GenRenderbuffers(1, (GLuint *)&renderbuffer->renderbuffer_obj);

    if (!renderbuffer->renderbuffer_obj) {
        // TODO: error
        return 0;
    }

    gl.BindRenderbuffer(GL_RENDERBUFFER, renderbuffer->renderbuffer_obj);

    if (samples == 0) {
        gl.RenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
    } else {
        gl.RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internal_format, width, height);
    }

    renderbuffer->width = width;
    renderbuffer->height = height;
    renderbuffer->components = 1;
    renderbuffer->samples = samples;
    renderbuffer->data_type = &f4;
    renderbuffer->depth = true;

    renderbuffer->wrapper = Renderbuffer_New("O(ii)", renderbuffer, width, height);
    return renderbuffer->wrapper;
}

void MGLRenderbuffer_dealloc(MGLRenderbuffer * self) {
    printf("MGLRenderbuffer_dealloc\n");
    Py_TYPE(self)->tp_free(self);
}

PyType_Slot MGLRenderbuffer_slots[] = {
    {Py_tp_dealloc, (void *)MGLRenderbuffer_dealloc},
    {0},
};

PyType_Spec MGLRenderbuffer_spec = {
    "moderngl.mgl.new.MGLRenderbuffer",
    sizeof(MGLRenderbuffer),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLRenderbuffer_slots,
};

PyTypeObject * MGLRenderbuffer_class;
