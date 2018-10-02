from typing import Any


class Scope:
    __slots__ = ['__mglo', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.extra = None  # type: Any

    def __enter__(self):
        self.__mglo.begin()

    def __exit__(self, *args):
        self.__mglo.end()
