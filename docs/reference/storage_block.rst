StorageBlock
============

.. py:class:: StorageBlock

    Available in :py:meth:`Program.__getitem__`

    Storage Blocks are OpenGL 4.3+ Program accessible data blocks.
    Compared to UniformBlocks they can be larger in size and also support write operations.
    For less than one page (64KB) read-only data use UniformBlocks.

.. py:attribute:: StorageBlock.binding
    :type: int

    The binding of the Storage block. Same as the value.

.. py:attribute:: StorageBlock.value
    :type: int

    The value of the Storage block. Same as the binding.

.. py:attribute:: StorageBlock.name
    :type: str

    The name of the Storage block.

.. py:attribute:: StorageBlock.index
    :type: int

    The index of the Storage block.

.. py:attribute:: StorageBlock.size
    :type: int

    The size of the Storage block.

.. py:attribute:: StorageBlock.extra
    :type: Any

    User defined data.
