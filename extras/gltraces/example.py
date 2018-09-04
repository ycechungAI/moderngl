import struct

import numpy as np

import gltraces
import moderngl

ctx = moderngl.create_context(standalone=True, debug=True)

mglprocs = moderngl.glprocs(ctx)
gltraces.glprocs[:] = mglprocs
mglprocs[:] = gltraces.gltraces

prog = ctx.program(
    vertex_shader='''
        #version 130

        in vec2 in_vert;
        out vec2 v_vert;

        void main() {
            v_vert = in_vert;
            gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
        }
    ''',
    varyings=['v_vert'],
)

buf = ctx.buffer(reserve=16)
buf.clear()
buf.write(b'1234')
print(buf.read())
