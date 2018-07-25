import pytest
import numpy as np


@pytest.mark.usefixtures('ctx')
def test_hello(ctx):
    print(ctx.program(
        vertex_shader='''
            #version 130

            in vec2 in_vert;
            out vec2 v_vert;

            void main() {
                v_vert = in_vert;
                // gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
            }
        ''',
        varyings=['v_vert'],
    ))
    # np.testing.assert_almost_equal([1.0, 1.0 + 1e-8], [1.0, 1.0])
