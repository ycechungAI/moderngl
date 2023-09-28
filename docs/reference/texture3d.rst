Texture3D
=========

.. py:class:: Texture3D

    Returned by :py:meth:`Context.texture3d`

    A Texture is an OpenGL object that contains one or more images that all have the same image format.

    A texture can be used in two ways. It can
    be the source of a texture access from a Shader, or it can be used
    as a render target.

    A Texture3D object cannot be instantiated directly, it requires a context.
    Use :py:meth:`Context.texture3d` to create one.

Methods
-------

.. py:method:: Texture3D.read
.. py:method:: Texture3D.read_into
.. py:method:: Texture3D.write
.. py:method:: Texture3D.build_mipmaps
.. py:method:: Texture3D.bind_to_image
.. py:method:: Texture3D.use
.. py:method:: Texture3D.release
.. py:method:: Texture3D.get_handle

Attributes
----------

.. py:attribute:: Texture3D.repeat_x
.. py:attribute:: Texture3D.repeat_y
.. py:attribute:: Texture3D.repeat_z
.. py:attribute:: Texture3D.filter
.. py:attribute:: Texture3D.swizzle
.. py:attribute:: Texture3D.width
.. py:attribute:: Texture3D.height
.. py:attribute:: Texture3D.depth
.. py:attribute:: Texture3D.size
.. py:attribute:: Texture3D.dtype
.. py:attribute:: Texture3D.components

.. py:attribute:: Texture3D.ctx
    :type: Context

    The context this object belongs to

.. py:attribute:: Texture3D.glo
    :type: int

    The internal OpenGL object.
    This values is provided for interoperability and debug purposes only.

.. py:attribute:: Texture3D.extra
    :type: Any

    User defined data.
