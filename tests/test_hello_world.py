import moderngl as mgl
import numpy as np
import pytest


@pytest.mark.usefixtures('ctx')
def test_hello(ctx: mgl.Context):
    prog = ctx.program(
        vertex_shader='''
            #version 130

            in vec2 in_vert;
            out vec2 v_vert;

            void main() {
                v_vert = in_vert;
                gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
            }
        ''',
        varyings=['v_vert'],
    )

    buf1 = ctx.buffer(np.array([1.0, 2.0, 3.0, 4.0], dtype='f4'))
    buf2 = ctx.buffer(reserve=buf1.size)
    buf2.clear()

    vao = ctx.vertex_array(prog, [(buf1, '2f', 'in_vert')])
    vao.transform(buf2)

    np.testing.assert_almost_equal(buf2.read(dtype='f4'), [1.0, 2.0, 3.0, 4.0])


if __name__ == '__main__':
    pytest.main()
