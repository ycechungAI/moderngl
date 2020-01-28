import os

import moderngl.next as mgl
import numpy as np
from objloader import Obj
from PIL import Image
from pyrr import Matrix44

import data
from window import Example, run_example


class RenderTextureCompute(Example):
    title = "Render Texture Using Compute Shader"
    gl_version = (4, 3)

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

        self.compute = self.ctx.compute_shader('''
            #version 430
            layout (local_size_x = 16, local_size_y = 16) in;
            layout(rg32f,location=0) writeonly uniform image2D destTex;
            uniform float time;
            void main() {
                ivec2 ij = ivec2(gl_GlobalInvocationID.xy);
                float localCoef = length(vec2(ivec2(gl_LocalInvocationID.xy)-8)/8.0);
                float globalCoef = sin(float(gl_WorkGroupID.x+gl_WorkGroupID.y)*0.1 + time)*0.5;
                imageStore(destTex, ij, vec4(1.0-globalCoef*localCoef, 0.0, 0.0, 0.0));
            }
        ''')

        self.objects = {}

        for name in ['ground', 'grass', 'billboard', 'billboard-holder', 'billboard-image']:
            obj = Obj.open(data.find('scene-1-%s.obj' % name))
            vbo = self.ctx.buffer(obj.pack('vx vy vz nx ny nz tx ty'))
            vao = self.ctx.simple_vertex_array(self.prog, vbo, 'in_vert', 'in_norm', 'in_text')
            self.objects[name] = vao

        img = Image.new('RGB', (512, 512))
        self.texture = self.ctx.texture(img.size, 3, img.tobytes())
        self.sampler = self.ctx.sampler(self.texture)
        self.scope = self.ctx.scope(mgl.DEPTH_TEST, self.ctx.screen, samplers=[(self.sampler, 0)])

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

        w, h = self.texture.size
        gw, gh = 16, 16
        nx, ny, nz = int(w/gw), int(h/gh), 1

        print('=' * 50)

        self.compute['time'] = time
        GL_WRITE_ONLY = 0x88B9
        GL_R32F = 0x822E
        self.texture.bind_to_image(0,GL_WRITE_ONLY, GL_R32F)
        self.compute.run(nx, ny, nz)
        print('-' * 50)

        with self.scope:
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
    run_example(RenderTextureCompute)
