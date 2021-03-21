Framebuffer
===========

.. py:module:: moderngl
.. py:currentmodule:: moderngl

.. autoclass:: moderngl.Framebuffer

Create
------

.. automethod:: Context.simple_framebuffer(size, components=4, samples=0, dtype='f1') -> Framebuffer
    :noindex:

.. automethod:: Context.framebuffer(color_attachments=(), depth_attachment=None) -> Framebuffer
    :noindex:

Methods
-------

.. automethod:: Framebuffer.clear(red=0.0, green=0.0, blue=0.0, alpha=0.0, depth=1.0, viewport=None, color=None)
.. automethod:: Framebuffer.read(viewport=None, components=3, attachment=0, alignment=1, dtype='f1', clamp=False) -> bytes
.. automethod:: Framebuffer.read_into(buffer, viewport=None, components=3, attachment=0, alignment=1, dtype='f1', write_offset=0)
.. automethod:: Framebuffer.use()
.. automethod:: Framebuffer.release()

Attributes
----------

.. autoattribute:: Framebuffer.viewport
.. autoattribute:: Framebuffer.scissor
.. autoattribute:: Framebuffer.color_mask
.. autoattribute:: Framebuffer.depth_mask
.. autoattribute:: Framebuffer.width
.. autoattribute:: Framebuffer.height
.. autoattribute:: Framebuffer.size
.. autoattribute:: Framebuffer.samples
.. autoattribute:: Framebuffer.bits
.. autoattribute:: Framebuffer.color_attachments
.. autoattribute:: Framebuffer.depth_attachment
.. autoattribute:: Framebuffer.glo
.. autoattribute:: Framebuffer.mglo
.. autoattribute:: Framebuffer.extra
.. autoattribute:: Framebuffer.ctx

.. toctree::
    :maxdepth: 2
