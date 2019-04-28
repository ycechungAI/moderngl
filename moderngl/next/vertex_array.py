from typing import Any

from .buffer import Buffer
from .program import Program
from .scope import Scope


class VertexArray:
    __slots__ = ['mglo', 'program', 'scope', 'mode', 'vertices', 'extra']

    def __init__(self, mglo, program, scope, mode, vertices):
        self.mglo = mglo  # type: Any
        self.program = program  # type: Program
        self.scope = scope  # type: Scope
        self.mode = mode  # type: int
        self.vertices = vertices  # type: int
        self.extra = None  # type: Any

    def render(self, mode=None, vertices=-1, first=0, instances=1, color_mask=0xffffffffffffffff, depth_mask=1):
        self.mglo.render(mode, vertices, first, instances, color_mask, depth_mask)

    def render_indirect(self, buffer, mode=None, count=-1, first=0, color_mask=0xffffffffffffffff, depth_mask=1):
        self.mglo.render_indirect(mode, buffer, mode, count, first, color_mask, depth_mask)

    def transform(self, output, mode=None, vertices=-1, first=0, instances=1, flush=True):
        self.mglo.transform(output, mode, vertices, first, instances, flush)

    def transform_indirect(self, output, buffer, mode=None, count=-1, first=0, flush=True):
        self.mglo.transform_indirect(output, buffer, mode, count, first, flush)

    @property
    def index_buffer(self):
        return self.mglo.ibo

    @index_buffer.setter
    def index_buffer(self, value):
        self.mglo.ibo = value
