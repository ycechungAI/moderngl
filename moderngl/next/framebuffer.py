from typing import Any, Tuple


class Framebuffer:
    __slots__ = ['mglo', 'size', 'extra']

    def __init__(self, mglo, size):
        self.mglo = mglo  # type: Any
        self.size = size  # type: Tuple[int, int]
        self.extra = None  # type: Any

    @property
    def viewport(self) -> Tuple[int, int, int, int]:
        return self.mglo.viewport

    @viewport.setter
    def viewport(self, value):
        self.mglo.viewport = value

    def clear(self, attachment, color, viewport=None, color_mask=0xF):
        self.mglo.clear(attachment, color, viewport, color_mask)

    # def read(self, viewport=None, components=3, attachment=0, alignment=1, dtype='f1', np=False):
    #     return self.mglo.read(viewport, components, attachment, alignment, dtype, np)

    # def read_into(self, buffer, viewport=None, components=3, attachment=0, alignment=1):
    #     return self.mglo.read_into(buffer, viewport, components, attachment, alignment)

    def use(self):
        self.mglo.use()
