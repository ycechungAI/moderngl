#include "vertex_array.hpp"
#include "buffer.hpp"
#include "context.hpp"
#include "program.hpp"
#include "scope.hpp"

#include "internal/wrapper.hpp"

int MGLVertexArray_set_ibo(MGLVertexArray * self, PyObject * value);

/* MGLContext.vertex_array(program, content, index_buffer)
 */
PyObject * MGLContext_meth_vertex_array(MGLContext * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(3);

    PyObject * program = args[0];
    PyObject * content = args[1];
    PyObject * index_buffer = args[2];

    if (index_buffer != Py_None && !Buffer_Check(index_buffer)) {
        return 0;
    }

    PyObject * attributes = get_slot(program, "attributes");

    content = PySequence_Fast(content, "content is not iterable");
    if (!content) {
        return 0;
    }

    MGLVertexArray * vertex_array = PyObject_New(MGLVertexArray, MGLVertexArray_class);
    chain_objects(self, vertex_array);
    vertex_array->context = self;

    const GLMethods & gl = self->gl;
    gl.GenVertexArrays(1, (GLuint *)&vertex_array->vertex_array_obj);

    if (!vertex_array->vertex_array_obj) {
        PyErr_Format(moderngl_error, "cannot create vertex array");
        return 0;
    }

    self->bind_vertex_array(vertex_array->vertex_array_obj);

    int vertices = 0x7fffffff;
    int content_len = (int)PySequence_Fast_GET_SIZE(content);

    for (int i = 0; i < content_len; ++i) {
        PyObject * item = PySequence_Fast_GET_ITEM(content, i);
        int item_len = (int)PySequence_Fast_GET_SIZE(item);
        if (item_len < 3) {
            PyErr_Format(moderngl_error, "err1");
            return 0;
        }

        PyObject * buffer_wrapper = PySequence_Fast_GET_ITEM(item, 0);
        if (!Buffer_Check(buffer_wrapper)) {
            return 0;
        }

        PyObject * format = PySequence_Fast_GET_ITEM(item, 1);
        if (!PyUnicode_Check(format)) {
            PyErr_Format(moderngl_error, "err2");
            return 0;
        }

        PyObject * format_split = PyObject_CallFunctionObjArgs(moderngl_split_format, format, 0);
        if (!format_split) {
            return 0;
        }

        MGLBuffer * buffer = (MGLBuffer *)get_slot(buffer_wrapper, "mglo");
        self->bind_array_buffer(buffer->buffer_obj);

        PyObject * nodes = PyTuple_GET_ITEM(format_split, 0);
        int divisor = PyLong_AsLong(PyTuple_GET_ITEM(format_split, 1));
        int stride = PyLong_AsLong(PyTuple_GET_ITEM(format_split, 2));

        if (!i && divisor) {
            return 0;
        }

        if (!divisor) {
            int cap = (int)(buffer->size / stride);
            vertices = vertices < cap ? vertices : cap;
        }

        int nodes_len = (int)PySequence_Fast_GET_SIZE(nodes);
        int nodes_cnt = 0;

        char * ptr = 0;
        for (int j = 2; j < item_len; ++j) {
            PyObject * name = PySequence_Fast_GET_ITEM(item, j);

            if (!PyUnicode_Check(name)) {
                PyErr_Format(moderngl_error, "err4");
                return 0;
            }

            PyObject * attrib = PyDict_GetItem(attributes, name);
            if (!attrib) {
                ptr += stride;
                // PyErr_Format(moderngl_error, "err4");
                // return 0;
                continue;
            }

            int location = PyLong_AsLong(get_slot(attrib, "location"));
            int rows = PyLong_AsLong(get_slot(attrib, "rows"));
            int size = PyLong_AsLong(get_slot(attrib, "size"));

            int count = 0;
            int shape = 0;
            int bytes = 0;
            int type = 0;

            while (true) {
                if (nodes_cnt == nodes_len) {
                    PyErr_Format(moderngl_error, "err4");
                    return 0;
                }

                PyObject * tup = PySequence_Fast_GET_ITEM(nodes, nodes_cnt);
                count = PyLong_AsLong(PyTuple_GET_ITEM(tup, 0));
                shape = PyUnicode_READ_CHAR(PyTuple_GET_ITEM(tup, 1), 0);
                bytes = PyLong_AsLong(PyTuple_GET_ITEM(tup, 2));
                nodes_cnt += 1;

                if (shape != 'x') {
                    break;
                }
            }

            switch (shape) {
                case 'f':
                    switch (bytes) {
                        case 1: type = GL_UNSIGNED_BYTE; break;
                        case 2: type = GL_HALF_FLOAT; break;
                        case 4: type = GL_FLOAT; break;
                        case 8: type = GL_DOUBLE; break;
                    }
                    break;
                case 'i':
                    switch (bytes) {
                        case 1: type = GL_BYTE; break;
                        case 2: type = GL_SHORT; break;
                        case 4: type = GL_INT; break;
                    }
                    break;
                case 'u':
                    switch (bytes) {
                        case 1: type = GL_UNSIGNED_BYTE; break;
                        case 2: type = GL_UNSIGNED_SHORT; break;
                        case 4: type = GL_UNSIGNED_INT; break;
                    }
                    break;
            }

            if (!type) {
                return 0;
            }

            int locations = rows * size;
            int vsize = count / rows;

            for (int r = 0; r < locations; ++r) {
                switch (shape) {
                    case 'f': gl.VertexAttribPointer(location, vsize, type, shape == 'f' && bytes == 1, stride, ptr); break;
                    case 'i': gl.VertexAttribIPointer(location, vsize, type, stride, ptr); break;
                    case 'u': gl.VertexAttribIPointer(location, vsize, type, stride, ptr); break;
                    case 'd': gl.VertexAttribLPointer(location, vsize, type, stride, ptr); break;
                }

                gl.VertexAttribDivisor(location, divisor);
                gl.EnableVertexAttribArray(location);
                ptr += count * bytes / rows;
                location += 1;
            }
        }

        Py_DECREF(item);
    }

    vertex_array->max_vertices = vertices;

    vertex_array->wrapper = VertexArray_New("OOOOI", vertex_array, program, Py_None, Py_None, vertices);

    vertex_array->index_buffer = NULL;
    MGLVertexArray_set_ibo(vertex_array, index_buffer);

    return vertex_array->wrapper;
}

