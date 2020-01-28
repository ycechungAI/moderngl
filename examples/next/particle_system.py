import moderngl.next as mgl
import numpy as np

from example_window import Example, run_example


def particle():
    a = np.random.uniform(0.0, np.pi * 2.0)
    r = np.random.uniform(0.0, 0.001)
    return np.array([0.0, 0.0, np.cos(a) * r - 0.003, np.sin(a) * r - 0.008]).astype('f4').tobytes()


class Particles(Example):
    def __init__(self):
        self.ctx = mgl.create_context()

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                in vec2 in_vert;

                void main() {
                    gl_Position = vec4(in_vert, 0.0, 1.0);
                }
            ''',
            fragment_shader='''
                #version 330

                out vec4 f_color;

                void main() {
                    f_color = vec4(0.30, 0.50, 1.00, 1.0);
                }
            ''',
        )

        self.transform = self.ctx.program(
            vertex_shader='''
            #version 330

            uniform vec2 Acc;

            in vec2 in_pos;
            in vec2 in_prev;

            out vec2 out_pos;
            out vec2 out_prev;

            void main() {
                out_pos = in_pos * 2.0 - in_prev + Acc;
                out_prev = in_pos;
            }
        ''',
            varyings=['out_pos', 'out_prev']
        )

        self.transform['Acc'] = (0.0, -0.0001)

        self.vbo1 = self.ctx.buffer(b''.join(particle() for i in range(1024)))
        self.vbo2 = self.ctx.buffer(reserve=self.vbo1.size)

        self.vao1 = self.ctx.simple_vertex_array(self.transform, self.vbo1, 'in_pos', 'in_prev')
        self.vao2 = self.ctx.simple_vertex_array(self.transform, self.vbo2, 'in_pos', 'in_prev')

        self.render_vao = self.ctx.vertex_array(self.prog, [
            (self.vbo1, '2f 2x4', 'in_vert'),
        ])

        self.idx = 0

    def render(self):
        self.ctx.screen.viewport = self.wnd.viewport
        self.ctx.clear(1.0, 1.0, 1.0)
        # self.ctx.point_size = 2.0

        for i in range(8):
            self.vbo1.write(particle(), offset=self.idx * 16)
            self.idx = (self.idx + 1) % 1024

        self.render_vao.render(mgl.POINTS, 1024)
        self.vao1.transform(self.vbo2, mgl.POINTS, 1024)
        self.ctx.copy_buffer(self.vbo1, self.vbo2)


run_example(Particles)
