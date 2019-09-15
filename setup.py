import platform
import sys

from setuptools import Extension, setup

if sys.version_info < (3, 0):
    raise Exception('Python 2 is not supported!')

PLATFORMS = {'windows', 'linux', 'darwin'}

target = platform.system().lower()

for known in PLATFORMS:
    if target.startswith(known):
        target = known

if target not in PLATFORMS:
    target = 'linux'

if target == 'darwin':
    import os
    from distutils.sysconfig import get_config_var
    from distutils.version import LooseVersion
    if 'MACOSX_DEPLOYMENT_TARGET' not in os.environ:
        current_system = LooseVersion(platform.mac_ver()[0])
        python_target = LooseVersion(get_config_var('MACOSX_DEPLOYMENT_TARGET'))
        if python_target < '10.9' and current_system >= '10.9':
            os.environ['MACOSX_DEPLOYMENT_TARGET'] = '10.9'

libraries = {
    'windows': ['gdi32', 'user32'],
    'linux': ['GL', 'dl'],
    'darwin': [],
}

extra_compile_args = {
    'windows': [],
    'linux': ['-fpermissive'],
    'darwin': ['-fpermissive', '-Wno-deprecated-declarations'],
}

extra_link_args = {
    'windows': [],
    'linux': [],
    'darwin': ['-Wno-deprecated'],
}

short_description = 'ModernGL: High performance rendering for Python 3'

ext = Extension(
    name='moderngl',
    sources=[
        'moderngl/moderngl.cpp',
        'moderngl/extras.cpp',
        'moderngl/converters.cpp',
    ],
    libraries=libraries[target],
    extra_compile_args=extra_compile_args[target],
    extra_link_args=extra_link_args[target],
    depends=[
        'moderngl/common.hpp',
        'moderngl/data_types.hpp',
        'moderngl/extras.hpp',
        'moderngl/gl_methods.hpp',
        'moderngl/moderngl.hpp',
        'moderngl/opengl.hpp',
        'moderngl/tools.hpp',
        'setup.py',
    ],
)

setup(
    name='moderngl',
    version='6.0.dev1',
    ext_modules=[ext],
    py_modules=['_moderngl'],
)
