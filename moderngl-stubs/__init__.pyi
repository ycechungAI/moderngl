from typing import Any


class Attribute:
    '''
    This class represents a program attribute.
    '''

    location: int
    '''
    int: The location of the attribute.

    The result of the glGetAttribLocation.
    '''

    array_length: int
    '''
    int: If the attribute is an array the array_length.

    is the length of the array otherwise `1`.
    '''

    dimension: int
    '''
    int: The attribute dimension.

    +-----------+-----------+
    | GLSL type | dimension |
    +===========+===========+
    | int       | 1         |
    +-----------+-----------+
    | ivec2     | 2         |
    +-----------+-----------+
    | ivec3     | 3         |
    +-----------+-----------+
    | ivec4     | 4         |
    +-----------+-----------+
    | uint      | 1         |
    +-----------+-----------+
    | uvec2     | 2         |
    +-----------+-----------+
    | uvec3     | 3         |
    +-----------+-----------+
    | uvec4     | 4         |
    +-----------+-----------+
    | float     | 1         |
    +-----------+-----------+
    | vec2      | 2         |
    +-----------+-----------+
    | vec3      | 3         |
    +-----------+-----------+
    | vec4      | 4         |
    +-----------+-----------+
    | double    | 1         |
    +-----------+-----------+
    | dvec2     | 2         |
    +-----------+-----------+
    | dvec3     | 3         |
    +-----------+-----------+
    | dvec4     | 4         |
    +-----------+-----------+
    | mat2      | 4         |
    +-----------+-----------+
    | mat2x3    | 6         |
    +-----------+-----------+
    | mat2x4    | 8         |
    +-----------+-----------+
    | mat3x2    | 6         |
    +-----------+-----------+
    | mat3      | 9         |
    +-----------+-----------+
    | mat3x4    | 12        |
    +-----------+-----------+
    | mat4x2    | 8         |
    +-----------+-----------+
    | mat4x3    | 12        |
    +-----------+-----------+
    | mat4      | 16        |
    +-----------+-----------+
    | dmat2     | 4         |
    +-----------+-----------+
    | dmat2x3   | 6         |
    +-----------+-----------+
    | dmat2x4   | 8         |
    +-----------+-----------+
    | dmat3x2   | 6         |
    +-----------+-----------+
    | dmat3     | 9         |
    +-----------+-----------+
    | dmat3x4   | 12        |
    +-----------+-----------+
    | dmat4x2   | 8         |
    +-----------+-----------+
    | dmat4x3   | 12        |
    +-----------+-----------+
    | dmat4     | 16        |
    +-----------+-----------+
    '''

    shape: int
    '''
    str: The shape is a single character, representing the scalar type of the attribute.

    +---------+--------------------------+
    | shape   | GLSL types               |
    +=========+==========================+
    | ``'i'`` | int                      |
    |         +--------------------------+
    |         | ivec2 ivec3 ivec4        |
    +---------+--------------------------+
    | ``'I'`` | uint                     |
    |         +--------------------------+
    |         | uvec2 uvec3 uvec4        |
    +---------+--------------------------+
    | ``'f'`` | float                    |
    |         +--------------------------+
    |         | vec2 vec3 vec4           |
    |         +--------------------------+
    |         | mat2 mat3 mat4           |
    |         +--------------------------+
    |         | mat2x3 mat2x4 mat3x4     |
    |         | mat4x2 mat4x2 mat4x3     |
    +---------+--------------------------+
    | ``'d'`` | double                   |
    |         +--------------------------+
    |         | dvec2 dvec3 dvec4        |
    |         +--------------------------+
    |         | dmat2 dmat3 dmat4        |
    |         +--------------------------+
    |         | dmat2x3 dmat2x4 dmat3x4  |
    |         | dmat4x2 dmat4x2 dmat4x3  |
    +---------+--------------------------+
    '''

    name: str
    '''
    str: The attribute name.

    The name will be filtered to have no array syntax on it's end.
    Attribute name without ``'[0]'`` ending if any.
    '''

    extra: Any
    '''
    Attribute for storing user defined objects
    '''

    mglo: Any
    '''
    Internal moderngl core object
    '''


