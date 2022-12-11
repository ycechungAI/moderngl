from typing import Any, Generator, Union

from _moderngl import InvalidObject

from .program_members import (
    Attribute,
    Subroutine,
    Uniform,
    UniformBlock,
    Varying,
)

__all__ = ['ComputeShader']


class ComputeShader:
    def __init__(self):
        self.mglo = None
        self._members = {}
        self._glo = None
        self.ctx = None
        self.extra = None
        raise TypeError()

    def __repr__(self) -> str:
        if hasattr(self, '_glo'):
            return f"<{self.__class__.__name__}: {self._glo}>"
        else:
            return f"<{self.__class__.__name__}: INCOMPLETE>"

    def __eq__(self, other: Any):
        return type(self) is type(other) and self.mglo is other.mglo

    def __hash__(self) -> int:
        return id(self)

    def __del__(self) -> None:
        if not hasattr(self, "ctx"):
            return

        if self.ctx.gc_mode == "auto":
            self.release()
        elif self.ctx.gc_mode == "context_gc":
            self.ctx.objects.append(self.mglo)

    def __getitem__(self, key: str) -> Union[Uniform, UniformBlock, Subroutine, Attribute, Varying]:
        return self._members[key]

    def __setitem__(self, key: str, value: Any):
        self._members[key].value = value

    def __iter__(self) -> Generator[str, None, None]:
        yield from self._members

    @property
    def glo(self) -> int:
        return self._glo

    def run(self, group_x: int = 1, group_y: int = 1, group_z: int = 1) -> None:
        return self.mglo.run(group_x, group_y, group_z)

    def get(self, key: str, default: Any) -> Union[Uniform, UniformBlock, Subroutine, Attribute, Varying]:
        return self._members.get(key, default)

    def release(self) -> None:
        if self.mglo is not None:
            self.mglo.release()
            self.mglo = InvalidObject()
