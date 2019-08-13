import moderngl

import _example


class Example(_example.Example):
    title = 'Geometry Generation'

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330
                void main() {
                }
            ''',
            geometry_shader='''
                #version 330
                layout (points) in;
                layout (triangle_strip, max_vertices = 3) out;

                void main() {
                    float x = float(gl_PrimitiveIDIn / 10) / 9 - 0.5;
                    float y = float(gl_PrimitiveIDIn % 10) / 9 - 0.5;
                    gl_Position = vec4(x - 0.03, y - 0.03, 0.0, 1.0);
                    EmitVertex();
                    gl_Position = vec4(x + 0.03, y - 0.03, 0.0, 1.0);
                    EmitVertex();
                    gl_Position = vec4(x, y + 0.03, 0.0, 1.0);
                    EmitVertex();
                    EndPrimitive();
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

        self.vao = self.ctx.vertex_array(self.prog)
        self.vao.mode = self.ctx.POINTS
        self.vao.vertices = 100
        self.vao.instances = 1

    def render(self, time, frame_time):
        self.ctx.screen.clear((1.0, 1.0, 1.0))
        self.vao.render()


if __name__ == '__main__':
    Example.run()
