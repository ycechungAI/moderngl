#include "sampler.hpp"
#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"
#include "internal/modules.hpp"
#include "internal/tools.hpp"
#include "internal/glsl.hpp"

PyObject * MGLContext_meth_sampler(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 0) {
        // TODO: error
        return 0;
    }

    MGLSampler * sampler = MGLContext_new_object(self, Sampler);
    return NEW_REF(sampler->wrapper);
}

PyObject * MGLSampler_meth_write(MGLSampler * self, PyObject * const * args, Py_ssize_t nargs) {
    Py_RETURN_NONE;
}
