import moderngl.next as mgl
import numpy as np
from PIL import Image

import data
from window import Example, run_example

from common.simple_object_renderer import SimpleObjectRenderer


class SamplerBasicExample(Example):
    gl_version = (3, 3)
    aspect_ratio = 1.0
    title = "Sampler Basic Example"

    img = Image.open(data.find('color_arrows.png'))
    texrect = (-2.0, -2.0, 3.0, 3.0)

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.renderer = SimpleObjectRenderer(self.ctx)

        self.vertices = np.array([
            (-0.9, -0.9),
            (-0.9, 0.9),
            (0.9, -0.9),
            (0.9, 0.9),
        ], dtype='f4')

        left, bottom, right, top = self.texrect
        self.texcoords = np.array([
            (left, bottom),
            (left, top),
            (right, bottom),
            (right, top),
        ], dtype='f4')

        self.texture = self.ctx.texture(self.img.transpose(Image.FLIP_TOP_BOTTOM).convert('RGB'))
        self.sampler = self.ctx.sampler(self.texture)

    def render(self, time: float, frame_time: float):
        # clear screen
        self.ctx.clear(1.0, 1.0, 1.0)

        # render texture
        self.sampler.use()
        self.renderer.render(mgl.TRIANGLE_STRIP, vertices=self.vertices, texcoords=self.texcoords)

        # render grid
        self.renderer.render(mgl.LINES, vertices=np.array([
            (i * 0.18, -0.9, i * 0.18, 0.9, -0.9, i * 0.18, 0.9, i * 0.18) for i in range(-5, 7, 2)
        ], 'f4').reshape(-1, 2), texture=False, color=(0.0, 0.0, 0.0, 1.0))


if __name__ == '__main__':
    run_example(SamplerBasicExample)
