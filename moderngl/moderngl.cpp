#include "moderngl.hpp"

#include "extras.hpp"
#include "gl_context.hpp"
#include "gl_methods.hpp"
#include "data_types.hpp"
#include "tools.hpp"

PyTypeObject * Blending_type;
PyTypeObject * Buffer_type;
PyTypeObject * Context_type;
PyTypeObject * Framebuffer_type;
PyTypeObject * Program_type;
PyTypeObject * Query_type;
PyTypeObject * Renderbuffer_type;
PyTypeObject * Sampler_type;
PyTypeObject * Scope_type;
PyTypeObject * Texture_type;
PyTypeObject * VertexArray_type;

PyObject * empty_dict;
PyObject * empty_tuple;
PyObject * default_filter;
PyObject * default_wrap;
PyObject * default_anisotropy;
PyObject * default_lod_range;
PyObject * default_lod_bias;
PyObject * default_border;

unsigned default_draw_buffers[64];

struct BaseObject {
    PyObject_HEAD
    BaseObject * next;
    BaseObject * prev;
    PyObject * extra;
};

void BaseObject_dealloc(BaseObject * self) {
    // fprintf(stderr, "dealloc %p (%s)", self, Py_TYPE(self)->tp_name);
    Py_TYPE(self)->tp_free(self);
}

#pragma region Objects

struct Context : public BaseObject {
    GLContext ctx;
    GLMethods gl;

    PyObject * limits;
    PyObject * module;
    PyObject * tools;

    struct Framebuffer * screen;
    struct Scope * default_scope;
    struct Blending * default_blending;
    int default_texture_unit;

    struct ContextConstants {
        PyObject * version;
        PyObject * renderer;
        PyObject * vendor;

        PyObject * blend;
        PyObject * depth_test;
        PyObject * cull_face;

        PyObject * zero;
        PyObject * one;
        PyObject * src_color;
        PyObject * src_alpha;
        PyObject * dst_color;
        PyObject * dst_alpha;
        PyObject * one_minus_src_color;
        PyObject * one_minus_src_alpha;
        PyObject * one_minus_dst_color;
        PyObject * one_minus_dst_alpha;

        PyObject * first_vertex_convention;
        PyObject * last_vertex_convention;

        PyObject * nearest;
        PyObject * linear;
        PyObject * nearest_mipmap_nearest;
        PyObject * linear_mipmap_nearest;
        PyObject * nearest_mipmap_linear;
        PyObject * linear_mipmap_linear;

        PyObject * clamp_to_edge_x;
        PyObject * clamp_to_edge_y;
        PyObject * clamp_to_edge_z;
        PyObject * repeat_x;
        PyObject * repeat_y;
        PyObject * repeat_z;
        PyObject * mirrored_repeat_x;
        PyObject * mirrored_repeat_y;
        PyObject * mirrored_repeat_z;
        PyObject * mirror_clamp_to_edge_x;
        PyObject * mirror_clamp_to_edge_y;
        PyObject * mirror_clamp_to_edge_z;
        PyObject * clamp_to_border_x;
        PyObject * clamp_to_border_y;
        PyObject * clamp_to_border_z;

        PyObject * points;
        PyObject * lines;
        PyObject * line_loop;
        PyObject * line_strip;
        PyObject * triangles;
        PyObject * triangle_strip;
        PyObject * triangle_fan;
        PyObject * lines_adjacency;
        PyObject * line_strip_adjacency;
        PyObject * triangles_adjacency;
        PyObject * triangle_strip_adjacency;
        PyObject * patches;
    } consts;
};

struct Blending : public BaseObject {
    struct Context * ctx;
    float blend_color[4];
    int blend_functions;
    int blend_equations;
    int params[1];
};

struct Buffer : public BaseObject {
    struct Context * ctx;
    PyObject * mem;
    int flags;
    int glo;
    int size;
};

struct Attachment {
    union {
        struct Renderbuffer * renderbuffer;
        struct Texture * texture;
    };
    int components;
    char shape;
};

struct Framebuffer : public BaseObject {
    struct Context * ctx;
    PyObject * attachments_lst;
    int glo;
    int width;
    int height;
    int samples;
    Attachment depth_attachment;
    Attachment color_attachments[1];
};

struct Program : public BaseObject {
    struct Context * ctx;
    PyObject * uniforms;
    PyObject * attributes;
    bool compute;
    int glo;
};

struct Query : public BaseObject {
    struct Context * ctx;
    int glo[4];
    int elapsed;
    int primitives;
    int samples;
};

struct Renderbuffer : public BaseObject {
    struct Context * ctx;
    struct DataType * dtype;
    int glo;
    int width;
    int height;
    int components;
    int samples;
};

struct Sampler : public BaseObject {
    struct Context * ctx;
    struct Texture * texture;
    int glo;
};

struct ScopeBinding {
    union {
        Buffer * buffer;
        Sampler * sampler;
    };
    int bind;
};

struct Scope : public BaseObject {
    struct Context * ctx;
    struct Framebuffer * framebuffer;
    struct Blending * blending;
    PyObject * bindings_lst;
    int enable;
    float line_width;
    float point_size;
    int viewport[4];
    int num_samplers;
    int num_uniform_buffers;
    int num_storage_buffers;
    int num_texture_images;
    ScopeBinding bindings[1];
};

struct Texture : public BaseObject {
    struct Context * ctx;
    struct DataType * dtype;
    int glo;
    int width;
    int height;
    int length;
    int array;
    int components;
    int samples;
    int levels;
    int texture_target;
};

struct VertexArray : public BaseObject {
    struct Context * ctx;
    struct Program * program;
    struct Scope * scope;
    struct Buffer * indirect_buffer;
    struct Buffer * index_buffer;
    struct Buffer * output_buffer;
    int glo;
    int mode;
    int vertices;
    int instances;
    int index_format;
};

#pragma endregion
#pragma region Blending

Blending * Context_meth_blending(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"blend_functions", "blend_equations", "blend_color", NULL};

    PyObject * blend_functions = empty_tuple;
    PyObject * blend_equations = empty_tuple;
    float color[4] = {};

    int args_ok = PyArg_ParseTupleAndKeywords(
        args, kwa, "|OOO&", kw, &blend_functions, &blend_equations, parse_float_color, color
    );

    if (!args_ok) {
        return NULL;
    }

    PyObject * params = PyObject_CallMethod(self->tools, "serialize_blending", "OO", blend_functions, blend_equations);
    if (!params) {
        return NULL;
    }

    int num_params = (int)PySequence_Fast_GET_SIZE(params);
    Blending * res = PyObject_NewVar(Blending, Blending_type, num_params);
    ensurei(res);

    res->prev = self;
    res->next = self->next;
    self->next = res;
    res->ctx = self;
    res->extra = NULL;

    memcpy(res->blend_color, color, sizeof(color));
    res->blend_functions = (int)PySequence_Size(blend_functions);
    res->blend_equations = (int)PySequence_Size(blend_equations);

    for (int i = 0; i < num_params; ++i) {
        res->params[i] = PyLong_AsLong(PySequence_Fast_GET_ITEM(params, i));
    }
    Py_DECREF(params);

    if (PyErr_Occurred()) {
        return NULL;
    }

    return new_ref(res);
}

PyMemberDef Blending_members[] = {
    {"extra", T_OBJECT_EX, offsetof(BaseObject, extra), 0, NULL},
    {},
};

PyType_Slot Blending_slots[] = {
    {Py_tp_members, Blending_members},
    {Py_tp_dealloc, (void *)BaseObject_dealloc},
    {},
};

PyType_Spec Blending_spec = {
    "moderngl.Blending",
    sizeof(Blending) - sizeof(int),
    sizeof(int),
    Py_TPFLAGS_DEFAULT,
    Blending_slots
};

#pragma endregion
#pragma region Buffer

Buffer * Context_meth_buffer(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {
        "data", "reserve", "dynamic", "readable", "writable", "persistent", "coherent", "local", NULL,
    };

    PyObject * data = Py_None;
    int reserve = 0;
    int dynamic = true;
    int readable = true;
    int writable = true;
    int persistent = false;
    int coherent = false;
    int local = false;

    int args_ok = PyArg_ParseTupleAndKeywords(
        args,
        kwa,
        "|OO&pppppp",
        kw,
        &data,
        parse_reserve,
        &reserve,
        &dynamic,
        &readable,
        &writable,
        &persistent,
        &coherent,
        &local
    );

    if (!args_ok) {
        return NULL;
    }

    Buffer * res = PyObject_New(Buffer, Buffer_type);
    ensurei(res);

    res->prev = self;
    res->next = self->next;
    self->next = res;
    res->ctx = self;
    res->extra = NULL;

    res->mem = NULL;
    res->flags = 0;

    if (reserve) {
        res->size = reserve;
    } else {
        Py_buffer view = {};
        if (PyObject_GetBuffer(data, &view, PyBUF_STRIDED_RO) < 0) {
            return NULL;
        }
        res->size = (int)view.len;
        PyBuffer_Release(&view);
    }

    self->gl.GenBuffers(1, (GLuint *)&res->glo);
    self->gl.BindBuffer(GL_ARRAY_BUFFER, res->glo);

    if (self->gl.BufferStorage) {
        res->flags = local << 12 | dynamic << 8 | coherent << 7 | persistent << 6 | writable << 1 | readable;
        self->gl.BufferStorage(GL_ARRAY_BUFFER, res->size, 0, res->flags);
    } else {
        self->gl.BufferData(GL_ARRAY_BUFFER, res->size, 0, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }

    if (data != Py_None) {
        PyObject * call = PyObject_CallMethod((PyObject *)res, "write", "O", data);
        Py_XDECREF(call);
        if (!call) {
            return NULL;
        }
    }

    return new_ref(res);
}

PyObject * Buffer_meth_clear(Buffer * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"size", "offset", NULL};

    int size = -1;
    int offset = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "|ii", kw, &size, &offset, &size, &offset)) {
        return NULL;
    }

    if (size < 0) {
        size = self->size - offset;
    }

    self->ctx->gl.BindBuffer(GL_ARRAY_BUFFER, self->glo);
    self->ctx->gl.ClearBufferSubData(GL_ARRAY_BUFFER, GL_R8, offset, size, GL_RED, GL_UNSIGNED_BYTE, NULL);
    Py_RETURN_NONE;
}

PyObject * Buffer_meth_read(Buffer * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"size", "offset", "into", "write_offset", NULL};

    int size = -1;
    int offset = 0;
    PyObject * into = Py_None;
    int write_offset = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "|iiOiO", kw, &size, &offset, &into, &write_offset)) {
        return NULL;
    }

    if (size < 0) {
        size = self->size - offset;
    }

    if (Py_TYPE(into) == Buffer_type) {
        Buffer * dst = (Buffer *)into;
        self->ctx->gl.BindBuffer(GL_COPY_READ_BUFFER, self->glo);
        self->ctx->gl.BindBuffer(GL_COPY_WRITE_BUFFER, dst->glo);
        self->ctx->gl.CopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, offset, write_offset, size);
        self->ctx->gl.BindBuffer(GL_COPY_WRITE_BUFFER, 0);
        Py_RETURN_NONE;
    }

    if (is_buffer(into)) {
        self->ctx->gl.BindBuffer(GL_ARRAY_BUFFER, self->glo);
        Py_buffer view = {};
        if (PyObject_GetBuffer(into, &view, PyBUF_WRITABLE) < 0) {
            return NULL;
        }
        void * map = self->ctx->gl.MapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_READ_BIT);
        ensurei(map);
        memcpy((char *)view.buf + write_offset, map, size);
        PyBuffer_Release(&view);
        self->ctx->gl.UnmapBuffer(GL_ARRAY_BUFFER);
        Py_RETURN_NONE;
    }

    if (into == Py_None) {
        self->ctx->gl.BindBuffer(GL_ARRAY_BUFFER, self->glo);
        void * map = self->ctx->gl.MapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_READ_BIT);
        ensurei(map);
        PyObject * res = PyBytes_FromStringAndSize((const char *)map, size);
        ensurei(res);
        self->ctx->gl.UnmapBuffer(GL_ARRAY_BUFFER);
        return res;
    }

    PyErr_Format(PyExc_ValueError, "invalid into");
    return NULL;
}

PyObject * Buffer_meth_write(Buffer * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"data", "offset", NULL};

    Py_buffer view;
    int offset = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "y*|i", kw, &view, &offset)) {
        return NULL;
    }

    self->ctx->gl.BindBuffer(GL_ARRAY_BUFFER, self->glo);
    self->ctx->gl.BufferSubData(GL_ARRAY_BUFFER, offset, view.len, view.buf);
    PyBuffer_Release(&view);
    Py_RETURN_NONE;
}

PyObject * Buffer_meth_map(Buffer * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"size", "offset", "readable", "writable", "persistent", "coherent", NULL};

    int size = -1;
    int offset = 0;
    int readable = self->flags & 1;
    int writable = self->flags >> 1 & 1;
    int persistent = self->flags >> 6 & 1;
    int coherent = self->flags >> 7 & 1;

    int args_ok = PyArg_ParseTupleAndKeywords(
        args, kwa, "|iipppp", kw, &size, &offset, &readable, &writable, &persistent, &coherent
    );

    if (!args_ok) {
        return NULL;
    }

    if (size < 0) {
        size = self->size - offset;
    }

    ensure(!self->mem, "already mapped");

    self->ctx->gl.BindBuffer(GL_ARRAY_BUFFER, self->glo);
    int access = coherent << 7 | persistent << 6 | writable << 1 | readable;
    void * ptr = self->ctx->gl.MapBufferRange(GL_ARRAY_BUFFER, offset, size, access);
    ensure(ptr, "cannot map the buffer");
    self->mem = PyMemoryView_FromMemory((char *)ptr, size, writable ? PyBUF_WRITE : PyBUF_READ);
    return new_ref(self->mem);
}

PyObject * Buffer_meth_unmap(Buffer * self) {
    if (self->mem) {
        self->ctx->gl.BindBuffer(GL_ARRAY_BUFFER, self->glo);
        self->ctx->gl.UnmapBuffer(GL_ARRAY_BUFFER);
        Py_XDECREF(self->mem);
        self->mem = NULL;
    }
    Py_RETURN_NONE;
}

PyObject * Buffer_meth_bind(Buffer * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"layout", "stride", "divisor", "offset", NULL};

    PyObject * attributes = args;
    PyObject * layout = Py_None;
    int stride = 0;
    int divisor = 0;
    int offset = 0;

    if (!PyArg_ParseTupleAndKeywords(empty_tuple, kwa, "|$Oiii", kw, &layout, &stride, &divisor, &offset)) {
        return NULL;
    }

    return Py_BuildValue("OOOiii", self, attributes, layout, offset, stride, divisor);
}

PyObject * Buffer_meth_assign(Buffer * self, PyObject * arg) {
    return Py_BuildValue("OO", self, arg);
}

PyMethodDef Buffer_methods[] = {
    {"clear", (PyCFunction)Buffer_meth_clear, METH_VARARGS | METH_KEYWORDS, NULL},
    {"read", (PyCFunction)Buffer_meth_read, METH_VARARGS | METH_KEYWORDS, NULL},
    {"write", (PyCFunction)Buffer_meth_write, METH_VARARGS | METH_KEYWORDS, NULL},
    {"map", (PyCFunction)Buffer_meth_map, METH_VARARGS | METH_KEYWORDS, NULL},
    {"unmap", (PyCFunction)Buffer_meth_unmap, METH_NOARGS, NULL},
    {"bind", (PyCFunction)Buffer_meth_bind, METH_VARARGS | METH_KEYWORDS, NULL},
    {"assign", (PyCFunction)Buffer_meth_assign, METH_O, NULL},
    {},
};

