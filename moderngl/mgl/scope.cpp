#include "query.hpp"

#include "context.hpp"
#include "framebuffer.hpp"
#include "tools.hpp"

PyObject * MGLContext_meth_scope(MGLContext * self, PyObject * args) { TRACE_VARAGS
	PyObject * framebuffer;
	PyObject * enable_flags;
	PyObject * textures;
	PyObject * uniform_buffers;
	PyObject * shader_storage_buffers;

	int args_ok = PyArg_ParseTuple(
		args,
		"OOOOO",
		&framebuffer,
		&enable_flags,
		&textures,
		&uniform_buffers,
		&shader_storage_buffers
	);

	if (!args_ok) {
		return 0;
	}

	MGLScope * scope = MGLContext_new_object(self, Scope);
	scope->enable_flags = -1;
	if (enable_flags != Py_None) {
		scope->enable_flags = PyLong_AsLong(enable_flags);
		if (PyErr_Occurred()) {
			// MGLError_Set("invalid enable_flags");
			return 0;
		}
	}

	textures = PySequence_Fast(textures, "textures is not iterable");
	if (!textures) {
		return 0;
	}

	uniform_buffers = PySequence_Fast(uniform_buffers, "uniform_buffers is not iterable");
	if (!uniform_buffers) {
		return 0;
	}

	shader_storage_buffers = PySequence_Fast(shader_storage_buffers, "shader_storage_buffers is not iterable");
	if (!shader_storage_buffers) {
		return 0;
	}

	int textures_len = (int)PySequence_Fast_GET_SIZE(textures);
	int uniform_buffers_len = (int)PySequence_Fast_GET_SIZE(uniform_buffers);
	int shader_storage_buffers_len = (int)PySequence_Fast_GET_SIZE(shader_storage_buffers);

	scope->num_textures = textures_len;
	scope->textures = new int[scope->num_textures * 3];
	scope->num_buffers = uniform_buffers_len + shader_storage_buffers_len;
	scope->buffers = new int[scope->num_buffers * 3];

	// TODO: textures + buffers

	Py_DECREF(textures);
	Py_DECREF(uniform_buffers);
	Py_DECREF(shader_storage_buffers);
	return NEW_REF(scope->wrapper);
}

PyObject * MGLScope_meth_begin(MGLScope * self) { TRACE_NOARGS
	const GLMethods & gl = self->context->gl;

	// for (int i = 0; i < self->num_textures; ++i) {
	// 	gl.ActiveTexture(self->textures[i * 3]);
	// 	gl.BindTexture(self->textures[i * 3 + 1], self->textures[i * 3 + 2]);
	// }

	// for (int i = 0; i < self->num_buffers; ++i) {
	// 	gl.BindBufferBase(self->buffers[i * 3], self->buffers[i * 3 + 1], self->buffers[i * 3 + 2]);
	// }

	enable_only(gl, self->enable_flags);
	self->context->enable_flags = self->enable_flags;
	return MGLFramebuffer_meth_use(self->framebuffer);
}

PyObject * MGLScope_meth_end(MGLScope * self) { TRACE_NOARGS
	const GLMethods & gl = self->context->gl;
	enable_only(gl, self->old_enable_flags);
	self->context->enable_flags = self->old_enable_flags;
	return MGLFramebuffer_meth_use(self->old_framebuffer);
}

PyTypeObject * MGLScope_define(int version_code) {
	PyMethodDef MGLScope_methods[] = {
		{"begin", (PyCFunction)MGLScope_meth_begin, METH_NOARGS, 0},
		{"end", (PyCFunction)MGLScope_meth_end, METH_NOARGS, 0},
		{0},
	};

	PyType_Slot MGLScope_slots[] = {
		{Py_tp_methods, dup(MGLScope_methods)},
		{0},
	};

	PyType_Spec MGLScope_spec = {"moderngl.mgl.Scope", sizeof(MGLScope), 0, Py_TPFLAGS_DEFAULT, MGLScope_slots};
	return (PyTypeObject *)PyType_FromSpec(&MGLScope_spec);
}
