from PIL import Image

import _example
import moderngl


class Example(_example.Example):
    title = 'Texture Array'

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                in vec2 in_vert;
                in vec2 in_text;
                out vec2 v_text;

                void main() {
                    gl_Position = vec4(in_vert, 0.0, 1.0);
                    v_text = in_text;
                }
            ''',
            fragment_shader='''
                #version 330

                uniform sampler1DArray Texture;

                in vec2 v_text;
                out vec4 f_color;

                void main() {
                    f_color = texture(Texture, v_text);
                }
            ''',
        )

        pixels = moderngl.pack([
            255, 0, 0,
            255, 127, 0,
            255, 255, 0,
            0, 255, 0,
            0, 0, 255,

            0, 0, 0,
            40, 40, 40,
            80, 80, 80,
            120, 120, 120,
            160, 160, 160,
        ], 'u1')

        self.texture = self.ctx.texture(5, 3, pixels, array=2)
        self.sampler = self.ctx.sampler(self.texture)

        vertex_data = moderngl.pack([
            -0.4, 0.4, 0.0, 0.0,
            -0.7, -0.4, 1.0, 0.0,
            -0.1, -0.4, 1.0, 0.0,
            0.4, 0.4, 0.0, 1.0,
            0.1, -0.4, 1.0, 1.0,
            0.7, -0.4, 1.0, 1.0,
        ])

        self.vbo = self.ctx.buffer(vertex_data)
        self.vao = self.ctx.vertex_array(self.prog, [
            self.vbo.bind('in_vert', 'in_text'),
        ])

        self.vao.scope = self.ctx.scope(samplers=[self.sampler.assign(0)])

    def render(self, time, frame_time):
        self.ctx.screen.clear((1.0, 1.0, 1.0))
        self.vao.render()


if __name__ == '__main__':
    Example.run()
