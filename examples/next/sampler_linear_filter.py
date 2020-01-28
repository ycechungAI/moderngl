import os

from PIL import Image

import moderngl.next as mgl
from sampler_basic_example import SamplerBasicExample
from window import run_example


class SamplerLinearFilter(SamplerBasicExample):
    gl_version = (3, 3)
    aspect_ratio = 1.0
    title = "Sampler Linear Filter"

    img = Image.frombytes('RGB', (8, 8), os.urandom(192))

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        # self.sampler.filter = (mgl.LINEAR, mgl.LINEAR)
        self.sampler.filter = mgl.LINEAR

        self.sampler.wrap = mgl.REPEAT_X | mgl.REPEAT_Y


if __name__ == '__main__':
    run_example(SamplerLinearFilter)
