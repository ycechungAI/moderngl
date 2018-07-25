#include "vertex_array_class.hpp"
#include "tools.hpp"

/* Slot offsets of the VertexArray class */

PyTypeObject * VertexArray_class;
int VertexArray_class_mglo;
int VertexArray_class_ibo;
int VertexArray_class_program;
int VertexArray_class_mode;
int VertexArray_class_vertices;
int VertexArray_class_extra;

/* Detect VertexArray wrapper class */

void MGLVertexArray_init_wrapper() {
    int VertexArray_slots = 0;
    VertexArray_class = detect_class(moderngl, "VertexArray", VertexArray_slots);
    VertexArray_class_mglo = slot_offset(VertexArray_class, "_VertexArray__mglo", VertexArray_slots);
    VertexArray_class_ibo = slot_offset(VertexArray_class, "_VertexArray__ibo", VertexArray_slots);
    VertexArray_class_program = slot_offset(VertexArray_class, "program", VertexArray_slots);
    VertexArray_class_mode = slot_offset(VertexArray_class, "mode", VertexArray_slots);
    VertexArray_class_vertices = slot_offset(VertexArray_class, "vertices", VertexArray_slots);
    VertexArray_class_extra = slot_offset(VertexArray_class, "extra", VertexArray_slots);
    assert_slots_len(VertexArray_class, VertexArray_slots);
}
