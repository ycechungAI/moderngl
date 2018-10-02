from typing import Any, Tuple


class Texture:
    __slots__ = ['__mglo', '__level', '__layer', 'size', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.__level = None  # type: int
        self.__layer = None  # type: int
        self.size = None  # type: Union[Tuple[int, int], Tuple[int, int, int]]
        self.extra = None  # type: Any

    @property
    def swizzle(self):
        raise NotImplementedError()

    @swizzle.setter
    def swizzle(self, value):
        self.__mglo.swizzle = value

    @property
    def filter(self):
        raise NotImplementedError()

    @filter.setter
    def filter(self, value):
        self.__mglo.filter = value

    def level(self, level) -> 'Texture':
        return self.__mglo.level(level)

    def layer(self, layer) -> 'Texture':
        return self.__mglo.layer(layer)

    def write(self, data, viewport=None, alignment=1) -> None:
        self.__mglo.write(data, viewport, alignment, self.__level)

    def read(self, alignment=1, np=False) -> None:
        return self.__mglo.read(self.__level, self.__layer, alignment, np)

    def build_mipmaps(self, base=0, max=-1) -> None:
        self.__mglo.build_mipmaps(base, max)
