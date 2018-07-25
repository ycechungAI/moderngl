class Uniform:
    __slots__ = ['__type', 'location', 'cols', 'rows', 'size', 'shape']

    def __init__(self):
        self.__type = None  # type: int
        self.location = None  # type: int
        self.cols = None  # type: int
        self.rows = None  # type: int
        self.size = None  # type: int
        self.shape = None  # type: int
