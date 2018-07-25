import moderngl as mgl
import pytest

@pytest.fixture(scope='module')
def ctx():
    return mgl.create_context(standalone=True, debug=True)
