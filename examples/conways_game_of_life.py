"""
A Game of Life implementation using transform feedback.

We calculate the next state of the map with transform()
meaning a vertex shader will geneate the new state into a buffer.
This buffer is then written into the texture we display.
This is a fast vram to vram copy.

Comments:

Another way to do this is simply rendering to Framebuffers.
We could also drop the grid and modulo gl_VertexID tranforming
with an empty vertex array.
"""
import numpy as np

import moderngl
from ported._example import Example


class Conway(Example):
    title = "Conway's Game of Life"
    window_size = (640, 640)
    aspect_ratio = 1.0

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        width, height = self.window_size
        # vertex data for rendering the texture
        canvas = np.array([0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0]).astype('f4')
        # Initial state of the map (random)
        pixels = np.round(np.random.rand(width, height)).astype('f4')
        # Generate a buffer with all the pixel positions in the map
        grid = np.dstack(np.mgrid[0:height, 0:width][::-1]).astype('i4')

        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                in vec2 in_vert;
                out vec2 v_text;

                void main() {
                    v_text = in_vert;
                    gl_Position = vec4(in_vert * 2.0 - 1.0, 0.0, 1.0);
                }
            ''',
            fragment_shader='''
                #version 330

                uniform sampler2D Texture;

                in vec2 v_text;
                out vec4 f_color;

                void main() {
                    f_color = texture(Texture, v_text);
                }
            ''',
        )

        self.transform = self.ctx.program(
            vertex_shader='''
                #version 330

                uniform sampler2D Texture;
                uniform int Width;
                uniform int Height;

                in ivec2 in_text;
                out float out_vert;

                #define LIVING 0.0
                #define DEAD 1.0

                bool cell(int x, int y) {
                    return texelFetch(Texture, ivec2((x + Width) % Width, (y + Height) % Height), 0).r < 0.5;
                }

                void main() {
                    bool living = cell(in_text.x, in_text.y);

                    int neighbours = 0;
                    if (cell(in_text.x - 1, in_text.y - 1)) neighbours++;
                    if (cell(in_text.x - 1, in_text.y + 0)) neighbours++;
                    if (cell(in_text.x - 1, in_text.y + 1)) neighbours++;
                    if (cell(in_text.x + 1, in_text.y - 1)) neighbours++;
                    if (cell(in_text.x + 1, in_text.y + 0)) neighbours++;
                    if (cell(in_text.x + 1, in_text.y + 1)) neighbours++;
                    if (cell(in_text.x + 0, in_text.y + 1)) neighbours++;
                    if (cell(in_text.x + 0, in_text.y - 1)) neighbours++;

                    if (living) {
                        out_vert = (neighbours == 2 || neighbours == 3) ? LIVING : DEAD;
                    } else {
                        out_vert = (neighbours == 3) ? LIVING : DEAD;
                    }
                }
            ''',
            varyings=['out_vert']
        )

        self.transform['Width'].value = width
        self.transform['Height'].value = height

        self.texture = self.ctx.texture((width, height), 1, pixels.tobytes(), dtype='f4')
        self.texture.filter = (moderngl.NEAREST, moderngl.NEAREST)
        self.texture.swizzle = 'RRR1'  # What components texelFetch will get from the texture (in shader)

        # A quad covering the screen (scaled in vertex shader)
        self.vbo = self.ctx.buffer(canvas)
        self.vao = self.ctx.simple_vertex_array(self.prog, self.vbo, 'in_vert')

        # Transform vertex array to generate new map state
        self.text = self.ctx.buffer(grid)
        self.tao = self.ctx.simple_vertex_array(self.transform, self.text, 'in_text')
        self.pbo = self.ctx.buffer(reserve=pixels.nbytes)  # buffer to store the result

    def render(self, time, frame_time):
        self.ctx.clear(1.0, 1.0, 1.0)
        self.texture.use()

        # Generate the new map and write that to the pbo buffer
        self.tao.transform(self.pbo)
        # Copy the pbo into the texture
        self.texture.write(self.pbo)

        # Render the texture
        self.vao.render(moderngl.TRIANGLE_STRIP)


if __name__ == '__main__':
    Conway.run()
