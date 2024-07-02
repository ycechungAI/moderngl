import math
import os
import sys

import glm
import moderngl
import pygame
from objloader import Obj
from PIL import Image

os.environ['SDL_WINDOWS_DPI_AWARENESS'] = 'permonitorv2'

pygame.init()
pygame.display.set_mode((800, 800), flags=pygame.OPENGL | pygame.DOUBLEBUF, vsync=True)


class Scene:
    def __init__(self):
        self.ctx = moderngl.get_context()

        self.program = self.ctx.program(
            vertex_shader='''
                #version 330 core

                uniform mat4 camera;

                layout (location = 0) in vec3 in_vertex;
                layout (location = 1) in vec3 in_normal;
                layout (location = 2) in vec2 in_uv;

                out vec3 v_vertex;
                out vec3 v_normal;
                out vec2 v_uv;

                void main() {
                    v_vertex = in_vertex;
                    v_normal = in_normal;
                    v_uv = in_uv;

                    gl_Position = camera * vec4(v_vertex, 1.0);
                }
            ''',
            fragment_shader='''
                #version 330 core

                uniform sampler2D Texture;
                uniform vec3 light_direction;

                in vec3 v_vertex;
                in vec3 v_normal;
                in vec2 v_uv;

                layout (location = 0) out vec4 out_color;

                void main() {
                    float lum = max(dot(normalize(v_normal), normalize(light_direction)), 0.0) * 0.5 + 0.5;
                    out_color = texture(Texture, v_uv) * lum;
                }
            ''',
        )

        self.texture = self.load_texture('examples/data/textures/crate.png')
        self.vertex_buffer = self.load_model('examples/data/models/crate.obj')

        self.sampler = self.ctx.sampler(texture=self.texture)
        self.vertex_array = self.ctx.vertex_array(self.program, [
            (self.vertex_buffer, '3f 3f 2f', 'in_vertex', 'in_normal', 'in_uv'),
        ])

    def load_model(self, path):
        from OpenGL import GL
        obj = Obj.open(path)
        data = obj.pack('vx vy vz nx ny nz tx ty')
        buffer = GL.glGenBuffers(1)
        GL.glBindBuffer(GL.GL_ARRAY_BUFFER, buffer)
        GL.glBufferData(GL.GL_ARRAY_BUFFER, data, GL.GL_STATIC_DRAW)
        return self.ctx.external_buffer(buffer, len(data))

    def load_texture(self, path):
        img = Image.open(path).convert('RGBA')
        return self.ctx.texture(img.size, 4, img.tobytes())

    def camera_matrix(self):
        now = pygame.time.get_ticks() / 1000.0
        eye = (math.cos(now) * 3.0, math.sin(now) * 3.0, 1.75)
        proj = glm.perspective(45.0, 1.0, 0.1, 1000.0)
        look = glm.lookAt(eye, (0.0, 0.0, 0.5), (0.0, 0.0, 1.0))
        return proj * look

    def render(self):
        self.ctx.clear()
        self.ctx.enable_only(self.ctx.DEPTH_TEST)
        self.program['light_direction'] = (1.0, 2.0, 3.0)
        self.program['camera'].write(self.camera_matrix())
        self.sampler.use()
        self.vertex_array.render()


scene = Scene()

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    scene.render()

    pygame.display.flip()
