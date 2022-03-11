Renderbuffer
============

.. py:currentmodule:: moderngl

.. autoclass:: moderngl.Renderbuffer

Create
------

.. automethod:: Context.renderbuffer(size: Tuple[int, int], components: int = 4, samples: int = 0, dtype: str = 'f1') -> Renderbuffer
    :noindex:

.. automethod:: Context.depth_renderbuffer(size: Tuple[int, int], samples: int = 0) -> Renderbuffer
    :noindex:

Methods
-------

.. automethod:: Renderbuffer.release()

Attributes
----------

.. autoattribute:: Renderbuffer.width
.. autoattribute:: Renderbuffer.height
.. autoattribute:: Renderbuffer.size
.. autoattribute:: Renderbuffer.samples
.. autoattribute:: Renderbuffer.components
.. autoattribute:: Renderbuffer.depth
.. autoattribute:: Renderbuffer.dtype
.. autoattribute:: Renderbuffer.glo
.. autoattribute:: Renderbuffer.mglo
.. autoattribute:: Renderbuffer.extra
.. autoattribute:: Renderbuffer.ctx

.. toctree::
    :maxdepth: 2
