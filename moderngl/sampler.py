from typing import Tuple

from _moderngl import InvalidObject

__all__ = ['Sampler']


class Sampler:
    def __init__(self):
        self.mglo = None
        self._glo = None
        self.ctx = None
        self.extra = None
        self.texture = None
        raise TypeError()

    def __hash__(self) -> int:
        return id(self)

    def __repr__(self):
        if hasattr(self, '_glo'):
            return f"<{self.__class__.__name__}: {self._glo}>"
        else:
            return f"<{self.__class__.__name__}: INCOMPLETE>"

    def __del__(self):
        if not hasattr(self, "ctx"):
            return

        if self.ctx.gc_mode == "auto":
            self.release()
        elif self.ctx.gc_mode == "context_gc":
            self.ctx.objects.append(self.mglo)

    def use(self, location: int = 0) -> None:
        if self.texture is not None:
            self.texture.use(location)
        self.mglo.use(location)

    def clear(self, location: int = 0) -> None:
        self.mglo.clear(location)

    def release(self) -> None:
        if self.mglo is not None:
            self.mglo.release()
            self.mglo = InvalidObject()

    @property
    def repeat_x(self) -> bool:
        return self.mglo.repeat_x

    @repeat_x.setter
    def repeat_x(self, value: bool) -> None:
        self.mglo.repeat_x = value

    @property
    def repeat_y(self) -> bool:
        return self.mglo.repeat_y

    @repeat_y.setter
    def repeat_y(self, value: bool) -> None:
        self.mglo.repeat_y = value

    @property
    def repeat_z(self) -> bool:
        return self.mglo.repeat_z

    @repeat_z.setter
    def repeat_z(self, value: bool) -> None:
        self.mglo.repeat_z = value

    @property
    def filter(self) -> Tuple[int, int]:
        return self.mglo.filter

    @filter.setter
    def filter(self, value: Tuple[int, int]) -> None:
        self.mglo.filter = value

    @property
    def compare_func(self) -> str:
        return self.mglo.compare_func

    @compare_func.setter
    def compare_func(self, value: str) -> None:
        self.mglo.compare_func = value

    @property
    def anisotropy(self) -> float:
        return self.mglo.anisotropy

    @anisotropy.setter
    def anisotropy(self, value: float) -> None:
        self.mglo.anisotropy = value

    @property
    def border_color(self) -> Tuple[float, float, float, float]:
        return self.mglo.border_color

    @border_color.setter
    def border_color(self, value: Tuple[float, float, float, float]) -> None:
        self.mglo.border_color = value

    @property
    def min_lod(self) -> float:
        return self.mglo.min_lod

    @min_lod.setter
    def min_lod(self, value: float) -> None:
        self.mglo.min_lod = value

    @property
    def max_lod(self) -> float:
        return self.mglo.max_lod

    @max_lod.setter
    def max_lod(self, value: float) -> None:
        self.mglo.max_lod = value

    def assign(self, index: int) -> Tuple["Sampler", int]:
        return (self, index)
