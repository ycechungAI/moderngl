#include "mgl.hpp"
#include "extensions.hpp"

#include "buffer.hpp"
#include "compute_shader.hpp"
#include "context.hpp"
#include "framebuffer.hpp"
#include "limits.hpp"
#include "program.hpp"
#include "query.hpp"
#include "recorder.hpp"
#include "renderbuffer.hpp"
#include "sampler.hpp"
#include "scope.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"

#include "internal/wrapper.hpp"
#include "internal/data_type.hpp"
#include "adaptor/adaptor.hpp"

/* moderngl.core.glprocs(context)
 */
PyObject * meth_glprocs(PyObject * self, PyObject * context) {
    if (!Context_Check(context)) {
        // TODO: error
        return 0;
    }

    MGLContext * ctx = (MGLContext *)get_slot(context, "mglo");
    return PyMemoryView_FromMemory((char *)&ctx->gl, sizeof(ctx->gl), PyBUF_WRITE);
}

/* moderngl.core.release(obj)
 */
PyObject * meth_release(PyObject * self, PyObject * obj) {
    PyErr_BadInternalCall();
    return 0;
}

fastcallable(meth_create_context)

PyMethodDef module_methods[] = {
    {"create_context", fastcall(meth_create_context), fastcall_flags, NULL},
    {"extensions", (PyCFunction)meth_extensions, METH_O, 0},
    {"hwinfo", (PyCFunction)meth_hwinfo, METH_O, 0},
    // {"inspect", (PyCFunction)meth_inspect, METH_O, 0},
    {"glprocs", (PyCFunction)meth_glprocs, METH_O, 0},
    {"release", (PyCFunction)meth_release, METH_O, 0},

    {"extend_context", (PyCFunction)meth_extend_context, METH_VARARGS, 0},
    {"extend_buffer", (PyCFunction)meth_extend_buffer, METH_VARARGS, 0},
    {0},
};

/* Module definition */

PyModuleDef module_def = {PyModuleDef_HEAD_INIT, "moderngl.mgl.new", 0, -1, module_methods, 0, 0, 0, 0};

inline PyTypeObject * _new_type(PyObject * module, const char * name, PyType_Spec * spec) {
    PyObject * type = must_have(PyType_FromSpec(spec));
    PyModule_AddObject(module, name, new_ref(type));
    return (PyTypeObject *)type;
};

#define new_type(name) name ## _class = _new_type(module, #name, &name ## _spec)

PyObject * PyInit_mgl_new() {
    PyObject * module = PyModule_Create(&module_def);

    interns = PyList_New(0);
    PyModule_AddObject(module, "interns", interns);

    new_type(MGLBuffer);
    new_type(MGLComputeShader);
    new_type(MGLContext);
    new_type(MGLFramebuffer);
    new_type(MGLProgram);
    new_type(MGLQuery);
    new_type(MGLRecorder);
    new_type(MGLRenderbuffer);
    new_type(MGLSampler);
    new_type(MGLScope);
    new_type(MGLTexture);
    new_type(MGLVertexArray);

    f1.numpy_dtype = intern(PyUnicode_FromString("f1"));
    f2.numpy_dtype = intern(PyUnicode_FromString("f2"));
    f4.numpy_dtype = intern(PyUnicode_FromString("f4"));
    u1.numpy_dtype = intern(PyUnicode_FromString("u1"));
    u2.numpy_dtype = intern(PyUnicode_FromString("u2"));
    u4.numpy_dtype = intern(PyUnicode_FromString("u4"));
    i1.numpy_dtype = intern(PyUnicode_FromString("i1"));
    i2.numpy_dtype = intern(PyUnicode_FromString("i2"));
    i4.numpy_dtype = intern(PyUnicode_FromString("i4"));

    return module;
}
