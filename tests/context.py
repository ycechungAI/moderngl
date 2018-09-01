import inspect

import moderngl as mgl
import pytest


errors = {
    0x0000: 'GL_NO_ERROR',
    0x0500: 'GL_INVALID_ENUM',
    0x0501: 'GL_INVALID_VALUE',
    0x0502: 'GL_INVALID_OPERATION',
}


@mgl.tracer
def tracer(name, error):
    print(name, errors.get(error, hex(error)))
    # if error:
    #     info = inspect.stack()[-2]
    #     print(info.function, info.frame.f_locals, name, error)


@pytest.fixture(scope='module')
def ctx():
    return mgl.create_context(standalone=True, debug=True)
