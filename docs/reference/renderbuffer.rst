Renderbuffer
============

.. py:class:: Renderbuffer

    Returned by :py:meth:`Context.renderbuffer` or :py:meth:`Context.depth_renderbuffer`

    Renderbuffer objects are OpenGL objects that contain images.

    They are created and used specifically with :py:class:`Framebuffer` objects.
    They are optimized for use as render targets, while :py:class:`Texture` objects
    may not be, and are the logical choice when you do not need to sample
    from the produced image. If you need to resample, use Textures instead.
    Renderbuffer objects also natively accommodate multisampling.

    A Renderbuffer object cannot be instantiated directly, it requires a context.
    Use :py:meth:`Context.renderbuffer` or :py:meth:`Context.depth_renderbuffer`
    to create one.

Methods
-------

.. py:method:: Renderbuffer.release

Attributes
----------

.. py:attribute:: Renderbuffer.width
    :type: int

    The width of the renderbuffer.

.. py:attribute:: Renderbuffer.height
    :type: int

    The height of the renderbuffer.

.. py:attribute:: Renderbuffer.size
    :type: Tuple[int, int]

    The size of the renderbuffer.

.. py:attribute:: Renderbuffer.samples
    :type: int

    The number of samples for multisampling.

.. py:attribute:: Renderbuffer.components
    :type: int

    The number components.

.. py:attribute:: Renderbuffer.depth
    :type: bool

    Determines if the renderbuffer contains depth values.

.. py:attribute:: Renderbuffer.dtype
    :type: str

    Data type.

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
