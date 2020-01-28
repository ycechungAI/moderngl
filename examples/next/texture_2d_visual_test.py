import os

import moderngl.next as mgl
import numpy as np
from objloader import Obj
from PIL import Image
from pyrr import Matrix44

import data
from window import Example, run_example


class CrateExample(Example):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.ctx = mgl.create_context()

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
                uniform sampler2D Sampler;

                in vec3 v_vert;
                in vec3 v_norm;
                in vec2 v_text;

                out vec4 f_color;

                void main() {
                    float lum = clamp(dot(normalize(Light - v_vert), normalize(v_norm)), 0.0, 1.0) * 0.8 + 0.2;
                    f_color = vec4(texture(Sampler, v_text).rgb * lum, 1.0);
                }
            ''',
        )

        obj = Obj.open(data.find('texture-test-cube.obj'))
        self.texture = self.ctx.texture((4, 4), 3, np.random.randint(128, 255, (4, 4, 3), 'u1'))
        self.sampler = self.ctx.sampler(self.texture, wrap=mgl.REPEAT_X | mgl.REPEAT_Y)

        self.vbo = self.ctx.buffer(obj.pack('vx vy vz nx ny nz tx ty'))
        self.vao = self.ctx.simple_vertex_array(self.prog, self.vbo, 'in_vert', 'in_norm', 'in_text')
        self.vao.scope = self.ctx.scope(mgl.DEPTH_TEST, samplers=[(self.sampler, 0)])

    def render(self, time, frame_time):
        angle = time
        self.ctx.clear(1.0, 1.0, 1.0)

        camera_pos = (np.cos(angle) * 5.0, np.sin(angle) * 5.0, 2.0)

        proj = Matrix44.perspective_projection(45.0, self.aspect_ratio, 0.1, 1000.0)
        lookat = Matrix44.look_at(
            camera_pos,
            (0.0, 0.0, 0.0),
            (0.0, 0.0, 1.0),
        )

        self.prog['Mvp'] = (proj * lookat).astype('f4').tobytes()
        self.prog['Light'] = camera_pos
        self.vao.render()


run_example(CrateExample)
