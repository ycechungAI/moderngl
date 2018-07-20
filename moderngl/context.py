import importlib
from typing import Any

from . import mgl


class Context:
    __slots__ = ['__mglo', 'version_code', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.version_code = None  # type: int
        self.extra = None  # type: Any


def create_context(standalone=False, debug=False):
    if debug:
        moderngl_debug = importlib.import_module('moderngl.debug')
        return moderngl_debug.create_context(standalone)
    else:
        return mgl.create_context(standalone)
