from typing import Any


class Buffer:
    __slots__ = ['mglo', 'size', 'extra', 'old']

    def __init__(self, mglo, size):
        self.mglo = mglo  # type: Any
        self.size = size  # type: int
        self.extra = None  # type: Any

    def write(self, data, offset=0):
        self.mglo.write(data, offset)

    def read(self, size=-1, offset=0, dtype=None) -> bytes:
        return self.mglo.read(size, offset, dtype)

    def read_into(self, buffer, size=-1, offset=0, write_offset=0):
        write_end = write_offset + (self.size if size < 0 else size)
        memoryview(buffer)[write_offset:write_end] = self.mglo.map(size, offset, True, False, None)
        self.mglo.unmap()

    def map(self, size=-1, offset=0, readable=False, writable=False, dtype=None) -> memoryview:
        return self.mglo.map(size, offset, readable, writable, dtype)

    def unmap(self):
        self.mglo.unmap()

    def clear(self):
        self.mglo.clear()

    def bind_to_uniform_block(self, binding=0, offset=0, size=-1):
        self.mglo.bind(binding, offset, size, 0)

    def bind_to_storage_buffer(self, binding=0, offset=0, size=-1):
        self.mglo.bind(binding, offset, size, 1)
