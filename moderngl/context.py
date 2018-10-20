import importlib
from typing import Any

from . import mgl
from .buffer import Buffer
from .limits import Limits
from .program import Program
from .texture import Texture
from .sampler import Sampler
from .vertex_array import VertexArray


class Context:
    __slots__ = ['__mglo', 'version_code', 'limits', 'screen', 'fbo', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.version_code = None  # type: int
        self.limits = None  # type: Limits
        self.screen = None  # type: Framebuffer
        self.fbo = None  # type: Framebuffer
        self.extra = None  # type: Any

    def buffer(self, data=None, reserve=0, readable=True, writable=True, local=False) -> Buffer:
        return self.__mglo.buffer(data, reserve, readable, writable, local)

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

    def scope(self, framebuffer=None, enable_only=-1, samplers=None, uniform_buffers=None, storage_buffers=None):
        return self.__mglo.scope(framebuffer, enable_only, samplers, uniform_buffers, storage_buffers)

    def query(self, time=False, primitives=False, samples=False, any_samples=False):
        return self.__mglo.query(time, primitives, samples, any_samples)

    def renderbuffer(self, size, components=4, samples=0, dtype='f1'):
        return self.__mglo.renderbuffer(size, components, samples, dtype)

    def depth_renderbuffer(self, size, samples=0):
        return self.__mglo.depth_renderbuffer(size, samples)

    def framebuffer(self, color_attachments=(), depth_attachment=None):
        return self.__mglo.framebuffer(color_attachments, depth_attachment)

    def configure(self, key, **params):
        return self.__mglo.configure(params)


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
