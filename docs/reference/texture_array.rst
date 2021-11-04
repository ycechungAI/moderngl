TextureArray
============

.. py:module:: moderngl
.. py:currentmodule:: moderngl

.. autoclass:: moderngl.TextureArray

Create
------

.. automethod:: Context.texture_array(size, components, data=None, alignment=1, dtype='f1') -> TextureArray
    :noindex:

Methods
-------

.. automethod:: TextureArray.read(alignment=1) -> bytes
.. automethod:: TextureArray.read_into(buffer, alignment=1, write_offset=0)
.. automethod:: TextureArray.write(data, viewport=None, alignment=1)
.. automethod:: TextureArray.bind_to_image(unit: int, read: bool = True, write: bool = True, level: int = 0, format: int = 0)
.. automethod:: TextureArray.build_mipmaps(base=0, max_level=1000)
.. automethod:: TextureArray.use(location=0)
.. automethod:: TextureArray.release()

Attributes
----------

.. autoattribute:: TextureArray.repeat_x
.. autoattribute:: TextureArray.repeat_y
.. autoattribute:: TextureArray.filter
.. autoattribute:: TextureArray.swizzle
.. autoattribute:: TextureArray.anisotropy
.. autoattribute:: TextureArray.width
.. autoattribute:: TextureArray.height
.. autoattribute:: TextureArray.layers
.. autoattribute:: TextureArray.size
.. autoattribute:: TextureArray.dtype
.. autoattribute:: TextureArray.components
.. autoattribute:: TextureArray.glo
.. autoattribute:: TextureArray.mglo
.. autoattribute:: TextureArray.extra
.. autoattribute:: TextureArray.ctx

.. toctree::
    :maxdepth: 2