/* MGLVertexArray.render(mode, vertices, first, instances, color_mask, depth_mask)
 */
PyObject * MGLVertexArray_meth_render(MGLVertexArray * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(6);

    PyObject * mode = args[0];
    int vertices = PyLong_AsLong(args[1]);
    int first = PyLong_AsLong(args[2]);
    int instances = PyLong_AsLong(args[3]);
    unsigned long long color_mask = PyLong_AsUnsignedLongLong(args[4]);
    bool depth_mask = (bool)PyObject_IsTrue(args[5]);

    if (vertices < 0) {
        vertices = PyLong_AsLong(get_slot(self->wrapper, "vertices"));
    }

    if (mode == Py_None) {
        mode = get_slot(self->wrapper, "mode");
        if (mode == Py_None) {
            mode = triangles_long;
        }
    }

    int render_mode = PyLong_AsLong(mode);
    ensure_no_error();

    const GLMethods & gl = self->context->gl;

    PyObject * program = get_slot(self->wrapper, "program");
    self->context->use_program(((MGLProgram *)get_slot(program, "mglo"))->program_obj);
    self->context->bind_vertex_array(self->vertex_array_obj);
    self->context->set_write_mask(color_mask, depth_mask);

    bool scoped = false;
    PyObject * scope = get_slot(self->wrapper, "scope");
    MGLScope * scope_mglo = 0;

    if (scope != Py_None) {
        scope_mglo = (MGLScope *)get_slot(scope, "mglo");
        if (self->context->bound_scope != scope_mglo) {
            MGLScope_begin_core(scope_mglo);
            scoped = true;
        }
    } else if (self->context->bound_scope != self->context->active_scope) {
        scope_mglo = self->context->bound_scope;
        MGLScope_begin_core(scope_mglo);
        scoped = true;
    }

    if (self->index_buffer) {
        const void * ptr = (const void *)((GLintptr)first * 4);
        gl.DrawElementsInstanced(render_mode, vertices, GL_UNSIGNED_INT, ptr, instances);
    } else {
        gl.DrawArraysInstanced(render_mode, first, vertices, instances);
    }

    if (scoped) {
        MGLScope_end_core(scope_mglo);
    }

    Py_RETURN_NONE;
}

