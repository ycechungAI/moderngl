import moderngl

import _example


class Example(_example.Example):
    title = 'Color Cube'

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                uniform mat4 Mvp;

                in vec3 in_vert;
                in vec3 in_color;
                out vec3 v_color;

                void main() {
                    gl_Position = Mvp * vec4(in_vert, 1.0);
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
            -1.0, -1.0, -1.0, 0.0, 0.0, 0.0,
            -1.0, 1.0, -1.0, 0.0, 1.0, 0.0,
            1.0, 1.0, -1.0, 1.0, 1.0, 0.0,
            1.0, -1.0, -1.0, 1.0, 0.0, 0.0,
            -1.0, -1.0, 1.0, 0.0, 0.0, 1.0,
            1.0, -1.0, 1.0, 1.0, 0.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            -1.0, 1.0, 1.0, 0.0, 1.0, 1.0,
        ])

        index_data = moderngl.pack([
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4,
            0, 3, 5,
            5, 4, 0,
            3, 2, 6,
            6, 5, 3,
            2, 1, 7,
            7, 6, 2,
            1, 0, 4,
            4, 7, 1,
        ], 'i4')

        self.vbo = self.ctx.buffer(vertex_data)
        self.ibo = self.ctx.buffer(index_data)
        self.vao = self.ctx.vertex_array(self.prog, [
            self.vbo.bind('in_vert', 'in_color'),
        ], self.ibo)

        self.vao.scope = self.ctx.scope(self.ctx.DEPTH_TEST)

    def render(self, time, frame_time):
        self.ctx.screen.clear((1.0, 1.0, 1.0))
        projection = (60.0, self.aspect_ratio, 0.1, 1000.0)
        self.prog['Mvp'] = moderngl.math.camera(projection, eye=(4.0, 3.0, 2.0))
        self.vao.render()


if __name__ == '__main__':
    Example.run()