PyMemberDef Buffer_members[] = {
    {"size", T_INT, offsetof(Buffer, size), READONLY, NULL},
    {"mem", T_OBJECT_EX, offsetof(Buffer, mem), READONLY, NULL},
    {"glo", T_INT, offsetof(Buffer, glo), READONLY, NULL},
    {"extra", T_OBJECT_EX, offsetof(BaseObject, extra), 0, NULL},
    {},
};

PyType_Slot Buffer_slots[] = {
    {Py_tp_methods, Buffer_methods},
    {Py_tp_members, Buffer_members},
    {Py_tp_dealloc, (void *)BaseObject_dealloc},
    {},
};

PyType_Spec Buffer_spec = {
    "moderngl.Buffer",
    sizeof(Buffer),
    0,
    Py_TPFLAGS_DEFAULT,
    Buffer_slots
};

#pragma endregion
#pragma region Framebuffer

Framebuffer * Context_meth_framebuffer(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"color_attachments", "depth_attachment", NULL};

    PyObject * color_attachments;
    PyObject * depth_attachment = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "O|O", kw, &color_attachments, &depth_attachment)) {
        return NULL;
    }

    if (!PySequence_Check(color_attachments)) {
        color_attachments = Py_BuildValue("(O)", color_attachments);
    } else {
        color_attachments = PySequence_Fast(color_attachments, "not iterable");
    }

    int num_attachments = (int)PySequence_Fast_GET_SIZE(color_attachments);
    Framebuffer * res = PyObject_NewVar(Framebuffer, Framebuffer_type, num_attachments);
    ensurei(res);

    res->prev = self;
    res->next = self->next;
    self->next = res;
    res->ctx = self;

    res->extra = NULL;
    res->attachments_lst = PyList_New(0);

    self->gl.GenFramebuffers(1, (GLuint *)&res->glo);
    self->gl.BindFramebuffer(GL_FRAMEBUFFER, res->glo);

    for (int i = 0; i < num_attachments; ++i) {
        PyObject * item = PySequence_Fast_GET_ITEM(color_attachments, i);
        PyList_Append(res->attachments_lst, item);
        if (Py_TYPE(item) == Renderbuffer_type) {
            Renderbuffer * renderbuffer = (Renderbuffer *)item;
            self->gl.FramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + i,
                GL_RENDERBUFFER,
                renderbuffer->glo
            );
            res->width = renderbuffer->width;
            res->height = renderbuffer->height;
            res->samples = renderbuffer->samples;
            res->color_attachments[i].renderbuffer = new_ref(renderbuffer);
            res->color_attachments[i].components = renderbuffer->components;
            res->color_attachments[i].shape = renderbuffer->dtype->shape;
            continue;
        }
        if (Py_TYPE(item) == Texture_type) {
            Texture * texture = (Texture *)item;
            res->width = texture->width;
            res->height = texture->height;
            res->samples = texture->samples;
            res->color_attachments[i].texture = new_ref(texture);
            res->color_attachments[i].components = texture->components;
            res->color_attachments[i].shape = texture->dtype->shape;
            self->gl.FramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + i,
                texture->texture_target,
                texture->glo,
                0
            );
            continue;
        }

        PyErr_Format(PyExc_ValueError, "invalid color attachment");
        return NULL;
    }

    if (depth_attachment) {
        PyList_Append(res->attachments_lst, depth_attachment);
        if (Py_TYPE(depth_attachment) == Renderbuffer_type) {
            Renderbuffer * renderbuffer = (Renderbuffer *)depth_attachment;
            self->gl.FramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
                GL_RENDERBUFFER,
                renderbuffer->glo
            );
            res->width = renderbuffer->width;
            res->height = renderbuffer->height;
            res->samples = renderbuffer->samples;
            res->depth_attachment.renderbuffer = new_ref(renderbuffer);
            res->depth_attachment.components = renderbuffer->components;
            res->depth_attachment.shape = renderbuffer->dtype->shape;
        }
        if (Py_TYPE(depth_attachment) == Texture_type) {
            Texture * texture = (Texture *)depth_attachment;
            res->width = texture->width;
            res->height = texture->height;
            res->samples = texture->samples;
            res->depth_attachment.texture = new_ref(texture);
            res->depth_attachment.components = texture->components;
            res->depth_attachment.shape = texture->dtype->shape;
            self->gl.FramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
                texture->texture_target,
                texture->glo,
                0
            );
        }

        PyErr_Format(PyExc_ValueError, "invalid depth attachment");
        return NULL;
    }

    self->gl.DrawBuffers(num_attachments, default_draw_buffers);

    int status = self->gl.CheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        const char * error = "UNKNOWN";
        switch (status) {
            case GL_FRAMEBUFFER_UNDEFINED: error = "UNDEFINED"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: error = "INCOMPLETE_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: error = "INCOMPLETE_MISSING_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: error = "INCOMPLETE_DRAW_BUFFER"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: error = "INCOMPLETE_READ_BUFFER"; break;
            case GL_FRAMEBUFFER_UNSUPPORTED: error = "UNSUPPORTED"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: error = "INCOMPLETE_MULTISAMPLE"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: error = "INCOMPLETE_LAYER_TARGETS"; break;
        }
        PyErr_Format(PyExc_Exception, "the framebuffer is not complete (%s)", error);
        return NULL;
    }

    return new_ref(res);
}

PyObject * Framebuffer_meth_read(Framebuffer * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {
        "size", "components", "attachment", "read_offset", "write_offset", "into", "alignment", NULL,
    };

    int width = self->width;
    int height = self->height;
    int components = 3;
    int attachment = 0;
    int src_xy[2] = {};
    PyObject * write_offset = NULL;
    PyObject * into = Py_None;
    int alignment = 1;
    DataType * dtype = &f1;

    int args_ok = PyArg_ParseTupleAndKeywords(
        args,
        kwa,
        "|(ii)ii(ii)OOiO&",
        kw,
        &width,
        &height,
        &components,
        &attachment,
        &src_xy[0],
        &src_xy[1],
        &write_offset,
        &into,
        &alignment,
        parse_dtype, &dtype
    );

    if (!args_ok) {
        return NULL;
    }

    if (dtype->shape == 'd') {
        components = 1;
    }

    int base_format = dtype->base_format[components];
    int pixel_type = dtype->gl_type;

    if (Py_TYPE(into) == Framebuffer_type) {
        int dst_xy[2] = {};
        if (write_offset && !parse_offset2d(write_offset, dst_xy)) {
            return NULL;
        }
        Framebuffer * dst = (Framebuffer *)into;
        self->ctx->gl.BindFramebuffer(GL_READ_FRAMEBUFFER, self->glo);
        self->ctx->gl.BindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->glo);
        self->ctx->gl.ReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
        self->ctx->gl.BlitFramebuffer(
            src_xy[0], src_xy[1], width, height,
            dst_xy[0], dst_xy[1], width, height,
            GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
            GL_NEAREST
        );
        Py_RETURN_NONE;
    }

    self->ctx->gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    self->ctx->gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    self->ctx->gl.BindFramebuffer(GL_FRAMEBUFFER, self->glo);
    self->ctx->gl.ReadBuffer(attachment == -1 ? GL_NONE : (GL_COLOR_ATTACHMENT0 + attachment));

    if (Py_TYPE(into) == Buffer_type) {
        Buffer * dst = (Buffer *)into;
        char * ptr = (char *)NULL + (write_offset ? PyLong_AsLong(write_offset) : 0);
        self->ctx->gl.BindBuffer(GL_PIXEL_PACK_BUFFER, dst->glo);
        self->ctx->gl.ReadPixels(src_xy[0], src_xy[1], width, height, base_format, pixel_type, ptr);
        self->ctx->gl.BindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        Py_RETURN_NONE;
    }

    if (is_buffer(into)) {
        int offset = write_offset ? PyLong_AsLong(write_offset) : 0;
        self->ctx->gl.BindBuffer(GL_ARRAY_BUFFER, self->glo);
        Py_buffer view = {};
        if (PyObject_GetBuffer(into, &view, PyBUF_WRITABLE) < 0) {
            return NULL;
        }
        char * ptr = (char *)view.buf + offset;
        self->ctx->gl.ReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
        self->ctx->gl.ReadPixels(src_xy[0], src_xy[1], width, height, base_format, pixel_type, ptr);
        PyBuffer_Release(&view);
        Py_RETURN_NONE;
    }

    if (into == Py_None) {
        PyObject * res = PyBytes_FromStringAndSize(NULL, width * height * components);
        ensurei(res);
        char * data = PyBytes_AS_STRING(res);
        self->ctx->gl.ReadPixels(src_xy[0], src_xy[1], width, height, base_format, pixel_type, data);
        return res;
    }

    PyErr_Format(PyExc_ValueError, "invalid into");
    return NULL;
}

PyObject * Framebuffer_meth_clear(Framebuffer * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"color", "depth", "attachment", "viewport", NULL};

    PyObject * color = NULL;
    PyObject * depth = NULL;
    PyObject * attachment = NULL;
    PyObject * viewport = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "|OOOO", kw, &color, &depth, &attachment, &viewport)) {
        return NULL;
    }

    self->ctx->gl.BindFramebuffer(GL_FRAMEBUFFER, self->glo);

    if (!attachment && !color && !depth) {
        self->ctx->gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        self->ctx->gl.ClearDepth(1.0f);
        self->ctx->gl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Py_RETURN_NONE;
    }

    int idx = attachment ? PyLong_AsLong(attachment) : 0;

    if (color) {
        char shape = self->color_attachments[idx].shape;
        int components = self->color_attachments[idx].components;
        char color_bytes[32] = {};
        if (is_buffer(color)) {
            Py_buffer view = {};
            if (PyObject_GetBuffer(color, &view, PyBUF_STRIDED_RO) < 0) {
                return NULL;
            }
            ensure(view.len < 32, "buffer overflow");
            if (PyBuffer_ToContiguous(color_bytes, &view, view.len, 'C') < 0) {
                return NULL;
            }
            PyBuffer_Release(&view);
        } else {
            switch (shape) {
                case 'f': parse_float_color(color, (float *)color_bytes); break;
                case 'i': parse_int_color(color, (int *)color_bytes); break;
                case 'u': parse_uint_color(color, (unsigned *)color_bytes); break;
            }
            if (PyErr_Occurred()) {
                return NULL;
            }
        }
        switch (shape) {
            case 'f': self->ctx->gl.ClearBufferfv(GL_COLOR, idx, (float *)color_bytes); break;
            case 'i': self->ctx->gl.ClearBufferiv(GL_COLOR, idx, (int *)color_bytes); break;
            case 'u': self->ctx->gl.ClearBufferuiv(GL_COLOR, idx, (unsigned *)color_bytes); break;
        }
    }

    if (depth) {
        float val = (float)PyFloat_AsDouble(depth);
        if (PyErr_Occurred()) {
            return NULL;
        }
        self->ctx->gl.ClearBufferfv(GL_DEPTH, 0, &val);
    } else if (!attachment) {
        float val = 1.0f;
        self->ctx->gl.ClearBufferfv(GL_DEPTH, 0, &val);
    }

    Py_RETURN_NONE;
}

PyObject * Framebuffer_get_size(Framebuffer * self) {
    return Py_BuildValue("ii", self->width, self->height);
}

// backward compatibility
PyObject * Framebuffer_meth_use(Framebuffer * self) {
    Framebuffer * old = self->ctx->default_scope->framebuffer;
    self->ctx->default_scope->framebuffer = new_ref(self);
    Py_XDECREF(old);
    Py_RETURN_NONE;
}

PyMethodDef Framebuffer_methods[] = {
    {"clear", (PyCFunction)Framebuffer_meth_clear, METH_VARARGS | METH_KEYWORDS, NULL},
    {"read", (PyCFunction)Framebuffer_meth_read, METH_VARARGS | METH_KEYWORDS, NULL},

    // backward compatibility
    {"use", (PyCFunction)Framebuffer_meth_use, METH_VARARGS | METH_KEYWORDS, NULL},
    {},
};

PyGetSetDef Framebuffer_getset[] = {
    {"size", (getter)Framebuffer_get_size, NULL, NULL, NULL},
    {},
};

PyMemberDef Framebuffer_members[] = {
    {"attachments", T_OBJECT_EX, offsetof(Framebuffer, attachments_lst), READONLY, NULL},
    {"samples", T_INT, offsetof(Framebuffer, samples), READONLY, NULL},
    {"glo", T_INT, offsetof(Framebuffer, glo), READONLY, NULL},
    {"extra", T_OBJECT_EX, offsetof(BaseObject, extra), 0, NULL},
    {},
};

PyType_Slot Framebuffer_slots[] = {
    {Py_tp_methods, Framebuffer_methods},
    {Py_tp_getset, Framebuffer_getset},
    {Py_tp_members, Framebuffer_members},
    {Py_tp_dealloc, (void *)BaseObject_dealloc},
    {},
};

PyType_Spec Framebuffer_spec = {
    "moderngl.Framebuffer",
    sizeof(Framebuffer) - sizeof(Attachment),
    sizeof(Attachment),
    Py_TPFLAGS_DEFAULT,
    Framebuffer_slots
};

#pragma endregion
#pragma region Program

