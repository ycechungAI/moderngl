import os
import struct

import moderngl.next as mgl
import numpy as np
from objloader import Obj
from PIL import Image
from pyrr import Matrix44

import data
from example_window import Example, run_example


class CrateExample(Example):
    def __init__(self):
        self.ctx = mgl.create_context()
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
                    f_color = vec4(texture(Texture, v_text).rgb * lum, 1.0);
                }
            ''',
        )

        obj1 = Obj.open(data.find('crate_left.obj'))
        obj2 = Obj.open(data.find('crate.obj'))
        obj3 = Obj.open(data.find('crate_right.obj'))

        img1 = Image.open(data.find('crate.png')).transpose(Image.FLIP_TOP_BOTTOM).convert('RGB')
        img2 = Image.open(data.find('rock.jpg')).transpose(Image.FLIP_TOP_BOTTOM).convert('RGB')

        self.texture1 = self.ctx.texture(img1)
        self.texture2 = self.ctx.texture(img2)

        self.sampler1 = self.ctx.sampler(self.texture1)
        self.sampler2 = self.ctx.sampler(self.texture2)

        self.vbo1 = self.ctx.buffer(obj1.pack('vx vy vz nx ny nz tx ty'))
        self.vbo2 = self.ctx.buffer(obj2.pack('vx vy vz nx ny nz tx ty'))
        self.vbo3 = self.ctx.buffer(obj3.pack('vx vy vz nx ny nz tx ty'))

        self.vao1 = self.ctx.simple_vertex_array(self.prog, self.vbo1, 'in_vert', 'in_norm', 'in_text')
        self.vao1.scope = self.ctx.scope(mgl.DEPTH_TEST, samplers=[(self.sampler2, 0)])

        self.vao2 = self.ctx.simple_vertex_array(self.prog, self.vbo2, 'in_vert', 'in_norm', 'in_text')
        self.vao2.scope = self.ctx.scope(mgl.DEPTH_TEST, samplers=[(self.sampler1, 0)])

        self.vao3 = self.ctx.simple_vertex_array(self.prog, self.vbo3, 'in_vert', 'in_norm', 'in_text')
        self.vao3.scope = self.ctx.scope(mgl.DEPTH_TEST, samplers=[(self.sampler2, 0)])

        with self.ctx.recorder:
            self.ctx.clear(1.0, 1.0, 1.0)
            self.vao1.render()
            self.vao2.render()
            self.vao3.render()

        self.bytecode = self.ctx.recorder.dump()
        print(self.bytecode)

    def render(self):
        angle = self.wnd.time
        self.ctx.screen.viewport = self.wnd.viewport

        camera_pos = (np.cos(angle) * 5.0, np.sin(angle) * 5.0, 2.0)

        proj = Matrix44.perspective_projection(45.0, self.wnd.ratio, 0.1, 1000.0)
        lookat = Matrix44.look_at(
            camera_pos,
            (0.0, 0.0, 0.5),
            (0.0, 0.0, 1.0),
        )

        self.prog['Mvp'] = (proj * lookat).astype('f4').tobytes()
        self.prog['Light'] = camera_pos

        self.ctx.replay(self.bytecode)

run_example(CrateExample)
