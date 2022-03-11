TextureCube
===========

.. py:currentmodule:: moderngl

.. autoclass:: moderngl.TextureCube

Create
------

.. automethod:: Context.texture_cube(size: Tuple[int, int], components: int, data: Optional[Any] = None, alignment: int = 1, dtype: str = 'f1') -> TextureCube
    :noindex:

Methods
-------

.. automethod:: TextureCube.read(face: int, alignment: int = 1) -> bytes
.. automethod:: TextureCube.read_into(buffer: Any, face: int, alignment: int = 1, write_offset: int = 0)
.. automethod:: TextureCube.write(face: int, data: Any, viewport: Union[Tuple[int, int], Tuple[int, int, int, int], NoneType] = None, alignment: int = 1)
.. automethod:: TextureCube.bind_to_image(unit: int, read: bool = True, write: bool = True, level: int = 0, format: int = 0)
.. automethod:: TextureCube.use(location: int = 0)
.. automethod:: TextureCube.release()

Attributes
----------

.. autoattribute:: TextureCube.size
.. autoattribute:: TextureCube.dtype
.. autoattribute:: TextureCube.components
.. autoattribute:: TextureCube.filter
.. autoattribute:: TextureCube.swizzle
.. autoattribute:: TextureCube.anisotropy
.. autoattribute:: TextureCube.glo
.. autoattribute:: TextureCube.mglo
.. autoattribute:: TextureCube.extra
.. autoattribute:: TextureCube.ctx

.. toctree::
    :maxdepth: 2
