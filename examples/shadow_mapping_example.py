# Example of shadow rendering (shadow mapping)
# Description: This example shows how you can draw a shadow
#              from a single light source.
# Note: The left-hand rule coordinate system is used.

import moderngl as gl
import moderngl_window as glw
import struct
import math


# Create window
window_cls = glw.get_local_window_cls('pyglet')
window = window_cls(
    size=(1024, 512), fullscreen=False, title='Shadow rendering example',
    resizable=False, vsync=True, gl_version=(3, 3)
)
ctx = window.ctx
glw.activate_context(window, ctx=ctx)
window.clear()

# Window program
window_program = ctx.program(
    vertex_shader='''
    #version 330
    in vec3 in_vert;
    in vec4 in_color;

    uniform float camera_aspect_ratio;
    uniform vec3 camera_position;
    uniform mat3 camera_rotation_matrix;
    uniform mat4 camera_projection_matrix;

    out vec3 o_vert;
    out vec4 o_color;

    void main() {
        vec3 new_position = camera_rotation_matrix*(in_vert-camera_position);
        gl_Position = camera_projection_matrix*vec4(
            new_position.x, new_position.y*camera_aspect_ratio,
            new_position.z, 1.0
        );

        o_vert = new_position;
        o_color = in_color;
    }
    ''',
    fragment_shader='''
    #version 330

    in vec3 o_vert;
    in vec4 o_color;

    uniform float light_aspect_ratio;
    uniform vec2 light_clip_space;
    uniform float light_perspective;
    uniform vec3 light_position;
    uniform mat3 light_rotation_matrix;

    uniform vec3 camera_position;
    uniform mat3 camera_revers_rotation_matrix;

    uniform float shadow_bias;

    uniform sampler2D light_depthI;

    out vec4 f_color;

    void main() {
        f_color = o_color;

        // Camera pixels position //
        vec3 pixel_coords = o_vert;
        ////////////

        // ROTATION 3D PROG moving prog //
        // Convert to light pixel coords //
        pixel_coords = camera_revers_rotation_matrix*
              pixel_coords;
        pixel_coords += camera_position;
        vec3 pxc = pixel_coords;
        pixel_coords -= light_position;
        pixel_coords = light_rotation_matrix*
              pixel_coords;
        /////////////////

        // Convert .xy to 2D coords //
        mediump float dist = length(pixel_coords);
        pixel_coords.y*=light_aspect_ratio;
        pixel_coords.xy = vec2(pixel_coords.xy/
                (pixel_coords.z+(1.0-pixel_coords.z)*(1.0-light_perspective))+1.0)/2.0;
        ///////////


        // Get depth
        float depth = texture(light_depthI, pixel_coords.xy).x;
        depth = 2.0 * depth - 1.0;


        vec2 lcs = light_clip_space;

        depth = (
            (2.0*lcs.x*lcs.y/(lcs.y+lcs.x-depth*(lcs.y-lcs.x)))*
                light_perspective+
            ((depth+(lcs.y+lcs.x)/(lcs.y-lcs.x))*(lcs.y - lcs.x))/2*
                (1.0-light_perspective)
        );

        // Compare depth
        if (!(
             pixel_coords.x > 0.0 && pixel_coords.y > 0.0
            && pixel_coords.x < 1.0 && pixel_coords.y < 1.0
            && pixel_coords.z > 0.0
            && pixel_coords.z < depth+shadow_bias
        )) {
            f_color.xyz = f_color.xyz*0.3;
        }
    }
    '''
)

# Light program
light_program = ctx.program(
    vertex_shader='''
    #version 330
    in vec3 in_vert;
    in vec4 in_color;

    uniform float light_aspect_ratio;
    uniform vec3 light_position;
    uniform mat3 light_rotation_matrix;
    uniform mat4 light_projection_matrix;

    out vec4 o_color;

    void main() {
        vec3 new_position = light_rotation_matrix*(in_vert-light_position);
        gl_Position = light_projection_matrix*vec4(
            new_position.x, new_position.y*light_aspect_ratio,
            new_position.z, 1.0
        );

        o_color = in_color;
    }
    ''',
    fragment_shader='''
    #version 330

    in vec4 o_color;
    out vec4 f_color;

    void main() {
        f_color = o_color;
    }
    '''
)

# Preparing scene
vertex = [
    -.5, -.1, -.5,
    -.5, -.1, .5,
    .5, -.1, .5,

    .5, -.1, -.5,
    .5, -.1, .5,
    -.5, -.1, -.5,

    .2, -.1, -.2,
    .2, -.1, .2,
    -.2, .05, .2,
]
vertex_buf = ctx.buffer(struct.pack(f'{len(vertex)}f', *vertex))

colors = [
    .5, .5, .5, 1,
    .5, .5, .5, 1,
    .5, .5, .5, 1,

    .5, .5, .5, 1,
    .5, .5, .5, 1,
    .5, .5, .5, 1,

    1, 0, 0, 1,
    0, 1, 0, 1,
    0, 0, 1, 1,
]
colors_buf = ctx.buffer(struct.pack(f'{len(colors)}f', *colors))

