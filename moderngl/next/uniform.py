class Uniform:
    __slots__ = ['_type', 'location', 'cols', 'rows', 'size', 'shape']

    def __init__(self, _type, location, cols, rows, size, shape):
        self._type = _type  # type: int
        self.location = location  # type: int
        self.cols = cols  # type: int
        self.rows = rows  # type: int
        self.size = size  # type: int
        self.shape = shape  # type: int
