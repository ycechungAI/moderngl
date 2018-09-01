
class Renderbuffer:
    __slots__ = ['__mglo', 'size', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.size = None  # type: Tuple[int, int]
        self.extra = None  # type: Any
