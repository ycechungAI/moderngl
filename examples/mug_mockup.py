import numpy as np
from pyrr import Matrix44

import moderngl
from moderngl_window.geometry.attributes import AttributeNames
from moderngl_window import geometry

from _example import Example


class MugExample(Example):
    title = "Mug"
    gl_version = (3, 3)

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        # Create a custom attribute name spec
        # so attribute names are not forced to follow gltf standard
        attr_names = AttributeNames(position='in_vert', texcoord_0='in_tex', normal='in_norm')

        # Programs
        self.canvas_prog = self.load_program('mug_mockup/programs/canvas.glsl')
        self.sticker_prog = self.load_program('mug_mockup/programs/sticker.glsl')
        self.mug_prog = self.load_program('mug_mockup/programs/mug.glsl')

        # textures
        self.bg_texture = self.load_texture_2d('mug_mockup/textures/mug-background.jpg')
        self.sticker_texture = self.load_texture_2d('mug_mockup/textures/mug-pymet-logo.png')

        self.canvas_vao = geometry.quad_fs(attr_names=attr_names).instance(self.canvas_prog)

        obj = self.load_scene('mug_mockup/scenes/mug.obj', attr_names=attr_names)
        self.mug_vao = obj.root_nodes[0].mesh.vao.instance(self.mug_prog)

        # Create sticker geometry
        segs = 32
        radius = 29.94
        bottom = 6.601
        top = 57.856
        left = -163.12 * np.pi / 180.0
        right = 11.25 * np.pi / 180.0

        lin = np.linspace(left, right, segs)
        sticker_vertices = np.array([
            np.repeat(np.cos(lin) * radius, 2),
            np.repeat(np.sin(lin) * radius, 2),
            np.tile([bottom, top], segs),
            np.repeat(np.cos(lin), 2),
            np.repeat(np.sin(lin), 2),
            np.tile([0.0, 0.0], segs),
            np.repeat(np.linspace(0.0, 1.0, segs), 2),
            np.tile([0.0, 1.0], segs),
        ])
        self.sticker_vbo = self.ctx.buffer(sticker_vertices.T.astype('f4').tobytes())
        self.sticker_vao = self.ctx.simple_vertex_array(self.sticker_prog, self.sticker_vbo, 'in_vert', 'in_norm', 'in_text')

        # Pre-fill uniforms. These currently do not change during rendering
        proj = Matrix44.perspective_projection(30.0, self.aspect_ratio, 1.0, 1000.0)
        lookat = Matrix44.look_at(
            (46.748, -280.619, 154.391),
            (-23.844, 2.698, 44.493),
            (0.0, 0.0, 1.0),
        )
        mvp = (proj * lookat).astype('f4')
        light = (-143.438, -159.072, 213.268)
        self.mug_prog['Mvp'].write(mvp)
        self.mug_prog['Light'].value = light
        self.sticker_prog['Mvp'].write(mvp)
        self.sticker_prog['Light'].value = light

    def render(self, time, frame_time):
        self.ctx.clear(1.0, 1.0, 1.0)

        # background
        self.ctx.enable_only(moderngl.BLEND)
        self.bg_texture.use()
        self.canvas_vao.render(moderngl.TRIANGLE_STRIP)

        # mug
        self.ctx.enable_only(moderngl.DEPTH_TEST)
        self.mug_vao.render()

        # sticker
        self.ctx.enable_only(moderngl.DEPTH_TEST | moderngl.BLEND)
        self.sticker_texture.use()
        self.sticker_vao.render(moderngl.TRIANGLE_STRIP)


if __name__ == '__main__':
    MugExample.run()