Program * Context_meth_program(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {
        "vertex_shader",
        "fragment_shader",
        "geometry_shader",
        "tess_control_shader",
        "tess_evaluation_shader",
        "compute_shader",
        "varyings",
        NULL,
    };

    const char * src[6] = {};
    PyObject * varyings = Py_None;

    int args_ok = PyArg_ParseTupleAndKeywords(
        args, kwa, "|zzzzzzO", kw, &src[0], &src[1], &src[2], &src[3], &src[4], &src[5], &varyings
    );

    if (!args_ok) {
        return NULL;
    }

    bool compute = !!src[5];
    bool render = src[0] || src[1] || src[2] || src[3] || src[4];\
    ensure(!compute || varyings == Py_None, "invalid varyings");
    ensure(render ^ compute, "invalid program");

    Program * res = PyObject_New(Program, Program_type);
    ensurei(res);

    res->prev = self;
    res->next = self->next;
    self->next = res;
    res->ctx = self;
    res->extra = NULL;

    res->glo = self->gl.CreateProgram();
    res->compute = compute;

    const int SHADER_TYPE[] = {
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER,
        GL_GEOMETRY_SHADER,
        GL_TESS_CONTROL_SHADER,
        GL_TESS_EVALUATION_SHADER,
        GL_COMPUTE_SHADER,
    };

    const char * SHADER_NAME[] = {
        "vertex_shader",
        "fragment_shader",
        "geometry_shader",
        "tess_control_shader",
        "tess_evaluation_shader",
        "compute_shader",
    };

    for (int i = 0; i < 6; ++i) {
        if (src[i]) {
            int shader_obj = self->gl.CreateShader(SHADER_TYPE[i]);
            self->gl.ShaderSource(shader_obj, 1, &src[i], 0);
            self->gl.CompileShader(shader_obj);

            int compiled = false;
            self->gl.GetShaderiv(shader_obj, GL_COMPILE_STATUS, &compiled);
            if (!compiled) {
                char log[16384] = {};
                self->gl.GetShaderInfoLog(shader_obj, 16384, 0, log);
                PyObject_CallMethod(self->tools, "compiler_error", "sss", SHADER_NAME[i], src[i], log);
                return NULL;
            }

            self->gl.AttachShader(res->glo, shader_obj);
            self->gl.DeleteShader(shader_obj);
        }
    }

    if (varyings != Py_None) {
        int varyings_len = (int)PySequence_Length(varyings);
        ensurei(varyings_len < 1024);
        const char * varyings_str[1024];
        for (int i = 0; i < varyings_len; ++i) {
            varyings_str[i] = PyUnicode_AsUTF8(PySequence_GetItem(varyings, i));
        }
        ensure(!PyErr_Occurred(), "invalid varying");
        self->gl.TransformFeedbackVaryings(res->glo, varyings_len, varyings_str, GL_INTERLEAVED_ATTRIBS);
    }

    self->gl.LinkProgram(res->glo);

    int linked = false;
    self->gl.GetProgramiv(res->glo, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[16384] = {};
        self->gl.GetProgramInfoLog(res->glo, 16384, 0, log);
        PyObject_CallMethod(self->tools, "linker_error", "s", log);
        return NULL;
    }

    res->attributes = PyDict_New();
    res->uniforms = PyDict_New();

    int attributes_len = 0;
    self->gl.GetProgramiv(res->glo, GL_ACTIVE_ATTRIBUTES, &attributes_len);
    for (int i = 0; i < attributes_len; ++i) {
        int gltype = 0;
        int size = 0;
        int name_len = 0;
        char name[256];
        self->gl.GetActiveAttrib(res->glo, i, 256, &name_len, &size, (GLenum *)&gltype, name);
        PyObject * location = PyLong_FromLong(self->gl.GetAttribLocation(res->glo, name));
        clean_glsl_name(name, name_len);
        PyObject * attrib = Py_BuildValue("Oiis", location, gltype, size, get_default_layout(gltype));
        PyDict_SetItemString(res->attributes, name, attrib);
        PyDict_SetItem(res->attributes, location, attrib);
        Py_DECREF(location);
        Py_DECREF(attrib);
    }

    int uniforms_len = 0;
    self->gl.GetProgramiv(res->glo, GL_ACTIVE_UNIFORMS, &uniforms_len);

    for (int i = 0; i < uniforms_len; ++i) {
        int gltype = 0;
        int size = 0;
        int name_len = 0;
        char name[256];
        self->gl.GetActiveUniform(res->glo, i, 256, &name_len, &size, (GLenum *)&gltype, name);
        int location = self->gl.GetUniformLocation(res->glo, name);
        clean_glsl_name(name, name_len);

        if (location < 0) {
            continue;
        }

        PyObject * uniform = Py_BuildValue("iiis", location, gltype, size, get_base_layout(gltype));
        PyDict_SetItemString(res->uniforms, name, uniform);
        Py_DECREF(uniform);
    }

    int uniform_blocks_len = 0;
    self->gl.GetProgramiv(res->glo, GL_ACTIVE_UNIFORM_BLOCKS, &uniform_blocks_len);

    for (int i = 0; i < uniform_blocks_len; ++i) {
        int size = 0;
        int name_len = 0;
        char name[256];
        self->gl.GetActiveUniformBlockName(res->glo, i, 256, &name_len, name);
        int index = self->gl.GetUniformBlockIndex(res->glo, name);
        self->gl.GetActiveUniformBlockiv(res->glo, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
        clean_glsl_name(name, name_len);

        PyObject * uniform_block = Py_BuildValue("iii", index, 0, size);
        PyDict_SetItemString(res->uniforms, name, uniform_block);
        Py_DECREF(uniform_block);
    }

    return new_ref(res);
}

PyObject * Program_meth_run(Program * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"x", "y", "z", NULL};
    int x, y, z;
    if (!PyArg_ParseTupleAndKeywords(args, kwa, "iii", kw, &x, &y, &z)) {
        return NULL;
    }
    self->ctx->gl.UseProgram(self->glo);
    self->ctx->gl.DispatchCompute(x, y, z);
    Py_RETURN_NONE;
}

PyObject * Program_get_item(Program * self, Program * key) {
    return PyObject_CallMethod(self->ctx->tools, "Uniform", "OO", self, key);
}

int Program_set_item(Program * self, PyObject * key, PyObject * value) {
    PyObject * uniform = PyDict_GetItem(self->uniforms, key);
    int location = PyLong_AsLong(PyTuple_GET_ITEM(uniform, 0));
    int gltype = PyLong_AsLong(PyTuple_GET_ITEM(uniform, 1));
    int size = PyLong_AsLong(PyTuple_GET_ITEM(uniform, 2));
    if (!gltype) {
        int binding = PyLong_AsLong(value);
        self->ctx->gl.UseProgram(self->glo);
        self->ctx->gl.UniformBlockBinding(self->glo, location, binding);
        return 0;
    }
    if (!is_buffer(value)) {
        PyObject * call = PyObject_CallMethod(
            self->ctx->tools, "serialize_uniform", "OOOO", self->ctx->module, self, key, value
        );
        Py_XDECREF(call);
        if (!call) {
            return -1;
        }
        return 0;
    }
    char buffer[16384];
    Py_buffer view = {};
    if (PyObject_GetBuffer(value, &view, PyBUF_STRIDED_RO) < 0) {
        return -1;
    }
    ensure(view.len < 16384, "buffer overflow");
    if (PyBuffer_ToContiguous(buffer, &view, view.len, 'C') < 0) {
        return NULL;
    }
    PyBuffer_Release(&view);
    self->ctx->gl.UseProgram(self->glo);
    set_uniform(self->ctx->gl, gltype, location, size, buffer);
    return 0;
}

PyMethodDef Program_methods[] = {
    {"run", (PyCFunction)Program_meth_run, METH_VARARGS | METH_KEYWORDS, NULL},
    {},
};

PyMemberDef Program_members[] = {
    {"uniforms", T_OBJECT_EX, offsetof(Program, uniforms), READONLY, NULL},
    {"attributes", T_OBJECT_EX, offsetof(Program, attributes), READONLY, NULL},
    {"glo", T_INT, offsetof(Program, glo), READONLY, NULL},
    {"extra", T_OBJECT_EX, offsetof(BaseObject, extra), 0, NULL},
    {},
};

PyType_Slot Program_slots[] = {
    {Py_tp_methods, Program_methods},
    {Py_tp_members, Program_members},
    {Py_mp_subscript, (void *)Program_get_item},
    {Py_mp_ass_subscript, (void *)Program_set_item},
    {Py_tp_dealloc, (void *)BaseObject_dealloc},
    {},
};

PyType_Spec Program_spec = {
    "moderngl.Program",
    sizeof(Program),
    0,
    Py_TPFLAGS_DEFAULT,
    Program_slots
};

#pragma endregion
#pragma region Query

Query * Context_meth_query(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {
        "time_elapsed", "primitives_generated", "samples_passed", "any_samples_passed", NULL,
    };

    int time_elapsed = false;
    int primitives_generated = false;
    int samples_passed = false;
    int any_samples_passed = false;

    int args_ok = PyArg_ParseTupleAndKeywords(
        args, kwa, "|pppp", kw, &time_elapsed, &primitives_generated, &samples_passed, &any_samples_passed
    );

    if (!args_ok) {
        return NULL;
    }

    if (!time_elapsed && !primitives_generated && !samples_passed && !any_samples_passed) {
        time_elapsed = true;
        primitives_generated = true;
        samples_passed = true;
        any_samples_passed = false;
    }

    if (samples_passed && any_samples_passed) {
        any_samples_passed = false;
    }

    Query * res = PyObject_New(Query, Query_type);
    ensurei(res);

    res->prev = self;
    res->next = self->next;
    self->next = res;
    res->ctx = self;
    res->extra = NULL;

    if (time_elapsed) {
        self->gl.GenQueries(1, (GLuint *)&res->glo[MGL_TIME_ELAPSED]);
    }
    if (primitives_generated) {
        self->gl.GenQueries(1, (GLuint *)&res->glo[MGL_PRIMITIVES_GENERATED]);
    }
    if (samples_passed) {
        self->gl.GenQueries(1, (GLuint *)&res->glo[MGL_SAMPLES_PASSED]);
    }
    if (any_samples_passed) {
        self->gl.GenQueries(1, (GLuint *)&res->glo[MGL_ANY_SAMPLES_PASSED]);
    }

    res->elapsed = 0;
    res->primitives = 0;
    res->samples = 0;
    return new_ref(res);
}

PyObject * Query_meth_enter(Query * self) {
    if (self->glo[MGL_ANY_SAMPLES_PASSED]) {
        self->ctx->gl.BeginQuery(GL_ANY_SAMPLES_PASSED, self->glo[MGL_ANY_SAMPLES_PASSED]);
    }
    if (self->glo[MGL_SAMPLES_PASSED]) {
        self->ctx->gl.BeginQuery(GL_SAMPLES_PASSED, self->glo[MGL_SAMPLES_PASSED]);
    }
    if (self->glo[MGL_PRIMITIVES_GENERATED]) {
        self->ctx->gl.BeginQuery(GL_PRIMITIVES_GENERATED, self->glo[MGL_PRIMITIVES_GENERATED]);
    }
    if (self->glo[MGL_TIME_ELAPSED]) {
        self->ctx->gl.BeginQuery(GL_TIME_ELAPSED, self->glo[MGL_TIME_ELAPSED]);
    }
    Py_RETURN_NONE;
}

PyObject * Query_meth_exit(Query * self) {
    if (self->glo[MGL_TIME_ELAPSED]) {
        self->ctx->gl.EndQuery(GL_TIME_ELAPSED);
        self->ctx->gl.GetQueryObjectiv(self->glo[MGL_TIME_ELAPSED], GL_QUERY_RESULT, &self->elapsed);
    }
    if (self->glo[MGL_PRIMITIVES_GENERATED]) {
        self->ctx->gl.EndQuery(GL_PRIMITIVES_GENERATED);
        self->ctx->gl.GetQueryObjectiv(self->glo[MGL_PRIMITIVES_GENERATED], GL_QUERY_RESULT, &self->primitives);
    }
    if (self->glo[MGL_SAMPLES_PASSED]) {
        self->ctx->gl.EndQuery(GL_SAMPLES_PASSED);
        self->ctx->gl.GetQueryObjectiv(self->glo[MGL_SAMPLES_PASSED], GL_QUERY_RESULT, &self->samples);
    }
    if (self->glo[MGL_ANY_SAMPLES_PASSED]) {
        self->ctx->gl.EndQuery(GL_ANY_SAMPLES_PASSED);
    }
    Py_RETURN_NONE;
}

PyMethodDef Query_methods[] = {
    {"__enter__", (PyCFunction)Query_meth_enter, METH_NOARGS, NULL},
    {"__exit__", (PyCFunction)Query_meth_exit, METH_VARARGS, NULL},
    {},
};

PyMemberDef Query_members[] = {
    {"elapsed", T_INT, offsetof(Query, elapsed), READONLY, NULL},
    {"primitives", T_INT, offsetof(Query, primitives), READONLY, NULL},
    {"samples", T_INT, offsetof(Query, samples), READONLY, NULL},
    {"extra", T_OBJECT_EX, offsetof(BaseObject, extra), 0, NULL},
    {},
};

PyType_Slot Query_slots[] = {
    {Py_tp_methods, Query_methods},
    {Py_tp_members, Query_members},
    {Py_tp_dealloc, (void *)BaseObject_dealloc},
    {},
};

PyType_Spec Query_spec = {
    "moderngl.Query",
    sizeof(Query),
    0,
    Py_TPFLAGS_DEFAULT,
    Query_slots
};

#pragma endregion
#pragma region Renderbuffer

Renderbuffer * Context_meth_renderbuffer(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"size", "components", "samples", "dtype", NULL};

    int width;
    int height;
    int components = 4;
    int samples = 0;
    DataType * dtype = &f1;

    int args_ok = PyArg_ParseTupleAndKeywords(
        args, kwa, "(ii)|iiO&", kw, &width, &height, &components, &samples, parse_dtype, &dtype
    );

    if (!args_ok) {
        return NULL;
    }

    if (dtype->shape == 'd') {
        components = 1;
    }

    Renderbuffer * res = PyObject_New(Renderbuffer, Renderbuffer_type);
    ensurei(res);

    res->prev = self;
    res->next = self->next;
    self->next = res;
    res->ctx = self;
    res->extra = NULL;

    res->width = width;
    res->height = height;
    res->components = components;
    res->samples = samples;
    res->dtype = dtype;

    self->gl.GenRenderbuffers(1, (GLuint *)&res->glo);
    self->gl.BindRenderbuffer(GL_RENDERBUFFER, res->glo);
    self->gl.RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, dtype->internal_format[components], width, height);
    return new_ref(res);
}

PyObject * Renderbuffer_get_size(Renderbuffer * self) {
    return Py_BuildValue("ii", self->width, self->height);
}

PyGetSetDef Renderbuffer_getset[] = {
    {"size", (getter)Renderbuffer_get_size, NULL, NULL, NULL},
    {},
};

PyMemberDef Renderbuffer_members[] = {
    {"samples", T_INT, offsetof(Renderbuffer, samples), READONLY, NULL},
    {"components", T_INT, offsetof(Renderbuffer, components), READONLY, NULL},
    {"width", T_INT, offsetof(Renderbuffer, width), READONLY, NULL},
    {"height", T_INT, offsetof(Renderbuffer, height), READONLY, NULL},
    {"glo", T_INT, offsetof(Renderbuffer, glo), READONLY, NULL},
    {"extra", T_OBJECT_EX, offsetof(BaseObject, extra), 0, NULL},
    {},
};

PyType_Slot Renderbuffer_slots[] = {
    {Py_tp_getset, Renderbuffer_getset},
    {Py_tp_members, Renderbuffer_members},
    {Py_tp_dealloc, (void *)BaseObject_dealloc},
    {},
};

PyType_Spec Renderbuffer_spec = {
    "moderngl.Renderbuffer",
    sizeof(Renderbuffer),
    0,
    Py_TPFLAGS_DEFAULT,
    Renderbuffer_slots
};

#pragma endregion
#pragma region Sampler

Sampler * Context_meth_sampler(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {
        "texture", "filter", "wrap", "compare_func", "anisotropy", "lod_range", "lod_bias", "border", NULL,
    };

    Texture * texture;
    PyObject * filter = default_filter;
    PyObject * wrap = default_wrap;
    PyObject * compare_func = Py_None;
    PyObject * anisotropy = default_anisotropy;
    PyObject * lod_range = default_lod_range;
    PyObject * lod_bias = default_lod_bias;
    PyObject * border = default_border;

    int args_ok = PyArg_ParseTupleAndKeywords(
        args,
        kwa,
        "O!|OOOOOOO",
        kw,
        Texture_type,
        &texture,
        &filter,
        &wrap,
        &compare_func,
        &anisotropy,
        &lod_range,
        &lod_bias,
        &border
    );

    if (!args_ok) {
        return NULL;
    }

    Sampler * res = PyObject_New(Sampler, Sampler_type);
    ensurei(res);

    res->prev = self;
    res->next = self->next;
    self->next = res;
    res->ctx = self;
    res->extra = NULL;

    res->texture = NULL;

    self->gl.GenSamplers(1, (GLuint *)&res->glo);
    PyObject_SetAttrString((PyObject *)res, "texture", (PyObject *)texture);
    PyObject_SetAttrString((PyObject *)res, "filter", filter);
    PyObject_SetAttrString((PyObject *)res, "wrap", wrap);
    PyObject_SetAttrString((PyObject *)res, "compare_func", compare_func);
    PyObject_SetAttrString((PyObject *)res, "anisotropy", anisotropy);
    PyObject_SetAttrString((PyObject *)res, "lod_range", lod_range);
    PyObject_SetAttrString((PyObject *)res, "lod_bias", lod_bias);
    PyObject_SetAttrString((PyObject *)res, "border", border);

    if (PyErr_Occurred()) {
        return NULL;
    }

    return new_ref(res);
}

