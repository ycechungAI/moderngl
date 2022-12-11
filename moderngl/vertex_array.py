from typing import TYPE_CHECKING, Any, List, Optional, Tuple, Union

from _moderngl import InvalidObject

if TYPE_CHECKING:
    from .program import Program
    from .buffer import Buffer

__all__ = ['VertexArray',
           'POINTS', 'LINES', 'LINE_LOOP', 'LINE_STRIP', 'TRIANGLES', 'TRIANGLE_STRIP', 'TRIANGLE_FAN',
           'LINES_ADJACENCY', 'LINE_STRIP_ADJACENCY', 'TRIANGLES_ADJACENCY', 'TRIANGLE_STRIP_ADJACENCY', 'PATCHES']

POINTS = 0x0000
LINES = 0x0001
LINE_LOOP = 0x0002
LINE_STRIP = 0x0003
TRIANGLES = 0x0004
TRIANGLE_STRIP = 0x0005
TRIANGLE_FAN = 0x0006
LINES_ADJACENCY = 0x000A
LINE_STRIP_ADJACENCY = 0x000B
TRIANGLES_ADJACENCY = 0x000C
TRIANGLE_STRIP_ADJACENCY = 0x0000D
PATCHES = 0x000E


class VertexArray:
    def __init__(self):
        self.mglo = None
        self._program = None
        self._index_buffer = None
        self._content = None
        self._index_element_size = None
        self._glo = None
        self._mode = None
        self.ctx = None
        self.extra = None
        self.scope = None
        raise TypeError()

    def __repr__(self) -> str:
        if hasattr(self, 'mglo'):
            return '<VertexArray: %d>' % self.glo
        else:
            return '<VertexArray: INCOMPLETE>'

    def __eq__(self, other: Any) -> bool:
        return type(self) is type(other) and self.mglo is other.mglo

    def __hash__(self) -> int:
        return id(self)

    def __del__(self) -> None:
        if not hasattr(self, "ctx"):
            return

        if self.ctx.gc_mode == "auto":
            self.release()
        elif self.ctx.gc_mode == "context_gc":
            self.ctx.objects.append(self.mglo)

    @property
    def mode(self) -> int:
        return self._mode

    @mode.setter
    def mode(self, value: int) -> None:
        self._mode = value

    @property
    def program(self) -> 'Program':
        return self._program

    @property
    def index_buffer(self) -> 'Buffer':
        return self._index_buffer

    @property
    def index_element_size(self) -> int:
        return self._index_element_size

    @property
    def vertices(self) -> int:
        return self.mglo.vertices

    @vertices.setter
    def vertices(self, value: int) -> None:
        self.mglo.vertices = int(value)

    @property
    def instances(self) -> int:
        return self.mglo.instances

    @instances.setter
    def instances(self, value: int) -> None:
        self.mglo.instances = int(value)

    @property
    def subroutines(self) -> Tuple[int, ...]:
        return self.mglo.subroutines

    @subroutines.setter
    def subroutines(self, value: Tuple[int, ...]) -> None:
        self.mglo.subroutines = tuple(value)

    @property
    def glo(self) -> int:
        return self._glo

    def render(
        self,
        mode: Optional[int] = None,
        vertices: int = -1,
        *,
        first: int = 0,
        instances: int = -1,
    ) -> None:
        if mode is None:
            mode = self._mode

        if self.scope:
            with self.scope:
                self.mglo.render(mode, vertices, first, instances)
        else:
            self.mglo.render(mode, vertices, first, instances)

    def render_indirect(
        self,
        buffer: "Buffer",
        mode: Optional[int] = None,
        count: int = -1,
        *,
        first: int = 0,
    ) -> None:
        if mode is None:
            mode = self._mode

        if self.scope:
            with self.scope:
                self.mglo.render_indirect(buffer.mglo, mode, count, first)
        else:
            self.mglo.render_indirect(buffer.mglo, mode, count, first)

    def transform(
        self,
        buffer: Union["Buffer", List["Buffer"]],
        mode: Optional[int] = None,
        vertices: int = -1,
        *,
        first: int = 0,
        instances: int = -1,
        buffer_offset: int = 0,
    ) -> None:
        if mode is None:
            mode = self._mode

        if isinstance(buffer, (list, tuple)):
            outputs = [buf.mglo for buf in buffer]
        else:
            outputs = [buffer.mglo]

        if self.scope:
            with self.scope:
                self.mglo.transform(outputs, mode, vertices, first, instances, buffer_offset)
        else:
            self.mglo.transform(outputs, mode, vertices, first, instances, buffer_offset)

    def bind(
        self,
        attribute: int,
        cls: str,
        buffer: "Buffer",
        fmt: str,
        *,
        offset: int = 0,
        stride: int = 0,
        divisor: int = 0,
        normalize: bool = False,
    ) -> None:
        self.mglo.bind(attribute, cls, buffer.mglo, fmt, offset, stride, divisor, normalize)

    def release(self) -> None:
        if self.mglo is not None:
            self._program = None
            self._index_buffer = None
            self._content = None
            self.mglo.release()
            self.mglo = InvalidObject()
