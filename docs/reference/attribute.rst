Attribute
=========

.. py:class:: Attribute

    Available in :py:meth:`Program.__getitem__`

    Represents a program input attribute.

.. py:attribute:: Attribute.location
    :type: int

    The location of the attribute.
    The result of the glGetAttribLocation.

.. py:attribute:: Attribute.array_length
    :type: int

    If the attribute is an array the array_length is the length of the array otherwise `1`.

.. py:attribute:: Attribute.dimension
    :type: int

    The attribute dimension.

.. py:attribute:: Attribute.shape
    :type: str

    The shape is a single character, representing the scalar type of the attribute.
    It is either ``'i'`` (int), ``'f'`` (float), ``'I'`` (unsigned int), ``'d'`` (double).

.. py:attribute:: Attribute.name
    :type: str

    The attribute name.

    The name will be filtered to have no array syntax on it's end.
    Attribute name without ``'[0]'`` ending if any.

.. py:attribute:: Attribute.extra
    :type: Any

    User defined data.
