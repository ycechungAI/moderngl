import glob
import os
import re

import jinja2

os.chdir(os.path.join(__file__, '../../..'))

re_slots = []

for fn in glob.glob('moderngl/**/*.py', recursive=True):
    with open(fn) as f:
        code = f.read()

    re_slots += re.findall(r'^class ([A-Za-z0-9]+):\n\s+__slots__ = (\[[^\]]+\])$', code, re.M)

c_slots = []

for c_name, slots in re_slots:
    c_slots.append((c_name, eval(slots)))

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

with open(os.path.join(__file__, '../templates/py_classes.cpp')) as f:
    template = f.read()

with open('moderngl/mgl/generated/py_classes.cpp', 'w') as f:
    f.write(env.from_string(template).render(c_slots=c_slots))

with open(os.path.join(__file__, '../templates/py_classes.hpp')) as f:
    template = f.read()

with open('moderngl/mgl/generated/py_classes.hpp', 'w') as f:
    f.write(env.from_string(template).render(c_slots=c_slots))
