import os

from PIL import Image

import moderngl.next as mgl
from sampler_basic_example import SamplerBasicExample
from window import run_example


class ClampToEdgeAndBorder(SamplerBasicExample):
    gl_version = (3, 3)
    aspect_ratio = 1.0
    title = "Clamp to Edge and Border"

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        # self.sampler.border = 0.3
        # self.sampler.border = (0.0, 0.5, 1.0, 1.0)
        self.sampler.border = (0.0, 0.5, 1.0)

        self.sampler.wrap = mgl.CLAMP_TO_EDGE_X | mgl.CLAMP_TO_BORDER_Y


if __name__ == '__main__':
    run_example(ClampToEdgeAndBorder)
