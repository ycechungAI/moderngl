#include "adaptor.hpp"

#include "internal/python.hpp"
#include "internal/opengl/gl_methods.hpp"
#include "internal/opengl/gl_context.hpp"
#include "internal/wrapper.hpp"
#include "mgl/context.hpp"
#include "mgl/buffer.hpp"

struct MGLFramebuffer;
struct MGLScope;

struct MGLContext_old {
	PyObject_HEAD

	MGLFramebuffer * default_framebuffer;
	MGLFramebuffer * bound_framebuffer;

	GLContext gl_context;

	int version_code;

	int max_samples;
	int max_integer_samples;
	int max_color_attachments;

	int max_texture_units;
	int default_texture_unit;
	float max_anisotropy;

	int enable_flags;
	int front_face;

	int depth_func;
	int blend_func_src;
	int blend_func_dst;

	bool wireframe;
	bool multisample;

	int provoking_vertex;

	GLMethods gl;
};

struct MGLBuffer_old {
	PyObject_HEAD

	MGLContext * context;

	int buffer_obj;

	Py_ssize_t size;
	bool dynamic;
};

extern PyTypeObject * MGLContext_class;
extern PyObject * meth_create_context(PyObject * self, PyObject * const * args, Py_ssize_t nargs);

PyObject * meth_extend_context(PyObject * self, PyObject * args) {
    PyObject * old_context;
    PyObject * glhooks;

    if (!PyArg_ParseTuple(args, "OO", &old_context, &glhooks)) {
        return 0;
    }

    PyObject * newargs[3] = {Py_False, Py_None, glhooks};
    PyObject * new_context = meth_create_context(self, newargs, 3);
    ensure_no_error();
    set_slot(old_context, "new", new_context);
    set_slot(new_context, "old", new_ref(old_context));
    MGLContext_old * old_mglo = (MGLContext_old *)get_slot(old_context, "mglo");
    MGLContext * new_mglo = (MGLContext *)get_slot(new_context, "mglo");
    old_mglo->gl = new_mglo->gl;
    Py_RETURN_NONE;
}

PyObject * meth_extend_buffer(PyObject * self, PyObject * args) {
    PyObject * old_buffer;
    PyObject * new_context;

    if (!PyArg_ParseTuple(args, "OO", &old_buffer, &new_context)) {
        return 0;
    }

	if (!Context_Check(new_context)) {
		return 0;
	}

    MGLBuffer_old * old_mglo = (MGLBuffer_old *)get_slot(old_buffer, "mglo");
	
	MGLContext * context_mglo = (MGLContext *)get_slot(new_context, "mglo");
    MGLBuffer * buffer = PyObject_New(MGLBuffer, MGLBuffer_class);
    chain_objects(context_mglo, buffer);
    buffer->context = context_mglo;
    buffer->flags = MGL_BUFFER_READABLE | MGL_BUFFER_WRITABLE;
    buffer->buffer_obj = old_mglo->buffer_obj;
    buffer->wrapper = Buffer_New("On", buffer, buffer->size);
	
    set_slot(old_buffer, "new", new_ref(buffer->wrapper));
    set_slot(buffer->wrapper, "old", new_ref(old_buffer));

    Py_RETURN_NONE;
}

PyObject * get_new_wrapper(PyObject * old_wrapper) {
	if (PyObject_HasAttrString(old_wrapper, "new")) {
    	PyObject * new_wrapper = PyObject_GetAttrString(old_wrapper, "new");
        Py_DECREF(new_wrapper);
        return new_wrapper;
    }
    return old_wrapper;
}
