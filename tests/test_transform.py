import struct
from array import array
import unittest
import numpy as np

import moderngl

from common import get_context


class TestCase(unittest.TestCase):
    """Test all combinations of transforms"""

    @classmethod
    def setUpClass(cls):
        cls.ctx = get_context()

    def test_transform(self):

        program = self.ctx.program(
            vertex_shader='''
                #version 330

                in vec4 vert;
                out float vert_length;

                void main() {
                    vert_length = length(vert);
                }
            ''',
            varyings=['vert_length']
        )

        self.assertIs(program.geometry_input, None)
        self.assertIs(program.geometry_output, None)

        self.assertIn('vert', program)
        self.assertIn('vert_length', program)

    def test_geometry_points(self):
        """Transform with points"""
        data = (0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0)
        buffer = self.ctx.buffer(array('f', data))
        program = self.ctx.program(
            vertex_shader="""
            #version 330

            in vec2 in_pos;
            out vec2 out_pos;

            void main() {
                out_pos = in_pos;
            }
            """,
            varyings=["out_pos"],
        )

        vao = self.ctx.vertex_array(program, [(buffer, "2f", "in_pos"),])
        buffer2 = self.ctx.buffer(reserve=buffer.size)
        vao.transform(buffer2, mode=self.ctx.POINTS)
        self.assertEqual(data, struct.unpack(f"{len(data)}f", buffer.read()))

    def test_geometry_lines(self):
        data = (
            0.0, 0.0,
            1.0, 1.0,
            2.0, 2.0,
            3.0, 3.0,
        )
        buffer = self.ctx.buffer(array('f', data))
        program = self.ctx.program(
            vertex_shader="""
            #version 330

            in vec2 in_pos;
            out vec2 out_pos;

            void main() {
                out_pos = in_pos * 2;
            }
            """,
            varyings=["out_pos"],
        )

        vao = self.ctx.vertex_array(program, [(buffer, "2f", "in_pos"),])
        buffer2 = self.ctx.buffer(reserve=buffer.size)
        vao.transform(buffer2, mode=self.ctx.LINES)
        self.assertEqual(tuple(v * 2 for v in data), struct.unpack(f"{len(data)}f", buffer2.read()))

    def test_geometry_lines_adjacency(self):
        data = (
            1.0, 1.0,
            2.0, 2.0,
            3.0, 3.0,
            4.0, 4.0,
        )
        buffer = self.ctx.buffer(array('f', data))
        program = self.ctx.program(
            vertex_shader="""
            #version 330

            in vec2 in_pos;
            out vec2 out_pos;

            void main() {
                out_pos = in_pos;
            }
            """,
            varyings=["out_pos"],
        )
        vao = self.ctx.vertex_array(program, [(buffer, "2f", "in_pos"),])
        buffer2 = self.ctx.buffer(reserve=4 * 4)
        vao.transform(buffer2, mode=self.ctx.LINES_ADJACENCY)

        # Start end end line is removed
        self.assertEqual(tuple(data[2:6]), struct.unpack(f"4f", buffer2.read()))

    def test_geometry_line_strip(self):
        pass

    def test_geometry_line_strip_adjacency(self):
        pass

    def test_geometry_triangles(self):
        pass

    def test_geometry_triangles_adjacency(self):
        pass

    def test_geometry_triangle_strip(self):
        pass

    def test_geometry_triangle_adjacency(self):
        pass

    def test_geometry_triangle_fan(self):
        vertices = np.array(
            [
                0.0, 0.0,  # Initial triangle
                1.0, 0.0,
                1.0, 1.0,
                0.0, 1.0,  # second triangle 
            ],
            dtype='f4',
        )

        prog = self.ctx.program(vertex_shader=
            """
            #version 130
            in vec2 in_vert;
            out vec2 out_vert;
            void main(void) {
                out_vert = in_vert;
            }
            """,
            varyings=['out_vert'],
        )

        vbo = self.ctx.buffer(vertices)
        vao = self.ctx.vertex_array(prog, [(vbo, '2f', 'in_vert')])
        N = 6  # 6 vertices in end buffer
        buffer = self.ctx.buffer(reserve=N * 4 * 2)
        vao.transform(buffer, vertices=4, mode=self.ctx.TRIANGLE_FAN)

        data = struct.unpack('{}f'.format(N * 2), buffer.read())
        self.assertEqual(data,
            (
                0.0, 0.0,  # Initial triangle
                1.0, 0.0,
                1.0, 1.0,
                0.0, 0.0,  # second triangle 
                1.0, 1.0, 
                0.0, 1.0,
            )
        )

    def gl_error(self, raise_exception=True):
        error = self.ctx.error
        if error != "GL_NO_ERROR" and raise_exception:
            raise ValueError(error)


if __name__ == '__main__':
    unittest.main()
