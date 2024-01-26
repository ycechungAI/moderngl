import pygame
import moderngl

pygame.init()
pygame.display.set_mode((400, 400), flags=pygame.OPENGL | pygame.DOUBLEBUF)

# ctx = moderngl.create_context()
ctx = moderngl.get_context()

prog = ctx.program(
    vertex_shader='''
        #version 330 core
        uniform float time;
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
            float r = float(gl_InstanceID - 4) * 0.15 * (0.8 + sin(time) * 0.2);
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
vao.instances = 9
vao.vertices = 3

clock = pygame.Clock()

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            quit()

    prog["time"].value += 1.0 / 60.0
    ctx.clear()
    vao.render()

    pygame.display.flip()
    clock.tick(60)
