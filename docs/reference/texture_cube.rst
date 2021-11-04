TextureCube
===========

.. py:module:: moderngl
.. py:currentmodule:: moderngl

.. autoclass:: moderngl.TextureCube

Create
------

.. automethod:: Context.texture_cube(size, components, data=None, alignment=1, dtype='f1') -> TextureCube
    :noindex:

Methods
-------

.. automethod:: TextureCube.read(face, alignment=1) -> bytes
.. automethod:: TextureCube.read_into(buffer, face, alignment=1, write_offset=0)
.. automethod:: TextureCube.write(face, data, viewport=None, alignment=1)
.. automethod:: TextureCube.bind_to_image(unit: int, read: bool = True, write: bool = True, level: int = 0, format: int = 0)
.. automethod:: TextureCube.use(location=0)
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
