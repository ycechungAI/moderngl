#include "uniform_class.hpp"
#include "tools.hpp"

/* Slot offsets of the Uniform class */

PyTypeObject * Uniform_class;
int Uniform_class_type;
int Uniform_class_location;
int Uniform_class_cols;
int Uniform_class_rows;
int Uniform_class_size;
int Uniform_class_shape;

/* Detect Uniform wrapper class */

void MGLUniform_init_wrapper() {
    int Uniform_slots = 0;
    Uniform_class = detect_class(moderngl, "Uniform", Uniform_slots);
    Uniform_class_type = slot_offset(Uniform_class, "_Uniform__type", Uniform_slots);
    Uniform_class_location = slot_offset(Uniform_class, "location", Uniform_slots);
    Uniform_class_cols = slot_offset(Uniform_class, "cols", Uniform_slots);
    Uniform_class_rows = slot_offset(Uniform_class, "rows", Uniform_slots);
    Uniform_class_size = slot_offset(Uniform_class, "size", Uniform_slots);
    Uniform_class_shape = slot_offset(Uniform_class, "shape", Uniform_slots);
    assert_slots_len(Uniform_class, Uniform_slots);
}
