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
    'moderngl/mgl/configuration.cpp',
    'moderngl/mgl/extensions.cpp',
    'moderngl/mgl/framebuffer.cpp',
    'moderngl/mgl/limits.cpp',
    'moderngl/mgl/program.cpp',
    'moderngl/mgl/query.cpp',
    'moderngl/mgl/renderbuffer.cpp',
    'moderngl/mgl/sampler.cpp',
    'moderngl/mgl/scope.cpp',
    'moderngl/mgl/texture.cpp',
    'moderngl/mgl/vertex_array.cpp',
    'moderngl/mgl/generated/tools.cpp',
    'moderngl/mgl/generated/cpp_classes.cpp',
    'moderngl/mgl/generated/py_classes.cpp',
    'moderngl/mgl/generated/methods.cpp',
    'moderngl/mgl/internal/data_type.cpp',
    'moderngl/mgl/internal/modules.cpp',
    'moderngl/mgl/internal/enable.cpp',
    'moderngl/mgl/internal/tools.cpp',
    'moderngl/mgl/internal/glsl.cpp',
    'moderngl/mgl/internal/opengl/gl_methods.cpp',
    'moderngl/mgl/internal/opengl/gl_context_%s.cpp' % glctx,
]

mgl = Extension(
    name='moderngl.mgl',
    sources=sources,
    libraries=libraries[target],
)

setup(
    name='moderngl',
    ext_modules=[mgl],
)
