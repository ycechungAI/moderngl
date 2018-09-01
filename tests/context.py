import inspect

import moderngl as mgl
import pytest


@mgl.tracer
def tracer(name, error):
    if error:
        info = inspect.stack()[-2]
        print(info.function, info.frame.f_locals, name, error)


@pytest.fixture(scope='module')
def ctx():
    return mgl.create_context(standalone=True, debug=True)
