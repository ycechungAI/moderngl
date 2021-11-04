Texture3D
=========

.. py:module:: moderngl
.. py:currentmodule:: moderngl

.. autoclass:: moderngl.Texture3D

Create
------

.. automethod:: Context.texture3d(size, components, data=None, alignment=1, dtype='f1') -> Texture3D
    :noindex:

Methods
-------

.. automethod:: Texture3D.read(alignment=1) -> bytes
.. automethod:: Texture3D.read_into(buffer, alignment=1, write_offset=0)
.. automethod:: Texture3D.write(data, viewport=None, alignment=1)
.. automethod:: Texture3D.build_mipmaps(base=0, max_level=1000)
.. automethod:: Texture3D.bind_to_image(unit: int, read: bool = True, write: bool = True, level: int = 0, format: int = 0)
.. automethod:: Texture3D.use(location=0)
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
