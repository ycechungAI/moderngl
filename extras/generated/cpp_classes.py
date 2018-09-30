import glob
import os
import re

import jinja2

os.chdir(os.path.join(__file__, '../../..'))

re_methods = []
re_getters = []
re_setters = []
includes = []

for fn in glob.glob('moderngl/mgl/**/*.cpp', recursive=True):
    with open(fn) as f:
        code = f.read()

    re_methods += re.findall(r'^PyObject \* ([A-Za-z0-9]+)_meth_([A-Za-z0-9_]+)\(\1 \* self(, PyObject \* const \* args, Py_ssize_t nargs)?\) \{$', code, re.M)
    re_setters += re.findall(r'^int ([A-Za-z0-9]+)_set_([A-Za-z0-9_]+)\(\1 \* self, PyObject \* value\) \{$', code, re.M)
    re_getters += re.findall(r'^PyObject \* ([A-Za-z0-9]+)_get_([A-Za-z0-9_]+)\(\1 \* self\) \{$', code, re.M)

for fn in glob.glob('moderngl/mgl/*.hpp'):
    with open(fn) as f:
        code = f.read()

    if 'struct MGL' in code:
        includes.append(os.path.relpath(fn, 'moderngl/mgl/generated').replace('\\', '/'))

c_names = sorted(set(c_name for c_name, m_name, args in re_methods))
c_methods = []

for name in c_names:
    methods = []
    for c_name, m_name, args in re_methods:
        if c_name == name:
            m_name_1 = f'{c_name}_meth_{m_name}'
            m_name_2 = f'{m_name_1}_va' if args else m_name_1
            methods.append((m_name, m_name_1, m_name_2, bool(args)))
    c_methods.append((name, methods))

c_getset = []
for name in c_names:
    getset = []
    temp = {}
    for c_name, m_name in re_getters:
        if c_name == name:
            temp.setdefault(m_name, set())
            temp[m_name].add('get')
    for c_name, m_name in re_setters:
        if c_name == name:
            temp.setdefault(m_name, set())
            temp[m_name].add('set')
    getset = list(temp.items())
    c_getset.append((name, getset))

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

with open(os.path.join(__file__, '../templates/cpp_classes.cpp')) as f:
    template = f.read()

with open('moderngl/mgl/generated/cpp_classes.cpp', 'w') as f:
    f.write(env.from_string(template).render(c_methods=c_methods, c_getset=c_getset, c_names=c_names, includes=includes))

with open(os.path.join(__file__, '../templates/cpp_classes.hpp')) as f:
    template = f.read()

with open('moderngl/mgl/generated/cpp_classes.hpp', 'w') as f:
    f.write(env.from_string(template).render(c_methods=c_methods, c_getset=c_getset, c_names=c_names))
