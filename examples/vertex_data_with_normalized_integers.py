import moderngl

import _example
from _print_memory import print_memory


class Example(_example.Example):
    title = 'Color Triangle'

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                in vec2 in_vert;
                in vec3 in_color;
                out vec3 v_color;

                void main() {
                    gl_Position = vec4(in_vert, 0.0, 1.0);
                    v_color = in_color;
                }
            ''',
            fragment_shader='''
                #version 330

                in vec3 v_color;
                out vec4 f_color;

                void main() {
                    f_color = vec4(v_color, 1.0);
                }
            ''',
        )

        vertex_data = moderngl.pack([
            0, 800000000, 1.0, 0.0, 0.0,
            -600000000, -800000000, 0.0, 1.0, 0.0,
            600000000, -800000000, 0.0, 0.0, 1.0,
        ], layout='2i 3f')

        print('%4s%9s%11s%10s%9s' % ('x', 'y', 'red', 'green', 'blue'))
        print_memory(vertex_data, 3, [4, 8, 8, 12, 16, 20, 20])

        self.vbo = self.ctx.buffer(vertex_data)
        self.vao = self.ctx.vertex_array(self.prog, [
            self.vbo.bind('in_vert', 'in_color', layout='2ni 3f'),
        ])

    def render(self, time, frame_time):
        self.ctx.screen.clear((1.0, 1.0, 1.0))
        self.vao.render()


if __name__ == '__main__':
    Example.run()
