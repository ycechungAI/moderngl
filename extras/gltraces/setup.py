import os
import platform

from setuptools import Extension, setup

os.chdir(os.path.normpath(os.path.join(__file__, '..')))
PLATFORMS = {'windows', 'linux', 'darwin', 'cygwin'}

target = platform.system().lower()

for known in PLATFORMS:
    if target.startswith(known):
        target = known

if target not in PLATFORMS:
    target = 'linux'

libraries = {
    'windows': ['gdi32', 'opengl32', 'user32'],
    'linux': ['GL', 'dl', 'X11'],
    'cygwin': ['GL', 'X11'],
    'darwin': [],
}

sources = [
    'gltraces.cpp',
    '../../moderngl/gl_methods.cpp',
]

gltraces = Extension(
    name='gltraces',
    sources=sources,
    libraries=libraries[target],
    include_dirs=['../../moderngl'],
)

setup(
    name='gltraces',
    version='6.0.dev1',
    ext_modules=[gltraces],
)
