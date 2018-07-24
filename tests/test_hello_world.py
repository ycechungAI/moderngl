import pytest
import numpy as np

@pytest.mark.usefixtures('ctx')
def test_adding(ctx):
    assert ctx.version_code != 0
