from typing import Any, Tuple

__all__ = ['Query']


class Query:
    def __init__(self):
        self.mglo = None
        self.crender = None
        self.ctx = None
        self.extra = None
        raise TypeError()

    def __repr__(self) -> str:
        return '<Query>'

    def __hash__(self) -> int:
        return id(self)

    def __enter__(self):
        self.mglo.begin()
        return self

    def __exit__(self, *args: Tuple[Any]):
        self.mglo.end()

    @property
    def samples(self) -> int:
        return self.mglo.samples

    @property
    def primitives(self) -> int:
        return self.mglo.primitives

    @property
    def elapsed(self) -> int:
        return self.mglo.elapsed
