import os
import struct

import moderngl.next as mgl
import numpy as np
from objloader import Obj
from PIL import Image
from pyrr import Matrix44

import data
from example_window import Example, run_example


class MugExample(Example):
    def __init__(self):
        self.ctx = mgl.create_context()

        self.canvas_prog = self.ctx.program(
            vertex_shader='''
                #version 330

                in vec2 in_vert;
                out vec2 v_vert;

                void main() {
                    gl_Position = vec4(in_vert * 2.0 - 1.0, 0.0, 1.0);
                    v_vert = in_vert;
                }
            ''',
            fragment_shader='''
                #version 330

                uniform sampler2D Texture;

                in vec2 v_vert;

                out vec4 f_color;

                void main() {
                    f_color = texture(Texture, v_vert);
                }
            ''',
        )

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                uniform mat4 Mvp;

                in vec3 in_vert;
                in vec3 in_norm;
                in vec2 in_text;

                out vec3 v_vert;
                out vec3 v_norm;
                out vec2 v_text;

                void main() {
                    gl_Position = Mvp * vec4(in_vert, 1.0);
                    v_vert = in_vert;
                    v_norm = in_norm;
                    v_text = in_text;
                }
            ''',
            fragment_shader='''
                #version 330

                uniform vec3 Light;
                uniform sampler2D Texture;

                in vec3 v_vert;
                in vec3 v_norm;
                in vec2 v_text;

                out vec4 f_color;

                void main() {
                    float lum = clamp(dot(normalize(Light - v_vert), normalize(v_norm)), 0.0, 1.0) * 0.8 + 0.2;
                    vec3 base = vec3(0.5, 0.5, 0.5) * lum;
                    vec3 spec = vec3(1.0, 1.0, 1.0) * pow(lum, 5.7);
                    vec4 tex = texture(Texture, v_text);
                    f_color = vec4(base * 0.1 + tex.rgb * lum + spec, tex.a);
                }
            ''',
        )

        self.canvas_vbo = self.ctx.buffer(np.array([0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0], dtype='f4').tobytes())
        self.canvas_vao = self.ctx.simple_vertex_array(self.canvas_prog, self.canvas_vbo, 'in_vert')

        bg_img = Image.open(data.find('mug-background.jpg')).transpose(Image.FLIP_TOP_BOTTOM).convert('RGB')
        self.bg_texture = self.ctx.texture(bg_img.size, 3, bg_img.tobytes())
        self.bg_sampler = self.ctx.sampler(self.bg_texture)

        sticker_img = Image.open(data.find('mug-pymet-logo.png')).transpose(Image.FLIP_TOP_BOTTOM).convert('RGBA')
        self.sticker_texture = self.ctx.texture(sticker_img.size, 4, sticker_img.tobytes())
        # self.sticker_texture.build_mipmaps(0, 2)
        self.sticker_sampler = self.ctx.sampler(self.sticker_texture)

        self.mug_texture = self.ctx.texture((1, 1), 3)
        self.mug_texture.write(struct.pack('3B', 10, 10, 10))
        self.mug_sampler = self.ctx.sampler(self.mug_texture)

        obj = Obj.open(data.find('mug.obj'))
        self.mug_vbo = self.ctx.buffer(obj.pack('vx vy vz nx ny nz tx ty'))
        self.mug_vao = self.ctx.simple_vertex_array(self.prog, self.mug_vbo, 'in_vert', 'in_norm', 'in_text')

        segs = 32
        radius = 29.94
        bottom = 6.601
        top = 57.856
        left = -163.12 * np.pi / 180.0
        right = 11.25 * np.pi / 180.0

        lin = np.linspace(left, right, segs)
        sticker_vertices = np.array([
            np.repeat(np.cos(lin) * radius, 2),
            np.repeat(np.sin(lin) * radius, 2),
            np.tile([bottom, top], segs),
            np.repeat(np.cos(lin), 2),
            np.repeat(np.sin(lin), 2),
            np.tile([0.0, 0.0], segs),
            np.repeat(np.linspace(0.0, 1.0, segs), 2),
            np.tile([0.0, 1.0], segs),
        ])

        self.sticker_vbo = self.ctx.buffer(sticker_vertices.T.astype('f4').tobytes())
        self.sticker_vao = self.ctx.simple_vertex_array(self.prog, self.sticker_vbo, 'in_vert', 'in_norm', 'in_text')

    def render(self):
        self.ctx.screen.viewport = self.wnd.viewport
        self.ctx.clear(1.0, 1.0, 1.0)
        self.bg_sampler.use()
        with self.ctx.scope(mgl.BLEND):
            self.canvas_vao.render(mgl.TRIANGLE_STRIP)
        with self.ctx.scope(mgl.DEPTH_TEST):

            proj = Matrix44.perspective_projection(30.0, self.wnd.ratio, 1.0, 1000.0)
            lookat = Matrix44.look_at(
                (46.748, -280.619, 154.391),
                (-23.844, 2.698, 44.493),
                (0.0, 0.0, 1.0),
            )

            self.prog['Mvp'] = (proj * lookat).astype('f4').tobytes()
            self.prog['Light'] = (-143.438, -159.072, 213.268)
            self.mug_sampler.use()
            self.mug_vao.render()
        with self.ctx.scope(mgl.DEPTH_TEST | mgl.BLEND):
            self.sticker_sampler.use()
            self.sticker_vao.render(mgl.TRIANGLE_STRIP)


run_example(MugExample)
