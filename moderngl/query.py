
class Query:
    __slots__ = ['__mglo', 'elapsed', 'primitives', 'samples', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.extra = None  # type: Any

    def __enter__(self):
        self.__mglo.begin()
        return self

    def __exit__(self, *args):
        self.__mglo.end()
