from typing import Any, Tuple

from _moderngl import InvalidObject

__all__ = ['Scope']


class Scope:
    def __init__(self):
        self.mglo = None
        self.ctx = None
        # Keep references to keep this objects alive
        self._framebuffer = None
        self._textures = None
        self._uniform_buffers = None
        self._storage_buffers = None
        self._samplers = None
        self.extra = None
        raise TypeError()

    def __repr__(self):
        return '<Scope>'

    def __hash__(self) -> int:
        return id(self)

    def __enter__(self):
        self.mglo.begin()
        return self

    def __exit__(self, *args: Tuple[Any]):
        self.mglo.end()

    def __del__(self):
        if not hasattr(self, "ctx"):
            return

        if self.ctx.gc_mode == "auto":
            self.release()
        elif self.ctx.gc_mode == "context_gc":
            self.ctx.objects.append(self.mglo)

    def release(self) -> None:
        if self.mglo is not None:
            self._framebuffer = None
            self._textures = None
            self._uniform_buffers = None
            self._storage_buffers = None
            self._samplers = None
            self.mglo.release()
            self.mglo = InvalidObject()
