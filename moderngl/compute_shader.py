from typing import Any


class ComputeShader:
    __slots__ = ['__mglo', 'attributes', 'uniforms', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.attributes = None  # type: Dict[str, Attribute]
        self.uniforms = None  # type: Dict[str, Uniform]
        self.extra = None  # type: Any

    def __setitem__(self, key, value):
        self.__mglo.uniform(self.uniforms.get(key), value)

    def run(self, group_x=1, group_y=1, group_z=1) -> None:
        self.__mglo.run(group_x, group_y, group_z)
