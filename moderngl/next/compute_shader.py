from typing import Any

from .attribute import Attribute
from .uniform import Uniform


class ComputeShader:
    __slots__ = ['mglo', 'uniforms', 'extra']

    def __init__(self, mglo, uniforms):
        self.mglo = mglo  # type: Any
        self.uniforms = uniforms  # type: Dict[str, Uniform]
        self.extra = None  # type: Any

    def __setitem__(self, key, value):
        self.mglo.uniform(self.uniforms.get(key), value)

    def run(self, group_x=1, group_y=1, group_z=1):
        self.mglo.run(group_x, group_y, group_z)
