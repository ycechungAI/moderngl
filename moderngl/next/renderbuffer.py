from typing import Any


class Renderbuffer:
    __slots__ = ['mglo', 'size', 'extra']

    def __init__(self, mglo, size):
        self.mglo = mglo  # type: Any
        self.size = size  # type: Tuple[int, int]
        self.extra = None  # type: Any
