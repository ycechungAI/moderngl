from typing import Any

from _moderngl import InvalidObject

__all__ = ['Renderbuffer']


class Renderbuffer:
    def __init__(self):
        self.mglo = None
        self._size = (None, None)
        self._components = None
        self._samples = None
        self._depth = None
        self._dtype = None
        self._glo = None
        self.ctx = None
        self.extra = None
        raise TypeError()

    def __repr__(self):
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
    def width(self) -> int:
        return self._size[0]

    @property
    def height(self) -> int:
        return self._size[1]

    @property
    def size(self) -> tuple:
        return self._size

    @property
    def samples(self) -> int:
        return self._samples

    @property
    def components(self) -> int:
        return self._components

    @property
    def depth(self) -> bool:
        return self._depth

    @property
    def dtype(self) -> str:
        return self._dtype

    @property
    def glo(self) -> int:
        return self._glo

    def release(self) -> None:
        if self.mglo is not None:
            self.mglo.release()
            self.mglo = InvalidObject()
