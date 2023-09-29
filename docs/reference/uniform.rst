Uniform
=======

.. py:class:: Uniform

    Available in :py:meth:`Program.__getitem__`

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

.. py:attribute:: Uniform.location
    :type: int

    The location of the uniform.
    The result of the glGetUniformLocation.

.. py:attribute:: Uniform.array_length
    :type: int

    If the uniform is an array the array_length is the length of the array otherwise `1`.

.. py:attribute:: Uniform.dimension
    :type: int

    The uniform dimension.

.. py:attribute:: Uniform.name
    :type: str

    The uniform name.

    The name does not contain leading `[0]`.
    The name may contain `[ ]` when the uniform is part of a struct.

.. py:attribute:: Uniform.value
    :type: Any

    The uniform value stored in the program object.

.. py:attribute:: Uniform.extra
    :type: Any

    User defined data.
