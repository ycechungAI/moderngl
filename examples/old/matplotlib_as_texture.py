import io

import numpy as np
from PIL import Image

from basic_colors_and_texture import ColorsAndTexture

import matplotlib
matplotlib.use('svg')
import matplotlib.pyplot as plt


class MatplotlibTexture(ColorsAndTexture):
    title = "Matplotlib as Texture"

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        figure_size = (640, 360)

        temp = io.BytesIO()
        plt.figure(0, figsize=(figure_size[0] / 72, figure_size[1] / 72))

        mu, sigma = 100, 15
        x = mu + sigma * np.random.randn(10000)
        n, bins, patches = plt.hist(x, 50, density=True, facecolor='r', alpha=0.75)

        plt.axis([40, 160, 0, 0.03])
        plt.grid(True)
        plt.show()

        plt.savefig(temp, format='raw', dpi=72)
        temp.seek(0)

        img = Image.frombytes('RGBA', figure_size, temp.read()).transpose(Image.FLIP_TOP_BOTTOM).convert('RGB')
        self.texture = self.ctx.texture(img.size, 3, img.tobytes())
        self.texture.build_mipmaps()


if __name__ == '__main__':
    MatplotlibTexture.run()
