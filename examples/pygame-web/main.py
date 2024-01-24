import moderngl
import pygame

pygame.display.set_mode((600, 600), pygame.OPENGL)

ctx = moderngl.get_context()
prog = ctx.program(
    vertex_shader='''
        #version 300 es
        precision highp float;
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
            float r = float(gl_InstanceID - 4) * 0.15;
            mat2 rot = mat2(cos(r), sin(r), -sin(r), cos(r));
            gl_Position = vec4(rot * vertex[gl_VertexID] * 0.95, 0.0, 1.0);
        }
    ''',
    fragment_shader='''
        #version 300 es
        precision highp float;
        in vec3 v_color;
        out vec4 f_color;
        void main() {
            f_color = vec4(pow(v_color, vec3(1.0 / 2.2)), 1.0);
        }
    ''',
)

vao = ctx.vertex_array(prog, [])
vao.instances = 9
vao.vertices = 3
ctx.clear()
vao.render()
