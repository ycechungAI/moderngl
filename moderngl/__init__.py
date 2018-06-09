import importlib
from typing import Any, Dict, Tuple, Union


class Error(Exception):
    pass


class Buffer:
    __slots__ = ['__mglo', 'size', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.size = None  # type: int
        self.extra = None  # type: Any

    def write(self, data, *, offset=0) -> None:
        self.__mglo.write(data, offset)

    def read(self, size=-1, *, offset=0, dtype=None) -> bytes:
        return self.__mglo.read(size, offset, dtype)

    def read_into(self, buffer, size=-1, *, offset=0, write_offset=0) -> None:
        memoryview(buffer)[:] = self.__mglo.map(size, offset, True, False, None)
        self.__mglo.unmap()

    def map(self, size=-1, *, offset=0, readable=False, writable=False, dtype=None) -> None:
        return self.__mglo.map(size, offset, readable, writable, dtype)

    def unmap(self) -> None:
        self.__mglo.unmap()

    def clear(self) -> None:
        self.__mglo.clear()

    def bind_to_uniform_block(self, binding=0, *, offset=0, size=-1) -> None:
        self.__mglo.bind_to_uniform_block(binding, offset, size)

    def bind_to_storage_buffer(self, binding=0, *, offset=0, size=-1) -> None:
        self.__mglo.bind_to_storage_buffer(binding, offset, size)


class Context:
    __slots__ = ['__mglo', '__framebuffers', 'limits', 'screen', 'fbo', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.__framebuffers = None  # type: Dict[int, Framebuffer]
        self.limits = None  # type: Limits
        self.screen = None  # type: Framebuffer
        self.fbo = None  # type: Framebuffer
        self.extra = None  # type: Any

    def buffer(self, data=None, *, reserve=0, readable=True, writable=True, local=False) -> Buffer:
        return self.__mglo.buffer(data, reserve, readable, writable, local)


def create_context(*, standalone=False, debug=False) -> Context:
    mgl = importlib.import_module('moderngl.mgl_dbg' if debug else 'moderngl.mgl')
    return mgl.create_context(standalone)