/* MGLVertexArray.render_indirect(buffer, mode, count, first, color_mask, depth_mask)
 */
PyObject * MGLVertexArray_meth_render_indirect(MGLVertexArray * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(4);

    if (!Buffer_Check(args[0])) {
        // TODO: error
        return 0;
    }

    MGLBuffer * indirect_buffer = (MGLBuffer *)get_slot(args[0], "mglo");
    PyObject * mode = args[1];
    int count = PyLong_AsLong(args[2]);
    int first = PyLong_AsLong(args[3]);
    unsigned long long color_mask = PyLong_AsUnsignedLongLong(args[4]);
    bool depth_mask = (bool)PyObject_IsTrue(args[5]);

    if (count < 0) {
        count = (int)(self->index_buffer ? self->index_buffer->size / 4 : indirect_buffer->size / 20);
    }

    if (mode == Py_None) {
        mode = get_slot(self->wrapper, "mode");
        if (mode == Py_None) {
            mode = triangles_long;
        }
    }

    int render_mode = PyLong_AsLong(mode);
    ensure_no_error();

    const GLMethods & gl = self->context->gl;

    PyObject * program = get_slot(self->wrapper, "program");
    self->context->use_program(((MGLProgram *)get_slot(program, "mglo"))->program_obj);
    self->context->bind_vertex_array(self->vertex_array_obj);
    self->context->set_write_mask(color_mask, depth_mask);

    bool scoped = false;
    PyObject * scope = get_slot(self->wrapper, "scope");
    MGLScope * scope_mglo = 0;

    if (scope != Py_None) {
        scope_mglo = (MGLScope *)get_slot(scope, "mglo");
        if (self->context->bound_scope != scope_mglo) {
            MGLScope_begin_core(scope_mglo);
            scoped = true;
        }
    } else if (self->context->bound_scope != self->context->active_scope) {
        scope_mglo = self->context->bound_scope;
        MGLScope_begin_core(scope_mglo);
        scoped = true;
    }

	gl.BindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer->buffer_obj);

	const void * ptr = (const void *)((GLintptr)first * 20);

	if (self->index_buffer) {
		gl.MultiDrawElementsIndirect(render_mode, GL_UNSIGNED_INT, ptr, count, 20);
	} else {
		gl.MultiDrawArraysIndirect(render_mode, ptr, count, 20);
	}

    if (scoped) {
        MGLScope_end_core(scope_mglo);
    }

    Py_RETURN_NONE;
}

/* MGLVertexArray.transform(output, mode, vertices, first, instances, flush)
 */
