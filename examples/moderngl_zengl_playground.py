# forked from 09_models_and_images.py

import moderngl
import zengl_playground
from objloader import Obj
from PIL import Image


class ImageTexture:
    def __init__(self, path):
        self.ctx = moderngl.get_context()

        img = Image.open(path).convert('RGBA')
        self.texture = self.ctx.texture(img.size, 4, img.tobytes())
        self.sampler = self.ctx.sampler(texture=self.texture)

    def use(self):
        self.sampler.use()


class ModelGeometry:
    def __init__(self, path):
        self.ctx = moderngl.get_context()

        obj = Obj.open(path)
        self.vbo = self.ctx.buffer(obj.pack('vx vy vz nx ny nz tx ty'))

    def vertex_array(self, program):
        return self.ctx.vertex_array(program, [(self.vbo, '3f 12x 2f', 'in_vertex', 'in_uv')])


class Mesh:
    def __init__(self, program, geometry, texture=None):
        self.ctx = moderngl.get_context()
        self.vao = geometry.vertex_array(program)
        self.texture = texture

    def render(self, position, color, scale):
        self.vao.program['use_texture'] = False

        if self.texture:
            self.vao.program['use_texture'] = True
            self.texture.use()

        self.vao.program['position'] = position
        self.vao.program['color'] = color
        self.vao.program['scale'] = scale
        self.vao.render()


class Example:
    def __init__(self):
        self.ctx = moderngl.get_context()

        app = zengl_playground.get_app()
        app.setup_moderngl(self.ctx)

        self.program = self.ctx.program(
            vertex_shader='''
                #version 330 core

                #include "main_uniform_buffer"

                uniform vec3 position;
                uniform float scale;

                layout (location = 0) in vec3 in_vertex;
                layout (location = 1) in vec3 in_normal;
                layout (location = 2) in vec2 in_uv;

                out vec3 v_vertex;
                out vec3 v_normal;
                out vec2 v_uv;

                void main() {
                    v_vertex = position + in_vertex * scale;
                    v_normal = in_normal;
                    v_uv = in_uv;

                    gl_Position = camera_matrix * vec4(v_vertex, 1.0);
                }
            ''',
            fragment_shader='''
                #version 330 core

                uniform sampler2D Texture;
                uniform bool use_texture;
                uniform vec3 color;

                in vec3 v_vertex;
                in vec3 v_normal;
                in vec2 v_uv;

                layout (location = 0) out vec4 out_color;

                void main() {
                    out_color = vec4(color, 1.0);
                    if (use_texture) {
                        out_color *= texture(Texture, v_uv);
                    }
                }
            ''',
        )

        self.texture = ImageTexture('examples/data/textures/crate.png')

        self.car_geometry = ModelGeometry('examples/data/models/lowpoly_toy_car.obj')
        self.car = Mesh(self.program, self.car_geometry)

        self.crate_geometry = ModelGeometry('examples/data/models/crate.obj')
        self.crate = Mesh(self.program, self.crate_geometry, self.texture)

    def render(self):
        self.ctx.screen.use()
        self.ctx.enable(self.ctx.DEPTH_TEST)
        self.car.render((-0.4, 0.0, 0.0), (1.0, 0.0, 0.0), 0.2)
        self.crate.render((0.0, 0.0, 0.0), (1.0, 1.0, 1.0), 0.2)
        self.car.render((0.4, 0.0, 0.0), (0.0, 0.0, 1.0), 0.2)


if __name__ == '__main__':
    zengl_playground.run(Example)
