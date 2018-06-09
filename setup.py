from setuptools import Extension, setup

sources = [
    'moderngl/mgl/buffer.cpp',
    'moderngl/mgl/context.cpp',
    'moderngl/mgl/mgl.cpp',
    'moderngl/mgl/gl_context.cpp',
    'moderngl/mgl/gl_methods.cpp',
    'moderngl/mgl/tools.cpp',
]

mgl = Extension(
    name='moderngl.mgl',
    sources=sources,
    libraries=[
        'opengl32',
        'gdi32',
        'user32',
    ]
)

mgl_dbg = Extension(
    name='moderngl.mgl_dbg',
    define_macros=[('MGL_DEBUG', None)],
    sources=sources,
    libraries=[
        'opengl32',
        'gdi32',
        'user32',
    ]
)

setup(
    name='moderngl',
    ext_modules=[mgl, mgl_dbg],
)
