import math
import os
import sys

import moderngl
import pygame

os.environ['SDL_WINDOWS_DPI_AWARENESS'] = 'permonitorv2'

pygame.init()
pygame.display.set_mode((800, 800), flags=pygame.OPENGL | pygame.DOUBLEBUF, vsync=True)


class Scene:
    def __init__(self):
        self.ctx = moderngl.get_context()

        self.program = self.ctx.program(
            vertex_shader='''
                #version 330 core

                vec3 vertices[3] = vec3[](
                    vec3(0.0, 0.4, 0.0),
                    vec3(-0.4, -0.3, 0.0),
                    vec3(0.4, -0.3, 0.0)
                );

                void main() {
                    gl_Position = vec4(vertices[gl_VertexID], 1.0);
                }
            ''',
            fragment_shader='''
                #version 330 core

                layout (location = 0) out vec4 out_color;

                void main() {
                    out_color = vec4(1.0, 1.0, 1.0, 1.0);
                }
            ''',
        )

        self.vao = self.ctx.vertex_array(self.program, [])
        self.vao.vertices = 3

    def render(self):
        self.ctx.clear()
        self.vao.render()


scene = Scene()

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    scene.render()

    pygame.display.flip()
