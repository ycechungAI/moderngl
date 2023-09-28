Renderbuffer
============

.. py:class:: Renderbuffer

    Renderbuffer objects are OpenGL objects that contain images.

    They are created and used specifically with :py:class:`Framebuffer` objects.
    They are optimized for use as render targets, while :py:class:`Texture` objects
    may not be, and are the logical choice when you do not need to sample
    from the produced image. If you need to resample, use Textures instead.
    Renderbuffer objects also natively accommodate multisampling.

    A Renderbuffer object cannot be instantiated directly, it requires a context.
    Use :py:meth:`Context.renderbuffer` or :py:meth:`Context.depth_renderbuffer`
    to create one.

Create
------

.. py:method:: Context.renderbuffer
   :noindex:

.. py:method:: Context.depth_renderbuffer
   :noindex:

Methods
-------

.. py:method:: Renderbuffer.release

Attributes
----------

.. py:attribute:: Renderbuffer.width
.. py:attribute:: Renderbuffer.height
.. py:attribute:: Renderbuffer.size
.. py:attribute:: Renderbuffer.samples
.. py:attribute:: Renderbuffer.components
.. py:attribute:: Renderbuffer.depth
.. py:attribute:: Renderbuffer.dtype

.. py:attribute:: Renderbuffer.ctx
    :type: Context

    The context this object belongs to

.. py:attribute:: Renderbuffer.glo
    :type: int

    The internal OpenGL object.
    This values is provided for interoperability and debug purposes only.


.. py:attribute:: Renderbuffer.extra
    :type: Any

    User defined data.

.. toctree::
    :maxdepth: 2
