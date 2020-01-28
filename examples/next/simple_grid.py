import moderngl.next as mgl
import numpy as np
from pyrr import Matrix44

from example_window import Example, run_example


def grid(size, steps):
    u = np.repeat(np.linspace(-size, size, steps), 2)
    v = np.tile([-size, size], steps)
    w = np.zeros(steps * 2)
    return np.concatenate([np.dstack([u, v, w]), np.dstack([v, u, w])])


class SimpleGrid(Example):
    def __init__(self):
        self.ctx = mgl.create_context()

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                uniform mat4 Mvp;

                in vec3 in_vert;

                void main() {
                    gl_Position = Mvp * vec4(in_vert, 1.0);
                }
            ''',
            fragment_shader='''
                #version 330

                out vec4 f_color;

                void main() {
                    f_color = vec4(0.1, 0.1, 0.1, 1.0);
                }
            ''',
        )

        self.vbo = self.ctx.buffer(grid(15, 10).astype('f4').tobytes())
        self.vao = self.ctx.simple_vertex_array(self.prog, self.vbo, 'in_vert')

    def render(self):
        self.ctx.screen.viewport = self.wnd.viewport
        self.ctx.clear(1.0, 1.0, 1.0)
        with self.ctx.scope(mgl.DEPTH_TEST):
            proj = Matrix44.perspective_projection(45.0, self.wnd.ratio, 0.1, 1000.0)
            lookat = Matrix44.look_at(
                (40.0, 30.0, 30.0),
                (0.0, 0.0, 0.0),
                (0.0, 0.0, 1.0),
            )

            self.prog['Mvp'] = (proj * lookat).astype('f4').tobytes()
            self.vao.render(mgl.LINES)


run_example(SimpleGrid)
