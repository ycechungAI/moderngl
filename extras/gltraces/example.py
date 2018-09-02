
import struct

import numpy as np

import gltraces
import moderngl

ctx = moderngl.create_context(standalone=True, debug=True)

mglprocs = moderngl.glprocs(ctx)
gltraces.glprocs[:] = mglprocs
mglprocs[:] = gltraces.gltraces

buf = ctx.buffer(reserve=1024)
