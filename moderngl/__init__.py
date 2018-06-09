import importlib
from typing import Any, Dict, Tuple, Union


class Error(Exception):
    pass


class Context:
    __slots__ = ['__mglo', '__framebuffers', 'limits', 'screen', 'fbo', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.__framebuffers = None  # type: Dict[int, Framebuffer]
        self.limits = None  # type: Limits
        self.screen = None  # type: Framebuffer
        self.fbo = None  # type: Framebuffer
        self.extra = None  # type: Any


def create_context(*, standalone=False, debug=False) -> Context:
    mgl = importlib.import_module('moderngl.mgl_dbg' if debug else 'moderngl.mgl')
    return mgl.create_context(standalone)
