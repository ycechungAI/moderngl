import numpy as np


class SimpleObjectRenderer:
    def __init__(self, ctx):
        self.ctx = ctx
        self.prog = self.ctx.program(
            vertex_shader='''
                #version 330

                uniform mat4 Mvp;

                in vec3 in_vert;
                in vec3 in_norm;
                in vec2 in_text;

                out vec3 v_vert;
                out vec3 v_norm;
                out vec2 v_text;

                void main() {
                    gl_Position = Mvp * vec4(in_vert, 1.0);
                    v_vert = in_vert;
                    v_norm = in_norm;
                    v_text = in_text;
                }
            ''',
            fragment_shader='''
                #version 330

                uniform sampler2D Sampler;
                uniform vec3 Light;
                uniform vec4 Color;
                uniform float AmbientLight;
                uniform bool Texture;

                in vec3 v_vert;
                in vec3 v_norm;
                in vec2 v_text;

                out vec4 f_color;

                void main() {
                    float lum = clamp(dot(normalize(Light - v_vert), normalize(v_norm)), 0.0, 1.0) * (1.0 - AmbientLight) + AmbientLight;
                    vec4 color = Color;
                    if (Texture) {
                        color *= texture(Sampler, v_text);
                    }
                    color.rgb *= lum;
                    f_color = color;
                }
            ''',
        )

        self.vbo_vert = self.ctx.buffer(reserve='3MB')
        self.vbo_norm = self.ctx.buffer(reserve='3MB')
        self.vbo_text = self.ctx.buffer(reserve='2MB')
        self.vao2d = self.ctx.vertex_array(self.prog, [
            (self.vbo_vert, '2f', 'in_vert'),
            (self.vbo_norm, '3f', 'in_norm'),
            (self.vbo_text, '2f', 'in_text'),
        ])
        self.vao3d = self.ctx.vertex_array(self.prog, [
            (self.vbo_vert, '3f', 'in_vert'),
            (self.vbo_norm, '3f', 'in_norm'),
            (self.vbo_text, '2f', 'in_text'),
        ])

    def render(self, mode, vertices, normals=None, texcoords=None, light=(0.0, 0.0, 0.0), color=(1.0, 1.0, 1.0, 1.0), ambient_light=1.0, texture=True, mvp=None):
        if mvp is None:
            mvp = np.array([
                1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0,
            ], dtype='f4').tobytes()
        self.prog['Mvp'] = mvp
        self.prog['Texture'] = texture
        self.prog['AmbientLight'] = ambient_light
        self.prog['Light'] = light
        self.prog['Color'] = color
        assert len(vertices.shape) == 2 and vertices.shape[1] in (2, 3)
        assert normals is None or normals.shape == (len(vertices), 3)
        assert texcoords is None or texcoords.shape == (len(vertices), 2)
        if normals is None:
            normals = np.zeros((len(vertices), 3), 'f4')
        if texcoords is None:
            texcoords = np.zeros((len(vertices), 2), 'f4')
        self.vbo_vert.write(vertices.astype('f4').tobytes())
        self.vbo_norm.write(normals.astype('f4').tobytes())
        self.vbo_text.write(texcoords.astype('f4').tobytes())
        if vertices.shape[1] == 2:
            self.vao2d.render(mode, len(vertices))
        if vertices.shape[1] == 3:
            self.vao3d.render(mode, len(vertices))
