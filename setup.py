from setuptools import Extension, setup

# tools = ','.join(str(x) for x in open('moderngl_tools.py', 'rb').read())
# tools = f'const char moderngl_tools[] = {{{tools}}};\n'
# open('moderngl_tools.hpp', 'w').write(tools)

ext = Extension(
    name='moderngl',
    sources=[
        'moderngl/moderngl.cpp',
        'moderngl/extras.cpp',
        'moderngl/gl_methods.cpp',
        'moderngl/gl_context_windows.cpp',
    ],
    extra_compile_args=['-fpermissive'],
    libraries=['opengl32', 'gdi32', 'user32'],
)

setup(
    name='moderngl',
    version='0.1.0',
    ext_modules=[ext],
    py_modules=['_moderngl'],
)
