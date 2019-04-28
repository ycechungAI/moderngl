from typing import Any, Tuple


class Texture:
    __slots__ = ['mglo', '_level', '_layer', 'size', 'extra']

    def __init__(self, mglo, level, layer, size):
        self.mglo = mglo  # type: Any
        self._level = level  # type: int
        self._layer = layer  # type: int
        self.size = size  # type: Union[Tuple[int, int], Tuple[int, int, int]]
        self.extra = None  # type: Any

    @property
    def swizzle(self) -> str:
        return self.mglo._swizzle

    @swizzle.setter
    def swizzle(self, value):
        self.mglo.swizzle = value

    def level(self, level) -> 'Texture':
        return self.mglo.level(level)

    def layer(self, layer) -> 'Texture':
        return self.mglo.layer(layer)

    def write(self, data, viewport=None, alignment=1):
        self.mglo.write(data, viewport, alignment, self._level)

    def read(self, alignment=1, np=False) -> bytes:
        return self.mglo.read(self._level, self._layer, alignment, np)

    def build_mipmaps(self, base=0, max=-1):
        self.mglo.build_mipmaps(base, max)

    def bind_to_image(self, binding, access, format):
        self.mglo.bind(binding, access, format)
