from typing import Any, Dict

from .attribute import Attribute
from .uniform import Uniform


class Program:
    __slots__ = ['mglo', 'attributes', 'uniforms', 'extra']

    def __init__(self, mglo, attributes, uniforms):
        self.mglo = mglo  # type: Any
        self.attributes = attributes  # type: Dict[str, Attribute]
        self.uniforms = uniforms  # type: Dict[str, Uniform]
        self.extra = None  # type: Any

    def __setitem__(self, key, value):
        self.mglo.uniform(self.uniforms.get(key), value)
