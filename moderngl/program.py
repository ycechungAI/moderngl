from typing import Any, Generator, Tuple, Union

from _moderngl import InvalidObject

from .program_members import (
    Attribute,
    Subroutine,
    Uniform,
    UniformBlock,
    Varying,
)

__all__ = ['Program', 'detect_format']


class Program:
    def __init__(self):
        self.mglo = None
        self._members = {}
        self._subroutines = None
        self._geom = (None, None, None)
        self._glo = None
        self._is_transform = None
        self.ctx = None
        self.extra = None
        raise TypeError()

    def __repr__(self):
        if hasattr(self, '_glo'):
            return f"<{self.__class__.__name__}: {self._glo}>"
        else:
            return f"<{self.__class__.__name__}: INCOMPLETE>"

    def __eq__(self, other: Any) -> bool:
        return type(self) is type(other) and self.mglo is other.mglo

    def __hash__(self) -> int:
        return id(self)

    def __del__(self):
        if not hasattr(self, "ctx"):
            return

        if self.ctx.gc_mode == "auto":
            self.release()
        elif self.ctx.gc_mode == "context_gc":
            self.ctx.objects.append(self.mglo)

    def __getitem__(self, key: str) -> Union[Uniform, UniformBlock, Subroutine, Attribute, Varying]:
        return self._members[key]

    def __setitem__(self, key: str, value: Any) -> None:
        self._members[key].value = value

    def __iter__(self) -> Generator[str, None, None]:
        yield from self._members

    @property
    def is_transform(self) -> bool:
        return self._is_transform

    @property
    def geometry_input(self) -> int:
        return self._geom[0]

    @property
    def geometry_output(self) -> int:
        return self._geom[1]

    @property
    def geometry_vertices(self) -> int:
        return self._geom[2]

    @property
    def subroutines(self) -> Tuple[str, ...]:
        return self._subroutines

    @property
    def glo(self) -> int:
        return self._glo

    def get(self, key: str, default: Any) -> Union[Uniform, UniformBlock, Subroutine, Attribute, Varying]:
        return self._members.get(key, default)

    def release(self) -> None:
        if self.mglo is not None:
            self.mglo.release()
            self.mglo = InvalidObject()


def detect_format(
    program: Program,
    attributes: Any,
    mode: str = 'mgl',
) -> str:
    def fmt(attr: Any) -> Tuple[int, str]:
        # Translate shape format into attribute format
        mgl_fmt = {
            'd': 'f8',
            'I': 'u'
        }
        # moderngl attribute format uses f, i and u
        if mode == 'mgl':
            return attr.array_length * attr.dimension, mgl_fmt.get(attr.shape) or attr.shape
        # struct attribute format uses f, d, i and I
        elif mode == 'struct':
            return attr.array_length * attr.dimension, attr.shape
        else:
            raise ValueError("invalid format mode: {0}".format(mode))

    return ' '.join('%d%s' % fmt(program[a]) for a in attributes)
