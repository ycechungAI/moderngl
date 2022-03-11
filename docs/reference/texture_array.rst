TextureArray
============

.. py:currentmodule:: moderngl

.. autoclass:: moderngl.TextureArray

Create
------

.. automethod:: Context.texture_array(size: Tuple[int, int, int], components: int, data: Optional[Any] = None, alignment: int = 1, dtype: str = 'f1') -> TextureArray
    :noindex:

Methods
-------

.. automethod:: TextureArray.read(alignment: int = 1) -> bytes
.. automethod:: TextureArray.read_into(buffer: Any, alignment: int = 1, write_offset: int = 0)
.. automethod:: TextureArray.write(data: Any, viewport: Union[Tuple[int, int, int], Tuple[int, int, int, int, int, int], NoneType] = None, alignment: int = 1)
.. automethod:: TextureArray.bind_to_image(unit: int, read: bool = True, write: bool = True, level: int = 0, format: int = 0)
.. automethod:: TextureArray.build_mipmaps(base: int = 0, max_level: int = 1000)
.. automethod:: TextureArray.use(location: int = 0)
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
