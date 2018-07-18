#include "buffer_class.hpp"
#include "../internal/tools.hpp"
#include "../internal/modules.hpp"

#include "../buffer.hpp"

PyTypeObject * Buffer_class;
int Buffer_class_mglo;

void MGLBuffer_define(MGLContext * ctx) {
	PyType_Slot MGLBuffer_slots[] = {
		{0},
	};

	PyType_Spec MGLBuffer_spec = {mgl_name ".Buffer", sizeof(MGLBuffer), 0, Py_TPFLAGS_DEFAULT, MGLBuffer_slots};
	ctx->MGLBuffer_class = (PyTypeObject *)PyType_FromSpec(&MGLBuffer_spec);
}

void MGLBuffer_init_wrapper() {
	int Buffer_slots = 0;
	Buffer_class = detect_class(moderngl, "Buffer", Buffer_slots);
	Buffer_class_mglo = slot_offset(Buffer_class, "_Buffer__mglo", Buffer_slots);
	assert_slots_len(Buffer_class, Buffer_slots);
}
