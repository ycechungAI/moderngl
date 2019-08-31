import moderngl


def glhooks(glprocs):
    import gltraces
    gltraces.glprocs[:] = glprocs
    glprocs[:] = gltraces.gltraces
    for x in ['glGetFloatv', 'glGetIntegerv', 'glGetInteger64v', 'glGetString']:
        glprocs[gltraces.lookup[x]] = gltraces.glprocs[gltraces.lookup[x]]


ctx = moderngl.create_context(glhooks=glhooks, standalone=True)

prog = ctx.program(
    vertex_shader='''
        #version 330

        in vec2 in_vert;

        void main() {
            gl_Position = vec4(in_vert, 0.0, 1.0);
        }
    ''',
    fragment_shader='''
        #version 330

        out vec4 f_color;

        void main() {
            f_color = vec4(0.3, 0.5, 1.0, 1.0);
        }
    ''',
)

vertex_data = moderngl.pack([
    0.0, 0.8,
    -0.6, -0.8,
    0.6, -0.8,
])

vbo = ctx.buffer(vertex_data)
vao = ctx.vertex_array(prog, [
    vbo.bind('in_vert'),
])
