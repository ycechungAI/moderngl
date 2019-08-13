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
    include_dirs=['src', 'moderngl', 'moderngl/mgl'],
    define_macros=[],
    libraries=libraries[target],
    extra_compile_args=extra_compile_args[target],
    extra_link_args=extra_linker_args[target],
    sources=[
        'moderngl/old/gl_context_%s.cpp' % target,
        'moderngl/old/gl_methods.cpp',
        'moderngl/old/Sampler.cpp',
        'moderngl/old/Attribute.cpp',
        'moderngl/old/Buffer.cpp',
        'moderngl/old/BufferFormat.cpp',
        'moderngl/old/ComputeShader.cpp',
        'moderngl/old/Context.cpp',
        'moderngl/old/DataType.cpp',
        'moderngl/old/Error.cpp',
        'moderngl/old/Framebuffer.cpp',
        'moderngl/old/InvalidObject.cpp',
        'moderngl/old/ModernGL.cpp',
        'moderngl/old/Program.cpp',
        'moderngl/old/Query.cpp',
        'moderngl/old/Renderbuffer.cpp',
        'moderngl/old/Scope.cpp',
        'moderngl/old/Texture.cpp',
        'moderngl/old/Texture3D.cpp',
        'moderngl/old/TextureArray.cpp',
        'moderngl/old/TextureCube.cpp',
        'moderngl/old/Uniform.cpp',
        'moderngl/old/UniformBlock.cpp',
        'moderngl/old/UniformGetters.cpp',
        'moderngl/old/UniformSetters.cpp',
        'moderngl/old/VertexArray.cpp',
    ],
    depends=[
        'moderngl/old/gl_context.hpp',
        'moderngl/old/gl_methods.hpp',
        'moderngl/old/OpenGL.hpp',

        'moderngl/old/BufferFormat.hpp',
        'moderngl/old/Error.hpp',
        'moderngl/old/InlineMethods.hpp',
        'moderngl/old/OpenGL.hpp',
        'moderngl/old/Python.hpp',
        'moderngl/old/Types.hpp',
        'moderngl/old/UniformGetSetters.hpp',
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
    version='5.5.2',
    description=short_description,
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/moderngl/moderngl',
    author='Szabolcs Dombi',
    author_email='cprogrammer1994@gmail.com',
    license='MIT',
    classifiers=classifiers,
    keywords=keywords,
    packages=['moderngl', 'moderngl.program_members'],
    ext_modules=[mgl],
    platforms=['any'],
)
