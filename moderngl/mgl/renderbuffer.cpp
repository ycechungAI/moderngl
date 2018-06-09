#include "renderbuffer.hpp"

#include "context.hpp"
#include "data_type.hpp"

PyObject * MGLContext_meth_renderbuffer(MGLContext * self, PyObject * args) { TRACE_VARAGS
	int width;
	int height;
	int components;
	int samples;
	PyObject * dtype;

	int args_ok = PyArg_ParseTuple(
		args,
		"(II)IIO",
		&width,
		&height,
		&components,
		&samples,
		&dtype
	);

	MGLDataType * data_type = from_dtype(dtype);
	int format = components > 0 ? data_type->internal_format[components] : GL_DEPTH_COMPONENT24;

	const GLMethods & gl = self->gl;

	MGLRenderbuffer * renderbuffer = MGLContext_new_object(self, Renderbuffer);
	gl.GenRenderbuffers(1, (GLuint *)&renderbuffer->renderbuffer_obj);

	gl.BindRenderbuffer(GL_RENDERBUFFER, renderbuffer->renderbuffer_obj);
	gl.RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);

	renderbuffer->width = width;
	renderbuffer->height = height;
	renderbuffer->components = components;
	renderbuffer->samples = samples;

	SLOT(renderbuffer->wrapper, PyObject, Renderbuffer_class_size) = int_tuple(width, height);
	return NEW_REF(renderbuffer->wrapper);
}

PyTypeObject * MGLRenderbuffer_define(int version_code) {
	PyType_Slot MGLRenderbuffer_slots[] = {
		{0},
	};

	PyType_Spec MGLRenderbuffer_spec = {"moderngl.mgl.Renderbuffer", sizeof(MGLRenderbuffer), 0, Py_TPFLAGS_DEFAULT, MGLRenderbuffer_slots};
	return (PyTypeObject *)PyType_FromSpec(&MGLRenderbuffer_spec);
}
