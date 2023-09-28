Texture
=======

.. py:class:: Texture

    Returned by :py:meth:`Context.texture` and :py:meth:`Context.depth_texture`

    A Texture is an OpenGL object that contains one or more images that all have the same image format.

    A texture can be used in two ways. It can
    be the source of a texture access from a Shader, or it can be used
    as a render target.

    A Texture object cannot be instantiated directly, it requires a context.
    Use :py:meth:`Context.texture` or :py:meth:`Context.depth_texture`
    to create one.

Methods
-------

.. py:method:: Texture.read
.. py:method:: Texture.read_into
.. py:method:: Texture.write
.. py:method:: Texture.build_mipmaps
.. py:method:: Texture.bind_to_image
.. py:method:: Texture.use
.. py:method:: Texture.release
.. py:method:: Texture.get_handle

Attributes
----------

.. py:attribute:: Texture.repeat_x
.. py:attribute:: Texture.repeat_y
.. py:attribute:: Texture.filter
.. py:attribute:: Texture.swizzle
.. py:attribute:: Texture.compare_func
.. py:attribute:: Texture.anisotropy
.. py:attribute:: Texture.width
.. py:attribute:: Texture.height
.. py:attribute:: Texture.size
.. py:attribute:: Texture.dtype
.. py:attribute:: Texture.components
.. py:attribute:: Texture.samples
.. py:attribute:: Texture.depth

.. py:attribute:: Texture.ctx
    :type: Context

    The context this object belongs to

.. py:attribute:: Texture.glo
    :type: int

    The internal OpenGL object.
    This values is provided for interoperability and debug purposes only.

.. py:attribute:: Texture.extra
    :type: Any

    User defined data.
