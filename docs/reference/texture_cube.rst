TextureCube
===========

.. py:class:: TextureCube

    Returned by :py:meth:`Context.texture_cube` and :py:meth:`Context.depth_texture_cube`

    Cubemaps are a texture using the type GL_TEXTURE_CUBE_MAP.

    They are similar to 2D textures in that they have two dimensions.
    However, each mipmap level has 6 faces, with each face having the
    same size as the other faces.

    The width and height of a cubemap must be the same (ie: cubemaps are squares),
    but these sizes need not be powers of two.

    .. Note:: ModernGL enables ``GL_TEXTURE_CUBE_MAP_SEAMLESS`` globally
                to ensure filtering will be done across the cube faces.

    A Texture3D object cannot be instantiated directly, it requires a context.
    Use :py:meth:`Context.texture_cube` to create one.

Methods
-------

.. py:method:: TextureCube.read
.. py:method:: TextureCube.read_into
.. py:method:: TextureCube.write
.. py:method:: TextureCube.bind_to_image
.. py:method:: TextureCube.use
.. py:method:: TextureCube.release
.. py:method:: TextureCube.get_handle

Attributes
----------

.. py:attribute:: TextureCube.size
.. py:attribute:: TextureCube.dtype
.. py:attribute:: TextureCube.components
.. py:attribute:: TextureCube.filter
.. py:attribute:: TextureCube.swizzle
.. py:attribute:: TextureCube.anisotropy

.. py:attribute:: TextureCube.ctx
    :type: Context

    The context this object belongs to

.. py:attribute:: TextureCube.glo
    :type: int

    The internal OpenGL object.
    This values is provided for interoperability and debug purposes only.

.. py:attribute:: TextureCube.extra
    :type: Any

    User defined data.
