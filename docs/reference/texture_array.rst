TextureArray
============

.. py:class:: TextureArray

    Returned by :py:meth:`Context.texture_array`

    An Array Texture is a Texture where each mipmap level contains an array of images of the same size.

    Array textures may have Mipmaps, but each mipmap
    in the texture has the same number of levels.

    A TextureArray object cannot be instantiated directly, it requires a context.
    Use :py:meth:`Context.texture_array` to create one.

Methods
-------

.. py:method:: TextureArray.read
.. py:method:: TextureArray.read_into
.. py:method:: TextureArray.write
.. py:method:: TextureArray.bind_to_image
.. py:method:: TextureArray.build_mipmaps
.. py:method:: TextureArray.use
.. py:method:: TextureArray.release
.. py:method:: TextureArray.get_handle

Attributes
----------

.. py:attribute:: TextureArray.repeat_x
.. py:attribute:: TextureArray.repeat_y
.. py:attribute:: TextureArray.filter
.. py:attribute:: TextureArray.swizzle
.. py:attribute:: TextureArray.anisotropy
.. py:attribute:: TextureArray.width
.. py:attribute:: TextureArray.height
.. py:attribute:: TextureArray.layers
.. py:attribute:: TextureArray.size
.. py:attribute:: TextureArray.dtype
.. py:attribute:: TextureArray.components

.. py:attribute:: TextureArray.ctx
    :type: Context

    The context this object belongs to

.. py:attribute:: TextureArray.glo
    :type: int

    The internal OpenGL object.
    This values is provided for interoperability and debug purposes only.

.. py:attribute:: TextureArray.extra
    :type: Any

    User defined data.