# Prepair render
window_vao = ctx.vertex_array(window_program, [
    (vertex_buf, '3f', 'in_vert'),
    (colors_buf, '4f', 'in_color')
])
window_fbo = ctx.framebuffer(
    color_attachments=[ctx.texture(window.size, 4)],
    depth_attachment=ctx.depth_texture(window.size)
)
window_fbo.color_attachments[0].repeat_x = False
window_fbo.color_attachments[0].repeat_y = False
window_fbo.depth_attachment.repeat_x = False
window_fbo.depth_attachment.repeat_y = False
window_fbo.depth_attachment.compare_func = ''


light_vao = ctx.vertex_array(light_program, [
    (vertex_buf, '3f', 'in_vert'),
    (colors_buf, '4f', 'in_color')
])
light_fbo = ctx.framebuffer(
    color_attachments=[ctx.texture(window.size, 4)],
    depth_attachment=ctx.depth_texture(window.size)
)
light_fbo.color_attachments[0].repeat_x = False
light_fbo.color_attachments[0].repeat_y = False
light_fbo.depth_attachment.repeat_x = False
light_fbo.depth_attachment.repeat_y = False
light_fbo.depth_attachment.filter = gl.LINEAR, gl.LINEAR_MIPMAP_LINEAR
light_fbo.depth_attachment.compare_func = ''


# Define matrix methods
def get_projection_matrix(near, far, perspective=1):
    perspective_projection = (
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, (far+near)/(far-near), 1,
        0, 0, -(2*far*near)/(far-near), 0
    )
    orthographic_projection = (
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 2/(far-near), 0,
        0, 0, -(far+near)/(far-near), 1
    )

    result_projection = []
    for x, y in zip(perspective_projection, orthographic_projection):
        result_projection.append(x*perspective+y*(1-perspective))

    return result_projection


def get_rotation_matrix(rx, ry, rz, revers=True):
    rx *= (math.pi / 180)
    ry *= (math.pi / 180)
    rz *= (math.pi / 180)
    if not revers:
        srx, crx = math.sin(rx), math.cos(rx)
        sry, cry = math.sin(-ry), math.cos(-ry)
        srz, crz = math.sin(-rz), math.cos(-rz)
        rotation_matrix = (
            cry * crz - sry * srx * srz,
            cry * srz + sry * srx * crz,
            -sry * crx,

            -crx * srz,
            crx * crz,
            srx,

            sry * crz + cry * srx * srz,
            sry * srz - cry * srx * crz,
            cry * crx
        )
    else:
        srx, crx = math.sin(-rx), math.cos(-rx)
        sry, cry = math.sin(ry), math.cos(ry)
        srz, crz = math.sin(rz), math.cos(rz)
        rotation_matrix = (
            crz * cry + srz * srx * sry,
            srz * crx,
            srz * srx * cry - crz * sry,

            crz * srx * sry - srz * cry,
            crz * crx,
            srz * sry + crz * srx * cry,

            crx * sry,
            -srx,
            crx * cry
        )

    return rotation_matrix


#####################################################################
# Define constants
far = 10  # Camera projection far
near = 0.001  # Camera projection near
perpective = 1  # Select perpective/orthographic projection
position = [0., 0.1, -0.4]  # Camera position
rotate = [-20, 0, 0]  # Camera angle

light_far = 10  # Light projection far
light_near = 0.001  # Light projection near
light_perspective = 0  # Select perpective/orthographic projection
light_position = [0, 0.4, 0]  # Light position
light_rotate = [-90, 0, 0]  # Light angle

shadow_bias = 0.001  # Allowed shadow offset

view_as_light = False  # Show what the light sees
#####################################################################

# Filling uniforms
window_vao.program['camera_aspect_ratio'] = \
    window_fbo.size[0]/window_fbo.size[1]
window_vao.program['camera_position'] = position
window_vao.program['camera_projection_matrix'] = \
    get_projection_matrix(near, far, perpective)
window_vao.program['camera_rotation_matrix'] = \
    get_rotation_matrix(*rotate, revers=False)
window_vao.program['light_aspect_ratio'] = light_fbo.size[0]/light_fbo.size[1]
window_vao.program['light_clip_space'] = light_near, light_far
window_vao.program['light_perspective'] = light_perspective
window_vao.program['light_position'] = light_position
window_vao.program['light_rotation_matrix'] = \
    get_rotation_matrix(*light_rotate, revers=False)
window_vao.program['camera_revers_rotation_matrix'] = \
    get_rotation_matrix(*rotate, revers=True)

light_vao.program['light_aspect_ratio'] = light_fbo.size[0]/light_fbo.size[1]
light_vao.program['light_position'] = light_position
light_vao.program['light_projection_matrix'] = \
    get_projection_matrix(light_near, light_far, light_perspective)
light_vao.program['light_rotation_matrix'] = \
    get_rotation_matrix(*light_rotate, revers=False)

window_vao.program['shadow_bias'] = shadow_bias


# Render loop
ctx.enable(gl.DEPTH_TEST)
ctx.depth_func = '<'

while not window.is_closing:
    light_fbo.use()
    light_fbo.clear()
    light_vao.render(vertices=vertex_buf.size // 4 // 3)

    window_fbo.use()
    window_fbo.clear()
    light_fbo.depth_attachment.use(location=0)
    window_vao.render(vertices=vertex_buf.size // 4 // 3)

    ctx.copy_framebuffer(
        window.fbo,
        light_fbo if view_as_light else window_fbo
    )

    window.swap_buffers()
