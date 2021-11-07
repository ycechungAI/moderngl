Renderbuffer
============

.. py:currentmodule:: moderngl

.. autoclass:: moderngl.Renderbuffer

Create
------

.. automethod:: Context.renderbuffer(size, components=4, samples=0, dtype='f1') -> Renderbuffer
    :noindex:

.. automethod:: Context.depth_renderbuffer(size, samples=0) -> Renderbuffer
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
