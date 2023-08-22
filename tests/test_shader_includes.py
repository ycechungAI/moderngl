import moderngl


def test_shader_includes(ctx):
    ctx.includes['uniforms'] = '''
        uniform vec2 pos;
        uniform float scale;
    '''

    program = ctx.program(
        vertex_shader='''
            #version 330

            #include "uniforms"

            in vec2 vert;
            out vec2 v_vert;

            void main() {
                gl_Position = vec4(pos + vert * scale, 0.0, 1.0);
                v_vert = vert;
            }
        ''',
        fragment_shader='''
            #version 330

            in vec2 v_vert;
            out vec4 color;

            void main() {
                color = vec4(v_vert, 0.0, 1.0);
            }
        ''',
    )
    assert program.glo > 0
    assert program.subroutines == ()

    assert 'vert' in program
    assert 'pos' in program
    assert 'scale' in program
    assert program.is_transform is False

    assert isinstance(program['vert'], moderngl.Attribute)
    assert isinstance(program['pos'], moderngl.Uniform)
    assert isinstance(program['scale'], moderngl.Uniform)
