#pragma once

#include "tools.hpp"

//! for name, slots in c_slots:
extern PyTypeObject * /*{name}*/_class;
//! for slot in slots:
extern int /*{name}*/_class_/*{slot.lstrip('_')}*/;
//! endfor
///
//! endfor

void init_wrappers();
///
