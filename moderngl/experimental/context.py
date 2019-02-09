import importlib
from typing import Any

from . import mgl
from .buffer import Buffer
from .compute_shader import ComputeShader
from .framebuffer import Framebuffer
from .limits import Limits
from .program import Program
from .query import Query
from .renderbuffer import Renderbuffer
from .sampler import Sampler
from .scope import Scope
from .texture import Texture
from .vertex_array import VertexArray


def _attr_fmt(attr):
    return '%d%c' % (attr.cols * attr.rows * attr.size, attr.shape)


class Context:
    __slots__ = ['__mglo', 'version_code', 'limits', 'screen', 'fbo', 'recorder', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.version_code = None  # type: int
        self.limits = None  # type: Limits
        self.screen = None  # type: Framebuffer
        self.fbo = None  # type: Framebuffer
        self.recorder = None  # type: Any
        self.extra = None  # type: Any

    def buffer(self, data=None, reserve=0, readable=True, writable=True, local=False) -> Buffer:
        return self.__mglo.buffer(data, reserve, readable, writable, local)

    def compute_shader(self, source) -> ComputeShader:
        return self.__mglo.compute_shader(source)

    def program(self, vertex_shader, fragment_shader=None, geometry_shader=None, tess_control_shader=None, tess_evaluation_shader=None, varyings=()) -> Program:
        return self.__mglo.program(vertex_shader, fragment_shader, geometry_shader, tess_control_shader, tess_evaluation_shader, varyings)

    def texture(self, size, components=3, data=None, levels=-1, samples=0, aligment=1, dtype='f1') -> Texture:
        return self.__mglo.texture(size, components, data, levels, samples, aligment, dtype)

    def depth_texture(self, size, data=None, levels=-1, samples=0, aligment=1) -> Texture:
        return self.__mglo.depth_texture(size, data, levels, samples, aligment)

    def sampler(self, texture) -> Sampler:
        return self.__mglo.sampler(texture)

    def vertex_array(self, program, content, index_buffer=None) -> VertexArray:
        return self.__mglo.vertex_array(program, content, index_buffer)

    def simple_vertex_array(self, program, buffer, *attributes, index_buffer=None) -> VertexArray:
        content = [(buffer, ' '.join(_attr_fmt(program.attributes[a]) for a in attributes)) + attributes]
        return self.__mglo.vertex_array(program, content, index_buffer)

    def scope(self, enable_only=-1, framebuffer=None, samplers=None, uniform_buffers=None, storage_buffers=None) -> Scope:
        return self.__mglo.scope(enable_only, framebuffer, samplers, uniform_buffers, storage_buffers)

    def query(self, time=False, primitives=False, samples=False, any_samples=False) -> Query:
        return self.__mglo.query(time, primitives, samples, any_samples)

    def renderbuffer(self, size, components=4, samples=0, dtype='f1') -> Renderbuffer:
        return self.__mglo.renderbuffer(size, components, samples, dtype)

    def depth_renderbuffer(self, size, samples=0) -> Renderbuffer:
        return self.__mglo.depth_renderbuffer(size, samples)

    def framebuffer(self, color_attachments=(), depth_attachment=None) -> Framebuffer:
        return self.__mglo.framebuffer(color_attachments, depth_attachment)

    def configure(self, key, **params):
        return self.__mglo.configure(params)

    def clear(self, red=1.0, green=1.0, blue=1.0, alpha=1.0, depth=1.0, viewport=None):
        self.fbo.clear(0, (red, green, blue, alpha), viewport)
        self.fbo.clear(-1, depth, viewport)

    def copy_buffer(self, dst, src, size=-1, *, read_offset=0, write_offset=0) -> None:
        self.__mglo.copy_buffer(dst, src, size, read_offset, write_offset)

    def record(self):
        return self.__mglo.record()

    def replay(self, bytecode):
        return self.__mglo.replay(bytecode)


def create_context(standalone=False, debug=False, glhook=None, gc=None):
    return mgl.create_context(standalone, debug, glhook, gc)


def extensions(context):
    return mgl.extensions(context)


def hwinfo(context):
    return mgl.hwinfo(context)


def glprocs(context):
    return mgl.glprocs(context)


def release(obj):
    return mgl.release(obj)


def inspect(obj):
    return mgl.inspect(obj)
