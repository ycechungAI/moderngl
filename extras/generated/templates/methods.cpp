#include "methods.hpp"
///
//! for m_name, m_name_1, m_name_2, m_args in c_methods:
//! if m_args
extern PyObject * /*{m_name_1}*/(PyObject * self, PyObject * const * args, Py_ssize_t nargs);
//! else
extern PyObject * /*{m_name_1}*/(PyObject * self);
//! endif
//! endfor
///
#if PY_VERSION_HEX >= 0x03070000
///
PyMethodDef module_methods[] = {
    //! for m_name, m_name_1, m_name_2, m_args in c_methods:
    {"/*{m_name}*/", (PyCFunction)/*{m_name_1}*/, /*{'METH_FASTCALL' if m_args else 'METH_NOARGS'}*/, 0},
    //! endfor
    {0},
};
///
#else
///
//! for m_name, m_name_1, m_name_2, m_args in c_methods:
//! if m_args
PyObject * /*{m_name_2}*/(PyObject * self, PyObject * args) {
    return /*{m_name_1}*/(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size);
}
//! if not loop.last
///
//! endif
//! endif
//! endfor
///
PyMethodDef module_methods[] = {
    //! for m_name, m_name_1, m_name_2, m_args in c_methods:
    {"/*{m_name}*/", (PyCFunction)/*{m_name_2}*/, /*{'METH_VARARGS' if m_args else 'METH_NOARGS'}*/, 0},
    //! endfor
    {0},
};
///
#endif
///
