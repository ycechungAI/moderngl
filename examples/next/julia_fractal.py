import moderngl.next as mgl

import numpy as np

from example_window import Example, run_example


class Fractal(Example):
    def __init__(self):
        self.ctx = mgl.create_context()

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                in vec2 in_vert;
                out vec2 v_text;

                void main() {
                    gl_Position = vec4(in_vert, 0.0, 1.0);
                    v_text = in_vert / 2.0 + 0.5;
                }
            ''',
            fragment_shader='''
                #version 330

                in vec2 v_text;
                out vec4 f_color;

                uniform vec2 Center;
                uniform int Iter;

                void main() {
                    vec2 z = vec2(5.0 * (v_text.x - 0.5), 3.0 * (v_text.y - 0.5));
                    vec2 c = Center;

                    int i;
                    for(i = 0; i < Iter; i++) {
                        vec2 v = vec2(
                            (z.x * z.x - z.y * z.y) + c.x,
                            (z.y * z.x + z.x * z.y) + c.y
                        );
                        if (dot(v, v) > 4.0) break;
                        z = v;
                    }

                    float cm = fract((i == Iter ? 0.0 : float(i)) * 10 / Iter);
                    f_color = vec4(
                        fract(cm + 0.0 / 3.0),
                        fract(cm + 1.0 / 3.0),
                        fract(cm + 2.0 / 3.0),
                        1.0
                    );
                }
            ''',
        )

        vertices = np.array([-1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0])

        self.vbo = self.ctx.buffer(vertices.astype('f4').tobytes())
        self.vao = self.ctx.simple_vertex_array(self.prog, self.vbo, 'in_vert')

    def render(self):
        self.ctx.screen.viewport = self.wnd.viewport
        self.ctx.clear(1.0, 1.0, 1.0)

        self.prog['Center'] = (0.49, 0.32)
        self.prog['Iter'] = 100

        self.vao.render(mgl.TRIANGLE_STRIP)


run_example(Fractal)
