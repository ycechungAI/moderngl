#include "buffer_class.hpp"
#include "tools.hpp"

/* Slot offsets of the Buffer class */

PyTypeObject * Buffer_class;
int Buffer_class_mglo;
int Buffer_class_size;
int Buffer_class_extra;

/* Detect Buffer wrapper class */

void MGLBuffer_init_wrapper() {
    int Buffer_slots = 0;
    Buffer_class = detect_class(moderngl, "Buffer", Buffer_slots);
    Buffer_class_mglo = slot_offset(Buffer_class, "_Buffer__mglo", Buffer_slots);
    Buffer_class_size = slot_offset(Buffer_class, "size", Buffer_slots);
    Buffer_class_extra = slot_offset(Buffer_class, "extra", Buffer_slots);
    assert_slots_len(Buffer_class, Buffer_slots);
}
