Program
=======

.. py:class:: Program

    Returned by :py:meth:`Context.program`

    A Program object represents fully processed executable code in the OpenGL Shading Language, \
    for one or more Shader stages.

    In ModernGL, a Program object can be assigned to :py:class:`VertexArray` objects.
    The VertexArray object  is capable of binding the Program object once the
    :py:meth:`VertexArray.render` or :py:meth:`VertexArray.transform` is called.

    Program objects has no method called ``use()``, VertexArrays encapsulate this mechanism.

    A Program object cannot be instantiated directly, it requires a context.
    Use :py:meth:`Context.program` to create one.

    Uniform buffers can be bound using :py:meth:`Buffer.bind_to_uniform_block`
    or can be set individually. For more complex binding yielding higher
    performance consider using :py:class:`moderngl.Scope`.

Methods
-------

.. py:method:: Program.get(key: str, default: Any) -> Uniform | UniformBlock | Attribute | Varying

    Returns a Uniform, UniformBlock, Attribute or Varying.

    :param default: This is the value to be returned in case key does not exist.

.. py:method:: Program.__getitem__

    Get a member such as uniforms, uniform blocks, attributes and varyings by name.

    .. code-block:: python

        # Get a uniform
        uniform = program['color']

        # Uniform values can be set on the returned object
        # or the `__setitem__` shortcut can be used.
        program['color'].value = 1.0, 1.0, 1.0, 1.0

        # Still when writing byte data we need to use the `write()` method
        program['color'].write(buffer)

.. py:method:: Program.__setitem__

    Set a value of uniform or uniform block.

    .. code-block:: python

        # Set a vec4 uniform
        uniform['color'] = 1.0, 1.0, 1.0, 1.0

        # Optionally we can store references to a member and set the value directly
        uniform = program['color']
        uniform.value = 1.0, 0.0, 0.0, 0.0

        uniform = program['cameraMatrix']
        uniform.write(camera_matrix)

.. py:method:: Program.__iter__

    Yields the internal members names as strings.

    This includes all members such as uniforms, attributes etc.

    Example::

        # Print member information
        for name in program:
            member = program[name]
            print(name, type(member), member)

    Output::

        vert <class 'moderngl.program_members.attribute.Attribute'> <Attribute: 0>
        vert_color <class 'moderngl.program_members.attribute.Attribute'> <Attribute: 1>
        gl_InstanceID <class 'moderngl.program_members.attribute.Attribute'> <Attribute: -1>
        rotation <class 'moderngl.program_members.uniform.Uniform'> <Uniform: 0>
        scale <class 'moderngl.program_members.uniform.Uniform'> <Uniform: 1>

    We can filter on member type if needed::

        for name in prog:
            member = prog[name]
            if isinstance(member, moderngl.Uniform):
                print('Uniform', name, member)

    or a less verbose version using dict comprehensions::

        uniforms = {name: self.prog[name] for name in self.prog
                    if isinstance(self.prog[name], moderngl.Uniform)}
        print(uniforms)

    Output::

        {'rotation': <Uniform: 0>, 'scale': <Uniform: 1>}

.. py:method:: Program.release() -> None

    Release the ModernGL object.

Attributes
----------

.. py:attribute:: Program.geometry_input
    :type: int

    The geometry input primitive.

    The GeometryShader's input primitive if the GeometryShader exists.
    The geometry input primitive will be used for validation.
    (from ``layout(input_primitive) in;``)

    This can only be ``POINTS``, ``LINES``, ``LINES_ADJACENCY``, ``TRIANGLES``, ``TRIANGLE_ADJACENCY``.

.. py:attribute:: Program.geometry_output
    :type: int

    The geometry output primitive.

    The GeometryShader's output primitive if the GeometryShader exists.
    This can only be ``POINTS``, ``LINE_STRIP`` and ``TRIANGLE_STRIP``
    (from ``layout(output_primitive, max_vertices = vert_count) out;``)

.. py:attribute:: Program.geometry_vertices
    :type: int

    The maximum number of vertices that the geometry shader will output.
    (from ``layout(output_primitive, max_vertices = vert_count) out;``)

.. py:attribute:: Program.is_transform
    :type: int

    If this is a tranform program (no fragment shader).

.. py:attribute:: Program.ctx
    :type: Context

    The context this object belongs to

.. py:attribute:: Program.glo
    :type: int

    The internal OpenGL object.
    This values is provided for interoperability and debug purposes only.

.. py:attribute:: Program.extra
    :type: Any

    User defined data.

Examples
--------

.. rubric:: A simple program designed for rendering

.. code-block:: python
    :linenos:

    my_render_program = ctx.program(
        vertex_shader='''
            #version 330

            in vec2 vert;

            void main() {
                gl_Position = vec4(vert, 0.0, 1.0);
            }
        ''',
        fragment_shader='''
            #version 330

            out vec4 color;

            void main() {
                color = vec4(0.3, 0.5, 1.0, 1.0);
            }
        ''',
    )

.. rubric:: A simple program designed for transforming

.. code-block:: python
    :linenos:

    my_transform_program = ctx.program(
        vertex_shader='''
            #version 330

            in vec4 vert;
            out float vert_length;

            void main() {
                vert_length = length(vert);
            }
        ''',
        varyings=['vert_length']
    )

Program Members
---------------

.. toctree::
    :maxdepth: 2

    uniform.rst
    uniform_block.rst
    storage_block.rst
    attribute.rst
    varying.rst
