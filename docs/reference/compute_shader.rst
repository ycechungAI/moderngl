ComputeShader
=============

.. py:class:: ComputeShader

    A Compute Shader is a Shader Stage that is used entirely for computing arbitrary information.

    While it can do rendering, it is generally used
    for tasks not directly related to drawing.

    - Compute shaders support uniforms similar to :py:class:`moderngl.Program` objects.
    - Storage buffers can be bound using :py:meth:`Buffer.bind_to_storage_buffer`.
    - Uniform buffers can be bound using :py:meth:`Buffer.bind_to_uniform_block`.
    - Images can be bound using :py:meth:`Texture.bind_to_image`.

Create
------

.. py:method:: Context.compute_shader
   :noindex:

Methods
-------

.. py:method:: ComputeShader.run(group_x: int = 1, group_y: int = 1, group_z: int = 1) -> None:

    Run the compute shader.

.. py:method:: run_indirect(self, buffer: Buffer, offset: int = 0) -> None:

    Run the compute shader indirectly from a buffer object.

.. py:method:: ComputeShader.get(key, default)

    Returns a Uniform, UniformBlock or StorageBlock.

    Args:
        default: This is the value to be returned in case key does not exist.

    Returns:
        :py:class:`Uniform`, :py:class:`UniformBlock` or :py:class:`StorageBlock`

.. py:method:: ComputeShader.__getitem__(key)

    Get a member such as uniforms, uniform blocks and storage blocks.

    .. code-block:: python

        # Get a uniform
        uniform = program['color']

        # Uniform values can be set on the returned object
        # or the `__setitem__` shortcut can be used.
        program['color'].value = 1.0, 1.0, 1.0, 1.0

        # Still when writing byte data we need to use the `write()` method
        program['color'].write(buffer)

        # Set binding for a storage block (if supported)
        program['DataBlock'].binding = 0

.. py:method:: ComputeShader.__setitem__(key, value)

    Set a value of uniform or uniform block.

    .. code-block:: python

        # Set a vec4 uniform
        uniform['color'] = 1.0, 1.0, 1.0, 1.0

        # Optionally we can store references to a member and set the value directly
        uniform = program['color']
        uniform.value = 1.0, 0.0, 0.0, 0.0

        uniform = program['cameraMatrix']
        uniform.write(camera_matrix)

        # Set binding for a storage block (if supported)
        program['DataBlock'].binding = 0

.. py:method:: ComputeShader.__iter__()

    Yields the internal members names as strings.

    Example::

        for member in program:
            obj = program[member]
            print(member, obj)
            if isinstance(obj, moderngl.StorageBlock):
                print('This is a storage block member')

    This includes all members such as uniforms, uniform blocks and storage blocks.

Attributes
----------

.. py:attribute:: ComputeShader.ctx
    :type: Context

    The context this object belongs to

.. py:attribute:: ComputeShader.glo
    :type: int

    The internal OpenGL object.
    This values is provided for interoperability and debug purposes only.

.. py:attribute:: ComputeShader.extra
    :type: Any

    User defined data.

.. toctree::
    :maxdepth: 2
