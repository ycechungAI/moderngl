#include "mgl.hpp"
#include "extensions.hpp"
#include "internal/modules.hpp"

#include "context.hpp"
#include "buffer.hpp"
#include "framebuffer.hpp"
#include "limits.hpp"
#include "program.hpp"
#include "query.hpp"
#include "renderbuffer.hpp"
#include "sampler.hpp"
#include "scope.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"

#include "generated/py_classes.hpp"
#include "generated/cpp_classes.hpp"
#include "generated/methods.hpp"

/* moderngl.core.initialize()
 * Initializes internal objects that cannot be initialized import time.
 */
PyObject * meth_initialize(PyObject * self) {
    static bool initialized = false;
    if (initialized) {
        Py_RETURN_NONE;
    }

    initialized = true;

    if (!load_modules()) {
        return 0;
    }

    /* Define MGLContext only.
     * The rest of the internal types will be defined by MGLContext.
     */

    MGLContext_class = (PyTypeObject *)PyType_FromSpec(&MGLContext_spec);

    /* Detect wrapper classes for internal types */

    init_wrappers();

    /* Errors are not recoverable at this point */

    if (PyErr_Occurred()) {
        return 0;
    }

    Py_RETURN_NONE;
}

/* moderngl.core.glprocs(context)
 */
PyObject * meth_glprocs(PyObject * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 1) {
        // TODO: error
        return 0;
    }

    if (args[0]->ob_type != Context_class) {
        // TODO: error
        return 0;
    }

    MGLContext * ctx = SLOT(args[0], MGLContext, Context_class_mglo);
    return PyMemoryView_FromMemory((char *)&ctx->gl, sizeof(ctx->gl), PyBUF_WRITE);
}

/* moderngl.core.release(obj)
 */
PyObject * meth_release(PyObject * self, PyObject * const * args, Py_ssize_t nargs) {
    if (nargs != 1) {
        return 0;
    }

    PyObject * obj = args[0];

    if (obj->ob_type == Buffer_class) {
        MGLBuffer * buffer = MGLObject_pop_mglo(Buffer, obj);
        const GLMethods & gl = buffer->context->gl;
        gl.DeleteBuffers(1, (GLuint *)&buffer->buffer_obj);
        return MGLObject_release(buffer);
    }

    if (obj->ob_type == Framebuffer_class) {
        MGLFramebuffer * framebuffer = MGLObject_pop_mglo(Framebuffer, obj);
        const GLMethods & gl = framebuffer->context->gl;
        gl.DeleteFramebuffers(1, (GLuint *)&framebuffer->framebuffer_obj);
        return MGLObject_release(framebuffer);
    }

    if (obj->ob_type == Program_class) {
        MGLProgram * program = MGLObject_pop_mglo(Program, obj);
        const GLMethods & gl = program->context->gl;
        gl.DeleteProgram(program->program_obj);
        for (int i = 0; i < 5; ++i) {
            if (program->shader_obj[i]) {
                gl.DeleteShader(program->shader_obj[i]);
            }
        }
        return MGLObject_release(program);
    }

    if (obj->ob_type == Query_class) {
        MGLQuery * query = MGLObject_pop_mglo(Query, obj);
        const GLMethods & gl = query->context->gl;
        for (int i = 0; i < 5; ++i) {
            if (query->query_obj[i]) {
                gl.DeleteQueries(1, (GLuint *)&query->query_obj[i]);
            }
        }
        return MGLObject_release(query);
    }

    if (obj->ob_type == Renderbuffer_class) {
        MGLRenderbuffer * renderbuffer = MGLObject_pop_mglo(Renderbuffer, obj);
        const GLMethods & gl = renderbuffer->context->gl;
        gl.DeleteRenderbuffers(1, (GLuint *)&renderbuffer->renderbuffer_obj);
        return MGLObject_release(renderbuffer);
    }

    if (obj->ob_type == Sampler_class) {
        MGLSampler * sampler = MGLObject_pop_mglo(Sampler, obj);
        const GLMethods & gl = sampler->context->gl;
        gl.DeleteSamplers(1, (GLuint *)&sampler->sampler_obj);
        return MGLObject_release(sampler);
    }

    if (obj->ob_type == Scope_class) {
        MGLScope * scope = MGLObject_pop_mglo(Scope, obj);
        return MGLObject_release(scope);
    }

    if (obj->ob_type == Texture_class) {
        MGLTexture * texture = MGLObject_pop_mglo(Texture, obj);
        const GLMethods & gl = texture->context->gl;
        gl.DeleteTextures(1, (GLuint *)&texture->texture_obj);
        return MGLObject_release(texture);
    }

    if (obj->ob_type == VertexArray_class) {
        MGLVertexArray * vertex_array = MGLObject_pop_mglo(VertexArray, obj);
        const GLMethods & gl = vertex_array->context->gl;
        gl.DeleteVertexArrays(1, (GLuint *)&vertex_array->vertex_array_obj);
        return MGLObject_release(vertex_array);
    }

    return 0;
}

/* Module definition */

PyModuleDef mgl_def = {PyModuleDef_HEAD_INIT, mgl_name, 0, -1, module_methods, 0, 0, 0, 0};

extern "C" PyObject * PyInit_mgl() {
    PyObject * module = PyModule_Create(&mgl_def);
    return module;
}
