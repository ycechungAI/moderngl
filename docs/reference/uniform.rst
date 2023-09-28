Uniform
=======

.. py:class:: Uniform

    A uniform is a global GLSL variable declared with the 'uniform' storage qualifier.

    These act as parameters that the user of a shader program can pass to that program.

    In ModernGL, Uniforms can be accessed using :py:meth:`Program.__getitem__`
    or :py:meth:`Program.__iter__`

Methods
-------

.. py:method:: read() -> bytes:

    Read the value of the uniform.

.. py:method:: write(data: Any) -> None:

    Write the value of the uniform.

Attributes
----------

.. py:attribute:: location
    :type: int

    The location of the uniform.
    The result of the glGetUniformLocation.

.. py:attribute:: array_length
    :type: int

    If the uniform is an array the array_length is the length of the array otherwise `1`.

.. py:attribute:: dimension
    :type: int

    The uniform dimension.

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

.. py:attribute:: name
    :type: str

    The uniform name.

    The name does not contain leading `[0]`.
    The name may contain `[ ]` when the uniform is part of a struct.

.. py:attribute:: value
    :type: Any

    The uniform value stored in the program object.

.. py:attribute:: extra
    :type: Any

    Attribute for storing user defined objects

.. toctree::
    :maxdepth: 2
