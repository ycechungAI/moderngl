from PIL import Image, ImageDraw, ImageFont

import moderngl.next as mgl
import numpy as np
from objloader import Obj

from pyrr import Matrix44
import data
from window import Example, run_example


class HelloWorld(Example):
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

                uniform mat4 Mvp;

                in vec3 in_vert;
                in vec3 in_norm;

                out vec3 v_vert;
                out vec3 v_norm;

                void main() {
                    gl_Position = Mvp * vec4(in_vert, 1.0);
                    v_vert = in_vert;
                    v_norm = in_norm;
                }
            ''',
            fragment_shader='''
                #version 330

                uniform samplerCube Sampler;
                uniform vec3 Eye;

                in vec3 v_vert;
                in vec3 v_norm;

                out vec4 f_color;

                void main() {
                    f_color = texture(Sampler, reflect(v_vert - Eye, v_norm));
                }
            ''',
        )

        img1 = Image.new('RGB', (200, 200), '#fee')
        img2 = Image.new('RGB', (200, 200), '#efe')
        img3 = Image.new('RGB', (200, 200), '#eef')
        img4 = Image.new('RGB', (200, 200), '#ffe')
        img5 = Image.new('RGB', (200, 200), '#fef')
        img6 = Image.new('RGB', (200, 200), '#eff')

        ImageDraw.ImageDraw(img1).text((50, 30), 'i1', '#000', ImageFont.truetype('arial', 128))
        ImageDraw.ImageDraw(img2).text((50, 30), 'i2', '#000', ImageFont.truetype('arial', 128))
        ImageDraw.ImageDraw(img3).text((50, 30), 'i3', '#000', ImageFont.truetype('arial', 128))
        ImageDraw.ImageDraw(img4).text((50, 30), 'i4', '#000', ImageFont.truetype('arial', 128))
        ImageDraw.ImageDraw(img5).text((50, 30), 'i5', '#000', ImageFont.truetype('arial', 128))
        ImageDraw.ImageDraw(img6).text((50, 30), 'i6', '#000', ImageFont.truetype('arial', 128))

        def join(*images):
            return b''.join(img.tobytes('raw', 'RGB', 0, -1) for img in images)

        self.texture = self.ctx.texture_cube((200, 200), 3, join(img1, img2, img3, img4, img5, img6))
        self.sampler = self.ctx.sampler(self.texture)
        self.sampler.filter = mgl.LINEAR
        self.sampler.use()

        obj = Obj.open(data.find('sitting_dummy.obj'))
        self.vbo = self.ctx.buffer(obj.pack('vx vy vz nx ny nz'))
        self.vao = self.ctx.simple_vertex_array(self.prog, self.vbo, 'in_vert', 'in_norm')
        self.scope = self.ctx.scope(mgl.DEPTH_TEST)

    def render(self, time: float, frame_time: float):
        camera_pos = (np.cos(time) * 300.0, np.sin(time) * 300.0, 120.0)
        proj = Matrix44.perspective_projection(45.0, self.aspect_ratio, 0.1, 1000.0)
        lookat = Matrix44.look_at(
            camera_pos,
            (0.0, 0.0, 50.0),
            (0.0, 0.0, 1.0),
        )

        with self.scope:
            self.prog['Mvp'] = (proj * lookat).astype('f4').tobytes()
            self.prog['Eye'] = camera_pos
            self.ctx.clear(1.0, 1.0, 1.0)
            self.vao.render()


run_example(HelloWorld)
