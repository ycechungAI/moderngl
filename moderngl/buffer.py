from typing import Any, Tuple

from _moderngl import InvalidObject

__all__ = ['Buffer']


class Buffer:
    def __init__(self):
        self.mglo = None
        self._size = None
        self._dynamic = None
        self._glo = None
        self.ctx = None
        self.extra = None
        raise TypeError()

    def __repr__(self) -> str:
        if hasattr(self, '_glo'):
            return f"<{self.__class__.__name__}: {self._glo}>"
        else:
            return f"<{self.__class__.__name__}: INCOMPLETE>"

    def __eq__(self, other: Any):
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
    def size(self) -> int:
        return self.mglo.size()

    @property
    def dynamic(self) -> bool:
        return self._dynamic

    @property
    def glo(self) -> int:
        return self._glo

    def write(self, data: Any, *, offset: int = 0) -> None:
        self.mglo.write(data, offset)

    def write_chunks(self, data: Any, start: int, step: int, count: int) -> None:
        self.mglo.write_chunks(data, start, step, count)

    def read(self, size: int = -1, *, offset: int = 0) -> bytes:
        return self.mglo.read(size, offset)

    def read_into(self, buffer: Any, size: int = -1, *, offset: int = 0, write_offset: int = 0) -> None:
        return self.mglo.read_into(buffer, size, offset, write_offset)

    def read_chunks(self, chunk_size: int, start: int, step: int, count: int) -> bytes:
        return self.mglo.read_chunks(chunk_size, start, step, count)

    def read_chunks_into(
        self,
        buffer: Any,
        chunk_size: int,
        start: int,
        step: int,
        count: int,
        *,
        write_offset: int = 0
    ) -> None:
        return self.mglo.read(buffer, chunk_size, start, step, count, write_offset)

    def clear(self, size: int = -1, *, offset: int = 0, chunk: Any = None) -> None:
        self.mglo.clear(size, offset, chunk)

    def bind_to_uniform_block(self, binding: int = 0, *, offset: int = 0, size: int = -1) -> None:
        self.mglo.bind_to_uniform_block(binding, offset, size)

    def bind_to_storage_buffer(self, binding: int = 0, *, offset: int = 0, size: int = -1) -> None:
        self.mglo.bind_to_storage_buffer(binding, offset, size)

    def orphan(self, size: int = -1) -> None:
        self.mglo.orphan(size)

    def release(self) -> None:
        if self.mglo is not None:
            self.mglo.release()
            self.mglo = InvalidObject()

    def bind(self, *attribs, layout=None):
        return (self, layout, *attribs)

    def assign(self, index: int) -> Tuple["Buffer", int]:
        return (self, index)
