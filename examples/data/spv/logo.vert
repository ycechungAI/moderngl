#version 450 core

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

layout (location = 0) out vec3 v_color;

void main() {
    v_color = color[gl_VertexIndex];
    float r = (gl_InstanceIndex - 4) * 0.15;
    mat2 rot = mat2(cos(r), sin(r), -sin(r), cos(r));
    gl_Position = vec4(rot * vertex[gl_VertexIndex] * 0.95, 0.0, 1.0);
}
