from setuptools import Extension, setup

sources = [
    'moderngl/mgl/mgl.cpp',
    'moderngl/mgl/buffer.cpp',
    'moderngl/mgl/context.cpp',
    'moderngl/mgl/classes/buffer_class.cpp',
    'moderngl/mgl/classes/context_class.cpp',
    'moderngl/mgl/internal/modules.cpp',
    'moderngl/mgl/internal/tools.cpp',
]

libraries = [
    # 'opengl32',
    # 'gdi32',
    # 'user32',
]

mgl = Extension(
    name='moderngl.mgl',
    sources=sources,
    libraries=libraries,
)

mgl_dbg = Extension(
    name='moderngl.mgl_dbg',
    define_macros=[('MGL_DEBUG', None)],
    sources=sources,
    libraries=libraries,
)

setup(
    name='moderngl',
    ext_modules=[mgl, mgl_dbg],
)
