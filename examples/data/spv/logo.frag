#version 450 core
layout (location = 0) in vec3 v_color;
layout (location = 0) out vec4 f_color;
void main() {
    f_color = vec4(pow(v_color, vec3(1.0 / 2.2)), 1.0);
}
