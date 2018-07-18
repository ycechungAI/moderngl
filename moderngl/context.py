import importlib

from . import mgl


class Context:
    __slots__ = ['__mglo']


def create_context(standalone=False, debug=False):
    if debug:
        moderngl_debug = importlib.import_module('moderngl.debug')
        return moderngl_debug.create_context(standalone)
    else:
        return mgl.create_context(standalone)
