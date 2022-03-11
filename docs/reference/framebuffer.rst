Framebuffer
===========

.. py:currentmodule:: moderngl

.. autoclass:: moderngl.Framebuffer

Create
------

.. automethod:: Context.simple_framebuffer(size: Tuple[int, int], components: int = 4, samples: int = 0, dtype: str = 'f1') -> Framebuffer
    :noindex:

.. automethod:: Context.framebuffer(color_attachments: Any = (), depth_attachment: Union[Texture, Renderbuffer, NoneType] = None) -> Framebuffer
    :noindex:

Methods
-------

.. automethod:: Framebuffer.clear(red: float = 0.0, green: float = 0.0, blue: float = 0.0, alpha: float = 0.0, depth: float = 1.0, viewport: Union[Tuple[int, int], Tuple[int, int, int, int], NoneType] = None, color: Optional[Tuple[float, float, float, float]] = None)
.. automethod:: Framebuffer.read(viewport: Union[Tuple[int, int], Tuple[int, int, int, int], NoneType] = None, components: int = 3, attachment: int = 0, alignment: int = 1, dtype: str = 'f1', clamp: bool = False) -> bytes
.. automethod:: Framebuffer.read_into(buffer: Any, viewport: Union[Tuple[int, int], Tuple[int, int, int, int], NoneType] = None, components: int = 3, attachment: int = 0, alignment: int = 1, dtype: str = 'f1', write_offset: int = 0)
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
