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

        # import gltraces
        # mglprocs = mgl.glprocs(self.ctx)
        # gltraces.glprocs[:] = mglprocs
        # mglprocs[:] = gltraces.gltraces

        self.prog1 = self.ctx.program(
            vertex_shader='''
                #version 330

                uniform mat4 Mvp;

                in vec3 in_vert;
                in vec3 in_norm;
                in vec3 in_text;

                out vec3 v_vert;
                out vec3 v_norm;
                out vec3 v_text;

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
                uniform samplerCube Sampler;

                in vec3 v_vert;
                in vec3 v_norm;
                in vec3 v_text;

                out vec4 f_color;

                void main() {
                    float lum = clamp(dot(normalize(Light - v_vert), normalize(v_norm)), 0.0, 1.0) * 0.8 + 0.2;
                    f_color = vec4(texture(Sampler, normalize(v_text)).rgb * lum, 1.0);
                }
            ''',
        )

        obj = Obj.open(data.find('texture-test-cube.obj'))
        self.texture1 = self.ctx.texture_cube((512, 512), 3, np.random.randint(128, 255, (512, 512, 3, 6), 'u1').tobytes())
        self.depth1 = self.ctx.texture_cube((512, 512), 1, None, dtype='d3')
        self.sampler1 = self.ctx.sampler(self.texture1, filter=mgl.LINEAR)
        self.fbo1 = self.ctx.framebuffer(self.texture1, self.depth1)
        # exit()

        self.vbo1 = self.ctx.buffer(obj.pack('vx vy vz nx ny nz tx ty tz'))
        self.vao1 = self.ctx.simple_vertex_array(self.prog1, self.vbo1, 'in_vert', 'in_norm', 'in_text')
        self.vao1.scope = self.ctx.scope(mgl.DEPTH_TEST, self.ctx.screen, samplers=[(self.sampler1, 0)])

        self.prog2 = self.ctx.program(
            vertex_shader='''
                #version 330

                in vec3 in_vert;
                in vec3 in_norm;
                in vec3 in_text;

                out vec3 v_vert;
                out vec3 v_norm;
                out vec3 v_text;

                void main() {
                    v_vert = in_vert;
                    v_norm = in_norm;
                    v_text = in_text;
                }
            ''',
            geometry_shader='''
                #version 330
                layout(triangles) in;
                layout(triangle_strip, max_vertices=18) out;

                in vec3 v_vert[3];
                in vec3 v_norm[3];
                in vec3 v_text[3];

                out vec3 g_vert;
                out vec3 g_norm;
                out vec3 g_text;

                uniform mat4 Mvp[6];

                void main() {
                    for (int layer = 0; layer < 6; ++layer) {
                        gl_Layer = layer;
                        for (int i = 0; i < 3; ++i) {
                            g_vert = v_vert[i];
                            g_norm = v_norm[i];
                            g_text = v_text[i];
                            gl_Position = Mvp[layer] * vec4(g_vert, 1.0);
                            EmitVertex();
                        }
                        EndPrimitive();
                    }
                }
            ''',
            fragment_shader='''
                #version 330

                uniform sampler3D Sampler;
                uniform vec3 Light;

                in vec3 g_vert;
                in vec3 g_norm;
                in vec3 g_text;

                out vec4 f_color;

                void main() {
                    float lum = clamp(dot(normalize(Light - g_vert), normalize(g_norm)), 0.0, 1.0) * 0.8 + 0.2;
                    f_color = vec4(texture(Sampler, g_text).rgb * lum, 1.0);
                }
            ''',
        )

        obj = Obj.open(data.find('test_scene.obj'))
        self.texture2 = self.ctx.texture((4, 4, 4), 3, np.repeat(np.random.randint(180, 220, (4, 4, 4), 'u1'), 3))
        self.sampler2 = self.ctx.sampler(self.texture2, wrap=mgl.REPEAT_X | mgl.REPEAT_Y | mgl.REPEAT_Z, filter=mgl.LINEAR)

        self.vbo2 = self.ctx.buffer(obj.pack('vx vy vz nx ny nz vx vy vz'))
        self.vao2 = self.ctx.simple_vertex_array(self.prog2, self.vbo2, 'in_vert', 'in_norm', 'in_text')
        self.vao2.scope = self.ctx.scope(mgl.DEPTH_TEST, self.fbo1, samplers=[(self.sampler2, 0)])

    def render(self, time, frame_time):
        proj = Matrix44.perspective_projection(90.0, 1.0, 0.1, 1000.0)
        x, y, z = 0.0, 0.0, 1.0
        lookats = [
            Matrix44.look_at((x, y, z), (x - 1.0, y, z), (0.0, 1.0, 0.0)),
            Matrix44.look_at((x, y, z), (x + 1.0, y, z), (0.0, 1.0, 0.0)),
            Matrix44.look_at((x, y, z), (x, y - 1.0, z), (0.0, 0.0, 1.0)),
            Matrix44.look_at((x, y, z), (x, y + 1.0, z), (0.0, 0.0, -1.0)),
            Matrix44.look_at((x, y, z), (x, y, z + 1.0), (0.0, 1.0, 0.0)),
            Matrix44.look_at((x, y, z), (x, y, z - 1.0), (0.0, 1.0, 0.0)),
        ]

        self.prog2['Mvp'] = b''.join((proj * lookat).astype('f4').tobytes() for lookat in lookats)
        self.prog2['Light'] = (0.0, 0.0, 1.0)
        self.fbo1.clear(0, (1.0, 1.0, 1.0))
        self.fbo1.clear(-1, 1.0)
        self.vao2.render()

        camera_pos = (np.cos(time) * 5.0, np.sin(time) * 5.0, 2.0)
        proj = Matrix44.perspective_projection(45.0, self.aspect_ratio, 0.1, 1000.0)
        lookat = Matrix44.look_at(
            camera_pos,
            (0.0, 0.0, 0.0),
            (0.0, 0.0, 1.0),
        )

        self.prog1['Mvp'] = (proj * lookat).astype('f4').tobytes()
        self.prog1['Light'] = camera_pos

        self.ctx.screen.use()
        self.ctx.screen.clear(0, (1.0, 1.0, 1.0))
        self.ctx.screen.clear(-1, 1.0)
        self.vao1.render()

run_example(CrateExample)
