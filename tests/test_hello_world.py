import pytest
import numpy as np


@pytest.mark.usefixtures('ctx')
def test_hello(ctx):
    np.testing.assert_almost_equal([1.0, 1.0 + 1e-8], [1.0, 1.0])
