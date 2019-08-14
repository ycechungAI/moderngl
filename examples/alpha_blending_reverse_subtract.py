import numpy as np
import moderngl

import _example


class Example(_example.Example):
    title = 'Alpha Blending'
    gl_version = (4, 0)

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 400

                in vec2 in_vert;
                in vec3 in_color;
                out vec3 v_color;

                void main() {
                    gl_Position = vec4(in_vert, 0.0, 1.0);
                    v_color = in_color;
                }
            ''',
            fragment_shader='''
                #version 400

                in vec3 v_color;
                out vec4 f_color;

                void main() {
                    f_color = vec4(v_color, 0.3);
                }
            ''',
        )

        one = np.ones(50)
        zero = np.zeros(50)
        it = np.linspace(-np.pi, np.pi, 50)

        vertex_data = moderngl.pack(np.concatenate([
            [np.cos(it) * 0.3 - 0.2, np.sin(it) * 0.5 - 0.3, one, zero, zero],
            [np.cos(it) * 0.3 + 0.2, np.sin(it) * 0.5 - 0.3, zero, one, zero],
            [np.cos(it) * 0.3 + 0.0, np.sin(it) * 0.5 + 0.3, zero, zero, one],
        ], 1).T.flatten())

        self.vbo = self.ctx.buffer(vertex_data)
        self.vao = self.ctx.vertex_array(self.prog, [
            self.vbo.bind('in_vert', 'in_color'),
        ])
        self.vao.indirect_buffer = self.ctx.buffer(moderngl.pack([
            50, 1, 0, 0,
            50, 1, 50, 0,
            50, 1, 100, 0,
        ], 'i'))

        self.vao.scope = self.ctx.scope('BLEND')
        self.vao.scope.blending = self.ctx.blending(
            blend_functions=[
                (self.ctx.SRC_ALPHA, self.ctx.ONE_MINUS_SRC_ALPHA),
            ],
            blend_equations=[
                self.ctx.FUNC_REVERSE_SUBTRACT,
            ],
        )
        self.vao.mode = self.ctx.TRIANGLE_FAN
        self.vao.instances = 3

    def render(self, time, frame_time):
        self.ctx.screen.clear((1.0, 1.0, 1.0))
        self.vao.render()


if __name__ == '__main__':
    Example.run()
