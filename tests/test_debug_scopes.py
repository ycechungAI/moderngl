from contextlib import ExitStack

import pytest
from OpenGL import GL

import moderngl

def test_debug_scope(ctx):
    with ctx.debug_scope("test scope"):
        pass


def test_debug_scope_error_when_label_is_too_long(ctx):
    max_label_length = ctx.max_debug_message_length
    message = "F" * (max_label_length + 1)

    with pytest.raises(moderngl.Error):
        with ctx.debug_scope(message):
            pass


def test_debug_scope_error_when_stack_overflow(ctx_new):
    max_stack_depth = ctx_new.max_debug_group_stack_depth
    with ExitStack() as stack:
        with pytest.raises(moderngl.Error):
            for _ in range(max_stack_depth + 1):
                stack.enter_context(ctx_new.debug_scope("test scope"))


def test_debug_scope_error_when_stack_underflow(ctx_new):
    if GL.glPopDebugGroup:
        with pytest.raises(moderngl.Error):
            with ctx_new.debug_scope("test scope"):
                GL.glPopDebugGroup()


def test_debug_scope_error_when_label_wrong_type(ctx):
    with pytest.raises(TypeError):
        with ctx.debug_scope(46):
            pass