PyObject * Sampler_meth_assign(Sampler * self, PyObject * arg) {
    return Py_BuildValue("OO", self, arg);
}

Texture * Sampler_get_texture(Sampler * self) {
    return new_ref(self->texture);
}

int Sampler_set_texture(Sampler * self, Texture * value) {
    if (Py_TYPE(value) != Texture_type) {
        return -1;
    }
    Texture * old = self->texture;
    self->texture = new_ref(value);
    Py_XDECREF(old);
    return 0;
}

PyObject * Sampler_get_filter(Sampler * self) {
    int filter[2];
    self->ctx->gl.GetSamplerParameteriv(self->glo, GL_TEXTURE_MIN_FILTER, &filter[0]);
    self->ctx->gl.GetSamplerParameteriv(self->glo, GL_TEXTURE_MAG_FILTER, &filter[1]);
    return Py_BuildValue("ii", filter[0], filter[1]);
}

int Sampler_set_filter(Sampler * self, PyObject * value) {
    int filter[2];
    if (!parse_filter(value, filter)) {
        return -1;
    }
    self->ctx->gl.SamplerParameteri(self->glo, GL_TEXTURE_MIN_FILTER, filter[0]);
    self->ctx->gl.SamplerParameteri(self->glo, GL_TEXTURE_MAG_FILTER, filter[1]);
    return 0;
}

PyObject * Sampler_get_wrap(Sampler * self) {
    Py_RETURN_NONE;
}

int Sampler_set_wrap(Sampler * self, PyObject * value) {
    int wrap[3];
    if (!parse_wrap(value, wrap)) {
        return -1;
    }
    self->ctx->gl.SamplerParameteri(self->glo, GL_TEXTURE_WRAP_S, wrap[0]);
    self->ctx->gl.SamplerParameteri(self->glo, GL_TEXTURE_WRAP_T, wrap[1]);
    self->ctx->gl.SamplerParameteri(self->glo, GL_TEXTURE_WRAP_R, wrap[2]);
    return 0;
}

PyObject * Sampler_get_compare_func(Sampler * self) {
    Py_RETURN_NONE;
}

int Sampler_set_compare_func(Sampler * self, PyObject * value) {
    if (value == Py_None) {
        self->ctx->gl.SamplerParameteri(self->glo, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        return 0;
    }
    int compare_func = PyLong_AsLong(value);
    if (PyErr_Occurred()) {
        return -1;
    }
    self->ctx->gl.SamplerParameteri(self->glo, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    self->ctx->gl.SamplerParameteri(self->glo, GL_TEXTURE_COMPARE_FUNC, compare_func);
    return 0;
}

PyObject * Sampler_get_anisotropy(Sampler * self) {
    Py_RETURN_NONE;
}

int Sampler_set_anisotropy(Sampler * self, PyObject * value) {
    float anisotropy = (float)PyFloat_AsDouble(value);
    if (PyErr_Occurred()) {
        return -1;
    }
    self->ctx->gl.SamplerParameterf(self->glo, GL_TEXTURE_MAX_ANISOTROPY, anisotropy);
    return 0;
}

PyObject * Sampler_get_lod_range(Sampler * self) {
    Py_RETURN_NONE;
}

int Sampler_set_lod_range(Sampler * self, PyObject * value) {
    float lod_range[2];
    if (!parse_lod_range(value, lod_range)) {
        return -1;
    }
    self->ctx->gl.SamplerParameterf(self->glo, GL_TEXTURE_MIN_LOD, lod_range[0]);
    self->ctx->gl.SamplerParameterf(self->glo, GL_TEXTURE_MAX_LOD, lod_range[1]);
    return 0;
}

PyObject * Sampler_get_lod_bias(Sampler * self) {
    Py_RETURN_NONE;
}

int Sampler_set_lod_bias(Sampler * self, PyObject * value) {
    float lod_bias = (float)PyFloat_AsDouble(value);
    if (PyErr_Occurred()) {
        return -1;
    }
    self->ctx->gl.SamplerParameterf(self->glo, GL_TEXTURE_LOD_BIAS, lod_bias);
    return 0;
}

PyObject * Sampler_get_border(Sampler * self) {
    Py_RETURN_NONE;
}

int Sampler_set_border(Sampler * self, PyObject * value) {
    float border[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    if (!parse_float_color(value, border)) {
        return -1;
    }
    self->ctx->gl.SamplerParameterfv(self->glo, GL_TEXTURE_BORDER_COLOR, border);
    return 0;
}

PyMethodDef Sampler_methods[] = {
    {"assign", (PyCFunction)Sampler_meth_assign, METH_O, NULL},
    {},
};

PyGetSetDef Sampler_getset[] = {
    {"texture", (getter)Sampler_get_texture, (setter)Sampler_set_texture, NULL, NULL},
    {"filter", (getter)Sampler_get_filter, (setter)Sampler_set_filter, NULL, NULL},
    {"wrap", (getter)Sampler_get_wrap, (setter)Sampler_set_wrap, NULL, NULL},
    {"anisotropy", (getter)Sampler_get_anisotropy, (setter)Sampler_set_anisotropy, NULL, NULL},
    {"compare_func", (getter)Sampler_get_compare_func, (setter)Sampler_set_compare_func, NULL, NULL},
    {"lod_range", (getter)Sampler_get_lod_range, (setter)Sampler_set_lod_range, NULL, NULL},
    {"lod_bias", (getter)Sampler_get_lod_bias, (setter)Sampler_set_lod_bias, NULL, NULL},
    {"border", (getter)Sampler_get_border, (setter)Sampler_set_border, NULL, NULL},
    {},
};

PyMemberDef Sampler_members[] = {
    {"glo", T_INT, offsetof(Sampler, glo), READONLY, NULL},
    {"extra", T_OBJECT_EX, offsetof(BaseObject, extra), 0, NULL},
    {},
};

PyType_Slot Sampler_slots[] = {
    {Py_tp_methods, Sampler_methods},
    {Py_tp_getset, Sampler_getset},
    {Py_tp_members, Sampler_members},
    {Py_tp_dealloc, (void *)BaseObject_dealloc},
    {},
};

PyType_Spec Sampler_spec = {
    "moderngl.Sampler",
    sizeof(Sampler),
    0,
    Py_TPFLAGS_DEFAULT,
    Sampler_slots
};

#pragma endregion
#pragma region Scope

Scope * Context_meth_scope(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {
        "enable",
        "framebuffer",
        "samplers",
        "uniform_buffers",
        "storage_buffers",
        "blending",
        "line_width",
        "point_size",
        "viewport",
        NULL,
    };

    int enable = MGL_NOTHING;
    Framebuffer * framebuffer = self->screen;
    PyObject * samplers = NULL;
    PyObject * uniform_buffers = NULL;
    PyObject * storage_buffers = NULL;
    Blending * blending = self->default_blending;
    float line_width = 1.0f;
    float point_size = 1.0f;
    int viewport[4] = {};

    int args_ok = PyArg_ParseTupleAndKeywords(
        args,
        kwa,
        "|iO!OOOOOff(iiii)",
        kw,
        &enable,
        Framebuffer_type,
        &framebuffer,
        &samplers,
        &uniform_buffers,
        &storage_buffers,
        &blending,
        &line_width,
        &point_size,
        &viewport[0],
        &viewport[1],
        &viewport[2],
        &viewport[3]
    );

    if (!args_ok) {
        return NULL;
    }

    if (!viewport[0] && !viewport[1] && !viewport[2] && !viewport[3]) {
        viewport[2] = framebuffer->width;
        viewport[3] = framebuffer->height;
    }

    samplers = samplers ? PySequence_Fast(samplers, "not iterable") : NULL;
    uniform_buffers = uniform_buffers ? PySequence_Fast(uniform_buffers, "not iterable") : NULL;
    storage_buffers = storage_buffers ? PySequence_Fast(storage_buffers, "not iterable") : NULL;

    int num_samplers = samplers ? (int)PySequence_Fast_GET_SIZE(samplers) : 0;
    int num_uniform_buffers = uniform_buffers ? (int)PySequence_Fast_GET_SIZE(uniform_buffers) : 0;
    int num_storage_buffers = storage_buffers ? (int)PySequence_Fast_GET_SIZE(storage_buffers) : 0;
    int num_bindings = num_samplers + num_uniform_buffers + num_storage_buffers;

    Scope * res = PyObject_NewVar(Scope, Scope_type, num_bindings);
    ensurei(res);

    res->prev = self;
    res->next = self->next;
    self->next = res;
    res->ctx = self;
    res->extra = NULL;

    res->bindings_lst = PyList_New(0);

    res->line_width = line_width;
    res->point_size = point_size;

    memcpy(res->viewport, viewport, sizeof(viewport));

    res->enable = enable;
    res->framebuffer = new_ref(framebuffer);
    res->blending = new_ref(blending);
    res->num_samplers = num_samplers;
    res->num_uniform_buffers = num_uniform_buffers;
    res->num_storage_buffers = num_storage_buffers;

    int binding = 0;
    for (int i = 0; i < num_samplers; ++i) {
        Sampler * sampler = (Sampler *)PyTuple_GetItem(PySequence_Fast_GET_ITEM(samplers, i), 0);
        if (Py_TYPE(sampler) != Sampler_type) {
            PyErr_Format(PyExc_ValueError, "not a sampler");
            return NULL;
        }
        PyList_Append(res->bindings_lst, (PyObject *)sampler);
        int bind = PyLong_AsLong(PyTuple_GetItem(PySequence_Fast_GET_ITEM(samplers, i), 1));
        res->bindings[binding].sampler = new_ref(sampler);
        res->bindings[binding].bind = bind;
        binding += 1;
    }

    for (int i = 0; i < num_uniform_buffers; ++i) {
        Buffer * buffer = (Buffer *)PyTuple_GetItem(PySequence_Fast_GET_ITEM(uniform_buffers, i), 0);
        if (Py_TYPE(buffer) != Buffer_type) {
            PyErr_Format(PyExc_ValueError, "not a buffer");
            return NULL;
        }
        PyList_Append(res->bindings_lst, (PyObject *)buffer);
        int bind = PyLong_AsLong(PyTuple_GetItem(PySequence_Fast_GET_ITEM(uniform_buffers, i), 1));
        res->bindings[binding].buffer = new_ref(buffer);
        res->bindings[binding].bind = bind;
        binding += 1;
    }

    for (int i = 0; i < num_storage_buffers; ++i) {
        Buffer * buffer = (Buffer *)PyTuple_GetItem(PySequence_Fast_GET_ITEM(storage_buffers, i), 0);
        if (Py_TYPE(buffer) != Buffer_type) {
            PyErr_Format(PyExc_ValueError, "not a buffer");
            return NULL;
        }
        PyList_Append(res->bindings_lst, (PyObject *)buffer);
        int bind = PyLong_AsLong(PyTuple_GetItem(PySequence_Fast_GET_ITEM(storage_buffers, i), 1));
        res->bindings[binding].buffer = new_ref(buffer);
        res->bindings[binding].bind = bind;
        binding += 1;
    }

    return new_ref(res);
}

PyObject * Scope_get_viewport(Scope * self) {
    return Py_BuildValue("iiii", self->viewport[0], self->viewport[1], self->viewport[2], self->viewport[3]);
}

int Scope_set_viewport(Scope * self, PyObject * value) {
    if (!parse_viewport2d(value, self->viewport)) {
        return -1;
    }
    return 0;
}

Framebuffer * Scope_get_framebuffer(Scope * self) {
    return self->framebuffer;
}

int Scope_set_framebuffer(Scope * self, Framebuffer * value) {
    if (Py_TYPE(value) != Framebuffer_type) {
        return -1;
    }
    Framebuffer * old = self->framebuffer;
    self->framebuffer = new_ref(value);
    Py_XDECREF(old);
    return 0;
}

Blending * Scope_get_blending(Scope * self) {
    return self->blending;
}

int Scope_set_blending(Scope * self, Blending * value) {
    if (Py_TYPE(value) != Blending_type) {
        return -1;
    }
    Blending * old = self->blending;
    self->blending = new_ref(value);
    Py_XDECREF(old);
    return 0;
}

PyGetSetDef Scope_getset[] = {
    {"viewport", (getter)Scope_get_viewport, (setter)Scope_set_viewport, NULL, NULL},
    {"framebuffer", (getter)Scope_get_framebuffer, (setter)Scope_set_framebuffer, NULL, NULL},
    {"blending", (getter)Scope_get_blending, (setter)Scope_set_blending, NULL, NULL},
    {},
};

PyMemberDef Scope_members[] = {
    {"point_size", T_FLOAT, offsetof(Scope, point_size), 0, NULL},
    {"line_width", T_FLOAT, offsetof(Scope, line_width), 0, NULL},
    {"bindings", T_OBJECT_EX, offsetof(Scope, bindings_lst), READONLY, NULL},
    {"extra", T_OBJECT_EX, offsetof(BaseObject, extra), 0, NULL},
    {},
};

PyType_Slot Scope_slots[] = {
    {Py_tp_getset, Scope_getset},
    {Py_tp_members, Scope_members},
    {Py_tp_dealloc, (void *)BaseObject_dealloc},
    {},
};

PyType_Spec Scope_spec = {
    "moderngl.Scope",
    sizeof(Scope) - sizeof(ScopeBinding),
    sizeof(ScopeBinding),
    Py_TPFLAGS_DEFAULT,
    Scope_slots
};

#pragma endregion
#pragma region Texture

int get_texture_target(int width, int height, int length, int samples, int array, int cubemap) {
    if (cubemap) {
        if (samples || length || !height) {
            return 0;
        }
        return array ? GL_TEXTURE_CUBE_MAP_ARRAY : GL_TEXTURE_CUBE_MAP;
    }
    if (samples) {
        if (length || !height) {
            return 0;
        }
        return array ? GL_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_2D_MULTISAMPLE;
    }
    if (length) {
        if (array) {
            return 0;
        }
        return GL_TEXTURE_3D;
    }
    if (height) {
        return array ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
    }
    return array ? GL_TEXTURE_1D_ARRAY : GL_TEXTURE_1D;
}

Texture * Context_meth_texture(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"size", "components", "data", "samples", "array", "levels", "cubemap", "dtype", NULL};

    int size[3] = {};
    int components;
    PyObject * data = NULL;
    int samples = 0;
    int array = 0;
    int cubemap = 0;
    int levels = 1;
    DataType * dtype = &f1;

    int args_ok = PyArg_ParseTupleAndKeywords(
        args, kwa, "O&O&|OO&iipO&", kw,
        parse_texture_size,
        size,
        parse_components,
        &components,
        &data,
        parse_samples,
        &samples,
        &array,
        &levels,
        &cubemap,
        parse_dtype,
        &dtype
    );

    if (!args_ok) {
        return NULL;
    }

    if (dtype->shape == 'd') {
        components = 1;
    }

    Texture * res = PyObject_New(Texture, Texture_type);
    ensurei(res);

    int tx = get_texture_target(size[0], size[1], size[2], samples, array, cubemap);
    if (!tx) {
        return NULL;
    }

    int internal_format = dtype->internal_format[components];
    int base_format = dtype->base_format[components];
    int pixel_type = dtype->gl_type;

    res->prev = self;
    res->next = self->next;
    self->next = res;
    res->ctx = self;
    res->extra = NULL;

    res->width = size[0];
    res->height = size[1];
    res->length = size[2];
    res->array = array;
    res->components = components;
    res->samples = samples;
    res->levels = levels;

    res->texture_target = tx;
    res->dtype = dtype;

    self->gl.GenTextures(1, (GLuint *)&res->glo);
	self->gl.ActiveTexture(self->default_texture_unit);
    self->gl.BindTexture(res->texture_target, res->glo);

    switch (tx) {
        case GL_TEXTURE_1D:
            if (self->gl.TexStorage1D) {
                self->gl.TexStorage1D(tx, levels, internal_format, size[0]);
            } else {
                self->gl.TexImage1D(
                    tx, 0, internal_format, size[0], 0, base_format, pixel_type, NULL
                );
            }
            break;

        case GL_TEXTURE_2D:
            if (self->gl.TexStorage2D) {
                self->gl.TexStorage2D(tx, levels, internal_format, size[0], size[1]);
            } else {
                self->gl.TexImage2D(
                    tx, 0, internal_format, size[0], size[1], 0, base_format, pixel_type, NULL
                );
            }
            break;

        case GL_TEXTURE_3D:
            if (self->gl.TexStorage3D) {
            self->gl.TexStorage3D(tx, levels, internal_format, size[0], size[1], size[2]);
            } else {
                self->gl.TexImage3D(
                    tx, 0, internal_format, size[0], size[1], size[2], 0, base_format, pixel_type, NULL
                );
            }
            break;

        case GL_TEXTURE_1D_ARRAY:
            if (self->gl.TexStorage2D) {
                self->gl.TexStorage2D(tx, levels, internal_format, size[0], array);
            } else {
                self->gl.TexImage2D(
                    tx, 0, internal_format, size[0], array, 0, base_format, pixel_type, NULL
                );
            }
            break;

        case GL_TEXTURE_2D_ARRAY:
            if (self->gl.TexStorage3D) {
                self->gl.TexStorage3D(tx, levels, internal_format, size[0], size[1], array);
            } else {
                self->gl.TexImage3D(
                    tx, 0, internal_format, size[0], size[1], array, 0, base_format, pixel_type, NULL
                );
            }
            break;

        case GL_TEXTURE_CUBE_MAP:
            if (self->gl.TexStorage2D) {
                self->gl.TexStorage2D(tx, levels, internal_format, size[0], size[1]);
            } else {
                for (int i = 0; i < 6; ++i) {
                    int txi = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
                    self->gl.TexImage2D(
                        txi, 0, internal_format, size[0], size[1], 0, base_format, pixel_type, NULL
                    );
                }
            }
            break;

        case GL_TEXTURE_CUBE_MAP_ARRAY:
            if (self->gl.TexStorage3D) {
                self->gl.TexStorage3D(tx, levels, internal_format, size[0], size[1], array);
            } else {
                self->gl.TexImage3D(
                    tx, 0, internal_format, size[0], size[1], array, 0, base_format, pixel_type, NULL
                );
            }
            break;

        case GL_TEXTURE_2D_MULTISAMPLE:
            if (self->gl.TexStorage2DMultisample) {
                self->gl.TexStorage2DMultisample(tx, samples, internal_format, size[0], size[1], true);
            } else {
                self->gl.TexImage2DMultisample(tx, samples, internal_format, size[0], size[1], true);
            }
            break;

        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            if (self->gl.TexStorage3DMultisample) {
                self->gl.TexStorage3DMultisample(tx, samples, internal_format, size[0], size[1], array, true);
            } else {
                self->gl.TexImage3DMultisample(tx, samples, internal_format, size[0], size[1], array, true);
            }
            break;
    }

    if (data) {
        PyObject * call = PyObject_CallMethod((PyObject *)res, "write", "O", data);
        Py_XDECREF(call);
        if (!call) {
            return NULL;
        }
    }

    return new_ref(res);
}

PyObject * Context_meth_texture_from(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"obj", "array", "cubemap", NULL};

    PyObject * obj;
    PyObject * array = Py_False;
    PyObject * cubemap = Py_False;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "O|OO", kw, &obj, &array, &cubemap)) {
        return NULL;
    }

    return PyObject_CallMethod(self->tools, "texture_from", "OOOO", self, obj, array, cubemap);
}

