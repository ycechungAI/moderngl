#pragma once
#include "types.hpp"

PyObject * MGLContext_meth_query(MGLContext * self, PyObject * args);

PyTypeObject * MGLQuery_define(int version_code);

enum MGLQueryKeys {
	SAMPLES_PASSED,
	ANY_SAMPLES_PASSED,
	TIME_ELAPSED,
	PRIMITIVES_GENERATED,
};
