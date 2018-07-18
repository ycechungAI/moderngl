#include "buffer_class.hpp"
#include "../internal/tools.hpp"
#include "../internal/modules.hpp"

#include "../buffer.hpp"

/* Slot offsets of the Buffer class */

PyTypeObject * Buffer_class;
int Buffer_class_mglo;

/* Detect Buffer wrapper class */

void MGLBuffer_init_wrapper() {
	int Buffer_slots = 0;
	Buffer_class = detect_class(moderngl, "Buffer", Buffer_slots);
	Buffer_class_mglo = slot_offset(Buffer_class, "_Buffer__mglo", Buffer_slots);
	assert_slots_len(Buffer_class, Buffer_slots);
}
