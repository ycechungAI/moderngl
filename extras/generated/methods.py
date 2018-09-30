import glob
import os
import re

import jinja2

os.chdir(os.path.join(__file__, '../../..'))

re_methods = []

for fn in glob.glob('moderngl/mgl/**/*.cpp', recursive=True):
    with open(fn) as f:
        code = f.read()

    re_methods += re.findall(r'^PyObject \* meth_([A-Za-z0-9_]+)\(PyObject \* self(, PyObject \* const \* args, Py_ssize_t nargs)?\) \{$', code, re.M)

c_methods = []

for m_name, args in re_methods:
    m_name_1 = f'meth_{m_name}'
    m_name_2 = f'{m_name_1}_va' if args else m_name_1
    c_methods.append((m_name, m_name_1, m_name_2, bool(args)))

env = jinja2.Environment(
    block_start_string = '/*[',
    block_end_string = ']*/',
    variable_start_string = '/*{',
    variable_end_string = '}*/',
    comment_start_string = '/*#',
    comment_end_string = '#*/',
    line_statement_prefix = '//!',
    line_comment_prefix = '///',
)

with open(os.path.join(__file__, '../templates/methods.cpp')) as f:
    template = f.read()

with open('moderngl/mgl/generated/methods.cpp', 'w') as f:
    f.write(env.from_string(template).render(c_methods=c_methods))

with open(os.path.join(__file__, '../templates/methods.hpp')) as f:
    template = f.read()

with open('moderngl/mgl/generated/methods.hpp', 'w') as f:
    f.write(env.from_string(template).render())
