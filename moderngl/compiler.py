import textwrap

from .error import Error


def number_source(source):
    lines = textwrap.dedent(source).split('\n')
    fmt = ' %%%dd | %%s' % len(str(len(lines)))
    return '\n'.join(fmt % (i + 1, line) for i, line in enumerate(lines))


def compiler_error(name, source, info):
    message = 'compiler error (%s)\n\n%s\n\n%s' % (name, number_source(source), info)
    raise Error(message)


def linker_error(vertex_shader, fragment_shader, geometry_shader, tess_control_shader, tess_evaluation_shader, info, *args):
    print(vertex_shader, fragment_shader, geometry_shader, tess_control_shader, tess_evaluation_shader, info, args)
    raise Error(info)
