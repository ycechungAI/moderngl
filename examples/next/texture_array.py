from PIL import Image, ImageDraw, ImageFont

import moderngl.next as mgl
import numpy as np

from window import Example, run_example


def rect(x, y, w, h, t):
    return [
        (x - w / 2, y - h / 2, 0.0, 0.0, t),
        (x - w / 2, y + h / 2, 0.0, 1.0, t),
        (x + w / 2, y - h / 2, 1.0, 0.0, t),
        (x - w / 2, y + h / 2, 0.0, 1.0, t),
        (x + w / 2, y - h / 2, 1.0, 0.0, t),
        (x + w / 2, y + h / 2, 1.0, 1.0, t),
    ]


class HelloWorld(Example):
    aspect_ratio = 1.0
    title = "Sampler Linear Filter"

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        # import gltraces
        # mglprocs = mgl.glprocs(self.ctx)
        # gltraces.glprocs[:] = mglprocs
        # mglprocs[:] = gltraces.gltraces

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                in vec2 in_vert;
                in vec3 in_text;

                out vec3 v_text;

                void main() {
                    gl_Position = vec4(in_vert, 0.0, 1.0);
                    v_text = in_text;
                }
            ''',
            fragment_shader='''
                #version 330

                uniform sampler2DArray Sampler;

                in vec3 v_text;
                out vec4 f_color;

                void main() {
                    f_color = texture(Sampler, v_text);
                }
            ''',
        )

        vertices = np.array([
            rect(-0.5, -0.5, 0.9, 0.9, 0.0),
            rect(-0.5, 0.5, 0.9, 0.9, 1.0),
            rect(0.5, -0.5, 0.9, 0.9, 2.0),
            rect(0.5, 0.5, 0.9, 0.9, 3.0),
        ])

        img1 = Image.new('RGB', (200, 200), '#fee')
        img2 = Image.new('RGB', (200, 200), '#efe')
        img3 = Image.new('RGB', (200, 200), '#eef')
        img4 = Image.new('RGB', (200, 200), '#ffe')

        ImageDraw.ImageDraw(img1).text((10, 10), 'img 1', '#000', ImageFont.truetype('arial', 32))
        ImageDraw.ImageDraw(img2).text((10, 10), 'img 2', '#000', ImageFont.truetype('arial', 32))
        ImageDraw.ImageDraw(img3).text((10, 10), 'img 3', '#000', ImageFont.truetype('arial', 32))
        ImageDraw.ImageDraw(img4).text((10, 10), 'img 4', '#000', ImageFont.truetype('arial', 32))

        def join(*images):
            return b''.join(img.tobytes('raw', 'RGB', 0, -1) for img in images)

        self.texture = self.ctx.texture_array((200, 200, 4), 3, join(img1, img2, img3, img4))
        self.sampler = self.ctx.sampler(self.texture)
        self.sampler.use()

        self.vbo = self.ctx.buffer(vertices.astype('f4').tobytes())
        self.vao = self.ctx.simple_vertex_array(self.prog, self.vbo, 'in_vert', 'in_text')

    def render(self, time: float, frame_time: float):
        self.ctx.clear(1.0, 1.0, 1.0)
        self.vao.render()


run_example(HelloWorld)
