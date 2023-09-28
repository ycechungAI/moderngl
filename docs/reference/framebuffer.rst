Framebuffer
===========

.. py:class:: Framebuffer

    A :py:class:`Framebuffer` is a collection of buffers that can be used as the destination for rendering.

    The buffers for Framebuffer objects reference images from either Textures or Renderbuffers.
    Create a :py:class:`Framebuffer` using :py:meth:`Context.framebuffer`.

Create
------

.. py:method:: Context.simple_framebuffer
   :noindex:

.. py:method:: Context.framebuffer
   :noindex:

Methods
-------

.. py:method:: Framebuffer.clear
.. py:method:: Framebuffer.read
.. py:method:: Framebuffer.read_into
.. py:method:: Framebuffer.use
.. py:method:: Framebuffer.release

Attributes
----------

.. py:attribute:: Framebuffer.viewport
    :type: tuple

    Get or set the viewport of the framebuffer.

.. py:attribute:: Framebuffer.scissor
    :type: tuple

    Get or set the scissor box of the framebuffer.

    When scissor testing is enabled fragments outside
    the defined scissor box will be discarded. This
    applies to rendered geometry or :py:meth:`Framebuffer.clear`.

    Setting is value enables scissor testing in the framebuffer.
    Setting the scissor to ``None`` disables scissor testing
    and reverts the scissor box to match the framebuffer size.

    Example::

        # Enable scissor testing
        >>> ctx.scissor = 100, 100, 200, 100
        # Disable scissor testing
        >>> ctx.scissor = None

.. py:attribute:: Framebuffer.color_mask
    :type: tuple

    The color mask of the framebuffer.

    Color masking controls what components in color attachments will be
    affected by fragment write operations.
    This includes rendering geometry and clearing the framebuffer.

    Default value: ``(True, True, True, True)``.

    Examples::

        # Block writing to all color components (rgba) in color attachments
        fbo.color_mask = False, False, False, False

        # Re-enable writing to color attachments
        fbo.color_mask = True, True, True, True

        # Block fragment writes to alpha channel
        fbo.color_mask = True, True, True, False

.. py:attribute:: Framebuffer.depth_mask
    :type: bool

    The depth mask of the framebuffer.

    Depth mask enables or disables write operations to the depth buffer.
    This also applies when clearing the framebuffer.
    If depth testing is enabled fragments will still be culled, but
    the depth buffer will not be updated with new values. This is
    a very useful tool in many rendering techniques.

    Default value: ``True``

.. py:attribute:: Framebuffer.width
    :type: int

    The width of the framebuffer.

    Framebuffers created by a window will only report its initial size.
    It's better get size information from the window itself.

.. py:attribute:: Framebuffer.height
    :type: int

    The height of the framebuffer.

    Framebuffers created by a window will only report its initial size.
    It's better get size information from the window itself.

.. py:attribute:: Framebuffer.size
    :type: Tuple[int, int]

    The size of the framebuffer.

    Framebuffers created by a window will only report its initial size.
    It's better get size information from the window itself.

.. py:attribute:: Framebuffer.samples
    :type: int

    The samples of the framebuffer.

.. py:attribute:: Framebuffer.bits
    :type: int

    The bits of the framebuffer.

.. py:attribute:: Framebuffer.color_attachments
    :type: tuple

.. py:attribute:: Framebuffer.depth_attachment
    :type: tuple

.. py:attribute:: Framebuffer.ctx
    :type: Context

    The context this object belongs to

.. py:attribute:: Framebuffer.glo
    :type: int

    The internal OpenGL object.
    This values is provided for interoperability and debug purposes only.

.. py:attribute:: Framebuffer.extra
    :type: Any

    User defined data.

.. toctree::
    :maxdepth: 2
