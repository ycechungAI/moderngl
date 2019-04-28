from typing import Any, Tuple, Union

from .texture import Texture


class Sampler:
    __slots__ = ['mglo', 'extra']

    def __init__(self, mglo):
        self.mglo = mglo  # type: Any
        self.extra = None  # type: Any

    @property
    def filter(self) -> Tuple[int, int]:
        return self.mglo.filter

    @filter.setter
    def filter(self, value):
        self.mglo.filter = value

    @property
    def wrap(self) -> int:
        return self.mglo.wrap

    @wrap.setter
    def wrap(self, value):
        self.mglo.wrap = value

    @property
    def anisotropy(self) -> float:
        return self.mglo.anisotropy

    @anisotropy.setter
    def anisotropy(self, value):
        self.mglo.anisotropy = value

    @property
    def compare_func(self) -> str:
        return self.mglo.compare_func

    @compare_func.setter
    def compare_func(self, value):
        self.mglo.compare_func = value

    @property
    def lod_range(self) -> Tuple[int, int]:
        return self.mglo.lod_range

    @lod_range.setter
    def lod_range(self, value):
        self.mglo.lod_range = value

    @property
    def lod_bias(self) -> float:
        return self.mglo.lod_bias

    @lod_bias.setter
    def lod_bias(self, value):
        self.mglo.lod_bias = value

    @property
    def border(self) -> Tuple[float, float, float, float]:
        return self.mglo.border

    @border.setter
    def border(self, value):
        self.mglo.border = value

    @property
    def texture(self) -> Texture:
        return self.mglo.texture

    @texture.setter
    def texture(self, value):
        self.mglo.texture = value

    def use(self, location=0) -> None:
        self.mglo.use(location)
