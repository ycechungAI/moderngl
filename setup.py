import platform

from setuptools import Extension, setup

PLATFORMS = {'windows', 'linux', 'darwin', 'cygwin'}

target = platform.system().lower()

for known in PLATFORMS:
    if target.startswith(known):
        target = known

if target not in PLATFORMS:
    target = 'linux'

glctx = target if target != 'cygwin' else 'linux'

libraries = {
    'windows': ['gdi32', 'opengl32', 'user32'],
    'linux': ['GL', 'dl', 'X11'],
    'cygwin': ['GL', 'X11'],
    'darwin': [],
}

sources = [
    'moderngl/mgl/mgl.cpp',
    'moderngl/mgl/buffer.cpp',
    'moderngl/mgl/context.cpp',
    'moderngl/mgl/limits.cpp',
    'moderngl/mgl/program.cpp',
    'moderngl/mgl/extensions.cpp',
    'moderngl/mgl/classes/buffer_class.cpp',
    'moderngl/mgl/classes/context_class.cpp',
    'moderngl/mgl/classes/limits_class.cpp',
    'moderngl/mgl/classes/program_class.cpp',
    'moderngl/mgl/classes/tools.cpp',
    'moderngl/mgl/internal/modules.cpp',
    'moderngl/mgl/internal/tools.cpp',
    'moderngl/mgl/internal/opengl/gl_methods.cpp',
    'moderngl/mgl/internal/opengl/gl_context_%s.cpp' % glctx,
]

mgl = Extension(
    name='moderngl.mgl',
    sources=sources,
    libraries=libraries[target],
)

mgl_dbg = Extension(
    name='moderngl.mgl_dbg',
    define_macros=[('MGL_DEBUG', None)],
    sources=sources,
    libraries=libraries[target],
)

setup(
    name='moderngl',
    ext_modules=[mgl, mgl_dbg],
)
