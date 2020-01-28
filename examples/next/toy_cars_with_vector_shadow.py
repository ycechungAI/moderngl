import os
import struct
from colorsys import hls_to_rgb as hls
from random import uniform

import moderngl.next as mgl
import numpy as np
from objloader import Obj
from pyrr import Matrix44

import data
from example_window import Example, run_example


def random_color():
    return hls(uniform(0.0, 1.0), 0.5, 0.5)


cars = []
cars += [{'color': random_color(), 'pos': (1.5, i * 2.0 - 9.0, 0.0), 'angle': uniform(-0.5, 0.5)} for i in range(10)]
cars += [{'color': random_color(), 'pos': (-1.5, i * 2.0 - 9.0, 0.0), 'angle': uniform(-0.5, 0.5)} for i in range(10)]


class ToyCars(Example):
    def __init__(self):
        self.ctx = mgl.create_context()

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                uniform mat4 Mvp;

                in vec3 in_vert;
                in vec3 in_norm;

                in vec3 in_color;
                in vec3 in_origin;
                in mat3 in_basis;

                out vec3 v_vert;
                out vec3 v_norm;
                out vec3 v_color;

                void main() {
                    v_vert = in_origin + in_basis * in_vert;
                    v_norm = in_basis * in_norm;
                    v_color = in_color;
                    gl_Position = Mvp * vec4(v_vert, 1.0);
                }
            ''',
            fragment_shader='''
                #version 330

                uniform vec3 Light;
                uniform sampler2D Texture;

                in vec3 v_vert;
                in vec3 v_norm;
                in vec3 v_color;

                out vec4 f_color;

                void main() {
                    float lum = clamp(dot(normalize(Light - v_vert), normalize(v_norm)), 0.0, 1.0) * 0.8 + 0.2;
                    f_color = vec4(v_color * lum, 1.0);
                }
            ''',
        )

        obj = Obj.open(data.find('lowpoly_toy_car.obj'))

        self.vbo1 = self.ctx.buffer(obj.pack('vx vy vz nx ny nz'))
        self.vbo2 = self.ctx.buffer(struct.pack(
            '15f',
            1.0, 1.0, 1.0,
            0.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
        ) * len(cars))
        self.vao = self.ctx.vertex_array(self.prog, [
            (self.vbo1, '3f 3f', 'in_vert', 'in_norm'),
            (self.vbo2, '3f 3f 9f/i', 'in_color', 'in_origin', 'in_basis'),
        ])

    def render(self):
        angle = self.wnd.time
        self.ctx.screen.viewport = self.wnd.viewport
        self.ctx.clear(1.0, 1.0, 1.0)

        with self.ctx.scope(mgl.DEPTH_TEST):
            camera_pos = (np.cos(angle) * 20.0, np.sin(angle) * 20.0, 5.0)

            proj = Matrix44.perspective_projection(45.0, self.wnd.ratio, 0.1, 1000.0)
            lookat = Matrix44.look_at(
                camera_pos,
                (0.0, 0.0, 0.5),
                (0.0, 0.0, 1.0),
            )

            self.prog['Mvp'] = (proj * lookat).astype('f4').tobytes()
            self.prog['Light'] = camera_pos

            # self.vbo2.write(Matrix33.from_z_rotation(self.wnd.time).astype('f4').tobytes(), offset=24)
            self.vbo2.write(b''.join(struct.pack(
                '15f',
                *car['color'],
                *car['pos'],
                1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 0.0, 1.0,
            ) for car in cars))
            self.vao.render(instances=len(cars))
            self.vbo2.write(b''.join(struct.pack(
                '15f',
                0.0, 0.0, 0.0,
                *car['pos'],
                1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                0.3, 0.6, 0.0,
            ) for car in cars))
            self.vao.render(instances=len(cars))


run_example(ToyCars)
