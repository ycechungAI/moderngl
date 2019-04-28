from typing import Any


class Query:
    __slots__ = ['mglo', 'elapsed', 'primitives', 'samples', 'extra']

    def __init__(self, mglo):
        self.mglo = mglo  # type: Any
        self.elapsed = None  # type: int
        self.primitives = None  # type: int
        self.samples = None  # type: int
        self.extra = None  # type: Any

    def __enter__(self):
        self.mglo.begin()
        return self

    def __exit__(self, *args):
        self.mglo.end()
