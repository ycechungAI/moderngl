Sampler
=======

.. py:currentmodule:: moderngl

.. autoclass:: moderngl.Sampler

Create
------

.. automethod:: Context.sampler(repeat_x: bool = True, repeat_y: bool = True, repeat_z: bool = True, filter: Tuple[int, int] = None, anisotropy: float = 1.0, compare_func: str = '?', border_color: Tuple[float, float, float, float] = None, min_lod: float = -1000.0, max_lod: float = 1000.0, texture: Optional[Texture] = None) -> Sampler

Methods
-------

.. automethod:: Sampler.use(location: int = 0)
.. automethod:: Sampler.clear(location: int = 0)
.. automethod:: Sampler.assign(index: int) -> Tuple[ForwardRef('Sampler'), int]
.. automethod:: Sampler.release()

Attributes
----------

.. autoattribute:: Sampler.texture
.. autoattribute:: Sampler.repeat_x
.. autoattribute:: Sampler.repeat_y
.. autoattribute:: Sampler.repeat_z
.. autoattribute:: Sampler.filter
.. autoattribute:: Sampler.compare_func
.. autoattribute:: Sampler.anisotropy
.. autoattribute:: Sampler.border_color
.. autoattribute:: Sampler.min_lod
.. autoattribute:: Sampler.max_lod
.. autoattribute:: Sampler.extra
.. autoattribute:: Sampler.mglo
.. autoattribute:: Sampler.ctx
