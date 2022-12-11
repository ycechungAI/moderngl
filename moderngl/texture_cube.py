from typing import Any, Optional, Tuple, Union

from _moderngl import InvalidObject

from .buffer import Buffer

__all__ = ['TextureCube']


class TextureCube:
    def __init__(self):
        self.mglo = None
        self._size = (None, None)
        self._components = None
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
    def size(self) -> Tuple[int, int]:
        return self._size

    @property
    def components(self) -> int:
        return self._components

    @property
    def dtype(self) -> str:
        return self._dtype

    @property
    def filter(self) -> Tuple[int, int]:
        return self.mglo.filter

    @filter.setter
    def filter(self, value: Tuple[int, int]) -> None:
        self.mglo.filter = value

    @property
    def swizzle(self) -> str:
        return self.mglo.swizzle

    @swizzle.setter
    def swizzle(self, value: str) -> None:
        self.mglo.swizzle = value

    @property
    def anisotropy(self) -> float:
        return self.mglo.anisotropy

    @anisotropy.setter
    def anisotropy(self, value: float) -> None:
        self.mglo.anisotropy = value

    @property
    def glo(self) -> int:
        return self._glo

    def read(self, face: int, *, alignment: int = 1) -> bytes:
        return self.mglo.read(face, alignment)

    def read_into(
        self,
        buffer: Any,
        face: int,
        *,
        alignment: int = 1,
        write_offset: int = 0,
    ) -> None:
        if type(buffer) is Buffer:
            buffer = buffer.mglo

        return self.mglo.read_into(buffer, face, alignment, write_offset)

    def write(
        self,
        face: int,
        data: Any,
        viewport: Optional[Union[Tuple[int, int], Tuple[int, int, int, int]]] = None,
        *,
        alignment: int = 1,
    ) -> None:
        if type(data) is Buffer:
            data = data.mglo

        self.mglo.write(face, data, viewport, alignment)

    def use(self, location: int = 0) -> None:
        self.mglo.use(location)

    def bind_to_image(self, unit: int, read: bool = True, write: bool = True, level: int = 0, format: int = 0) -> None:
        self.mglo.bind(unit, read, write, level, format)

    def release(self) -> None:
        if self.mglo is not None:
            self.mglo.release()
            self.mglo = InvalidObject()
