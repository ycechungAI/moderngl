#include "Types.hpp"

#include "InlineMethods.hpp"

PyObject * MGLContext_query(MGLContext * self, PyObject * args) {
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

	// If none of them is set, all will be set.
	if (!(samples_passed + any_samples_passed + time_elapsed + primitives_generated)) {
		samples_passed = 1;
		any_samples_passed = 1;
		time_elapsed = 1;
		primitives_generated = 1;
	}

    MGLQuery * query = PyObject_New(MGLQuery, MGLQuery_type);
    query->released = false;

	Py_INCREF(self);
	query->context = self;

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

	// PyObject * result = PyTuple_New(2);
	// PyTuple_SET_ITEM(result, 0, (PyObject *)query);
	// PyTuple_SET_ITEM(result, 1, PyLong_FromLong(query->query_obj));
	// return result;

	return (PyObject *)query;
}

PyObject * MGLQuery_begin(MGLQuery * self, PyObject * args) {
	int args_ok = PyArg_ParseTuple(
		args,
		""
	);

	if (!args_ok) {
		return 0;
	}

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

PyObject * MGLQuery_end(MGLQuery * self, PyObject * args) {
	int args_ok = PyArg_ParseTuple(
		args,
		""
	);

	if (!args_ok) {
		return 0;
	}

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

PyObject * MGLQuery_begin_render(MGLQuery * self, PyObject * args) {
	int args_ok = PyArg_ParseTuple(
		args,
		""
	);

	if (!args_ok) {
		return 0;
	}

	const GLMethods & gl = self->context->gl;

	if (self->query_obj[ANY_SAMPLES_PASSED]) {
		gl.BeginConditionalRender(self->query_obj[ANY_SAMPLES_PASSED], GL_QUERY_NO_WAIT);
	} else if (self->query_obj[SAMPLES_PASSED]) {
		gl.BeginConditionalRender(self->query_obj[SAMPLES_PASSED], GL_QUERY_NO_WAIT);
	} else {
		MGLError_Set("no samples");
		return 0;
	}

	Py_RETURN_NONE;
}

PyObject * MGLQuery_end_render(MGLQuery * self, PyObject * args) {
	int args_ok = PyArg_ParseTuple(
		args,
		""
	);

	if (!args_ok) {
		return 0;
	}

	const GLMethods & gl = self->context->gl;
	gl.EndConditionalRender();
	Py_RETURN_NONE;
}

PyObject * MGLQuery_get_samples(MGLQuery * self) {
	const GLMethods & gl = self->context->gl;

	int samples = 0;
	gl.GetQueryObjectiv(self->query_obj[SAMPLES_PASSED], GL_QUERY_RESULT, &samples);

	return PyLong_FromLong(samples);
}

PyObject * MGLQuery_get_primitives(MGLQuery * self) {
	const GLMethods & gl = self->context->gl;

	int primitives = 0;
	gl.GetQueryObjectiv(self->query_obj[PRIMITIVES_GENERATED], GL_QUERY_RESULT, &primitives);

	return PyLong_FromLong(primitives);
}

PyObject * MGLQuery_get_elapsed(MGLQuery * self) {
	const GLMethods & gl = self->context->gl;

	int elapsed = 0;
	gl.GetQueryObjectiv(self->query_obj[TIME_ELAPSED], GL_QUERY_RESULT, &elapsed);

	return PyLong_FromLong(elapsed);
}

void MGLQuery_Invalidate(MGLQuery * query) {
	if (query->released) {
		return;
	}
	query->released = true;

	// TODO: decref

	// const GLMethods & gl = query->context->gl;

	// TODO: release

	Py_DECREF(query->context);
	Py_DECREF(query);
}
