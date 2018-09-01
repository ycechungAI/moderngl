from typing import Any


class Scope:
    __slots__ = ['__mglo', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.extra = None  # type: Any
