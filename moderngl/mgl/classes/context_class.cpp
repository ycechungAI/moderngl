#include "context_class.hpp"
#include "../internal/tools.hpp"
#include "../internal/modules.hpp"

#include "../context.hpp"
#include "../buffer.hpp"

PyTypeObject * MGLContext_class;
PyTypeObject * Context_class;
int Context_class_mglo;

void MGLContext_init_wrapper() {
	int Context_slots = 0;
	Context_class = detect_class(moderngl, "Context", Context_slots);
	Context_class_mglo = slot_offset(Context_class, "_Context__mglo", Context_slots);
	assert_slots_len(Context_class, Context_slots);
}
