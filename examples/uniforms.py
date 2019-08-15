import moderngl

import _example


class Example(_example.Example):
    title = 'Uniforms'

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                uniform vec2 Position;
                uniform float Rotation;
                uniform vec2 Scale;
                in vec2 in_vert;

                void main() {
                    mat2 rot = mat2(cos(Rotation), -sin(Rotation), sin(Rotation), cos(Rotation));
                    gl_Position = vec4((Position + rot * in_vert) * Scale, 0.0, 1.0);
                }
            ''',
            fragment_shader='''
                #version 330
                uniform vec4 Color;
                out vec4 f_color;
                void main() {
                    f_color = Color;
                }
            ''',
        )

        vertex_data = moderngl.pack([
            0.0, 1.0,
            -0.866, -0.5,
            0.866, -0.5,
        ])

        self.vbo = self.ctx.buffer(vertex_data)
        self.vao = self.ctx.vertex_array(self.prog, [
            self.vbo.bind('in_vert'),
        ])

        self.prog['Scale'] = (0.4, 0.4 * self.aspect_ratio)


    def render(self, time, frame_time):
        self.ctx.screen.clear((1.0, 1.0, 1.0))
        self.prog['Color'] = (0.3, 0.5, 1.0)
        self.prog['Position'] = (-1.0, 0.0)
        self.prog['Rotation'] = time
        self.vao.render()
        self.prog['Color'] = (0.2, 0.4, 0.8)
        self.prog['Position'] = (0.0, 0.0)
        self.prog['Rotation'] = time
        self.vao.render()
        self.prog['Color'] = (0.1, 0.3, 0.6)
        self.prog['Position'] = (1.0, 0.0)
        self.prog['Rotation'] = time
        self.vao.render()


if __name__ == '__main__':
    Example.run()