PyObject * MGLVertexArray_meth_transform(MGLVertexArray * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(6);

    if (!Buffer_Check(args[0])) {
        // TODO: error
        return 0;
    }

    MGLBuffer * output_buffer = (MGLBuffer *)get_slot(args[0], "mglo");
    PyObject * mode = args[1];
    int vertices = PyLong_AsLong(args[2]);
    int first = PyLong_AsLong(args[3]);
    int instances = PyLong_AsLong(args[4]);
    int flush = PyObject_IsTrue(args[5]);

    if (vertices < 0) {
        vertices = PyLong_AsLong(get_slot(self->wrapper, "vertices"));
    }

    if (mode == Py_None) {
        mode = get_slot(self->wrapper, "mode");
        if (mode == Py_None) {
            mode = points_long;
        }
    }

    int render_mode = PyLong_AsLong(mode);
    ensure_no_error();

    const GLMethods & gl = self->context->gl;

    PyObject * program = get_slot(self->wrapper, "program");
    self->context->use_program(((MGLProgram *)get_slot(program, "mglo"))->program_obj);
    self->context->bind_vertex_array(self->vertex_array_obj);

    bool scoped = false;
    PyObject * scope = get_slot(self->wrapper, "scope");
    MGLScope * scope_mglo = 0;

    if (scope != Py_None) {
        scope_mglo = (MGLScope *)get_slot(scope, "mglo");
        if (self->context->bound_scope != scope_mglo) {
            MGLScope_begin_core(scope_mglo);
            scoped = true;
        }
    } else if (self->context->bound_scope != self->context->active_scope) {
        scope_mglo = self->context->bound_scope;
        MGLScope_begin_core(scope_mglo);
        scoped = true;
    }

    gl.BindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, output_buffer->buffer_obj);
    gl.Enable(GL_RASTERIZER_DISCARD);
    gl.BeginTransformFeedback(render_mode);

    if (self->index_buffer) {
        const void * ptr = (const void *)((GLintptr)first * 4);
        gl.DrawElementsInstanced(render_mode, vertices, GL_UNSIGNED_INT, ptr, instances);
    } else {
        gl.DrawArraysInstanced(render_mode, first, vertices, instances);
    }

    gl.EndTransformFeedback();
    if (~self->context->current_enable_only & MGL_RASTERIZER_DISCARD) {
        gl.Disable(GL_RASTERIZER_DISCARD);
    }

    if (scoped) {
        MGLScope_end_core(scope_mglo);
    }

    if (flush) {
        gl.Flush();
    }

    Py_RETURN_NONE;
}

/* MGLVertexArray.transform_indirect(output, buffer, mode, count, first, flush)
 */
PyObject * MGLVertexArray_meth_transform_indirect(MGLVertexArray * self, PyObject * const * args, Py_ssize_t nargs) {
    ensure_num_args(6);

    if (!Buffer_Check(args[0])) {
        // TODO: error
        return 0;
    }

    if (!Buffer_Check(args[1])) {
        // TODO: error
        return 0;
    }

    MGLBuffer * output_buffer = (MGLBuffer *)get_slot(args[0], "mglo");
    MGLBuffer * indirect_buffer = (MGLBuffer *)get_slot(args[1], "mglo");

    PyObject * mode = args[2];
    int count = PyLong_AsLong(args[3]);
    int first = PyLong_AsLong(args[4]);
    int flush = PyObject_IsTrue(args[5]);

    if (count < 0) {
        count = (int)(self->index_buffer ? self->index_buffer->size / 4 : indirect_buffer->size / 20);
    }

    if (mode == Py_None) {
        mode = get_slot(self->wrapper, "mode");
        if (mode == Py_None) {
            mode = points_long;
        }
    }

    int render_mode = PyLong_AsLong(mode);
    ensure_no_error();

    const GLMethods & gl = self->context->gl;

    PyObject * program = get_slot(self->wrapper, "program");
    self->context->use_program(((MGLProgram *)get_slot(program, "mglo"))->program_obj);
    self->context->bind_vertex_array(self->vertex_array_obj);

    // TODO: mglo of mglo??
    MGLBuffer * output = output_buffer; // (MGLBuffer *)get_slot(output_buffer, "mglo");
    gl.BindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, output->buffer_obj);
    gl.Enable(GL_RASTERIZER_DISCARD);
    gl.BeginTransformFeedback(render_mode);

    bool scoped = false;
    PyObject * scope = get_slot(self->wrapper, "scope");
    MGLScope * scope_mglo = 0;

    if (scope != Py_None) {
        scope_mglo = (MGLScope *)get_slot(scope, "mglo");
        if (self->context->bound_scope != scope_mglo) {
            MGLScope_begin_core(scope_mglo);
            scoped = true;
        }
    } else if (self->context->bound_scope != self->context->active_scope) {
        scope_mglo = self->context->bound_scope;
        MGLScope_begin_core(scope_mglo);
        scoped = true;
    }

	gl.BindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer->buffer_obj);

	const void * ptr = (const void *)((GLintptr)first * 20);

	if (self->index_buffer) {
		gl.MultiDrawElementsIndirect(render_mode, GL_UNSIGNED_INT, ptr, count, 20);
	} else {
		gl.MultiDrawArraysIndirect(render_mode, ptr, count, 20);
	}

    gl.EndTransformFeedback();
    if (~self->context->current_enable_only & MGL_RASTERIZER_DISCARD) {
        gl.Disable(GL_RASTERIZER_DISCARD);
    }

    if (scoped) {
        MGLScope_end_core(scope_mglo);
    }

    if (flush) {
        gl.Flush();
    }

    Py_RETURN_NONE;
}

