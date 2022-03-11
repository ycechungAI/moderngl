Texture
=======

.. py:currentmodule:: moderngl

.. autoclass:: moderngl.Texture

Create
------

.. automethod:: Context.texture(size: Tuple[int, int], components: int, data: Optional[Any] = None, samples: int = 0, alignment: int = 1, dtype: str = 'f1', internal_format: int = None) -> Texture
    :noindex:

.. automethod:: Context.depth_texture(size: Tuple[int, int], data: Optional[Any] = None, samples: int = 0, alignment: int = 4) -> Texture
    :noindex:

Methods
-------

.. automethod:: Texture.read(level: int = 0, alignment: int = 1) -> bytes
.. automethod:: Texture.read_into(buffer: Any, level: int = 0, alignment: int = 1, write_offset: int = 0)
.. automethod:: Texture.write(data: Any, viewport: Union[Tuple[int, int], Tuple[int, int, int, int], NoneType] = None, level: int = 0, alignment: int = 1)
.. automethod:: Texture.build_mipmaps(base: int = 0, max_level: int = 1000)
.. automethod:: Texture.bind_to_image(unit: int, read: bool = True, write: bool = True, level: int = 0, format: int = 0)
.. automethod:: Texture.use(location: int = 0)
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