PyObject * Texture_meth_read(Texture * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"into", "write_offset", NULL};

    PyObject * into = Py_None;
    PyObject * write_offset = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "|O", kw, &into)) {
        return NULL;
    }

	self->ctx->gl.ActiveTexture(self->ctx->default_texture_unit);
    self->ctx->gl.BindTexture(self->texture_target, self->glo);

    if (Py_TYPE(into) == Texture_type) {
        PyErr_Format(PyExc_NotImplementedError, "");
        return NULL;
    }

    if (is_buffer(into)) {
        int offset = write_offset ? PyLong_AsLong(write_offset) : 0;
        self->ctx->gl.BindBuffer(GL_ARRAY_BUFFER, self->glo);
        Py_buffer view = {};
        if (PyObject_GetBuffer(into, &view, PyBUF_WRITABLE) < 0) {
            return NULL;
        }
        self->ctx->gl.GetTexImage(self->texture_target, 0, GL_RGB, GL_UNSIGNED_BYTE, (char *)view.buf + offset);
        PyBuffer_Release(&view);
        Py_RETURN_NONE;
    }

    if (into == Py_None) {
        PyObject * res = PyBytes_FromStringAndSize(NULL, self->width * self->height * self->components);
        ensurei(res);
        self->ctx->gl.GetTexImage(self->texture_target, 0, GL_RGB, GL_UNSIGNED_BYTE, PyBytes_AS_STRING(res));
        return res;
    }

    return NULL;
}

PyObject * Texture_meth_write(Texture * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"data", "size", "offset", "alignment", NULL};

    PyObject * data;
    PyObject * temp[2];
    int alignment = 1;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "O|OOi", kw, &data, &temp[0], &temp[1], &alignment)) {
        return NULL;
    }

    Py_buffer view = {};

    if (Py_TYPE(data) == Buffer_type) {
        Buffer * buffer = (Buffer *)data;
        self->ctx->gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer->glo);
    } else {
        if (PyObject_GetBuffer(data, &view, PyBUF_STRIDED_RO) < 0) {
            return NULL;
        }
    }

    // int expected = expected_size(width, height, length, 0, self->components * self->dtype->size, alignment);

    int tx = self->texture_target;
    int base_format = self->dtype->base_format[self->components];
    int pixel_type = self->dtype->gl_type;

	self->ctx->gl.ActiveTexture(self->ctx->default_texture_unit);
    self->ctx->gl.BindTexture(tx, self->glo);
    self->ctx->gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    self->ctx->gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);

    switch (tx) {
        case GL_TEXTURE_1D: {
            int size[1] = {self->width};
            int offset[1] = {};
            int args_ok = PyArg_ParseTupleAndKeywords(
                args, kwa, "O|iii", kw, &data, &size[0], &offset[0], &alignment
            );
            self->ctx->gl.TexSubImage1D(
                tx, 0, offset[0], size[0], base_format, pixel_type, view.buf
            );
            break;
        }

        case GL_TEXTURE_2D: {
            int size[2] = {self->width, self->height};
            int offset[2] = {};
            int args_ok = PyArg_ParseTupleAndKeywords(
                args, kwa, "O|(ii)(ii)i", kw, &data, &size[0], &size[1], &offset[0], &offset[1], &alignment
            );
            self->ctx->gl.TexSubImage2D(
                tx, 0, offset[0], offset[1], size[0], size[1], base_format, pixel_type, view.buf
            );
            break;
        }

        case GL_TEXTURE_3D: {
            int size[3] = {self->width, self->height, self->length};
            int offset[3] = {};
            int args_ok = PyArg_ParseTupleAndKeywords(
                args, kwa, "O|(iii)(iii)i", kw, &data, &size[0], &size[1], &size[2], &offset[0], &offset[1], &offset[2], &alignment
            );
            self->ctx->gl.TexSubImage3D(
                tx, 0, offset[0], offset[1], offset[2], size[0], size[1], size[2], base_format, pixel_type, view.buf
            );
            break;
        }

        case GL_TEXTURE_1D_ARRAY: {
            int size[2] = {self->width, self->array};
            int offset[2] = {};
            int args_ok = PyArg_ParseTupleAndKeywords(
                args, kwa, "O|(ii)(ii)i", kw, &data, &size[0], &size[1], &offset[0], &offset[1], &alignment
            );
            self->ctx->gl.TexSubImage2D(
                tx, 0, offset[0], offset[1], size[0], size[1], base_format, pixel_type, view.buf
            );
            break;
        }

        case GL_TEXTURE_2D_ARRAY: {
            int size[3] = {self->width, self->height, self->array};
            int offset[3] = {};
            int args_ok = PyArg_ParseTupleAndKeywords(
                args, kwa, "O|(iii)(iii)i", kw, &data, &size[0], &size[1], &size[2], &offset[0], &offset[1], &offset[2], &alignment
            );
            self->ctx->gl.TexSubImage3D(
                tx, 0, offset[0], offset[1], offset[2], size[0], size[1], size[2], base_format, pixel_type, view.buf
            );
            break;
        }

        case GL_TEXTURE_CUBE_MAP: {
            int size[3] = {self->width, self->height, 6};
            int offset[3] = {};
            int args_ok = PyArg_ParseTupleAndKeywords(
                args, kwa, "O|(iii)(iii)i", kw, &data, &size[0], &size[1], &size[2], &offset[0], &offset[1], &offset[2], &alignment
            );
            char * ptr = (char *)view.buf;
            for (int i = offset[2]; i < size[2]; ++i) {
                int txi = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
                self->ctx->gl.TexSubImage2D(
                    txi, 0, offset[0], offset[1], size[0], size[1], base_format, pixel_type, ptr
                );
                ptr += size[0] * size[1] * self->components;
            }
            break;
        }

        case GL_TEXTURE_CUBE_MAP_ARRAY: {
            int size[3] = {self->width, self->height, self->array};
            int offset[3] = {};
            int args_ok = PyArg_ParseTupleAndKeywords(
                args, kwa, "O|(iii)(iii)i", kw, &data, &size[0], &size[1], &size[2], &offset[0], &offset[1], &offset[2], &alignment
            );
            self->ctx->gl.TexSubImage3D(
                tx, 0, offset[0], offset[1], offset[2], size[0], size[1], size[2], base_format, pixel_type, view.buf
            );
            break;
        }
    }

    if (Py_TYPE(data) != Buffer_type) {
        self->ctx->gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    } else {
        PyBuffer_Release(&view);
    }
    Py_RETURN_NONE;
}

PyObject * Texture_meth_build_mipmaps(Texture * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"base_level", "max_level", NULL};

    int base_level = 0;
    int max_level = -1;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "|ii", kw, &base_level, &max_level)) {
        return NULL;
    }

    if (max_level < 0) {
        max_level = self->levels;
    }

    int tx = self->texture_target;
	self->ctx->gl.ActiveTexture(self->ctx->default_texture_unit);
	self->ctx->gl.BindTexture(tx, self->glo);
	self->ctx->gl.TexParameteri(tx, GL_TEXTURE_BASE_LEVEL, base_level);
	self->ctx->gl.TexParameteri(tx, GL_TEXTURE_MAX_LEVEL, max_level);
	self->ctx->gl.GenerateMipmap(tx);
    Py_RETURN_NONE;
}

PyObject * Texture_get_size(Texture * self) {
    return Py_BuildValue("ii", self->width, self->height);
}

PyMethodDef Texture_methods[] = {
    {"read", (PyCFunction)Texture_meth_read, METH_VARARGS | METH_KEYWORDS, NULL},
    {"write", (PyCFunction)Texture_meth_write, METH_VARARGS | METH_KEYWORDS, NULL},

    // backward compatibility
    {"build_mipmaps", (PyCFunction)Texture_meth_build_mipmaps, METH_VARARGS | METH_KEYWORDS, NULL},
    {},
};

PyObject * Texture_get_swizzle(Texture * self) {
    int tex_swizzle[4] = {GL_ZERO, GL_ZERO, GL_ZERO, GL_ONE};
	self->ctx->gl.ActiveTexture(self->ctx->default_texture_unit);
    self->ctx->gl.BindTexture(self->texture_target, self->glo);
    self->ctx->gl.GetTexParameteriv(self->texture_target, GL_TEXTURE_SWIZZLE_RGBA, tex_swizzle);
    char swizzle[4] = {
        chr_from_swizzle(tex_swizzle[0]),
        chr_from_swizzle(tex_swizzle[1]),
        chr_from_swizzle(tex_swizzle[2]),
        chr_from_swizzle(tex_swizzle[3]),
    };
    return PyUnicode_FromStringAndSize(swizzle, 4);
}

int Texture_set_swizzle(Texture * self, PyObject * value) {
    int tex_swizzle[4];
    if (!parse_swizzle(value, tex_swizzle)) {
        return -1;
    }
	self->ctx->gl.ActiveTexture(self->ctx->default_texture_unit);
    self->ctx->gl.BindTexture(self->texture_target, self->glo);
    self->ctx->gl.TexParameteriv(self->texture_target, GL_TEXTURE_SWIZZLE_RGBA, tex_swizzle);
    return 0;
}

PyGetSetDef Texture_getset[] = {
    {"swizzle", (getter)Texture_get_swizzle, (setter)Texture_set_swizzle, NULL, NULL},
    {"size", (getter)Texture_get_size, NULL, NULL, NULL},
    {},
};

PyMemberDef Texture_members[] = {
    {"samples", T_INT, offsetof(Texture, samples), READONLY, NULL},
    {"components", T_INT, offsetof(Texture, components), READONLY, NULL},
    {"width", T_INT, offsetof(Texture, width), READONLY, NULL},
    {"height", T_INT, offsetof(Texture, height), READONLY, NULL},
    {"length", T_INT, offsetof(Texture, length), READONLY, NULL},
    {"glo", T_INT, offsetof(Texture, glo), READONLY, NULL},
    {"extra", T_OBJECT_EX, offsetof(BaseObject, extra), 0, NULL},
    {},
};

PyType_Slot Texture_slots[] = {
    {Py_tp_methods, Texture_methods},
    {Py_tp_getset, Texture_getset},
    {Py_tp_members, Texture_members},
    {Py_tp_dealloc, (void *)BaseObject_dealloc},
    {},
};

PyType_Spec Texture_spec = {
    "moderngl.Texture",
    sizeof(Texture),
    0,
    Py_TPFLAGS_DEFAULT,
    Texture_slots
};

#pragma endregion
#pragma region VertexArray

