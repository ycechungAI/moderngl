import moderngl
import pytest

def test_error_when_label_is_too_long(ctx):
    buf = ctx.buffer(reserve=1024)
    max_label_length = ctx.max_label_length

    with pytest.raises(moderngl.Error):
        buf.label = "F" * (max_label_length + 1)


def test_error_with_wrong_type(ctx):
    buf = ctx.buffer(reserve=1024)

    with pytest.raises(TypeError):
        buf.label = 123


def test_no_label_on_creation(ctx):
    buf = ctx.buffer(reserve=1024)

    assert buf.label is None


def test_clearing_label(ctx):
    buf = ctx.buffer(reserve=1024)
    buf.label = "test buffer"
    assert buf.label is not None

    buf.label = None

    assert buf.label is None