class Uniform:
    '''
    A uniform is a global GLSL variable declared with the 'uniform' storage qualifier.

    These act as parameters that the user of a shader program can pass to that program.

    In ModernGL, Uniforms can be accessed using :py:meth:`Program.__getitem__`
    or :py:meth:`Program.__iter__`
    '''

    location: int
    '''
    int: The location of the uniform.

    The location holds the value returned by the glGetUniformLocation.
    To set the value of the uniform use the :py:attr:`value` instead.
    '''

    dimension: int
    '''
    int: The dimension of the uniform.

    +-----------------+-----------+
    | GLSL type       | dimension |
    +=================+===========+
    | sampler2D       | 1         |
    +-----------------+-----------+
    | sampler2DCube   | 1         |
    +-----------------+-----------+
    | sampler2DShadow | 1         |
    +-----------------+-----------+
    | bool            | 1         |
    +-----------------+-----------+
    | bvec2           | 2         |
    +-----------------+-----------+
    | bvec3           | 3         |
    +-----------------+-----------+
    | bvec4           | 4         |
    +-----------------+-----------+
    | int             | 1         |
    +-----------------+-----------+
    | ivec2           | 2         |
    +-----------------+-----------+
    | ivec3           | 3         |
    +-----------------+-----------+
    | ivec4           | 4         |
    +-----------------+-----------+
    | uint            | 1         |
    +-----------------+-----------+
    | uvec2           | 2         |
    +-----------------+-----------+
    | uvec3           | 3         |
    +-----------------+-----------+
    | uvec4           | 4         |
    +-----------------+-----------+
    | float           | 1         |
    +-----------------+-----------+
    | vec2            | 2         |
    +-----------------+-----------+
    | vec3            | 3         |
    +-----------------+-----------+
    | vec4            | 4         |
    +-----------------+-----------+
    | double          | 1         |
    +-----------------+-----------+
    | dvec2           | 2         |
    +-----------------+-----------+
    | dvec3           | 3         |
    +-----------------+-----------+
    | dvec4           | 4         |
    +-----------------+-----------+
    | mat2            | 4         |
    +-----------------+-----------+
    | mat2x3          | 6         |
    +-----------------+-----------+
    | mat2x4          | 8         |
    +-----------------+-----------+
    | mat3x2          | 6         |
    +-----------------+-----------+
    | mat3            | 9         |
    +-----------------+-----------+
    | mat3x4          | 12        |
    +-----------------+-----------+
    | mat4x2          | 8         |
    +-----------------+-----------+
    | mat4x3          | 12        |
    +-----------------+-----------+
    | mat4            | 16        |
    +-----------------+-----------+
    | dmat2           | 4         |
    +-----------------+-----------+
    | dmat2x3         | 6         |
    +-----------------+-----------+
    | dmat2x4         | 8         |
    +-----------------+-----------+
    | dmat3x2         | 6         |
    +-----------------+-----------+
    | dmat3           | 9         |
    +-----------------+-----------+
    | dmat3x4         | 12        |
    +-----------------+-----------+
    | dmat4x2         | 8         |
    +-----------------+-----------+
    | dmat4x3         | 12        |
    +-----------------+-----------+
    | dmat4           | 16        |
    +-----------------+-----------+
    '''

    array_length: int
    '''
    int: The length of the array of the uniform.
    The array_length is `1` for non array uniforms.
    '''

    name: str
    '''
    str: The name of the uniform.

    The name does not contain leading `[0]`.
    The name may contain `[ ]` when the uniform is part of a struct.
    '''

    value: Any
    '''
    The value of the uniform.

    Reading the value of the uniform may force the GPU to sync.

    The value must be a tuple for non array uniforms.
    The value must be a list of tuples for array uniforms.
    '''

    extra: Any
    '''
    Attribute for storing user defined objects
    '''

    mglo: Any
    '''
    Internal moderngl core object
    '''

    def read(self) -> bytes:
        '''
        Read the value of the uniform.
        '''

    def write(self, data: Any) -> None:
        '''
        Write the value of the uniform.
        '''


class UniformBlock:
    '''
    Uniform Block metadata
    '''

    binding: int
    '''
    int: The binding of the uniform block.
    '''

    value: int
    '''
    int: The value of the uniform block.
    '''

    name: str
    '''
    str: The name of the uniform block.
    '''

    index: int
    '''
    int: The index of the uniform block.
    '''

    size: int
    '''
    int: The size of the uniform block.
    '''

    extra: Any
    '''
    Attribute for storing user defined objects
    '''

    mglo: Any
    '''
    Internal moderngl core object
    '''


class Subroutine:
    '''
    This class represents a program subroutine.
    '''

    index: int
    '''
    int: The index of the subroutine.
    '''

    name: str
    '''
    str: The name of the subroutine.
    '''

    extra: Any
    '''
    Attribute for storing user defined objects
    '''

    mglo: Any
    '''
    Internal moderngl core object
    '''


class Varying:
    '''
    This class represents a program varying.
    '''

    number: int
    '''
    int: The number of the varying.
    '''

    name: str
    '''
    str: The name of the varying.
    '''

    extra: Any
    '''
    Attribute for storing user defined objects
    '''

    mglo: Any
    '''
    Internal moderngl core object
    '''


class Error(Exception):
    '''
    Generic moderngl error.
    '''


class InvalidObject:
    '''
    DEPRECATED
    '''