VertexArray * Context_meth_vertex_array(Context * self, PyObject * args, PyObject * kwa) {
    if (PyTuple_Size(args) > 2 && Py_TYPE(PyTuple_GET_ITEM(args, 1)) == Buffer_type) {
        PyObject * attribs = PyTuple_GetSlice(args, 2, PY_SSIZE_T_MAX);
        PyObject * bindings = Py_BuildValue("ONOiii", PyTuple_GET_ITEM(args, 1), attribs, Py_None, 0, 0, 0);
        PyObject * call_args = Py_BuildValue("O[N]", PyTuple_GET_ITEM(args, 0), bindings);
        return Context_meth_vertex_array(self, call_args, kwa);
    }

    static char * kw[] = {"program", "bindings", "index_buffer", "mode", NULL};

    Program * program = NULL;
    PyObject * bindings = Py_None;
    PyObject * index_buffer = NULL;
    int mode = GL_TRIANGLES;

    int args_ok = PyArg_ParseTupleAndKeywords(
        args, kwa, "|O!OOi", kw, Program_type, &program, &bindings, &index_buffer, &mode
    );

    if (!args_ok) {
        return NULL;
    }

    VertexArray * res = PyObject_New(VertexArray, VertexArray_type);
    ensurei(res);

    res->prev = self;
    res->next = self->next;
    self->next = res;
    res->ctx = self;
    res->extra = NULL;

    res->program = program ? new_ref(program) : NULL;
    res->scope = new_ref(self->default_scope);
    res->mode = mode;
    res->vertices = 0;
    res->instances = 1;
    res->index_format = GL_UNSIGNED_INT;

    res->indirect_buffer = NULL;
    res->index_buffer = NULL;
    res->output_buffer = NULL;

    self->gl.GenVertexArrays(1, (GLuint *)&res->glo);

    if (bindings != Py_None) {
        PyObject * call = PyObject_CallMethod(self->tools, "bind_attributes", "OO", res, bindings);
        Py_XDECREF(call);
        if (!call) {
            return NULL;
        }
    }

    if (index_buffer) {
        if (PyObject_SetAttrString((PyObject *)res, "index_buffer", index_buffer) < 0) {
            return NULL;
        }
        // backward compatibility
        Buffer * ibo = (Buffer *)index_buffer;
        res->vertices = ibo->size / 4;
    }

    return new_ref(res);
}

PyObject * VertexArray_meth_bind(VertexArray * self, PyObject * args) {
    if (PyTuple_Size(args) == 1) {
        return PyObject_CallMethod(self->ctx->tools, "bind_attributes", "OOO", self, args, Py_False);
    }

    Buffer * buffer;
    int location;
    int gltype;
    int size;
    void * offset;
    int stride;
    int divisor;

    int args_ok = PyArg_ParseTuple(
        args, "O!iiinii", Buffer_type, &buffer, &location, &gltype, &size, &offset, &stride, &divisor
    );

    if (!args_ok) {
        return NULL;
    }

    self->ctx->gl.BindVertexArray(self->glo);
    self->ctx->gl.BindBuffer(GL_ARRAY_BUFFER, buffer->glo);

    if (gltype >> 16 & 1) {
        self->ctx->gl.VertexAttribIPointer(location, size, gltype & 0xffff, stride, offset);
    } else {
        self->ctx->gl.VertexAttribPointer(location, size, gltype & 0xffff, gltype >> 20 & 1, stride, offset);
    }

    self->ctx->gl.VertexAttribDivisor(location, divisor);
    self->ctx->gl.EnableVertexAttribArray(location);
    Py_RETURN_NONE;
}

PyObject * VertexArray_meth_render(VertexArray * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"mode", "vertices", "instances", "condition", NULL};

    // backward compatibility
    int mode = -1;
    int vertices = -1;
    int instances = -1;

    Query * condition = NULL;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "|iiiO!", kw, &mode, &vertices, &instances, Query_type, &condition)) {
        return NULL;
    }

    // backward compatibility
    if (mode == -1) {
        mode = self->mode;
    }

    // backward compatibility
    if (instances == -1) {
        instances = self->instances;
    }

    // backward compatibility
    if (vertices == -1) {
        vertices = self->vertices;
    }

    if (!self->program) {
        PyErr_Format(PyExc_AttributeError, "program");
        return NULL;
    }

    self->ctx->gl.BindVertexArray(self->glo);
    self->ctx->gl.UseProgram(self->program->glo);

    if (self->scope) {
        self->ctx->gl.Viewport(
            self->scope->viewport[0], self->scope->viewport[1], self->scope->viewport[2], self->scope->viewport[3]
        );
        self->ctx->gl.PointSize(self->scope->point_size);
        self->ctx->gl.LineWidth(self->scope->line_width);
        ((self->scope->enable & MGL_BLEND) ? self->ctx->gl.Enable : self->ctx->gl.Disable)(GL_BLEND);
        ((self->scope->enable & MGL_DEPTH_TEST) ? self->ctx->gl.Enable : self->ctx->gl.Disable)(GL_DEPTH_TEST);
        ((self->scope->enable & MGL_CULL_FACE) ? self->ctx->gl.Enable : self->ctx->gl.Disable)(GL_CULL_FACE);
        if (self->scope->bindings) {
            for (int i = 0; i < self->scope->num_samplers; ++i) {
                int location = self->scope->bindings[i].bind;
                int texture_target = self->scope->bindings[i].sampler->texture->texture_target;
                int texture_glo = self->scope->bindings[i].sampler->texture->glo;
                int sampler_glo = self->scope->bindings[i].sampler->glo;
                self->ctx->gl.ActiveTexture(GL_TEXTURE0 + location);
                self->ctx->gl.BindTexture(texture_target, texture_glo);
                self->ctx->gl.BindSampler(location, sampler_glo);
            }
            for (int i = 0; i < self->scope->num_uniform_buffers; ++i) {
                int index = self->scope->bindings[i].bind;
                int buffer_glo = self->scope->bindings[i].buffer->glo;
                int size = self->scope->bindings[i].buffer->size;
                self->ctx->gl.BindBufferRange(GL_UNIFORM_BUFFER, index, buffer_glo, 0, size);
            }
            for (int i = 0; i < self->scope->num_storage_buffers; ++i) {
                int index = self->scope->bindings[i].bind;
                int buffer_glo = self->scope->bindings[i].buffer->glo;
                int size = self->scope->bindings[i].buffer->size;
                self->ctx->gl.BindBufferRange(GL_SHADER_STORAGE_BUFFER, index, buffer_glo, 0, size);
            }
        }
        if (self->scope->framebuffer) {
            self->ctx->gl.BindFramebuffer(GL_FRAMEBUFFER, self->scope->framebuffer->glo);
        }
        if (self->scope->blending) {
            int * ptr = self->scope->blending->params;
            if (self->scope->blending->blend_functions == 1) {
                self->ctx->gl.BlendFuncSeparate(ptr[0], ptr[1], ptr[2], ptr[3]);
                ptr += 4;
            } else {
                for (int i = 0; i < self->scope->blending->blend_functions; ++i) {
                    self->ctx->gl.BlendFuncSeparatei(GL_DRAW_BUFFER0 + i, ptr[0], ptr[1], ptr[2], ptr[3]);
                    ptr += 4;
                }
            }
            if (self->scope->blending->blend_equations == 1) {
                self->ctx->gl.BlendEquationSeparate(ptr[0], ptr[1]);
                ptr += 2;
            } else {
                for (int i = 0; i < self->scope->blending->blend_equations; ++i) {
                    self->ctx->gl.BlendEquationSeparatei(GL_DRAW_BUFFER0 + i, ptr[0], ptr[1]);
                    ptr += 4;
                }
            }
        }
    }

    if (condition) {
        int cond = condition->glo[MGL_ANY_SAMPLES_PASSED];
        if (!cond) {
            cond = condition->glo[MGL_SAMPLES_PASSED];
        }
        self->ctx->gl.BeginConditionalRender(cond, GL_QUERY_NO_WAIT);
    }

    if (self->output_buffer) {
        self->ctx->gl.BindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, self->output_buffer->glo);
        self->ctx->gl.Enable(GL_RASTERIZER_DISCARD);
        self->ctx->gl.BeginTransformFeedback(mode);
    }

    // backward compatibility
    if (self->indirect_buffer) {
        self->ctx->gl.BindBuffer(GL_DRAW_INDIRECT_BUFFER, self->indirect_buffer->glo);
        if (self->index_buffer) {
            self->ctx->gl.MultiDrawElementsIndirect(mode, self->index_format, 0, instances, 20);
        } else {
            self->ctx->gl.MultiDrawArraysIndirect(mode, 0, instances, 16);
        }
        self->ctx->gl.BindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
    } else {
        if (self->index_buffer) {
            self->ctx->gl.DrawElementsInstanced(mode, vertices, self->index_format, 0, instances);
        } else {
            self->ctx->gl.DrawArraysInstanced(mode, 0, vertices, instances);
        }
    }

    if (self->output_buffer) {
        self->ctx->gl.EndTransformFeedback();
        self->ctx->gl.Disable(GL_RASTERIZER_DISCARD);
    }

    if (condition) {
        self->ctx->gl.EndConditionalRender();
    }

    Py_RETURN_NONE;
}

// backward compatibility
PyObject * VertexArray_meth_transform(VertexArray * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"into", "mode", "vertices", "instances", NULL};

    Buffer * into;
    int mode = -1;
    int vertices = -1;
    int instances = -1;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "O!|iii", kw, Buffer_type, &into, &mode, &vertices, &instances)) {
        return NULL;
    }

    Buffer * old = self->output_buffer;
    self->output_buffer = into;
    PyObject * res = PyObject_CallMethod((PyObject *)self, "render", "iii", mode, vertices, instances);
    ensurei(res);

    self->output_buffer = old;
    return res;
}

Scope * VertexArray_get_scope(VertexArray * self) {
    return new_ref(self->scope);
}

int VertexArray_set_scope(VertexArray * self, Scope * value) {
    if (Py_TYPE(value) != Scope_type) {
        return -1;
    }
    Scope * old = self->scope;
    self->scope = new_ref(value);
    Py_XDECREF(old);
    return 0;
}

PyObject * VertexArray_get_program(VertexArray * self) {
    if (!self->program) {
        Py_RETURN_NONE;
    }
    return (PyObject *)new_ref(self->program);
}

int VertexArray_set_program(VertexArray * self, Program * value) {
    if (Py_TYPE(value) != Program_type) {
        return -1;
    }
    Program * old = self->program;
    self->program = new_ref(value);
    Py_XDECREF(old);
    return 0;
}

PyObject * VertexArray_get_index_element_size(VertexArray * self) {
    switch (self->index_format) {
        case GL_UNSIGNED_BYTE: return PyLong_FromLong(1);
        case GL_UNSIGNED_SHORT: return PyLong_FromLong(2);
        case GL_UNSIGNED_INT: return PyLong_FromLong(4);
        default: return NULL;
    }
}

int VertexArray_set_index_element_size(VertexArray * self, PyObject * value) {
    int size = PyLong_AsLong(value);
    if (size != 1 && size != 2 && size != 4) {
        return -1;
    }
    const int formats[] = {0, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, 0, GL_UNSIGNED_INT};
    self->index_format = formats[size];
    return 0;
}

PyObject * VertexArray_get_index_buffer(VertexArray * self) {
    if (self->index_buffer) {
        return (PyObject *)new_ref(self->index_buffer);
    }
    Py_RETURN_NONE;
}

int VertexArray_set_index_buffer(VertexArray * self, Buffer * value) {
    self->ctx->gl.BindVertexArray(self->glo);
    Buffer * old = self->index_buffer;
    if (value == (Buffer *)Py_None) {
        self->index_buffer = NULL;
        self->ctx->gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    } else {
        if (Py_TYPE(value) != Buffer_type) {
            return -1;
        }
        self->index_buffer = new_ref(value);
        self->ctx->gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->index_buffer->glo);
    }
    Py_XDECREF(old);
    return 0;
}

PyObject * VertexArray_get_output_buffer(VertexArray * self) {
    if (self->output_buffer) {
        return (PyObject *)new_ref(self->output_buffer);
    }
    Py_RETURN_NONE;
}

int VertexArray_set_output_buffer(VertexArray * self, Buffer * value) {
    Buffer * old = self->output_buffer;
    if (value == (Buffer *)Py_None) {
        self->output_buffer = NULL;
    } else {
        if (Py_TYPE(value) != Buffer_type) {
            return -1;
        }
        self->output_buffer = new_ref(value);
    }
    Py_XDECREF(old);
    return 0;
}

PyObject * VertexArray_get_indirect_buffer(VertexArray * self) {
    if (self->indirect_buffer) {
        return (PyObject *)new_ref(self->indirect_buffer);
    }
    Py_RETURN_NONE;
}

int VertexArray_set_indirect_buffer(VertexArray * self, Buffer * value) {
    Buffer * old = self->indirect_buffer;
    if (value == (Buffer *)Py_None) {
        self->indirect_buffer = NULL;
    } else {
        if (Py_TYPE(value) != Buffer_type) {
            return -1;
        }
        self->indirect_buffer = new_ref(value);
    }
    Py_XDECREF(old);
    return 0;
}

PyMethodDef VertexArray_methods[] = {
    {"bind", (PyCFunction)VertexArray_meth_bind, METH_VARARGS, NULL},
    {"render", (PyCFunction)VertexArray_meth_render, METH_VARARGS | METH_KEYWORDS, NULL},
    {"transform", (PyCFunction)VertexArray_meth_transform, METH_VARARGS | METH_KEYWORDS, NULL},
    {},
};

PyGetSetDef VertexArray_getset[] = {
    {"scope", (getter)VertexArray_get_scope, (setter)VertexArray_set_scope, NULL, NULL},
    {"program", (getter)VertexArray_get_program, (setter)VertexArray_set_program, NULL, NULL},
    {"index_element_size", (getter)VertexArray_get_index_element_size, (setter)VertexArray_set_index_element_size, NULL, NULL},
    {"index_buffer", (getter)VertexArray_get_index_buffer, (setter)VertexArray_set_index_buffer, "ibo", NULL},
    {"output_buffer", (getter)VertexArray_get_output_buffer, (setter)VertexArray_set_output_buffer, NULL, NULL},
    {"indirect_buffer", (getter)VertexArray_get_indirect_buffer, (setter)VertexArray_set_indirect_buffer, NULL, NULL},
    {},
};

PyMemberDef VertexArray_members[] = {
    {"mode", T_INT, offsetof(VertexArray, mode), 0, NULL},
    {"vertices", T_INT, offsetof(VertexArray, vertices), 0, NULL},
    {"instances", T_INT, offsetof(VertexArray, instances), 0, NULL},
    {"glo", T_INT, offsetof(VertexArray, glo), READONLY, NULL},
    {"extra", T_OBJECT_EX, offsetof(BaseObject, extra), 0, NULL},
    {},
};

PyType_Slot VertexArray_slots[] = {
    {Py_tp_methods, VertexArray_methods},
    {Py_tp_getset, VertexArray_getset},
    {Py_tp_members, VertexArray_members},
    {Py_tp_dealloc, (void *)BaseObject_dealloc},
    {},
};

PyType_Spec VertexArray_spec = {
    "moderngl.VertexArray",
    sizeof(VertexArray),
    0,
    Py_TPFLAGS_DEFAULT,
    VertexArray_slots
};

#pragma endregion
#pragma region Context

