VertexArray
===========

.. py:class:: VertexArray

    A VertexArray object is an OpenGL object that stores all of the state needed to supply vertex data.

    It stores the format of the vertex data
    as well as the Buffer objects providing the vertex data arrays.

    In ModernGL, the VertexArray object also stores a reference
    for a :py:class:`Program` object.

    A VertexArray object cannot be instantiated directly, it requires a context.
    Use :py:meth:`Context.vertex_array` or :py:meth:`Context.simple_vertex_array`
    to create one.

    Compared to OpenGL, :py:class:`VertexArray` also stores a :py:class:`Program` object.

Create
------

.. py:method:: Context.simple_vertex_array
   :noindex:

.. py:method:: Context.vertex_array
   :noindex:

Methods
-------

.. py:method:: VertexArray.render(mode: int | None = None, vertices: int = -1, first: int = 0, instances: int = -1) -> None

    The render primitive (mode) must be the same as the input primitive of the GeometryShader.

    Args:
        mode (int): By default :py:data:`TRIANGLES` will be used.
        vertices (int): The number of vertices to transform.

    Keyword Args:
        first (int): The index of the first vertex to start with.
        instances (int): The number of instances.

.. py:method:: VertexArray.render_indirect(buffer: Buffer, mode: int | None = None, count: int = -1, first: int = 0) -> None

    The render primitive (mode) must be the same as the input primitive of the GeometryShader.

    The draw commands are 5 integers: (count, instanceCount, firstIndex, baseVertex, baseInstance).

    Args:
        buffer (Buffer): Indirect drawing commands.
        mode (int): By default :py:data:`TRIANGLES` will be used.
        count (int): The number of draws.

    Keyword Args:
        first (int): The index of the first indirect draw command.

.. py:method:: VertexArray.transform(buffer: Buffer | List[Buffer], mode: int | None = None, vertices: int = -1, first: int = 0, instances: int = -1, buffer_offset: int = 0) -> None

    Transform vertices.

    Stores the output in a single buffer.
    The transform primitive (mode) must be the same as
    the input primitive of the GeometryShader.

    Args:
        buffer (Buffer): The buffer to store the output.
        mode (int): By default :py:data:`POINTS` will be used.
        vertices (int): The number of vertices to transform.

    Keyword Args:
        first (int): The index of the first vertex to start with.
        instances (int): The number of instances.
        buffer_offset (int): Byte offset for the output buffer

.. py:method:: VertexArray.bind(attribute: int, cls: str, buffer: Buffer, fmt: str, offset: int = 0, stride: int = 0, divisor: int = 0, normalize: bool = False)

    Bind individual attributes to buffers.

    Args:
        location (int): The attribute location.
        cls (str): The attribute class. Valid values are ``f``, ``i`` or ``d``.
        buffer (Buffer): The buffer.
        format (str): The buffer format.

    Keyword Args:
        offset (int): The offset.
        stride (int): The stride.
        divisor (int): The divisor.
        normalize (bool): The normalize parameter, if applicable.

.. py:method:: VertexArray.release() -> None

    Release the ModernGL object.

Attributes
----------

.. py:attribute:: VertexArray.mode
    :type: int

    Get or set the default rendering mode.

    This value is used when ``mode`` is not passed in rendering calls.

    Examples::

        vao.mode = moderngl.TRIANGLE_STRIPS

.. py:attribute:: VertexArray.program
    :type: Program

    The program assigned to the VertexArray.
    The program used when rendering or transforming primitives.

.. py:attribute:: VertexArray.index_buffer
    :type: Buffer

    The index buffer if the index_buffer is set, otherwise ``None``.

.. py:attribute:: VertexArray.index_element_size
    :type: int

    The byte size of each element in the index buffer.

.. py:attribute:: VertexArray.scope
    :type: Scope

    The scope to use while rendering.

.. py:attribute:: VertexArray.vertices
    :type: int

    The number of vertices detected.

    This is the minimum of the number of vertices possible per Buffer.
    The size of the index_buffer determines the number of vertices.
    Per instance vertex attributes does not affect this number.

.. py:attribute:: VertexArray.instances
    :type: int

    Get or set the number of instances to render.

.. py:attribute:: VertexArray.ctx
    :type: Context

    The context this object belongs to

.. py:attribute:: VertexArray.glo
    :type: int

    The internal OpenGL object.

    This values is provided for debug purposes only.

.. py:attribute:: VertexArray.extra
    :type: Any

    User defined data.

.. toctree::
    :maxdepth: 2
