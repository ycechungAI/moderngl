'''
    Renders 2 triangles that share the same head using index buffers (ibo)
'''

import moderngl.next as mgl
import numpy as np

from example_window import Example, run_example


class IndexBuffer(Example):
    def __init__(self):
        self.ctx = mgl.create_context()

        self.prog = self.ctx.program(
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

        # 2 triangles sharing the head vertex (0,0)
        vertices = np.array([
            0.0, 0.0,

            -0.6, -0.8,
            0.6, -0.8,

            0.6, 0.8,
            -0.6, 0.8,
        ])

        # Indecies are given to specify the order of drawing
        indecies = np.array([0, 1, 2, 0, 3, 4])

        self.vbo = self.ctx.buffer(vertices.astype('f4').tobytes())
        self.ibo = self.ctx.buffer(indecies.astype('i4').tobytes())

        vao_content = [
            # 2 floats are assigned to the 'in' variable named 'in_vert' in the shader code
            (self.vbo, '2f', 'in_vert')
        ]

        self.vao = self.ctx.vertex_array(self.prog, vao_content, self.ibo)

    def render(self):
        self.ctx.screen.viewport = self.wnd.viewport
        self.ctx.clear(1.0, 1.0, 1.0)
        self.vao.render()


run_example(IndexBuffer)
