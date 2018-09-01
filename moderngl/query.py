
class Query:
    __slots__ = ['__mglo', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.extra = None  # type: Any

    def __enter__(self):
        self.__mglo.begin()
        return self

    def __exit__(self, *args):
        self.__mglo.end()

    @property
    def samples(self) -> int:
        return self.__mglo.samples

    @property
    def primitives(self) -> int:
        return self.__mglo.primitives

    @property
    def elapsed(self) -> int:
        return self.__mglo.elapsed
