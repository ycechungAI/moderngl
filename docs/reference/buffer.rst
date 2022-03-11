Buffer
======

.. py:module:: moderngl
.. py:currentmodule:: moderngl

.. autoclass:: moderngl.Buffer

Create
------

.. automethod:: Context.buffer(data: Optional[Any] = None, reserve: int = 0, dynamic: bool = False) -> Buffer
    :noindex:

Methods
-------

.. automethod:: Buffer.assign(index: int) -> Tuple[ForwardRef('Buffer'), int]
.. automethod:: Buffer.bind(*attribs, layout=None)
.. automethod:: Buffer.write(data: Any, offset: int = 0)
.. automethod:: Buffer.write_chunks(data: Any, start: int, step: int, count: int)
.. automethod:: Buffer.read(size: int = -1, offset: int = 0) -> bytes
.. automethod:: Buffer.read_into(buffer: Any, size: int = -1, offset: int = 0, write_offset: int = 0)
.. automethod:: Buffer.read_chunks(chunk_size: int, start: int, step: int, count: int) -> bytes
.. automethod:: Buffer.read_chunks_into(buffer: Any, chunk_size: int, start: int, step: int, count: int, write_offset: int = 0)
.. automethod:: Buffer.clear(size: int = -1, offset: int = 0, chunk: Any = None)
.. automethod:: Buffer.bind_to_uniform_block(binding: int = 0, offset: int = 0, size: int = -1)
.. automethod:: Buffer.bind_to_storage_buffer(binding: int = 0, offset: int = 0, size: int = -1)
.. automethod:: Buffer.orphan(size: int = -1)
.. automethod:: Buffer.release()


Attributes
----------

.. autoattribute:: Buffer.size
.. autoattribute:: Buffer.dynamic
.. autoattribute:: Buffer.glo
.. autoattribute:: Buffer.mglo
.. autoattribute:: Buffer.extra
.. autoattribute:: Buffer.ctx

.. toctree::
    :maxdepth: 2
