from PIL import Image

import moderngl.next as mgl
from sampler_basic_example import SamplerBasicExample
from window import run_example


class SamplerWrapModes(SamplerBasicExample):
    gl_version = (3, 3)
    aspect_ratio = 1.0
    title = "Sampler Wrap Modes"

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.sampler.wrap = mgl.REPEAT_X | mgl.MIRROR_CLAMP_TO_EDGE_Y


if __name__ == '__main__':
    run_example(SamplerWrapModes)
