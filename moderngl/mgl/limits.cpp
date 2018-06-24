#include "limits.hpp"
#include "tools.hpp"

PyObject * get_limits(const GLMethods & gl) {
	PyObject * limits = new_object(PyObject, Limits_class);
	SLOT(limits, PyObject, Limits_class_max_samples) = PyLong_FromLong(1234);
	return limits;
}
