#include "program_class.hpp"
#include "tools.hpp"

/* Slot offsets of the Program class */

PyTypeObject * Program_class;
int Program_class_mglo;
int Program_class_attributes;
int Program_class_uniforms;
int Program_class_extra;

/* Detect Program wrapper class */

void MGLProgram_init_wrapper() {
    int Program_slots = 0;
    Program_class = detect_class(moderngl, "Program", Program_slots);
    Program_class_mglo = slot_offset(Program_class, "_Program__mglo", Program_slots);
    Program_class_attributes = slot_offset(Program_class, "attributes", Program_slots);
    Program_class_uniforms = slot_offset(Program_class, "uniforms", Program_slots);
    Program_class_extra = slot_offset(Program_class, "extra", Program_slots);
    assert_slots_len(Program_class, Program_slots);
}
