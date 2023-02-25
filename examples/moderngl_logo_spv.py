import moderngl
from PIL import Image

ctx = moderngl.create_standalone_context()

fbo1 = ctx.framebuffer([ctx.renderbuffer((800, 800), components=4, samples=16)])
fbo2 = ctx.framebuffer([ctx.renderbuffer((800, 800), components=4)])

prog = ctx.program(
    vertex_shader=open('examples/data/logo.vert.spv', 'rb').read(),
    fragment_shader=open('examples/data/logo.frag.spv', 'rb').read(),
)

vao = ctx.vertex_array(prog, [])
vao.scope = ctx.scope(fbo1)
vao.instances = 9
vao.vertices = 3

fbo1.clear(0.0, 0.0, 0.0, 0.0)
vao.render()

ctx.copy_framebuffer(fbo2, fbo1)

img = Image.frombuffer('RGBA', fbo2.size, fbo2.read(components=4)).transpose(Image.Transpose.FLIP_TOP_BOTTOM)
img.show()
