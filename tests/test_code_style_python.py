import glob
import os

import moderngl
import pycodestyle
import pytest


@pytest.mark.before_build
def test_style():
    config_file = os.path.join(__file__, '../../tox.ini')
    style = pycodestyle.StyleGuide(config_file=config_file)
    files = glob.glob(os.path.normpath(os.path.join(__file__, '../../moderngl/**/*.py')), recursive=True)
    files += glob.glob(os.path.normpath(os.path.join(__file__, '../../tests/**/*.py')), recursive=True)
    assert len(files) > 0
    check = style.check_files(files)
    assert check.total_errors == 0


if __name__ == '__main__':
    pytest.main([__file__])
