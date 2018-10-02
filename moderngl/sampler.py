from typing import Tuple


class Sampler:
    __slots__ = ['__mglo', 'texture', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.extra = None  # type: Any

    def use(self, location=0) -> None:
        self.__mglo.use(location)
