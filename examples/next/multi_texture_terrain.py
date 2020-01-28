import os

import moderngl.next as mgl
import numpy as np
from PIL import Image
from pyrr import Matrix44

import data
from example_window import Example, run_example


def terrain(size):
    vertices = np.dstack(np.mgrid[0:size, 0:size][::-1]) / size
    temp = np.dstack([np.arange(0, size * size - size), np.arange(size, size * size)])
    index = np.pad(temp.reshape(size - 1, 2 * size), [[0, 0], [0, 1]], 'constant', constant_values=-1)
    return vertices, index


class MultiTextireTerrain(Example):
    def __init__(self):
        self.ctx = mgl.create_context()

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                uniform mat4 Mvp;
                uniform sampler2D Heightmap;

                in vec2 in_vert;
                out vec2 v_text;

                void main() {
                    vec4 vertex = vec4(in_vert - 0.5, texture(Heightmap, in_vert).r * 0.2, 1.0);
                    gl_Position = Mvp * vertex;
                    v_text = in_vert;
                }
            ''',
            fragment_shader='''
                #version 330

                uniform sampler2D Heightmap;

                uniform sampler2D Color1;
                uniform sampler2D Color2;

                uniform sampler2D Cracks;
                uniform sampler2D Darken;

                in vec2 v_text;

                out vec4 f_color;

                void main() {
                    float height = texture(Heightmap, v_text).r;
                    float border = smoothstep(0.5, 0.7, height);

                    vec3 color1 = texture(Color1, v_text * 7.0).rgb;
                    vec3 color2 = texture(Color2, v_text * 6.0).rgb;

                    vec3 color = color1 * (1.0 - border) + color2 * border;

                    color *= 0.8 + 0.2 * texture(Darken, v_text * 3.0).r;
                    color *= 0.5 + 0.5 * texture(Cracks, v_text * 5.0).r;
                    color *= 0.5 + 0.5 * height;

                    f_color = vec4(color, 1.0);
                }
            ''',
        )

        vertices, index = terrain(32)

        self.vbo = self.ctx.buffer(vertices.astype('f4').tobytes())
        self.ibo = self.ctx.buffer(index.astype('i4').tobytes())

        vao_content = [
            (self.vbo, '2f', 'in_vert'),
        ]

        self.vao = self.ctx.vertex_array(self.prog, vao_content, self.ibo)

        img0 = Image.open(data.find('heightmap.jpg')).convert('L').transpose(Image.FLIP_TOP_BOTTOM)
        img1 = Image.open(data.find('grass.jpg')).convert('RGB').transpose(Image.FLIP_TOP_BOTTOM)
        img2 = Image.open(data.find('rock.jpg')).convert('RGB').transpose(Image.FLIP_TOP_BOTTOM)
        img3 = Image.open(data.find('cracks.jpg')).convert('L').transpose(Image.FLIP_TOP_BOTTOM)
        img4 = Image.open(data.find('checked.jpg')).convert('L').transpose(Image.FLIP_TOP_BOTTOM)

        tex0 = self.ctx.texture(img0.size, 1, img0.tobytes())
        tex1 = self.ctx.texture(img1.size, 3, img1.tobytes())
        tex2 = self.ctx.texture(img2.size, 3, img2.tobytes())
        tex3 = self.ctx.texture(img3.size, 1, img3.tobytes())
        tex4 = self.ctx.texture(img4.size, 1, img4.tobytes())

        sampler0 = self.ctx.sampler(tex0)
        sampler1 = self.ctx.sampler(tex1)
        sampler2 = self.ctx.sampler(tex2)
        sampler3 = self.ctx.sampler(tex3)
        sampler4 = self.ctx.sampler(tex4)

        # tex0.build_mipmaps()
        # tex1.build_mipmaps()
        # tex2.build_mipmaps()
        # tex3.build_mipmaps()
        # tex4.build_mipmaps()

        sampler0.use(0)
        sampler1.use(1)
        sampler2.use(2)
        sampler3.use(3)
        sampler4.use(4)

        self.prog['Heightmap'] = 0
        self.prog['Color1'] = 1
        self.prog['Color2'] = 2
        self.prog['Cracks'] = 3
        self.prog['Darken'] = 4

    def render(self):
        angle = self.wnd.time * 0.2
        self.ctx.screen.viewport = self.wnd.viewport
        self.ctx.clear(1.0, 1.0, 1.0)

        with self.ctx.scope(mgl.DEPTH_TEST):
            proj = Matrix44.perspective_projection(45.0, self.wnd.ratio, 0.1, 1000.0)
            lookat = Matrix44.look_at(
                (np.cos(angle), np.sin(angle), 0.8),
                (0.0, 0.0, 0.1),
                (0.0, 0.0, 1.0),
            )

            self.prog['Mvp'] = (proj * lookat).astype('f4').tobytes()
            self.vao.render(mgl.TRIANGLE_STRIP)


run_example(MultiTextireTerrain)
