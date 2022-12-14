import moderngl
from PIL import Image

ctx = moderngl.create_standalone_context()

fbo1 = ctx.framebuffer([ctx.renderbuffer((800, 800), components=4, samples=16)])
fbo2 = ctx.framebuffer([ctx.renderbuffer((800, 800), components=4)])

prog = ctx.program(
    vertex_shader='''
        #version 330 core

        vec2 vertex[3] = vec2[](
            vec2(1.0, 0.0),
            vec2(-0.5, -0.86),
            vec2(-0.5, 0.86)
        );

        vec3 color[3] = vec3[](
            vec3(0.0, 0.0, 1.0),
            vec3(0.0, 1.0, 0.0),
            vec3(1.0, 0.0, 0.0)
        );

        out vec3 v_color;

        void main() {
            v_color = color[gl_VertexID];
            float r = (gl_InstanceID - 4) * 0.15;
            mat2 rot = mat2(cos(r), sin(r), -sin(r), cos(r));
            gl_Position = vec4(rot * vertex[gl_VertexID] * 0.95, 0.0, 1.0);
        }
    ''',
    fragment_shader='''
        #version 330 core
        in vec3 v_color;
        out vec4 f_color;
        void main() {
            f_color = vec4(pow(v_color, vec3(1.0 / 2.2)), 1.0);
        }
    ''',
)

vao = ctx.vertex_array(prog, [])
vao.scope = ctx.scope(fbo1)
vao.instances = 9
vao.vertices = 3

fbo1.clear(0.0, 0.0, 0.0, 0.0)
vao.render()

ctx.copy_framebuffer(fbo2, fbo1)

img = Image.frombuffer('RGBA', fbo2.size, fbo2.read(components=4)).transpose(Image.Transpose.FLIP_TOP_BOTTOM)
img.save('logo.png')
