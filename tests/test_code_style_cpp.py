import glob
import os
import re

import moderngl
import pytest


@pytest.mark.before_build
def test_style():
    files = glob.glob(os.path.normpath(os.path.join(__file__, '../../moderngl/mgl/**/*.cpp')), recursive=True)
    files += glob.glob(os.path.normpath(os.path.join(__file__, '../../moderngl/mgl/**/*.hpp')), recursive=True)
    assert len(files) > 0

    errors = 0
    for fn in files:
        with open(fn) as f:
            code = f.read()

        for i, line in enumerate(code.split('\n')):
            if re.search(r'^[ ]*\t[ ]*\S', line):
                print('%s:%d: indentation using tabs' % (fn, i + 1))
                errors += 1

            if re.search(r'\S\s+$', line):
                print('%s:%d: trailing whitespace' % (fn, i + 1))
                errors += 1

    assert errors == 0


if __name__ == '__main__':
    pytest.main([__file__])
