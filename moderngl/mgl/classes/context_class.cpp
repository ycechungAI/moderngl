#include "context_class.hpp"
#include "../internal/tools.hpp"
#include "../internal/modules.hpp"

#include "../context.hpp"
#include "../buffer.hpp"

/* Slot offsets of the Context class */

PyTypeObject * MGLContext_class;
PyTypeObject * Context_class;
int Context_class_mglo;
int Context_class_version_code;
int Context_class_limits;
int Context_class_extra;

/* Detect Context wrapper class */

void MGLContext_init_wrapper() {
    int Context_slots = 0;
    Context_class = detect_class(moderngl, "Context", Context_slots);
    Context_class_mglo = slot_offset(Context_class, "_Context__mglo", Context_slots);
    Context_class_version_code = slot_offset(Context_class, "version_code", Context_slots);
    Context_class_limits = slot_offset(Context_class, "limits", Context_slots);
    Context_class_extra = slot_offset(Context_class, "extra", Context_slots);
    assert_slots_len(Context_class, Context_slots);
}
