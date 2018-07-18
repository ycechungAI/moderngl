#include "context_class.hpp"
#include "../internal/tools.hpp"
#include "../internal/modules.hpp"

#include "../context.hpp"
#include "../buffer.hpp"

PyTypeObject * MGLContext_class;
PyTypeObject * Context_class;
int Context_class_mglo;

void MGLContext_define() {
	PyMethodDef MGLContext_methods[] = {
		METHOD(MGLContext, buffer, true),
		{0},
	};

	PyType_Slot MGLContext_slots[] = {
		{Py_tp_methods, dup(MGLContext_methods)},
		{0},
	};

	PyType_Spec MGLContext_spec = {mgl_name ".Context", sizeof(MGLContext), 0, Py_TPFLAGS_DEFAULT, MGLContext_slots};
	MGLContext_class = (PyTypeObject *)PyType_FromSpec(&MGLContext_spec);
}

void MGLContext_init_wrapper() {
	int Context_slots = 0;
	Context_class = detect_class(moderngl, "Context", Context_slots);
	Context_class_mglo = slot_offset(Context_class, "_Context__mglo", Context_slots);
	assert_slots_len(Context_class, Context_slots);
}
