from tkinter import *

from PIL import Image, ImageTk

import moderngl

ctx = moderngl.context(standalone=True)
prog = ctx.program(
    vertex_shader='''
        #version 330

        in vec2 in_vert;

        void main() {
            gl_Position = vec4(in_vert, 0.0, 1.0);
        }
    ''',
    fragment_shader='''
        #version 330

        out vec4 f_color;

        void main() {
            f_color = vec4(0.3, 0.5, 1.0, 1.0);
        }
    ''',
)

vertex_data = moderngl.pack([
    0.0, 0.8,
    -0.6, -0.8,
    0.6, -0.8,
])

vbo = ctx.buffer(vertex_data)
vao = ctx.vertex_array(prog, [
    vbo.bind('in_vert'),
])

size = (1280, 720)
fbo = ctx.framebuffer(ctx.renderbuffer(size, 3))
fbo.clear((1.0, 1.0, 1.0))
vao.scope = ctx.scope(framebuffer=fbo)
vao.render()

class Window(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.master = master
        self.pack(fill=BOTH, expand=1)

        load = Image.frombytes('RGB', size, fbo.read(), 'raw', 'RGB', 0, -1)
        render = ImageTk.PhotoImage(load)
        img = Label(self, image=render)
        img.image = render
        img.place(x=0, y=0)

root = Tk()
app = Window(root)
root.wm_title("Tkinter window")
root.geometry("1280x720")
root.mainloop()
