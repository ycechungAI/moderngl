
'''
Example of using shader storage buffer in compute shader.
We read from a buffer and write the result to another buffer.
Every frame we swap the buffers around transforming positions
of balls.

Buffer.bind_to_storage_buffer is used to bind a buffer as storage buffer
to a specific binding point specified in the compute program.

In addition we render the balls using a geometry shader to easily
batch draw them all in one render call.

author: minu jeong
modified by: einarf
'''
import math
import random
import numpy as np
from ported._example import Example


items_vertex_shader_code = """
    #version 430

    in vec4 in_vert;
    in vec4 in_col;

    out vec3 v_color;

    void main()
    {
        gl_Position = vec4(in_vert.xyzw); // x, y, 0, radius
        v_color = in_col.xyz;
    }
    """

# Geometry shader turning the points into triangle strips.
# This can also be done with point sprites.
items_geo_shader = """
    #version 330

    layout(points) in;
    layout(triangle_strip, max_vertices=4) out;

    in vec3 v_color[];
    out vec2 uv;
    out vec3 color;

    void main() {
        float radius = gl_in[0].gl_Position.w;
        vec2 pos = gl_in[0].gl_Position.xy;

        // Emit the triangle strip creating a "quad"
        // Lower left
        gl_Position = vec4(pos + vec2(-radius, -radius), 0, 1);
        color = v_color[0];
        uv = vec2(0, 0);
        EmitVertex();

        // upper left
        gl_Position = vec4(pos + vec2(-radius, radius), 0, 1);
        color = v_color[0];
        uv = vec2(0, 1);
        EmitVertex();

        // lower right
        gl_Position = vec4(pos + vec2(radius, -radius), 0, 1);
        color = v_color[0];
        uv = vec2(1, 0);
        EmitVertex();

        // upper right
        gl_Position = vec4(pos + vec2(radius, radius), 0, 1);
        color = v_color[0];
        uv = vec2(1, 1);
        EmitVertex();

        EndPrimitive();
    }
"""

items_fragment_shader_code = """
    #version 430

    in vec2 uv;
    in vec3 color;
    out vec4 out_color;
    void main()
    {
        if (length(vec2(0.5, 0.5) - uv.xy) > 0.25)
        {
            discard;
        }

        vec3 c = color;
        c.xy += uv.xy * 0.05;
        // c.xy += v_pos.xy * 0.75;
        out_color = vec4(c, 1.0);
    }
"""

# calc position with compute shader
compute_worker_shader_code = """
#version 430
#define GROUP_SIZE %COMPUTE_SIZE%

layout(local_size_x=GROUP_SIZE) in;

// All values are vec4s because of block alignment rules (keep it simple)
struct Ball
{
    vec4 pos; // x, y, 0, radius
    vec4 vel; // x, y (velocity)
    vec4 col; // r, g, b (color)
};

layout(std430, binding=0) buffer balls_in
{
    Ball balls[];
} In;
layout(std430, binding=1) buffer balls_out
{
    Ball balls[];
} Out;

void main()
{
    int x = int(gl_GlobalInvocationID);

    Ball in_ball = In.balls[x];

    vec4 p = in_ball.pos.xyzw;
    vec4 v = in_ball.vel.xyzw;

    p.xy += v.xy;

    float rad = p.w * 0.5;
    if (p.x - rad <= -1.0)
    {
        p.x = -1.0 + rad;
        v.x *= -0.98;
    }
    else if (p.x + rad >= 1.0)
    {
        p.x = 1.0 - rad;
        v.x *= -0.98;
    }

    if (p.y - rad <= -1.0)
    {
        p.y = -1.0 + rad;
        v.y *= -0.98;
    }
    else if (p.y + rad >= 1.0)
    {
        p.y = 1.0 - rad;
        v.y *= -0.98;
    }
    v.y += -0.001;

    Ball out_ball;
    out_ball.pos.xyzw = p.xyzw;
    out_ball.vel.xyzw = v.xyzw;

    vec4 c = in_ball.col.xyzw;
    out_ball.col.xyzw = c.xyzw;

    Out.balls[x] = out_ball;
}
"""

class ComputeShaderSSBO(Example):
    title = "Compute Shader SSBO"
    gl_version = 4, 3
    window_size = 600, 600
    aspect_ratio = 1.0

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.COUNT = 256  # number of balls
        self.STRUCT_SIZE = 12  # number of floats

        # Program for drawing the balls / items
        self.program = self.ctx.program(
            vertex_shader=items_vertex_shader_code,
            geometry_shader=items_geo_shader,
            fragment_shader=items_fragment_shader_code
        )

        # Load compute shader
        compute_shader_code_parsed = compute_worker_shader_code.replace("%COMPUTE_SIZE%", str(self.COUNT))
        self.compute_shader = self.ctx.compute_shader(compute_shader_code_parsed)

        # Generate initial data
        compute_data = []
        for i in range(self.COUNT):
            _angle = (i / self.COUNT) * math.pi * 2.0
            _dist = 0.125
            radius = random.random() * 0.02 + 0.02
            x = math.cos(_angle) * _dist
            y = math.sin(_angle) * _dist
            z = 0.0
            w = radius
            _v = random.random() * 0.005 + 0.01
            vx = math.cos(_angle) * _v
            vy = math.sin(_angle) * _v
            vz = 0.0
            vw = 0.0
            r = 1.0 * random.random()
            g = 1.0 * random.random()
            b = 1.0 * random.random()
            a = 1.0

            compute_data.append(
                [[x, y, z, w], [vx, vy, vz, vw], [r, g, b, a]])

        compute_data = np.array(compute_data, dtype="f4")
        self.compute_buffer_a = self.ctx.buffer(compute_data)
        self.compute_buffer_b = self.ctx.buffer(compute_data)
        self.target_buffer = self.compute_buffer_b

        # Prepare vertex arrays to drawing balls using the compute shader buffers are input
        # We use 4x4 (padding format) to skip the velocity date (not needed for drawing the balls)
        self.balls_a = self.ctx.vertex_array(
            self.program, [(self.compute_buffer_a, '4f 4x4 4f', 'in_vert', 'in_col')],
        )
        self.balls_b = self.ctx.vertex_array(
            self.program, [(self.compute_buffer_b, '4f 4x4 4f', 'in_vert', 'in_col')],
        )

    def render(self, time, frame_time):
        # Calculate the next position of the balls with compute shader
        self.compute_buffer_a.bind_to_storage_buffer(0)
        self.compute_buffer_b.bind_to_storage_buffer(1)
        self.compute_shader.run(group_x=self.STRUCT_SIZE)

        # Batch draw the balls
        self.balls_b.render(mode=self.ctx.POINTS)

        # Swap the buffers and vertex arrays around for next frame
        self.compute_buffer_a, self.compute_buffer_b = self.compute_buffer_b, self.compute_buffer_a
        self.balls_a, self.balls_b = self.balls_b, self.balls_a


if __name__ == "__main__":
    ComputeShaderSSBO.run()
