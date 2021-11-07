Sampler
=======

.. py:currentmodule:: moderngl

.. autoclass:: moderngl.Sampler

Create
------

.. automethod:: Context.sampler(repeat_x=True, repeat_y=True, repeat_z=True, filter=None, anisotropy=1.0, compare_func='?', border_color=None, min_lod=-1000.0, max_lod=1000.0, texture=None) -> Sampler

Methods
-------

.. automethod:: Sampler.use(location=0)
.. automethod:: Sampler.clear(location=0)
.. automethod:: Sampler.assign(index)
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