Context * moderngl_meth_context(PyObject * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"standalone", "glversion", "glhooks", NULL};
    int standalone = false;
    int glversion = 330;

    PyObject * glhooks = Py_None;
    if (!PyArg_ParseTupleAndKeywords(args, kwa, "|piO", kw, &standalone, &glversion, &glhooks)) {
        return NULL;
    }

    Context * res = PyObject_New(Context, Context_type);
    ensurei(res);

    res->next = res;
    res->prev = res;
    res->extra = NULL;

    if (!res->ctx.load(standalone, glversion)) {
        PyErr_Format(PyExc_Exception, "%s", res->ctx.error);
        return NULL;
    }

    if (!res->gl.load()) {
        PyErr_Format(PyExc_Exception, "cannot load OpenGL");
        return NULL;
    }

    if (glhooks != Py_None) {
        PyObject * glprocs = PyMemoryView_FromMemory((char *)&res->gl, sizeof(GLMethods), PyBUF_WRITE);
        PyObject * call = PyObject_CallFunction(glhooks, "N", glprocs);
        Py_XDECREF(call);
        if (!call) {
            return NULL;
        }
    }

    if (!glversion) {
        int major = 0;
        int minor = 0;
        res->gl.GetIntegerv(GL_MAJOR_VERSION, &major);
        res->gl.GetIntegerv(GL_MINOR_VERSION, &minor);
        glversion = major * 100 + minor * 10;
        if (!glversion) {
            res->gl.GetError();
            const char * ver = (const char *)res->gl.GetString(GL_VERSION);
            if (ver && '0' <= ver[0] && ver[0] <= '9' && ver[1] == '.' && '0' <= ver[2] && ver[2] <= '9') {
                glversion = (ver[0] - '0') * 100 + (ver[2] - '0') * 10;
            }
        }
    }

    res->module = self;
    res->tools = PyObject_GetAttrString(self, "mgl");

    res->gl.Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    res->gl.Enable(GL_PRIMITIVE_RESTART);
    res->gl.PrimitiveRestartIndex(-1);

    res->screen = PyObject_NewVar(Framebuffer, Framebuffer_type, 1);
    res->screen->prev = res;
    res->screen->next = res->next;
    res->next = res->screen;
    res->screen->ctx = res;
    res->screen->extra = NULL;

    res->default_blending = PyObject_NewVar(Blending, Blending_type, 4);
    res->default_blending->prev = res;
    res->default_blending->next = res->next;
    res->next = res->default_blending;
    res->default_blending->ctx = res;
    res->default_blending->extra = NULL;

    res->default_blending->blend_functions = -1;
    res->default_blending->blend_equations = 0;

    res->default_blending->params[0] = GL_SRC_ALPHA;
    res->default_blending->params[1] = GL_ONE_MINUS_SRC_ALPHA;
    res->default_blending->params[2] = GL_SRC_ALPHA;
    res->default_blending->params[3] = GL_ONE_MINUS_SRC_ALPHA;

    res->default_scope = PyObject_New(Scope, Scope_type);
    res->default_scope->prev = res;
    res->default_scope->next = res->next;
    res->next = res->default_scope;
    res->default_scope->ctx = res;
    res->default_scope->extra = NULL;
    res->default_scope->bindings_lst = PyList_New(0);

    res->default_scope->point_size = 1.0f;
    res->default_scope->line_width = 1.0f;
    res->default_scope->enable = MGL_NOTHING;
    res->default_scope->framebuffer = new_ref(res->screen);
    res->default_scope->num_samplers = 0;
    res->default_scope->num_uniform_buffers = 0;
    res->default_scope->num_storage_buffers = 0;

    res->gl.GetIntegerv(GL_VIEWPORT, res->default_scope->viewport);

    int max_texture_units = 0;
	res->gl.GetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
	res->default_texture_unit = GL_TEXTURE0 + max_texture_units - 1;

    res->screen->glo = 0;
    res->screen->color_attachments[0].shape = 'f';
    res->screen->color_attachments[0].components = 4;
    res->screen->width = res->default_scope->viewport[2];
    res->screen->height = res->default_scope->viewport[3];
    res->screen->samples = 0;

    res->consts.version = Py_BuildValue("s", res->gl.GetString(GL_VERSION));
    res->consts.renderer = Py_BuildValue("s", res->gl.GetString(GL_RENDERER));
    res->consts.vendor = Py_BuildValue("s", res->gl.GetString(GL_VENDOR));

    res->consts.blend = PyLong_FromLong(MGL_BLEND);
    res->consts.depth_test = PyLong_FromLong(MGL_DEPTH_TEST);
    res->consts.cull_face = PyLong_FromLong(MGL_CULL_FACE);

    res->consts.clamp_to_edge_x = PyLong_FromLong(MGL_CLAMP_TO_EDGE_X);
    res->consts.clamp_to_edge_y = PyLong_FromLong(MGL_CLAMP_TO_EDGE_Y);
    res->consts.clamp_to_edge_z = PyLong_FromLong(MGL_CLAMP_TO_EDGE_Z);
    res->consts.repeat_x = PyLong_FromLong(MGL_REPEAT_X);
    res->consts.repeat_y = PyLong_FromLong(MGL_REPEAT_Y);
    res->consts.repeat_z = PyLong_FromLong(MGL_REPEAT_Z);
    res->consts.mirrored_repeat_x = PyLong_FromLong(MGL_MIRRORED_REPEAT_X);
    res->consts.mirrored_repeat_y = PyLong_FromLong(MGL_MIRRORED_REPEAT_Y);
    res->consts.mirrored_repeat_z = PyLong_FromLong(MGL_MIRRORED_REPEAT_Z);
    res->consts.mirror_clamp_to_edge_x = PyLong_FromLong(MGL_MIRROR_CLAMP_TO_EDGE_X);
    res->consts.mirror_clamp_to_edge_y = PyLong_FromLong(MGL_MIRROR_CLAMP_TO_EDGE_Y);
    res->consts.mirror_clamp_to_edge_z = PyLong_FromLong(MGL_MIRROR_CLAMP_TO_EDGE_Z);
    res->consts.clamp_to_border_x = PyLong_FromLong(MGL_CLAMP_TO_BORDER_X);
    res->consts.clamp_to_border_y = PyLong_FromLong(MGL_CLAMP_TO_BORDER_Y);
    res->consts.clamp_to_border_z = PyLong_FromLong(MGL_CLAMP_TO_BORDER_Z);

    res->consts.zero = PyLong_FromLong(GL_ZERO);
    res->consts.one = PyLong_FromLong(GL_ONE);
    res->consts.src_color = PyLong_FromLong(GL_SRC_COLOR);
    res->consts.src_alpha = PyLong_FromLong(GL_SRC_ALPHA);
    res->consts.dst_color = PyLong_FromLong(GL_DST_COLOR);
    res->consts.dst_alpha = PyLong_FromLong(GL_DST_ALPHA);
    res->consts.one_minus_src_color = PyLong_FromLong(GL_ONE_MINUS_SRC_COLOR);
    res->consts.one_minus_src_alpha = PyLong_FromLong(GL_ONE_MINUS_SRC_ALPHA);
    res->consts.one_minus_dst_color = PyLong_FromLong(GL_ONE_MINUS_DST_COLOR);
    res->consts.one_minus_dst_alpha = PyLong_FromLong(GL_ONE_MINUS_DST_ALPHA);

    res->consts.first_vertex_convention = PyLong_FromLong(GL_FIRST_VERTEX_CONVENTION);
    res->consts.last_vertex_convention = PyLong_FromLong(GL_LAST_VERTEX_CONVENTION);

    res->consts.nearest = PyLong_FromLong(GL_NEAREST);
    res->consts.linear = PyLong_FromLong(GL_LINEAR);
    res->consts.nearest_mipmap_nearest = PyLong_FromLong(GL_NEAREST_MIPMAP_NEAREST);
    res->consts.linear_mipmap_nearest = PyLong_FromLong(GL_LINEAR_MIPMAP_NEAREST);
    res->consts.nearest_mipmap_linear = PyLong_FromLong(GL_NEAREST_MIPMAP_LINEAR);
    res->consts.linear_mipmap_linear = PyLong_FromLong(GL_LINEAR_MIPMAP_LINEAR);

    res->consts.points = PyLong_FromLong(GL_POINTS);
    res->consts.lines = PyLong_FromLong(GL_LINES);
    res->consts.line_loop = PyLong_FromLong(GL_LINE_LOOP);
    res->consts.line_strip = PyLong_FromLong(GL_LINE_STRIP);
    res->consts.triangles = PyLong_FromLong(GL_TRIANGLES);
    res->consts.triangle_strip = PyLong_FromLong(GL_TRIANGLE_STRIP);
    res->consts.triangle_fan = PyLong_FromLong(GL_TRIANGLE_FAN);
    res->consts.lines_adjacency = PyLong_FromLong(GL_LINES_ADJACENCY);
    res->consts.line_strip_adjacency = PyLong_FromLong(GL_LINE_STRIP_ADJACENCY);
    res->consts.triangles_adjacency = PyLong_FromLong(GL_TRIANGLES_ADJACENCY);
    res->consts.triangle_strip_adjacency = PyLong_FromLong(GL_TRIANGLE_STRIP_ADJACENCY);
    res->consts.patches = PyLong_FromLong(GL_PATCHES);

    res->limits = PyObject_CallMethod(res->tools, "Limits", "N", get_limits(res->gl, glversion));

    return new_ref(res);
}

PyObject * Context_meth_objects(Context * self) {
    PyObject * res = PyList_New(0);
    BaseObject * obj = self->next;
    while (obj != self) {
        PyList_Append(res, (PyObject *)obj);
    }
    return res;
}

PyObject * Context_meth_release(Context * self, BaseObject * arg) {
    PyTypeObject * t = Py_TYPE(arg);
    if (t != Buffer_type && t != Context_type && t != Framebuffer_type && t != Program_type && t != Query_type &&
        t != Renderbuffer_type && t != Sampler_type && t != Scope_type && t != Texture_type && t != VertexArray_type) {
        PyErr_BadInternalCall();
        return NULL;
    }
    if (!arg->next->prev && !arg->next->next) {
        PyErr_BadInternalCall();
        return NULL;
    }
    arg->next->prev = arg->prev;
    arg->prev->next = arg->next;
    arg->prev = NULL;
    arg->next = NULL;
    if (t == Buffer_type) {
        Buffer * obj = (Buffer *)arg;
        self->gl.DeleteBuffers(1, (GLuint *)&obj->glo);
        obj->glo = 0;
    }
    if (t == Context_type) {
        Context * obj = (Context *)arg;
    }
    if (t == Framebuffer_type) {
        Framebuffer * obj = (Framebuffer *)arg;
        self->gl.DeleteFramebuffers(1, (GLuint *)&obj->glo);
        obj->glo = 0;
    }
    if (t == Program_type) {
        Program * obj = (Program *)arg;
        self->gl.DeleteProgram(obj->glo);
        obj->glo = 0;
    }
    if (t == Query_type) {
        Query * obj = (Query *)arg;
        for (int i = 0; i < 4; ++i) {
            if (obj->glo[i]) {
                self->gl.DeleteQueries(1, (GLuint *)&obj->glo[i]);
                obj->glo[i] = 0;
            }
        }
    }
    if (t == Renderbuffer_type) {
        Renderbuffer * obj = (Renderbuffer *)arg;
        self->gl.DeleteRenderbuffers(1, (GLuint *)&obj->glo);
        obj->glo = 0;
    }
    if (t == Sampler_type) {
        Sampler * obj = (Sampler *)arg;
        self->gl.DeleteSamplers(1, (GLuint *)&obj->glo);
        obj->glo = 0;
    }
    if (t == Scope_type) {
        Scope * obj = (Scope *)arg;
    }
    if (t == Texture_type) {
        Texture * obj = (Texture *)arg;
        self->gl.DeleteTextures(1, (GLuint *)&obj->glo);
        obj->glo = 0;
    }
    if (t == VertexArray_type) {
        VertexArray * obj = (VertexArray *)arg;
        self->gl.DeleteVertexArrays(1, (GLuint *)&obj->glo);
        obj->glo = 0;
    }
    Py_DECREF(arg);
    Py_RETURN_NONE;
}

// backward compatibility
PyObject * Context_meth_clear(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"red", "green", "blue", "alpha", "depth", "viewport", NULL};
    float red = 0.0f, green = 0.0f, blue = 0.0f, alpha = 0.0f, depth = 1.0f;
    PyObject * viewport = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwa, "|fffffO", kw, &red, &green, &blue, &alpha, &depth, &viewport)) {
        return NULL;
    }
    return PyObject_CallMethod((PyObject *)self->screen, "clear", "(ffff)f", red, green, blue, alpha, depth);
}

// backward compatibility
PyObject * Context_meth_enable(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"enable", NULL};
    int flags;
    if (!PyArg_ParseTupleAndKeywords(args, kwa, "i", kw, &flags)) {
        return NULL;
    }
    self->default_scope->enable |= flags;
    Py_RETURN_NONE;
}

// backward compatibility
PyObject * Context_meth_disable(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"enable", NULL};
    int flags;
    if (!PyArg_ParseTupleAndKeywords(args, kwa, "i", kw, &flags)) {
        return NULL;
    }
    self->default_scope->enable &= ~flags;
    Py_RETURN_NONE;
}

// backward compatibility
PyObject * Context_meth_enable_only(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"enable", NULL};
    int flags;
    if (!PyArg_ParseTupleAndKeywords(args, kwa, "i", kw, &flags)) {
        return NULL;
    }
    self->default_scope->enable = flags;
    Py_RETURN_NONE;
}

// backward compatibility
PyObject * Context_meth_copy_buffer(Context * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"dst", "src", NULL};
    PyObject * dst;
    PyObject * src;
    if (!PyArg_ParseTupleAndKeywords(args, kwa, "OO", kw, &dst, &src)) {
        return NULL;
    }
    return PyObject_CallMethod(src, "read", "iiO", -1, 0, dst);
}

PyObject * Context_meth_enter(Context * self) {
    self->ctx.enter();
    Py_RETURN_NONE;
}

PyObject * Context_meth_exit(Context * self, PyObject * args) {
    self->ctx.exit();
    Py_RETURN_NONE;
}

PyMethodDef Context_methods[] = {
    {"blending", (PyCFunction)Context_meth_blending, METH_VARARGS | METH_KEYWORDS, NULL},
    {"buffer", (PyCFunction)Context_meth_buffer, METH_VARARGS | METH_KEYWORDS, NULL},
    {"framebuffer", (PyCFunction)Context_meth_framebuffer, METH_VARARGS | METH_KEYWORDS, NULL},
    {"program", (PyCFunction)Context_meth_program, METH_VARARGS | METH_KEYWORDS, NULL},
    {"query", (PyCFunction)Context_meth_query, METH_VARARGS | METH_KEYWORDS, NULL},
    {"renderbuffer", (PyCFunction)Context_meth_renderbuffer, METH_VARARGS | METH_KEYWORDS, NULL},
    {"sampler", (PyCFunction)Context_meth_sampler, METH_VARARGS | METH_KEYWORDS, NULL},
    {"scope", (PyCFunction)Context_meth_scope, METH_VARARGS | METH_KEYWORDS, NULL},
    {"texture", (PyCFunction)Context_meth_texture, METH_VARARGS | METH_KEYWORDS, NULL},
    {"texture_from", (PyCFunction)Context_meth_texture_from, METH_VARARGS | METH_KEYWORDS, NULL},
    {"vertex_array", (PyCFunction)Context_meth_vertex_array, METH_VARARGS | METH_KEYWORDS, NULL},
    {"objects", (PyCFunction)Context_meth_objects, METH_NOARGS, NULL},
    {"release", (PyCFunction)Context_meth_release, METH_O, NULL},

    // backward compatibility
    {"clear", (PyCFunction)Context_meth_clear, METH_VARARGS | METH_KEYWORDS, NULL},
    {"enable", (PyCFunction)Context_meth_enable, METH_VARARGS | METH_KEYWORDS, NULL},
    {"disable", (PyCFunction)Context_meth_disable, METH_VARARGS | METH_KEYWORDS, NULL},
    {"enable_only", (PyCFunction)Context_meth_enable_only, METH_VARARGS | METH_KEYWORDS, NULL},
    {"simple_vertex_array", (PyCFunction)Context_meth_vertex_array, METH_VARARGS | METH_KEYWORDS, NULL},
    {"copy_buffer", (PyCFunction)Context_meth_copy_buffer, METH_VARARGS | METH_KEYWORDS, NULL},

    {"__enter__", (PyCFunction)Context_meth_enter, METH_NOARGS, NULL},
    {"__exit__", (PyCFunction)Context_meth_exit, METH_VARARGS, NULL},
    {},
};

