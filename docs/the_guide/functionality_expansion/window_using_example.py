import moderngl_window as glw
import numpy as np

window_cls = glw.get_local_window_cls('pyglet')
window = window_cls(
    size=(512, 512), fullscreen=False, title='ModernGL Window',
    resizable=False, vsync=True, gl_version=(3, 3)
)
ctx = window.ctx
glw.activate_context(window, ctx=ctx)
window.clear()
window.swap_buffers()

prog = ctx.program(
    vertex_shader="""
        #version 330

        in vec2 in_vert;
        in vec3 in_color;

        out vec3 v_color;

        void main() {
            v_color = in_color;
            gl_Position = vec4(in_vert, 0.0, 1.0);
        }
    """,
    fragment_shader="""
        #version 330

        in vec3 v_color;

        out vec3 f_color;

        void main() {
            f_color = v_color;
        }
    """,
)

x = np.linspace(-1.0, 1.0, 50)
y = np.random.rand(50) - 0.5
r = np.zeros(50)
g = np.ones(50)
b = np.zeros(50)

vertices = np.dstack([x, y, r, g, b])

vbo = ctx.buffer(vertices.astype("f4").tobytes())
vao = ctx.vertex_array(prog, vbo, "in_vert", "in_color")

fbo = ctx.framebuffer(
    color_attachments=[ctx.texture((512, 512), 3)]
)


while not window.is_closing:
    fbo.use()
    fbo.clear(0.0, 0.0, 0.0, 1.0)
    vao.render(gl.LINE_STRIP)

    ctx.copy_framebuffer(window.fbo, fbo)

    window.swap_buffers()
