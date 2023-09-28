Attribute
=========

.. py:class:: Attribute

.. py:attribute:: Attribute.location
    :type: int

    The location of the attribute.
    The result of the glGetAttribLocation.

.. py:attribute:: Attribute.array_length
    :type: int

    If the attribute is an array the array_length.
    is the length of the array otherwise `1`.

.. py:attribute:: Attribute.dimension
    :type: int

    The attribute dimension.

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

.. py:attribute:: Attribute.shape
    :type: str

    The shape is a single character, representing the scalar type of the attribute.

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

.. py:attribute:: Attribute.name
    :type: str

    The attribute name.

    The name will be filtered to have no array syntax on it's end.
    Attribute name without ``'[0]'`` ending if any.

.. py:attribute:: Attribute.extra
    :type: Any

    Attribute for storing user defined objects

.. toctree::
    :maxdepth: 2
