import math
import os
import sys

import imgui
import moderngl
import pygame
from zengl_imgui import PygameBackend

os.environ['SDL_WINDOWS_DPI_AWARENESS'] = 'permonitorv2'

pygame.init()
pygame.display.set_mode((1280, 720), flags=pygame.OPENGL | pygame.DOUBLEBUF, vsync=True)


class MGLBackend(PygameBackend):
    def __init__(self):
        super().__init__()
        self.renderer.ctx.end_frame(flush=False)

    def render(self):
        self.renderer.ctx.new_frame(clear=False)
        super().render()
        self.renderer.ctx.end_frame()


impl = MGLBackend()


class Scene:
    def __init__(self):
        self.ctx = moderngl.get_context()

    def render(self):
        now = pygame.time.get_ticks() / 1000.0

        r = math.sin(now + 0.0) * 0.5 + 0.5
        g = math.sin(now + 2.1) * 0.5 + 0.5
        b = math.sin(now + 4.2) * 0.5 + 0.5
        self.ctx.clear(r, g, b)


scene = Scene()

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        impl.process_event(event)
    impl.process_inputs()

    imgui.new_frame()
    imgui.show_test_window()
    imgui.end_frame()
    imgui.render()

    scene.render()
    impl.render()

    pygame.display.flip()
