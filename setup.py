import os
import platform
import re
import sys
from distutils.sysconfig import get_config_var
from distutils.version import LooseVersion

from setuptools import Extension, setup

# pylint: disable=C0103, W0212

if sys.version_info < (3, 0):
    raise Exception('Python 2 is not supported!')

PLATFORMS = {'windows', 'linux', 'darwin', 'cygwin', 'android'}

target = platform.system().lower()

if 'pydroid3' in sys.executable.lower():
    target = 'android'

for known in PLATFORMS:
    if target.startswith(known):
        target = known

if target not in PLATFORMS:
    target = 'linux'

# For mac, ensure extensions are built for macos 10.9 when compiling on a
# 10.9 system or above, overriding distuitls behaviour which is to target
# the version that python was built for. This may be overridden by setting
# MACOSX_DEPLOYMENT_TARGET before calling setup.py
if target == 'darwin':
    if 'MACOSX_DEPLOYMENT_TARGET' not in os.environ:
        current_system = LooseVersion(platform.mac_ver()[0])
        python_target = LooseVersion(get_config_var('MACOSX_DEPLOYMENT_TARGET'))
        if python_target < '10.9' and current_system >= '10.9':
            os.environ['MACOSX_DEPLOYMENT_TARGET'] = '10.9'

if target in ['linux', 'cygwin']:
    from distutils import sysconfig
    cvars = sysconfig.get_config_vars()
    
    if hasattr(sysconfig, '_config_vars') and sysconfig._config_vars is not None:
        if 'OPT' in cvars:
            sysconfig._config_vars['OPT'] = cvars['OPT'].replace('-Wstrict-prototypes', '')
            sysconfig._config_vars['OPT'] = cvars['OPT'].replace('-Wimplicit-function-declaration', '')

        if 'CFLAGS' in cvars:
            sysconfig._config_vars['CFLAGS'] = cvars['CFLAGS'].replace('-Wstrict-prototypes', '')
            sysconfig._config_vars['CFLAGS'] = cvars['CFLAGS'].replace('-Wimplicit-function-declaration', '')

libraries = {
    'windows': ['gdi32', 'opengl32', 'user32'],
    'linux': ['GL', 'dl', 'X11'],
    'cygwin': ['GL', 'X11'],
    'darwin': [],
    'android': [],
}

extra_compile_args = {
    'windows': [],
    'linux': [],
    'cygwin': [],
    'darwin': ['-Wno-deprecated-declarations'],
    'android': [],
}

extra_linker_args = {
    'windows': [],
    'linux': [],
    'cygwin': [],
    'darwin': ['-framework', 'OpenGL', '-Wno-deprecated'],
    'android': [],
}

