from typing import Any


class Scope:
    __slots__ = ['mglo', 'extra']

    def __init__(self, mglo):
        self.mglo = mglo  # type: Any
        self.extra = None  # type: Any

    def __enter__(self):
        self.mglo.begin()

    def __exit__(self, *args):
        self.mglo.end()
