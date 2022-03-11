Texture3D
=========

.. py:currentmodule:: moderngl

.. autoclass:: moderngl.Texture3D

Create
------

.. automethod:: Context.texture3d(size: Tuple[int, int, int], components: int, data: Optional[Any] = None, alignment: int = 1, dtype: str = 'f1') -> Texture3D
    :noindex:

Methods
-------

.. automethod:: Texture3D.read(alignment: int = 1) -> bytes
.. automethod:: Texture3D.read_into(buffer: Any, alignment: int = 1, write_offset: int = 0)
.. automethod:: Texture3D.write(data: Any, viewport: Union[Tuple[int, int, int], Tuple[int, int, int, int, int, int], NoneType] = None, alignment: int = 1)
.. automethod:: Texture3D.build_mipmaps(base: int = 0, max_level: int = 1000)
.. automethod:: Texture3D.bind_to_image(unit: int, read: bool = True, write: bool = True, level: int = 0, format: int = 0)
.. automethod:: Texture3D.use(location: int = 0)
.. automethod:: Texture3D.release()

Attributes
----------

.. autoattribute:: Texture3D.repeat_x
.. autoattribute:: Texture3D.repeat_y
.. autoattribute:: Texture3D.repeat_z
.. autoattribute:: Texture3D.filter
.. autoattribute:: Texture3D.swizzle
.. autoattribute:: Texture3D.width
.. autoattribute:: Texture3D.height
.. autoattribute:: Texture3D.depth
.. autoattribute:: Texture3D.size
.. autoattribute:: Texture3D.dtype
.. autoattribute:: Texture3D.components
.. autoattribute:: Texture3D.glo
.. autoattribute:: Texture3D.mglo
.. autoattribute:: Texture3D.extra
.. autoattribute:: Texture3D.ctx

.. toctree::
    :maxdepth: 2
