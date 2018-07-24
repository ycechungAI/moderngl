class Program:
    __slots__ = ['__mglo', 'attributes', 'uniforms', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.attributes = None  # type: Dict[str, Attribute]
        self.uniforms = None  # type: Dict[str, Uniform]
        self.extra = None  # type: Any

    def __setitem__(self, key, value):
        self.__mglo.uniform(self.uniforms.get(key), value)
