import importlib
from typing import Any

from . import mgl
from .buffer import Buffer
from .limits import Limits
from .program import Program
from .texture import Texture
from .vertex_array import VertexArray


class Context:
    __slots__ = ['__mglo', 'version_code', 'limits', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.version_code = None  # type: int
        self.limits = None  # type: Limits
        self.extra = None  # type: Any

    def buffer(self, data=None, reserve=0, readable=True, writable=True, local=False) -> Buffer:
        return self.__mglo.buffer(data, reserve, readable, writable, local)

    def program(
            self, vertex_shader, fragment_shader=None, geometry_shader=None, tess_control_shader=None,
            tess_evaluation_shader=None, varyings=()) -> Program:
        return self.__mglo.program(
            vertex_shader, fragment_shader, geometry_shader, tess_control_shader, tess_evaluation_shader, varyings)

    def texture(self, size, components=3, data=None, levels=-1, samples=0, aligment=1, dtype='f1') -> Texture:
        return self.__mglo.texture(size, components, data, levels, samples, aligment, dtype)

    def vertex_array(self, program, content, index_buffer=None) -> VertexArray:
        return self.__mglo.vertex_array(program, content, index_buffer)


def create_context(standalone=False, debug=False):
    if debug:
        moderngl_debug = importlib.import_module('moderngl.debug')
        return moderngl_debug.create_context(standalone)
    else:
        return mgl.create_context(standalone)


def extensions(context):
    return mgl.extensions(context)


def hwinfo(context):
    return mgl.hwinfo(context)


def glprocs(context):
    return mgl.glprocs(context)
