#include "attribute_class.hpp"
#include "tools.hpp"

/* Slot offsets of the Attribute class */

PyTypeObject * Attribute_class;
int Attribute_class_type;
int Attribute_class_location;
int Attribute_class_cols;
int Attribute_class_rows;
int Attribute_class_size;
int Attribute_class_shape;

/* Detect Attribute wrapper class */

void MGLAttribute_init_wrapper() {
    int Attribute_slots = 0;
    Attribute_class = detect_class(moderngl, "Attribute", Attribute_slots);
    Attribute_class_type = slot_offset(Attribute_class, "_Attribute__type", Attribute_slots);
    Attribute_class_location = slot_offset(Attribute_class, "location", Attribute_slots);
    Attribute_class_cols = slot_offset(Attribute_class, "cols", Attribute_slots);
    Attribute_class_rows = slot_offset(Attribute_class, "rows", Attribute_slots);
    Attribute_class_size = slot_offset(Attribute_class, "size", Attribute_slots);
    Attribute_class_shape = slot_offset(Attribute_class, "shape", Attribute_slots);
    assert_slots_len(Attribute_class, Attribute_slots);
}
