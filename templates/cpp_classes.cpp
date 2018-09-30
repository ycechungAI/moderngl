#include "cpp_classes.hpp"
///
//! for include in includes:
#include "/*{include}*/"
//! endfor
///
//! for c_name, methods in c_methods:
//! for m_name, m_name_1, m_name_2, m_args in methods:
extern PyObject * /*{m_name_1}*/(/*{c_name}*/ * self, PyObject * const * args, Py_ssize_t nargs);
//! endfor
//! if not loop.last
///
//! endif
//! endfor
///
#if PY_VERSION_HEX >= 0x03070000
///
//! for c_name, methods in c_methods:
PyMethodDef /*{c_name}*/_methods[] = {
    //! for m_name, m_name_1, m_name_2, m_args in methods:
    {"/*{m_name}*/", (PyCFunction)/*{m_name_1}*/, /*{'METH_FASTCALL' if m_args else 'METH_NOARGS'}*/, 0},
    //! endfor
    {0},
};
//! if not loop.last
///
//! endif
//! endfor
///
#else
///
//! for c_name, methods in c_methods:
//! for m_name, m_name_1, m_name_2, m_args in methods:
//! if m_args
PyObject * /*{m_name_2}*/(/*{c_name}*/ * self, PyObejct * args) {
    return /*{m_name_1}*/(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}
//! if not loop.last
///
//! endif
//! endif
//! endfor
//! if not loop.last
///
//! endif
//! endfor
///
//! for c_name, methods in c_methods:
PyMethodDef /*{c_name}*/_methods[] = {
    //! for m_name, m_name_1, m_name_2, m_args in methods:
    {"/*{m_name}*/", (PyCFunction)/*{m_name_2}*/, /*{'METH_VARARGS' if m_args else 'METH_NOARGS'}*/, 0},
    //! endfor
    {0},
};
//! if not loop.last
///
//! endif
//! endfor
///
#endif
///
//! for c_name in c_names:
PyType_Slot /*{c_name}*/_slots[] = {
    {Py_tp_methods, /*{c_name}*/_methods},
    {0},
};
//! if not loop.last
///
//! endif
//! endfor
///
//! for c_name in c_names:
PyType_Spec /*{c_name}*/_spec = {
    mgl_name "./*{c_name[3:]}*/",
    sizeof(/*{c_name}*/),
    0,
    Py_TPFLAGS_DEFAULT,
    /*{c_name}*/_slots,
};
//! if not loop.last
///
//! endif
//! endfor
///
