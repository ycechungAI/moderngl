Texture
=======

.. py:module:: moderngl
.. py:currentmodule:: moderngl

.. autoclass:: moderngl.Texture

Create
------

.. automethod:: Context.texture(size, components, data=None, samples=0, alignment=1, dtype='f1', internal_format=None) -> Texture
    :noindex:

.. automethod:: Context.depth_texture(size, data=None, samples=0, alignment=4) -> Texture
    :noindex:

Methods
-------

.. automethod:: Texture.read(level=0, alignment=1) -> bytes
.. automethod:: Texture.read_into(buffer, level=0, alignment=1, write_offset=0)
.. automethod:: Texture.write(data, viewport=None, level=0, alignment=1)
.. automethod:: Texture.build_mipmaps(base=0, max_level=1000)
.. automethod:: Texture.bind_to_image(unit: int, read: bool = True, write: bool = True, level: int = 0, format: int = 0)
.. automethod:: Texture.use(location=0)
.. automethod:: Texture.release()

Attributes
----------

.. autoattribute:: Texture.repeat_x
.. autoattribute:: Texture.repeat_y
.. autoattribute:: Texture.filter
.. autoattribute:: Texture.swizzle
.. autoattribute:: Texture.compare_func
.. autoattribute:: Texture.anisotropy
.. autoattribute:: Texture.width
.. autoattribute:: Texture.height
.. autoattribute:: Texture.size
.. autoattribute:: Texture.dtype
.. autoattribute:: Texture.components
.. autoattribute:: Texture.samples
.. autoattribute:: Texture.depth
.. autoattribute:: Texture.glo
.. autoattribute:: Texture.mglo
.. autoattribute:: Texture.extra
.. autoattribute:: Texture.ctx

.. toctree::
    :maxdepth: 2