mgl = Extension(
    name='moderngl.mgl',
    include_dirs=['src', 'moderngl/next', 'moderngl/next/mgl'],
    define_macros=[],
    libraries=libraries[target],
    extra_compile_args=extra_compile_args[target],
    extra_link_args=extra_linker_args[target],
    sources=[
        'moderngl/next/mgl/adaptor/adaptor.cpp',

        'src/Sampler.cpp',
        'src/Attribute.cpp',
        'src/Buffer.cpp',
        'src/BufferFormat.cpp',
        'src/ComputeShader.cpp',
        'src/Context.cpp',
        'src/DataType.cpp',
        'src/Error.cpp',
        'src/Framebuffer.cpp',
        'src/InvalidObject.cpp',
        'src/ModernGL.cpp',
        'src/Program.cpp',
        'src/Query.cpp',
        'src/Renderbuffer.cpp',
        'src/Scope.cpp',
        'src/Texture.cpp',
        'src/Texture3D.cpp',
        'src/TextureArray.cpp',
        'src/TextureCube.cpp',
        'src/Uniform.cpp',
        'src/UniformBlock.cpp',
        'src/UniformGetters.cpp',
        'src/UniformSetters.cpp',
        'src/VertexArray.cpp',

        'moderngl/next/mgl/buffer.cpp',
        'moderngl/next/mgl/compute_shader.cpp',
        'moderngl/next/mgl/configuration.cpp',
        'moderngl/next/mgl/context.cpp',
        'moderngl/next/mgl/extensions.cpp',
        'moderngl/next/mgl/framebuffer.cpp',
        'moderngl/next/mgl/limits.cpp',
        'moderngl/next/mgl/mgl.cpp',
        'moderngl/next/mgl/program.cpp',
        'moderngl/next/mgl/query.cpp',
        'moderngl/next/mgl/recorder.cpp',
        'moderngl/next/mgl/renderbuffer.cpp',
        'moderngl/next/mgl/sampler.cpp',
        'moderngl/next/mgl/scope.cpp',
        'moderngl/next/mgl/texture.cpp',
        'moderngl/next/mgl/vertex_array.cpp',
        'moderngl/next/mgl/internal/bytecode.cpp',
        'moderngl/next/mgl/internal/compare_func.cpp',
        'moderngl/next/mgl/internal/data_type.cpp',
        'moderngl/next/mgl/internal/glsl.cpp',
        'moderngl/next/mgl/internal/intern.cpp',
        'moderngl/next/mgl/internal/python.cpp',
        'moderngl/next/mgl/internal/tools.cpp',
        'moderngl/next/mgl/internal/uniform.cpp',
        'moderngl/next/mgl/internal/wrapper.cpp',
        'moderngl/next/mgl/internal/opengl/gl_context_%s.cpp' % target,
        'moderngl/next/mgl/internal/opengl/gl_methods.cpp',
    ],
    depends=[
        'setup.py',
        'moderngl/next/mgl/adaptor/adaptor.hpp',
        'moderngl/next/mgl/buffer.hpp',
        'moderngl/next/mgl/compute_shader.hpp',
        'moderngl/next/mgl/configuration.hpp',
        'moderngl/next/mgl/context.hpp',
        'moderngl/next/mgl/extensions.hpp',
        'moderngl/next/mgl/framebuffer.hpp',
        'moderngl/next/mgl/limits.hpp',
        'moderngl/next/mgl/mgl.hpp',
        'moderngl/next/mgl/program.hpp',
        'moderngl/next/mgl/query.hpp',
        'moderngl/next/mgl/recorder.hpp',
        'moderngl/next/mgl/renderbuffer.hpp',
        'moderngl/next/mgl/sampler.hpp',
        'moderngl/next/mgl/scope.hpp',
        'moderngl/next/mgl/texture.hpp',
        'moderngl/next/mgl/vertex_array.hpp',
        'moderngl/next/mgl/internal/bytecode.hpp',
        'moderngl/next/mgl/internal/compare_func.hpp',
        'moderngl/next/mgl/internal/data_type.hpp',
        'moderngl/next/mgl/internal/glsl.hpp',
        'moderngl/next/mgl/internal/intern.hpp',
        'moderngl/next/mgl/internal/python.hpp',
        'moderngl/next/mgl/internal/tools.hpp',
        'moderngl/next/mgl/internal/wrapper.hpp',
        'moderngl/next/mgl/internal/opengl/gl_context.hpp',
        'moderngl/next/mgl/internal/opengl/gl_methods.hpp',
    ],
)

short_description = 'ModernGL: High performance rendering for Python 3'

with open('README.md') as f:
    long_description = f.read()
    long_description = re.sub(r'</?div[^>]*>|\r', '', long_description, flags=re.M)

keywords = [
    'ModernGL',
    'OpenGL',
    'PyOpenGL',
    'rendering',
    'graphics',
    'shader',
    'GLSL',
    'GPU',
    'visualization',
    '2D',
    '3D',
]

classifiers = [
    'Development Status :: 5 - Production/Stable',
    'License :: OSI Approved :: MIT License',
    'Operating System :: OS Independent',
    'Topic :: Games/Entertainment',
    'Topic :: Multimedia :: Graphics',
    'Topic :: Multimedia :: Graphics :: 3D Rendering',
    'Topic :: Scientific/Engineering :: Visualization',
    'Programming Language :: Python :: 3 :: Only',
]

setup(
    name='moderngl',
    version='5.5.0',
    description=short_description,
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/cprogrammer1994/ModernGL',
    author='Szabolcs Dombi',
    author_email='cprogrammer1994@gmail.com',
    license='MIT',
    classifiers=classifiers,
    keywords=keywords,
    packages=['moderngl', 'moderngl.next', 'moderngl.program_members'],
    ext_modules=[mgl],
    platforms=['any'],
)
