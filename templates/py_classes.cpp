#include "py_classes.hpp"

//! for name, slots in c_slots:
PyTypeObject * /*{name}*/_class;
//! for slot in slots:
int /*{name}*/_class_/*{slot.lstrip('_')}*/;
//! endfor
///
//! endfor

void init_wrappers() {
//! for name, slots in c_slots:
    int /*{name}*/_slots = 0;
    /*{name}*/_class = detect_class(moderngl, "/*{name}*/", /*{name}*/_slots);
//! for slot in slots:
    /*{name}*/_class_/*{slot.lstrip('_')}*/ = slot_offset(/*{name}*/_class, "/*{('_' + name + slot) if slot.startswith('__') else slot}*/", /*{name}*/_slots);
//! endfor
    assert_slots_len(/*{name}*/_class, /*{name}*/_slots);
//! if not loop.last
///
//! endif
//! endfor
}
///
