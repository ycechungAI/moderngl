import array
import time

import layered_window
import moderngl

mem = layered_window.init((400, 400), title='Animation')
# mem = layered_window.init((400, 400), title='Animation', tool_window=True, always_on_top=True)

ctx = moderngl.create_context(standalone=True)

image = ctx.texture((400, 400), components=4)
framebuffer = ctx.framebuffer(color_attachments=[image])

triangle = array.array('f', [
    1.0, 0.0, 1.0, 0.0, 0.0, 0.5,
    -0.5, 0.86, 0.0, 1.0, 0.0, 0.5,
    -0.5, -0.86, 0.0, 0.0, 1.0, 0.5,
])

vertex_buffer = ctx.buffer(triangle)

program = ctx.program(
    vertex_shader='''
        #version 330 core

        uniform vec2 scale;
        uniform float rotation;

        layout (location = 0) in vec2 in_vertex;
        layout (location = 1) in vec4 in_color;

        out vec4 v_color;

        void main() {
            float r = rotation * (0.5 + float(gl_InstanceID) * 0.05);
            mat2 rot = mat2(cos(r), sin(r), -sin(r), cos(r));
            gl_Position = vec4((rot * in_vertex) * scale, 0.0, 1.0);
            v_color = in_color;
        }
    ''',
    fragment_shader='''
        #version 330 core

        in vec4 v_color;

        layout (location = 0) out vec4 out_color;

        void main() {
            out_color = vec4(v_color);
            out_color.rgb = pow(out_color.rgb, vec3(1.0 / 2.2));
        }
    ''',
)

ctx.enable_only(moderngl.BLEND)
ctx.blend_func = (ctx.SRC_ALPHA, ctx.ONE_MINUS_SRC_ALPHA, ctx.SRC_ALPHA, ctx.ONE)
vao = ctx.vertex_array(program, [
    (vertex_buffer, '2f 4f', 'in_vertex', 'in_color'),
])
vao.vertices = 3
vao.instances = 10

start_time = time.perf_counter()

while True:
    now = time.perf_counter() - start_time
    framebuffer.use()
    framebuffer.clear(0.0, 0.0, 0.0, 0.0)
    program['scale'].value = (0.8, 0.8)
    program['rotation'].value = now
    vao.render()
    framebuffer.read_into(mem, components=4)
    layered_window.update()
