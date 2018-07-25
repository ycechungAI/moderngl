import textwrap
import re

from .error import Error


def number_source(source):
    lines = textwrap.dedent(source).split('\n')
    fmt = ' %%%dd | %%s' % len(str(len(lines)))
    return '\n'.join(fmt % (i + 1, line) for i, line in enumerate(lines))


def compiler_error(name, source, info):
    message = 'compiler error (%s)\n\n%s\n\n%s' % (name, number_source(source), info)
    raise Error(message)


def linker_error(info):
    raise Error(info)


def split_format(fmt):
    res = []
    divisor = 0
    stride = 0
    nodes = fmt.replace('/', '  /').split()
    if nodes[-1].startswith('/'):
        match = re.match(r'^/(?:(\d*)i|([vr]))$', nodes[-1])
        if not match:
            raise Error('%r is not a valid divisor' % nodes[-1])
        a, b = match.groups()
        divisor = int(a) if a else 1 if not b else 0 if b == 'v' else 0x7fffffff
        nodes = nodes[:-1]
    for node in nodes:
        match = re.match(r'^(\d*)([fiux])([1248]?)$', node)
        if not match:
            raise Error('%r is not a valid format node' % node)
        a, b, c = match.groups()
        a = int(a) if a else 1
        c = int(c) if c else 1 if b == 'x' else 4
        res.append((a, b, c))
        stride += a * c
    return res, divisor, stride
