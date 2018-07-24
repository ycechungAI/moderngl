import moderngl
import pytest

@pytest.fixture(scope='module')
def ctx():
    return moderngl.create_context(standalone=True, debug=True)
