#include "query.hpp"

#include "context.hpp"

PyObject * MGLContext_meth_query(MGLContext * self, PyObject * args) { TRACE_VARAGS
	int samples_passed;
	int any_samples_passed;
	int time_elapsed;
	int primitives_generated;

	int args_ok = PyArg_ParseTuple(
		args,
		"pppp",
		&samples_passed,
		&any_samples_passed,
		&time_elapsed,
		&primitives_generated
	);

	if (!args_ok) {
		return 0;
	}

	if (!samples_passed && !any_samples_passed && !time_elapsed && !primitives_generated) {
		samples_passed = 1;
		any_samples_passed = 1;
		time_elapsed = 1;
		primitives_generated = 1;
	}

	MGLQuery * query = MGLContext_new_object(self, Query);

	const GLMethods & gl = self->gl;

	if (samples_passed) {
		gl.GenQueries(1, (GLuint *)&query->query_obj[SAMPLES_PASSED]);
	}

	if (any_samples_passed) {
		gl.GenQueries(1, (GLuint *)&query->query_obj[ANY_SAMPLES_PASSED]);
	}

	if (time_elapsed) {
		gl.GenQueries(1, (GLuint *)&query->query_obj[TIME_ELAPSED]);
	}

	if (primitives_generated) {
		gl.GenQueries(1, (GLuint *)&query->query_obj[PRIMITIVES_GENERATED]);
	}

	return NEW_REF(query->wrapper);
}

PyObject * MGLQuery_meth_begin(MGLQuery * self) { TRACE_NOARGS
	const GLMethods & gl = self->context->gl;

	if (self->query_obj[SAMPLES_PASSED]) {
		gl.BeginQuery(GL_SAMPLES_PASSED, self->query_obj[SAMPLES_PASSED]);
	}

	if (self->query_obj[ANY_SAMPLES_PASSED]) {
		gl.BeginQuery(GL_ANY_SAMPLES_PASSED, self->query_obj[ANY_SAMPLES_PASSED]);
	}

	if (self->query_obj[TIME_ELAPSED]) {
		gl.BeginQuery(GL_TIME_ELAPSED, self->query_obj[TIME_ELAPSED]);
	}

	if (self->query_obj[PRIMITIVES_GENERATED]) {
		gl.BeginQuery(GL_PRIMITIVES_GENERATED, self->query_obj[PRIMITIVES_GENERATED]);
	}

	Py_RETURN_NONE;
}

PyObject * MGLQuery_meth_end(MGLQuery * self) { TRACE_NOARGS
	const GLMethods & gl = self->context->gl;

	if (self->query_obj[SAMPLES_PASSED]) {
		gl.EndQuery(GL_SAMPLES_PASSED);
	}

	if (self->query_obj[ANY_SAMPLES_PASSED]) {
		gl.EndQuery(GL_ANY_SAMPLES_PASSED);
	}

	if (self->query_obj[TIME_ELAPSED]) {
		gl.EndQuery(GL_TIME_ELAPSED);
	}

	if (self->query_obj[PRIMITIVES_GENERATED]) {
		gl.EndQuery(GL_PRIMITIVES_GENERATED);
	}

	Py_RETURN_NONE;
}

PyObject * MGLQuery_meth_begin_render(MGLQuery * self) { TRACE_NOARGS
	if (self->query_obj[ANY_SAMPLES_PASSED]) {
		self->context->gl.BeginConditionalRender(self->query_obj[ANY_SAMPLES_PASSED], GL_QUERY_NO_WAIT);
	} else if (self->query_obj[SAMPLES_PASSED]) {
		self->context->gl.BeginConditionalRender(self->query_obj[SAMPLES_PASSED], GL_QUERY_NO_WAIT);
	} else {
		PyErr_Format(module_error, "no samples");
		return 0;
	}
	Py_RETURN_NONE;
}

PyObject * MGLQuery_meth_end_render(MGLQuery * self) { TRACE_NOARGS
	self->context->gl.EndConditionalRender();
	Py_RETURN_NONE;
}

PyObject * MGLQuery_get_samples(MGLQuery * self) { TRACE_GETTER
	int samples = 0;
	self->context->gl.GetQueryObjectiv(self->query_obj[SAMPLES_PASSED], GL_QUERY_RESULT, &samples);
	return PyLong_FromLong(samples);
}

PyObject * MGLQuery_get_primitives(MGLQuery * self) { TRACE_GETTER
	int primitives = 0;
	self->context->gl.GetQueryObjectiv(self->query_obj[PRIMITIVES_GENERATED], GL_QUERY_RESULT, &primitives);
	return PyLong_FromLong(primitives);
}

PyObject * MGLQuery_get_elapsed(MGLQuery * self) { TRACE_GETTER
	int elapsed = 0;
	self->context->gl.GetQueryObjectiv(self->query_obj[TIME_ELAPSED], GL_QUERY_RESULT, &elapsed);
	return PyLong_FromLong(elapsed);
}

PyTypeObject * MGLQuery_define(int version_code) {
	PyMethodDef MGLQuery_methods[] = {
		{"begin", (PyCFunction)MGLQuery_meth_begin, METH_NOARGS, 0},
		{"end", (PyCFunction)MGLQuery_meth_end, METH_NOARGS, 0},
		{"begin_render", (PyCFunction)MGLQuery_meth_begin_render, METH_NOARGS, 0},
		{"end_render", (PyCFunction)MGLQuery_meth_end_render, METH_NOARGS, 0},
		{0},
	};

	PyGetSetDef MGLQuery_getset[] = {
		{"samples", (getter)MGLQuery_get_samples, 0, 0, 0},
		{"primitives", (getter)MGLQuery_get_primitives, 0, 0, 0},
		{"elapsed", (getter)MGLQuery_get_elapsed, 0, 0, 0},
		{0},
	};

	PyType_Slot MGLQuery_slots[] = {
		{Py_tp_methods, dup(MGLQuery_methods)},
		{Py_tp_getset, dup(MGLQuery_getset)},
		{0},
	};

	PyType_Spec MGLQuery_spec = {"moderngl.mgl.Query", sizeof(MGLQuery), 0, Py_TPFLAGS_DEFAULT, MGLQuery_slots};
	return (PyTypeObject *)PyType_FromSpec(&MGLQuery_spec);
}
