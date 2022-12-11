from typing import Any, Dict, Optional, TYPE_CHECKING, Tuple, Union

from _moderngl import InvalidObject

from .buffer import Buffer
from .renderbuffer import Renderbuffer
from .texture import Texture

if TYPE_CHECKING:
    from .context import Context

__all__ = ['Framebuffer']


class Framebuffer:
    def __init__(self):
        self.mglo = None
        self._color_attachments = None
        self._depth_attachment = None
        self._size = (None, None)
        self._samples: int = None
        self._glo: int = None
        self.ctx: Context = None
        self._is_reference = None
        self.extra: Any = None
        raise TypeError()

    def __repr__(self) -> str:
        if hasattr(self, '_glo'):
            return '<Framebuffer: %d>' % self._glo
        else:
            return "<Framebuffer: INCOMPLETE>"

    def __eq__(self, other: Any) -> bool:
        return type(self) is type(other) and self.mglo is other.mglo

    def __hash__(self) -> int:
        return id(self)

    def __del__(self):
        if not hasattr(self, "ctx"):
            return

        # Don't delete default framebuffer or a reference
        if self._is_reference:
            return

        # If object was initialized properly (ctx present) and gc_mode is auto
        if self.ctx.gc_mode == "auto":
            self.release()
        elif self.ctx.gc_mode == "context_gc":
            self.ctx.objects.append(self.mglo)

    @property
    def viewport(self) -> Tuple[int, int, int, int]:
        return self.mglo.viewport

    @viewport.setter
    def viewport(self, value: Tuple[int, int, int, int]) -> None:
        x, y, w, h = value
        self.mglo.viewport = (int(x), int(y), int(w), int(h))

    @property
    def scissor(self) -> Tuple[int, int, int, int]:
        return self.mglo.scissor

    @scissor.setter
    def scissor(self, value: Tuple[int, int, int, int]) -> None:
        if value is None:
            self.mglo.scissor = None
        else:
            x, y, w, h = value
            self.mglo.scissor = (int(x), int(y), int(w), int(h))

    @property
    def color_mask(self) -> Tuple[bool, bool, bool, bool]:
        return self.mglo.color_mask

    @color_mask.setter
    def color_mask(self, value: Tuple[bool, bool, bool, bool]) -> None:
        self.mglo.color_mask = value

    @property
    def depth_mask(self) -> bool:
        return self.mglo.depth_mask

    @depth_mask.setter
    def depth_mask(self, value: bool) -> None:
        self.mglo.depth_mask = value

    @property
    def width(self) -> int:
        return self._size[0]

    @property
    def height(self) -> int:
        return self._size[1]

    @property
    def size(self) -> tuple:
        return self._size

    @property
    def samples(self) -> int:
        return self._samples

    @property
    def bits(self) -> Dict[str, str]:
        return self.mglo.bits

    @property
    def color_attachments(self) -> Tuple[Union[Texture, Renderbuffer], ...]:
        return self._color_attachments

    @property
    def depth_attachment(self) -> Union[Texture, Renderbuffer]:
        return self._depth_attachment

    @property
    def glo(self) -> int:
        return self._glo

    def clear(
        self,
        red: float = 0.0,
        green: float = 0.0,
        blue: float = 0.0,
        alpha: float = 0.0,
        depth: float = 1.0,
        *,
        viewport: Optional[Union[Tuple[int, int], Tuple[int, int, int, int]]] = None,
        color: Optional[Tuple[float, float, float, float]] = None,
    ) -> None:
        if color is not None:
            red, green, blue, alpha, *_ = tuple(color) + (0.0, 0.0, 0.0, 0.0)

        if viewport is not None:
            viewport = tuple(viewport)  # type: ignore

        self.mglo.clear(red, green, blue, alpha, depth, viewport)

    def use(self) -> None:
        self.ctx.fbo = self
        self.mglo.use()

    def read(
        self,
        viewport: Optional[Union[Tuple[int, int], Tuple[int, int, int, int]]] = None,
        components: int = 3,
        *,
        attachment: int = 0,
        alignment: int = 1,
        dtype: str = 'f1',
        clamp: bool = False,
    ) -> bytes:
        return self.mglo.read(viewport, components, attachment, alignment, clamp, dtype)

    def read_into(
        self,
        buffer: Any,
        viewport: Optional[Union[Tuple[int, int], Tuple[int, int, int, int]]] = None,
        components: int = 3,
        *,
        attachment: int = 0,
        alignment: int = 1,
        dtype: str = 'f1',
        write_offset: int = 0,
    ) -> None:
        if type(buffer) is Buffer:
            buffer = buffer.mglo

        return self.mglo.read_into(buffer, viewport, components, attachment, alignment, dtype, write_offset)

    def release(self) -> None:
        if self.mglo is not None:
            self._color_attachments = None
            self._depth_attachment = None
            self.mglo.release()
            self.mglo = InvalidObject()
