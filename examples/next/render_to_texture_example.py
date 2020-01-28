import os

import moderngl.next as mgl
import numpy as np
from objloader import Obj
from PIL import Image
from pyrr import Matrix44

import data
from window import Example, run_example


class RenderToTexture(Example):
    title = "Render to Texture"
    gl_version = (3, 3)

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

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
                uniform vec3 Color;
                uniform bool UseTexture;
                uniform sampler2D Texture;

                in vec3 v_vert;
                in vec3 v_norm;
                in vec2 v_text;

                out vec4 f_color;

                void main() {
                    float lum = clamp(dot(normalize(Light - v_vert), normalize(v_norm)), 0.0, 1.0) * 0.8 + 0.2;
                    if (UseTexture) {
                        f_color = vec4(texture(Texture, v_text).rgb * lum, 1.0);
                    } else {
                        f_color = vec4(Color * lum, 1.0);
                    }
                }
            ''',
        )

        self.objects = {}

        for name in ['ground', 'grass', 'billboard', 'billboard-holder', 'billboard-image']:
            obj = Obj.open(data.find('scene-1-%s.obj' % name))
            vbo = self.ctx.buffer(obj.pack('vx vy vz nx ny nz tx ty'))
            vao = self.ctx.simple_vertex_array(self.prog, vbo, 'in_vert', 'in_norm', 'in_text')
            self.objects[name] = vao

        img = Image.open(data.find('infographic-1.jpg')).transpose(Image.FLIP_TOP_BOTTOM).convert('RGB')
        self.texture1 = self.ctx.texture(img.size, 3, img.tobytes())
        # self.texture1.build_mipmaps()

        self.texture2 = self.ctx.texture(self.wnd.size, 3)
        depth_attachment = self.ctx.depth_renderbuffer(self.wnd.size)
        self.fbo = self.ctx.framebuffer(self.texture2, depth_attachment)

        self.sampler1 = self.ctx.sampler(self.texture1)
        self.sampler2 = self.ctx.sampler(self.texture2)

        self.scope1 = self.ctx.scope(mgl.DEPTH_TEST, self.fbo, samplers=[(self.sampler1, 0)])
        self.scope2 = self.ctx.scope(mgl.DEPTH_TEST, self.ctx.screen, samplers=[(self.sampler2, 0)])

    def render(self, time, frame_time):
        self.ctx.clear(1.0, 1.0, 1.0)
        # self.ctx.enable(mgl.DEPTH_TEST)
        proj = Matrix44.perspective_projection(45.0, self.aspect_ratio, 0.1, 1000.0)
        lookat = Matrix44.look_at(
            (47.697, -8.147, 24.498),
            (0.0, 0.0, 8.0),
            (0.0, 0.0, 1.0),
        )

        rotate = Matrix44.from_z_rotation(np.sin(time) * 0.5 + 0.2)

        self.fbo.clear(0, (1.0, 1.0, 1.0))
        self.fbo.clear(-1, 1.0)
        for scope in [self.scope1, self.scope2]:
            with scope:
                self.prog['UseTexture'] = False

                self.prog['Light'] = (67.69, -8.14, 52.49)
                self.prog['Mvp'] = (proj * lookat * rotate).astype('f4').tobytes()

                self.prog['Color'] = (0.67, 0.49, 0.29)
                self.objects['ground'].render()

                self.prog['Color'] = (0.46, 0.67, 0.29)
                self.objects['grass'].render()

                self.prog['Color'] = (1.0, 1.0, 1.0)
                self.objects['billboard'].render()

                self.prog['Color'] = (0.2, 0.2, 0.2)
                self.objects['billboard-holder'].render()

                self.prog['UseTexture'] = True

                self.objects['billboard-image'].render()


if __name__ == '__main__':
    run_example(RenderToTexture)