// backward compatibility
PyObject * Context_get_info(Context * self) {
    PyObject * res = PyDict_New();
    PyDict_SetItemString(res, "GL_VENDOR", self->consts.vendor);
    PyDict_SetItemString(res, "GL_RENDERER", self->consts.renderer);
    PyDict_SetItemString(res, "GL_VERSION", self->consts.version);
    return res;
}

// backward compatibility
PyObject * Context_get_version_code(Context * self) {
    return PyLong_FromLong(self->ctx.glversion);
}

// backward compatibility
PyObject * Context_get_viewport(Context * self) {
    return PyObject_GetAttrString((PyObject *)self->default_scope, "viewport");
}

// backward compatibility
int Context_set_viewport(Context * self, PyObject * value) {
    return PyObject_SetAttrString((PyObject *)self->default_scope, "viewport", value);
}

// backward compatibility
PyObject * Context_get_point_size(Context * self) {
    return PyObject_GetAttrString((PyObject *)self->default_scope, "point_size");
}

// backward compatibility
int Context_set_point_size(Context * self, PyObject * value) {
    return PyObject_SetAttrString((PyObject *)self->default_scope, "point_size", value);
}

// backward compatibility
PyObject * Context_get_line_width(Context * self) {
    return PyObject_GetAttrString((PyObject *)self->default_scope, "line_width");
}

// backward compatibility
int Context_set_line_width(Context * self, PyObject * value) {
    return PyObject_SetAttrString((PyObject *)self->default_scope, "line_width", value);
}

PyGetSetDef Context_getset[] = {
    // backward compatibility
    {"info", (getter)Context_get_info, NULL, NULL, NULL},
    {"version_code", (getter)Context_get_version_code, NULL, NULL, NULL},
    {"viewport", (getter)Context_get_viewport, (setter)Context_set_viewport, NULL, NULL},
    {"point_size", (getter)Context_get_line_width, (setter)Context_set_point_size, NULL, NULL},
    {"line_width", (getter)Context_get_point_size, (setter)Context_set_line_width, NULL, NULL},
    {},
};

PyMemberDef Context_members[] = {
    {"limits", T_OBJECT_EX, offsetof(Context, limits), READONLY, NULL},
    {"screen", T_OBJECT_EX, offsetof(Context, screen), READONLY, NULL},
    {"extra", T_OBJECT_EX, offsetof(BaseObject, extra), 0, NULL},

    {"VERSION", T_OBJECT, offsetof(Context, consts.version), READONLY, NULL},
    {"RENDERER", T_OBJECT, offsetof(Context, consts.renderer), READONLY, NULL},
    {"VENDOR", T_OBJECT, offsetof(Context, consts.vendor), READONLY, NULL},

    {"BLEND", T_OBJECT, offsetof(Context, consts.blend), READONLY, NULL},
    {"DEPTH_TEST", T_OBJECT, offsetof(Context, consts.depth_test), READONLY, NULL},
    {"CULL_FACE", T_OBJECT, offsetof(Context, consts.cull_face), READONLY, NULL},

    {"CLAMP_TO_EDGE_X", T_OBJECT, offsetof(Context, consts.clamp_to_edge_x), READONLY, NULL},
    {"CLAMP_TO_EDGE_Y", T_OBJECT, offsetof(Context, consts.clamp_to_edge_y), READONLY, NULL},
    {"CLAMP_TO_EDGE_Z", T_OBJECT, offsetof(Context, consts.clamp_to_edge_z), READONLY, NULL},
    {"REPEAT_X", T_OBJECT, offsetof(Context, consts.repeat_x), READONLY, NULL},
    {"REPEAT_Y", T_OBJECT, offsetof(Context, consts.repeat_y), READONLY, NULL},
    {"REPEAT_Z", T_OBJECT, offsetof(Context, consts.repeat_z), READONLY, NULL},
    {"MIRRORED_REPEAT_X", T_OBJECT, offsetof(Context, consts.mirrored_repeat_x), READONLY, NULL},
    {"MIRRORED_REPEAT_Y", T_OBJECT, offsetof(Context, consts.mirrored_repeat_y), READONLY, NULL},
    {"MIRRORED_REPEAT_Z", T_OBJECT, offsetof(Context, consts.mirrored_repeat_z), READONLY, NULL},
    {"MIRROR_CLAMP_TO_EDGE_X", T_OBJECT, offsetof(Context, consts.mirror_clamp_to_edge_x), READONLY, NULL},
    {"MIRROR_CLAMP_TO_EDGE_Y", T_OBJECT, offsetof(Context, consts.mirror_clamp_to_edge_y), READONLY, NULL},
    {"MIRROR_CLAMP_TO_EDGE_Z", T_OBJECT, offsetof(Context, consts.mirror_clamp_to_edge_z), READONLY, NULL},
    {"CLAMP_TO_BORDER_X", T_OBJECT, offsetof(Context, consts.clamp_to_border_x), READONLY, NULL},
    {"CLAMP_TO_BORDER_Y", T_OBJECT, offsetof(Context, consts.clamp_to_border_y), READONLY, NULL},
    {"CLAMP_TO_BORDER_Z", T_OBJECT, offsetof(Context, consts.clamp_to_border_z), READONLY, NULL},

    {"ZERO", T_OBJECT, offsetof(Context, consts.zero), READONLY, NULL},
    {"ONE", T_OBJECT, offsetof(Context, consts.one), READONLY, NULL},
    {"SRC_COLOR", T_OBJECT, offsetof(Context, consts.src_color), READONLY, NULL},
    {"SRC_ALPHA", T_OBJECT, offsetof(Context, consts.src_alpha), READONLY, NULL},
    {"DST_COLOR", T_OBJECT, offsetof(Context, consts.dst_color), READONLY, NULL},
    {"DST_ALPHA", T_OBJECT, offsetof(Context, consts.dst_alpha), READONLY, NULL},
    {"ONE_MINUS_SRC_COLOR", T_OBJECT, offsetof(Context, consts.one_minus_src_color), READONLY, NULL},
    {"ONE_MINUS_SRC_ALPHA", T_OBJECT, offsetof(Context, consts.one_minus_src_alpha), READONLY, NULL},
    {"ONE_MINUS_DST_COLOR", T_OBJECT, offsetof(Context, consts.one_minus_dst_color), READONLY, NULL},
    {"ONE_MINUS_DST_ALPHA", T_OBJECT, offsetof(Context, consts.one_minus_dst_alpha), READONLY, NULL},

    {"FIRST_VERTEX_CONVENTION", T_OBJECT, offsetof(Context, consts.first_vertex_convention), READONLY, NULL},
    {"LAST_VERTEX_CONVENTION", T_OBJECT, offsetof(Context, consts.last_vertex_convention), READONLY, NULL},

    {"NEAREST", T_OBJECT, offsetof(Context, consts.nearest), READONLY, NULL},
    {"LINEAR", T_OBJECT, offsetof(Context, consts.linear), READONLY, NULL},
    {"NEAREST_MIPMAP_NEAREST", T_OBJECT, offsetof(Context, consts.nearest_mipmap_nearest), READONLY, NULL},
    {"LINEAR_MIPMAP_NEAREST", T_OBJECT, offsetof(Context, consts.linear_mipmap_nearest), READONLY, NULL},
    {"NEAREST_MIPMAP_LINEAR", T_OBJECT, offsetof(Context, consts.nearest_mipmap_linear), READONLY, NULL},
    {"LINEAR_MIPMAP_LINEAR", T_OBJECT, offsetof(Context, consts.linear_mipmap_linear), READONLY, NULL},

    {"POINTS", T_OBJECT, offsetof(Context, consts.points), READONLY, NULL},
    {"LINES", T_OBJECT, offsetof(Context, consts.lines), READONLY, NULL},
    {"LINE_LOOP", T_OBJECT, offsetof(Context, consts.line_loop), READONLY, NULL},
    {"LINE_STRIP", T_OBJECT, offsetof(Context, consts.line_strip), READONLY, NULL},
    {"TRIANGLES", T_OBJECT, offsetof(Context, consts.triangles), READONLY, NULL},
    {"TRIANGLE_STRIP", T_OBJECT, offsetof(Context, consts.triangle_strip), READONLY, NULL},
    {"TRIANGLE_FAN", T_OBJECT, offsetof(Context, consts.triangle_fan), READONLY, NULL},
    {"LINES_ADJACENCY", T_OBJECT, offsetof(Context, consts.lines_adjacency), READONLY, NULL},
    {"LINE_STRIP_ADJACENCY", T_OBJECT, offsetof(Context, consts.line_strip_adjacency), READONLY, NULL},
    {"TRIANGLES_ADJACENCY", T_OBJECT, offsetof(Context, consts.triangles_adjacency), READONLY, NULL},
    {"TRIANGLE_STRIP_ADJACENCY", T_OBJECT, offsetof(Context, consts.triangle_strip_adjacency), READONLY, NULL},
    {"PATCHES", T_OBJECT, offsetof(Context, consts.patches), READONLY, NULL},
    {},
};

PyType_Slot Context_slots[] = {
    {Py_tp_methods, Context_methods},
    {Py_tp_getset, Context_getset},
    {Py_tp_members, Context_members},
    {Py_tp_dealloc, (void *)BaseObject_dealloc},
    {},
};

PyType_Spec Context_spec = {
    "moderngl.Context",
    sizeof(Context),
    0,
    Py_TPFLAGS_DEFAULT,
    Context_slots
};

#pragma endregion
#pragma region Module

PyDoc_STRVAR(moderngl_meth_context_doc,
"context(standalone, glversion, glhooks)\n"
":rtype: _moderngl.Context");

PyDoc_STRVAR(moderngl_doc,
"High Performance Rendering for Python.");

// backward compatibility
PyObject * moderngl_meth_create_context(PyObject * self, PyObject * args, PyObject * kwa) {
    static char * kw[] = {"require", NULL};

    PyObject * require = Py_None;

    if (!PyArg_ParseTupleAndKeywords(args, kwa, "|O", kw, &require)) {
        return NULL;
    }

    if (require == Py_None) {
        require = PyLong_FromLong(330);
    }

    return PyObject_CallMethod(self, "context", "OO", Py_False, require);
}

PyMethodDef module_methods[] = {
    {"context", (PyCFunction)moderngl_meth_context, METH_VARARGS | METH_KEYWORDS, moderngl_meth_context_doc},
    {"pack", (PyCFunction)moderngl_meth_pack, METH_VARARGS | METH_KEYWORDS, NULL},

    // backward compatibility
    {"create_context", (PyCFunction)moderngl_meth_create_context, METH_VARARGS | METH_KEYWORDS, NULL},
    {},
};

PyModuleDef module_def = {PyModuleDef_HEAD_INIT, "moderngl", moderngl_doc, -1, module_methods};

extern "C" PyObject * PyInit_moderngl() {
    PyObject * module = PyModule_Create(&module_def);
    PyModule_AddStringConstant(module, "__version__", "6.0.dev1");

    for (int i = 0; i < 64; ++i) {
        default_draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    PyObject * mgl = PyImport_ImportModule("_moderngl");
    if (!mgl) {
        return NULL;
    }

    PyModule_AddObject(module, "mgl", mgl);
    Py_DECREF(mgl);

    empty_dict = PyDict_New();
    empty_tuple = PyTuple_New(0);
    default_filter = Py_BuildValue("ii", GL_NEAREST, GL_NEAREST);
    default_wrap = PyLong_FromLong(0);
    default_anisotropy = PyFloat_FromDouble(1.0);
    default_lod_range = Py_BuildValue("ff", -1000.0, 1000.0);
    default_lod_bias = PyFloat_FromDouble(0.0);
    default_border = Py_BuildValue("ffff", 0.0f, 0.0f, 0.0f, 0.0f);

    Context_type = (PyTypeObject *)PyType_FromSpec(&Context_spec);
    Blending_type = (PyTypeObject *)PyType_FromSpec(&Blending_spec);
    Buffer_type = (PyTypeObject *)PyType_FromSpec(&Buffer_spec);
    Framebuffer_type = (PyTypeObject *)PyType_FromSpec(&Framebuffer_spec);
    Program_type = (PyTypeObject *)PyType_FromSpec(&Program_spec);
    Query_type = (PyTypeObject *)PyType_FromSpec(&Query_spec);
    Renderbuffer_type = (PyTypeObject *)PyType_FromSpec(&Renderbuffer_spec);
    Sampler_type = (PyTypeObject *)PyType_FromSpec(&Sampler_spec);
    Scope_type = (PyTypeObject *)PyType_FromSpec(&Scope_spec);
    Texture_type = (PyTypeObject *)PyType_FromSpec(&Texture_spec);
    VertexArray_type = (PyTypeObject *)PyType_FromSpec(&VertexArray_spec);

    PyModule_AddObject(module, "Context", (PyObject *)Context_type);
    PyModule_AddObject(module, "Blending", (PyObject *)Blending_type);
    PyModule_AddObject(module, "Buffer", (PyObject *)Buffer_type);
    PyModule_AddObject(module, "Framebuffer", (PyObject *)Framebuffer_type);
    PyModule_AddObject(module, "Program", (PyObject *)Program_type);
    PyModule_AddObject(module, "Query", (PyObject *)Query_type);
    PyModule_AddObject(module, "Renderbuffer", (PyObject *)Renderbuffer_type);
    PyModule_AddObject(module, "Sampler", (PyObject *)Sampler_type);
    PyModule_AddObject(module, "Scope", (PyObject *)Scope_type);
    PyModule_AddObject(module, "Texture", (PyObject *)Texture_type);
    PyModule_AddObject(module, "VertexArray", (PyObject *)VertexArray_type);

    // backward compatibility
    PyModule_AddObject(module, "Texture3D", (PyObject *)Texture_type);
    PyModule_AddObject(module, "TextureArray", (PyObject *)Texture_type);
    PyModule_AddObject(module, "TextureCube", (PyObject *)Texture_type);

    // backward compatibility
    PyModule_AddIntConstant(module, "TRIANGLES", GL_TRIANGLES);
    PyModule_AddIntConstant(module, "TRIANGLE_FAN", GL_TRIANGLE_FAN);
    PyModule_AddIntConstant(module, "TRIANGLE_STRIP", GL_TRIANGLE_STRIP);
    PyModule_AddIntConstant(module, "TRIANGLES_ADJACENCY", GL_TRIANGLES_ADJACENCY);
    PyModule_AddIntConstant(module, "TRIANGLE_STRIP_ADJACENCY", GL_TRIANGLE_STRIP_ADJACENCY);
    PyModule_AddIntConstant(module, "POINTS", GL_POINTS);
    PyModule_AddIntConstant(module, "LINES", GL_LINES);
    PyModule_AddIntConstant(module, "LINE_STRIP", GL_LINE_STRIP);
    PyModule_AddIntConstant(module, "LINE_LOOP", GL_LINE_LOOP);
    PyModule_AddIntConstant(module, "LINES_ADJACENCY", GL_LINES_ADJACENCY);

    PyObject * math_module = PyInit_moderngl_math();
    PyModule_AddObject(module, "math", math_module);
    return module;
}

#pragma endregion