/* MGLVertexArray.ibo
 */
int MGLVertexArray_set_ibo(MGLVertexArray * self, PyObject * value) {
    if (value != Py_None && !Buffer_Check(value)) {
        PyErr_Format(PyExc_TypeError, "expected Buffer got %s", value->ob_type->tp_name);
        return -1;
    }

    Py_XDECREF(self->index_buffer);

    self->context->bind_vertex_array(self->vertex_array_obj);

    if (value == Py_None) {
        self->index_buffer = NULL;
        set_slot(self->wrapper, "vertices", PyLong_FromLong(self->max_vertices));
        self->context->gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    } else {
        self->index_buffer = (MGLBuffer *)get_slot(value, "mglo");
        set_slot(self->wrapper, "vertices", PyLong_FromLong((int)(self->index_buffer->size / 4)));
        self->context->gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->index_buffer->buffer_obj);
    }

    return 0;
}

void MGLVertexArray_dealloc(MGLVertexArray * self) {
    printf("MGLVertexArray_dealloc\n");
    Py_TYPE(self)->tp_free(self);
}

fastcallable(MGLVertexArray_meth_render)
fastcallable(MGLVertexArray_meth_render_indirect)
fastcallable(MGLVertexArray_meth_transform)
fastcallable(MGLVertexArray_meth_transform_indirect)

PyMethodDef MGLVertexArray_methods[] = {
    {"render", fastcall(MGLVertexArray_meth_render), fastcall_flags, NULL},
    {"render_indirect", fastcall(MGLVertexArray_meth_render_indirect), fastcall_flags, NULL},
    {"transform", fastcall(MGLVertexArray_meth_transform), fastcall_flags, NULL},
    {"transform_indirect", fastcall(MGLVertexArray_meth_transform_indirect), fastcall_flags, NULL},
    {0},
};

PyGetSetDef MGLVertexArray_getset[] = {
    {"ibo", 0, (setter)MGLVertexArray_set_ibo, 0, 0},
    {0},
};

PyType_Slot MGLVertexArray_slots[] = {
    {Py_tp_methods, MGLVertexArray_methods},
    {Py_tp_getset, MGLVertexArray_getset},
    {Py_tp_dealloc, (void *)MGLVertexArray_dealloc},
    {0},
};

PyType_Spec MGLVertexArray_spec = {
    "moderngl.mgl.new.MGLVertexArray",
    sizeof(MGLVertexArray),
    0,
    Py_TPFLAGS_DEFAULT,
    MGLVertexArray_slots,
};

PyTypeObject * MGLVertexArray_class;
