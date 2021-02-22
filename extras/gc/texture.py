"""
* Adding color texture attachment to framebuffer increases refcount

"""
import time
import gc
from ctypes import c_long

import numpy as np
import moderngl
import utils

ctx = moderngl.create_context(standalone=True)
ctx.gc_mode = "auto"
# ctx.gc_mode = None

while True:
    try:
        texture = ctx.texture((1000, 1000), 4, data=np.random.randint(0, 255, 1000 * 1000 * 4, dtype="u1"))
        fbo = ctx.framebuffer(color_attachments=[texture])
        # print("texture refcount", c_long.from_address(id(texture)).value)
        # print("texture.mglo refcount", c_long.from_address(id(texture.mglo)).value)
        time.sleep(0.01)
        print("----------------------")
        # utils.print_referrers(texture)
        print(ctx.error)
    except KeyboardInterrupt:
        # print(len(gc.get_objects()))
        # print(gc.get_stats())
        # print(gc.get_referents(texture.mglo))
        break


