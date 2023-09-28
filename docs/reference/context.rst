Context
=======

.. py:class:: Context

    Class exposing OpenGL features.

    ModernGL objects can be created from this class.

Objects
-------

.. py:method:: Context.program(vertex_shader: str, fragment_shader: str, geometry_shader: str, tess_control_shader: str, tess_evaluation_shader: str, varyings: Tuple[str, ...], fragment_outputs: Dict[str, int], varyings_capture_mode: str = 'interleaved') -> Program

    Create a :py:class:`Program` object.

    The ``varyings`` are only used when a transform program is created
    to specify the names of the output varyings to capture in the output buffer.

    ``fragment_outputs`` can be used to programmatically map named fragment
    shader outputs to a framebuffer attachment numbers. This can also be done
    by using ``layout(location=N)`` in the fragment shader.

    :param str vertex_shader: The vertex shader source.
    :param str fragment_shader: The fragment shader source.
    :param str geometry_shader: The geometry shader source.
    :param str tess_control_shader: The tessellation control shader source.
    :param str tess_evaluation_shader: The tessellation evaluation shader source.
    :param list varyings: A list of varyings.
    :param dict fragment_outputs: A dictionary of fragment outputs.

.. py:method:: Context.buffer(data = None, reserve: int = 0, dynamic: bool = False) -> Buffer

    Returns a new :py:class:`Buffer` object.

    The `data` can be anything supporting the buffer interface.

    The `data` and `reserve` parameters are mutually exclusive.

    :param bytes data: Content of the new buffer.
    :param int reserve: The number of bytes to reserve.
    :param bool dynamic: Treat buffer as dynamic.

.. py:method:: Context.vertex_array(program: Program, content: list, index_buffer: Buffer = None, index_element_size: int = 4, mode: int = ...) -> VertexArray

    Returns a new :py:class:`VertexArray` object.

    A VertexArray describes how buffers are read by a shader program.
    The content is a list of tuples containing a buffer, a format string and any number of attribute names.
    Attribute names are defined by the user in the Vertex Shader program stage.

    The default `mode` is :py:attr:`~Context.TRIANGLES`.

    :param Program program: The program used when rendering
    :param list content: A list of (buffer, format, attributes). See :ref:`buffer-format-label`.
    :param Buffer index_buffer: An index buffer (optional)
    :param int index_element_size: byte size of each index element, 1, 2 or 4.
    :param bool skip_errors: Ignore errors during creation
    :param int mode: The default draw mode (for example: ``TRIANGLES``)

    Examples::

        # Empty vertext array (no attribute input)
        vao = ctx.vertex_array(program)

        # Multiple buffers
        vao = ctx.vertex_array(program, [
            (buffer1, '3f', 'in_position'),
            (buffer2, '3f', 'in_normal'),
        ])
        vao = ctx.vertex_array(
            program,
            [
                (buffer1, '3f', 'in_position'),
                (buffer2, '3f', 'in_normal'),
            ],
            index_buffer=ibo,
            index_element_size=2,  # 16 bit / 'u2' index buffer
        )

    Backward Compatible Version::

        # Simple version with a single buffer
        vao = ctx.vertex_array(program, buffer, 'in_position', 'in_normal')
        vao = ctx.vertex_array(program, buffer, 'in_position', 'in_normal', index_buffer=ibo)

.. py:method:: Context.simple_vertex_array(...)

    Deprecated, use :py:meth:`Context.vertex_array` instead.

.. py:method:: Context.texture(size: Tuple[int, int], components: int, data: Any = None, samples: int = 0, alignment: int = 1, dtype: str = 'f1') -> Texture

    Returns a new :py:class:`Texture` object.

    A Texture is a 2D image that can be used for sampler2D uniforms or as render targets if framebuffers.

    :param tuple size: The width and height of the texture.
    :param int components: The number of components 1, 2, 3 or 4.
    :param bytes data: Content of the texture.
    :param int samples: The number of samples. Value 0 means no multisample format.
    :param int alignment: The byte alignment 1, 2, 4 or 8.
    :param str dtype: Data type.
    :param int internal_format: Override the internalformat of the texture (IF needed)

    Example::

        from PIL import Image

        img = Image.open(...).convert('RGBA')
        texture = ctx.texture(img.size, components=4, data=img.tobytes())

        # float texture
        texture = ctx.texture((64, 64), components=..., dtype='f4')

        # integer texture
        texture = ctx.texture((64, 64), components=..., dtype='i4')

    .. Note:: Do not play with ``internal_format`` unless you know exactly
                    you are doing. This is an override to support sRGB and
                    compressed textures if needed.

.. py:method:: Context.framebuffer(color_attachments: List[Texture], depth_attachment: Texture = None) -> Framebuffer

    Returns a new :py:class:`Framebuffer` object.

    A Framebuffer is a collection of images that can be used as render targets.
    The images of the Framebuffer object can be either Textures or Renderbuffers.

    :param list color_attachments: A list of :py:class:`Texture` or :py:class:`Renderbuffer` objects.
    :param Texture depth_attachment: The depth attachment.

.. py:method:: Context.sampler(repeat_x: bool, repeat_y: bool, repeat_z: bool, filter: tuple, anisotropy: float, compare_func: str, border_color: tuple, min_lod: float, max_lod: float, texture: Texture) -> Sampler

    Returns a new :py:class:`Sampler` object.

    Samplers bind Textures to uniform samplers within a Program object.
    Binding a Sampler object also binds the texture object attached to it.

    :param bool repeat_x: Repeat texture on x
    :param bool repeat_y: Repeat texture on y
    :param bool repeat_z: Repeat texture on z
    :param tuple filter: The min and max filter
    :param float anisotropy: Number of samples for anisotropic filtering. Any value greater than 1.0 counts as a use of anisotropic filtering
    :param str compare_func: Compare function for depth textures
    :param tuple border_color: The (r, g, b, a) color for the texture border. When this value is set the ``repeat_`` values are overridden setting the texture wrap to return the border color when outside ``[0, 1]`` range.
    :param float min_lod: Minimum level-of-detail parameter (Default ``-1000.0``). This floating-point value limits the selection of highest resolution mipmap (lowest mipmap level)
    :param float max_lod: Minimum level-of-detail parameter (Default ``1000.0``). This floating-point value limits the selection of the lowest resolution mipmap (highest mipmap level)
    :param Texture texture: The texture for this sampler

.. py:method:: Context.depth_texture(size: Tuple[int, int], data: Any = None, samples: int = 0, alignment: int = 4) -> Texture

    Returns a new :py:class:`Texture` object.

    A depth texture can be used for sampler2D and sampler2DShadow uniforms and as a depth attachment for framebuffers.

    :param tuple size: The width and height of the texture.
    :param bytes data: Content of the texture.
    :param int samples: The number of samples. Value 0 means no multisample format.
    :param int alignment: The byte alignment 1, 2, 4 or 8.

.. py:method:: Context.texture3d(size: Tuple[int, int, int], components: int, data: Any = None, alignment: int = 1, dtype: str = 'f1') -> Texture3D

    Returns a new :py:class:`Texture3D` object.

    :param tuple size: The width, height and depth of the texture.
    :param int components: The number of components 1, 2, 3 or 4.
    :param bytes data: Content of the texture.
    :param int alignment: The byte alignment 1, 2, 4 or 8.
    :param str dtype: Data type.

.. py:method:: Context.texture_array(size: Tuple[int, int, int], components: int, data: Any = None, *, alignment: int = 1, dtype: str = 'f1') -> TextureArray

    Returns a new :py:class:`TextureArray` object.

    :param tuple size: The ``(width, height, layers)`` of the texture.
    :param int components: The number of components 1, 2, 3 or 4.
    :param bytes data: Content of the texture. The size must be ``(width, height * layers)`` so each layer is stacked vertically.
    :param int alignment: The byte alignment 1, 2, 4 or 8.
    :param str dtype: Data type.

.. py:method:: Context.texture_cube(size: Tuple[int, int], components: int, data: Any = None, alignment: int = 1, dtype: str = 'f1') -> TextureCube

    Returns a new :py:class:`TextureCube` object.

    Note that the width and height of the cubemap must be the same.

    :param tuple size: The width, height of the texture. Each side of the cube will have this size.
    :param int components: The number of components 1, 2, 3 or 4.
    :param bytes data: Content of the texture. The data should be have the following ordering: positive_x, negative_x, positive_y, negative_y, positive_z, negative_z
    :param int alignment: The byte alignment 1, 2, 4 or 8.
    :param str dtype: Data type.
    :param int internal_format: Override the internalformat of the texture (IF needed)

.. py:method:: Context.depth_texture_cube(size: Tuple[int, int], data: Optional[Any] = None, alignment: int = 4) -> TextureCube

    Returns a new :py:class:`TextureCube` object.

    :param tuple size: The width and height of the texture.
    :param bytes data: Content of the texture.
    :param int alignment: The byte alignment 1, 2, 4 or 8.

.. py:method:: Context.simple_framebuffer(...)

    Deprecated, use :py:meth:`Context.framebuffer` instead.

.. py:method:: Context.renderbuffer(size: Tuple[int, int], components: int = 4, samples: int = 0, dtype: str = 'f1') -> Renderbuffer

    Returns a new :py:class:`Renderbuffer` object.

    Similar to textures, renderbuffers can be attached to framebuffers as render targets, but they cannot be sampled as textures.

    :param tuple size: The width and height of the renderbuffer.
    :param int components: The number of components 1, 2, 3 or 4.
    :param int samples: The number of samples. Value 0 means no multisample format.
    :param str dtype: Data type.

.. py:method:: Context.depth_renderbuffer(size: Tuple[int, int], samples: int = 0) -> Renderbuffer

    Returns a new :py:class:`Renderbuffer` object.

    :param tuple size: The width and height of the renderbuffer.
    :param int samples: The number of samples. Value 0 means no multisample format.

.. py:method:: Context.scope(framebuffer, enable_only, textures, uniform_buffers, storage_buffers, samplers)

    Returns a new :py:class:`Scope` object.

    Scope objects can be attached to VertexArray objects to minimize the possibility of rendering within the wrong scope.
    VertexArrays with an attached scope always have the scope settings at render time.

    :param Framebuffer framebuffer: The framebuffer to use when entering.
    :param int enable_only: The enable_only flags to set when entering.
    :param tuple textures: List of (texture, binding) tuples.
    :param tuple uniform_buffers: Tuple of (buffer, binding) tuples.
    :param tuple storage_buffers: Tuple of (buffer, binding) tuples.
    :param tuple samplers: Tuple of sampler bindings

.. py:method:: Context.query(samples: bool, any_samples: bool, time: bool, primitives: bool) -> Query

    Returns a new :py:class:`Query` object.

    :param bool samples: Query ``GL_SAMPLES_PASSED`` or not.
    :param bool any_samples: Query ``GL_ANY_SAMPLES_PASSED`` or not.
    :param bool time: Query ``GL_TIME_ELAPSED`` or not.
    :param bool primitives: Query ``GL_PRIMITIVES_GENERATED`` or not.

.. py:method:: Context.compute_shader(...)

    A :py:class:`ComputeShader` is a Shader Stage that is used entirely \
    for computing arbitrary information. While it can do rendering, it \
    is generally used for tasks not directly related to drawing.

    :param str source: The source of the compute shader.

External Objects
----------------

External objects are only useful for interoperability with other libraries.

.. py:method:: Context.external_buffer(glo: int, size: int, dynamic: bool) -> Buffer

    TBD

.. py:method:: Context.external_texture(glo: int, size: Tuple[int, int], components: int, samples: int, dtype: str) -> Texture

    Returns a new :py:class:`Texture` object from an existing OpenGL texture object.

    The content of the texture is referenced and it is not copied.

    :param int glo: External OpenGL texture object.
    :param tuple size: The width and height of the texture.
    :param int components: The number of components 1, 2, 3 or 4.
    :param int samples: The number of samples. Value 0 means no multisample format.
    :param str dtype: Data type.

Methods
-------

.. py:method:: Context.clear

    Clear the bound framebuffer.

    If a `viewport` passed in, a scissor test will be used to clear the given viewport.
    This viewport take prescense over the framebuffers :py:attr:`~moderngl.Framebuffer.scissor`.
    Clearing can still be done with scissor if no viewport is passed in.

    This method also respects the
    :py:attr:`~moderngl.Framebuffer.color_mask` and
    :py:attr:`~moderngl.Framebuffer.depth_mask`. It can for example be used to only clear
    the depth or color buffer or specific components in the color buffer.

    If the `viewport` is a 2-tuple it will clear the
    ``(0, 0, width, height)`` where ``(width, height)`` is the 2-tuple.

    If the `viewport` is a 4-tuple it will clear the given viewport.

    Args:
        red (float): color component.
        green (float): color component.
        blue (float): color component.
        alpha (float): alpha component.
        depth (float): depth value.

    Keyword Args:
        viewport (tuple): The viewport.
        color (tuple): Optional rgba color tuple

.. py:method:: Context.enable_only

    Clears all existing flags applying new ones.

    Note that the enum values defined in moderngl
    are not the same as the ones in opengl.
    These are defined as bit flags so we can logical
    `or` them together.

    Available flags:

    - :py:data:`moderngl.NOTHING`
    - :py:data:`moderngl.BLEND`
    - :py:data:`moderngl.DEPTH_TEST`
    - :py:data:`moderngl.CULL_FACE`
    - :py:data:`moderngl.RASTERIZER_DISCARD`
    - :py:data:`moderngl.PROGRAM_POINT_SIZE`

    Examples::

        # Disable all flags
        ctx.enable_only(moderngl.NOTHING)

        # Ensure only depth testing and face culling is enabled
        ctx.enable_only(moderngl.DEPTH_TEST | moderngl.CULL_FACE)

    Args:
        flags (EnableFlag): The flags to enable

.. py:method:: Context.enable

    Enable flags.

    Note that the enum values defined in moderngl
    are not the same as the ones in opengl.
    These are defined as bit flags so we can logical
    `or` them together.

    For valid flags, please see :py:meth:`enable_only`.

    Examples::

        # Enable a single flag
        ctx.enable(moderngl.DEPTH_TEST)

        # Enable multiple flags
        ctx.enable(moderngl.DEPTH_TEST | moderngl.CULL_FACE | moderngl.BLEND)

    Args:
        flag (int): The flags to enable.

.. py:method:: Context.disable

    Disable flags.

    For valid flags, please see :py:meth:`enable_only`.

    Examples::

        # Only disable depth testing
        ctx.disable(moderngl.DEPTH_TEST)

        # Disable depth testing and face culling
        ctx.disable(moderngl.DEPTH_TEST | moderngl.CULL_FACE)

    Args:
        flag (int): The flags to disable.

.. py:method:: Context.enable_direct

    Gives direct access to ``glEnable`` so unsupported capabilities in ModernGL can be enabled.

    Do not use this to set already supported context flags.

    Example::

        # Enum value from the opengl registry
        GL_CONSERVATIVE_RASTERIZATION_NV = 0x9346
        ctx.enable_direct(GL_CONSERVATIVE_RASTERIZATION_NV)

.. py:method:: Context.disable_direct

    Gives direct access to ``glDisable`` so unsupported capabilities in ModernGL can be disabled.

    Do not use this to set already supported context flags.

    Example::

        # Enum value from the opengl registry
        GL_CONSERVATIVE_RASTERIZATION_NV = 0x9346
        ctx.disable_direct(GL_CONSERVATIVE_RASTERIZATION_NV)

.. py:method:: Context.finish

    Wait for all drawing commands to finish.

.. py:method:: Context.clear_samplers

    Unbinds samplers from texture units.

    Sampler bindings do clear automatically between every frame,
    but lingering samplers can still be a source of weird bugs during
    the frame rendering. This methods provides a fairly brute force
    and efficient way to ensure texture units are clear.

    :param int start: The texture unit index to start the clearing samplers
    :param int stop: The texture unit index to stop clearing samplers

    Example::

        # Clear texture unit 0, 1, 2, 3, 4
        ctx.clear_samplers(start=0, end=5)

        # Clear texture unit 4, 5, 6, 7
        ctx.clear_samplers(start=4, end=8)

.. py:method:: Context.copy_buffer

    Copy buffer content.

    Args:
        dst (Buffer): The destination buffer.
        src (Buffer): The source buffer.
        size (int): The number of bytes to copy.

    Keyword Args:
        read_offset (int): The read offset.
        write_offset (int): The write offset.

.. py:method:: Context.copy_framebuffer

    Copy framebuffer content.

    Use this method to:

        - blit framebuffers.
        - copy framebuffer content into a texture.
        - downsample framebuffers. (it will allow to read the framebuffer's content)
        - downsample a framebuffer directly to a texture.

    Args:
        dst (Framebuffer or Texture): Destination framebuffer or texture.
        src (Framebuffer): Source framebuffer.

.. py:method:: Context.detect_framebuffer

    Detect a framebuffer.

    This is already done when creating a context,
    but if the underlying window library for some changes the default framebuffer
    during the lifetime of the application this might be necessary.

    Args:
        glo (int): Framebuffer object.

.. py:method:: Context.memory_barrier

    Applying a memory barrier.

    The memory barrier is needed in particular to correctly change buffers or textures
    between each shader. If the same buffer is changed in two shaders,
    it can cause an effect like 'depth fighting' on a buffer or texture.

    The method should be used between :py:class:`Program` -s, between :py:class:`ComputeShader` -s,
    and between :py:class:`Program` -s and :py:class:`ComputeShader` -s.

    Keyword Args:
        barriers (int): Affected barriers, default moderngl.ALL_BARRIER_BITS.
        by_region (bool): Memory barrier mode by region. More read on https://registry.khronos.org/OpenGL-Refpages/gl4/html/glMemoryBarrier.xhtml

.. py:method:: Context.release

.. py:method:: Context.gc

    TBD

.. py:method:: Context.__enter__

    TBD

.. py:method:: Context.__exit__

    TBD


Attributes
----------

.. py:attribute:: Context.gc_mode
.. py:attribute:: Context.objects
.. py:attribute:: Context.line_width
.. py:attribute:: Context.point_size
.. py:attribute:: Context.depth_func
.. py:attribute:: Context.depth_clamp_range
.. py:attribute:: Context.blend_func
.. py:attribute:: Context.blend_equation
.. py:attribute:: Context.viewport
.. py:attribute:: Context.scissor
.. py:attribute:: Context.version_code
.. py:attribute:: Context.screen
.. py:attribute:: Context.fbo
.. py:attribute:: Context.front_face
.. py:attribute:: Context.cull_face
.. py:attribute:: Context.wireframe
.. py:attribute:: Context.max_samples
.. py:attribute:: Context.max_integer_samples
.. py:attribute:: Context.max_texture_units
.. py:attribute:: Context.default_texture_unit
.. py:attribute:: Context.max_anisotropy
.. py:attribute:: Context.multisample
.. py:attribute:: Context.patch_vertices
.. py:attribute:: Context.provoking_vertex
.. py:attribute:: Context.polygon_offset
.. py:attribute:: Context.error
.. py:attribute:: Context.extensions
.. py:attribute:: Context.info

.. py:attribute:: Context.extra
    :type: Any

    User defined data.

Context Flags
-------------

Context flags are used to enable or disable states in the context.
These are not the same enum values as in opengl, but are rather
bit flags so we can ``or`` them together setting multiple states
in a simple way.

These values are available in the ``Context`` object and in the
``moderngl`` module when you don't have access to the context.

.. code:: python

    import moderngl

    # From moderngl
    ctx.enable_only(moderngl.DEPTH_TEST | moderngl.CULL_FACE)

    # From context
    ctx.enable_only(ctx.DEPTH_TEST | ctx.CULL_FACE)

.. py:attribute:: Context.NOTHING
.. py:attribute:: Context.BLEND
.. py:attribute:: Context.DEPTH_TEST
.. py:attribute:: Context.CULL_FACE
.. py:attribute:: Context.RASTERIZER_DISCARD
.. py:attribute:: Context.PROGRAM_POINT_SIZE

Primitive Modes
---------------

.. py:attribute:: Context.POINTS
.. py:attribute:: Context.LINES
.. py:attribute:: Context.LINE_LOOP
.. py:attribute:: Context.LINE_STRIP
.. py:attribute:: Context.TRIANGLES
.. py:attribute:: Context.TRIANGLE_STRIP
.. py:attribute:: Context.TRIANGLE_FAN
.. py:attribute:: Context.LINES_ADJACENCY
.. py:attribute:: Context.LINE_STRIP_ADJACENCY
.. py:attribute:: Context.TRIANGLES_ADJACENCY
.. py:attribute:: Context.TRIANGLE_STRIP_ADJACENCY
.. py:attribute:: Context.PATCHES

Texture Filters
~~~~~~~~~~~~~~~

Also available in the :py:class:`Context` instance
including mode details.

.. py:attribute:: Context.NEAREST
.. py:attribute:: Context.LINEAR
.. py:attribute:: Context.NEAREST_MIPMAP_NEAREST
.. py:attribute:: Context.LINEAR_MIPMAP_NEAREST
.. py:attribute:: Context.NEAREST_MIPMAP_LINEAR
.. py:attribute:: Context.LINEAR_MIPMAP_LINEAR

Blend Functions
---------------

Blend functions are used with :py:attr:`Context.blend_func`
to control blending operations.

.. code::

    # Default value
    ctx.blend_func = ctx.SRC_ALPHA, ctx.ONE_MINUS_SRC_ALPHA

.. py:attribute:: Context.ZERO
.. py:attribute:: Context.ONE
.. py:attribute:: Context.SRC_COLOR
.. py:attribute:: Context.ONE_MINUS_SRC_COLOR
.. py:attribute:: Context.SRC_ALPHA
.. py:attribute:: Context.ONE_MINUS_SRC_ALPHA
.. py:attribute:: Context.DST_ALPHA
.. py:attribute:: Context.ONE_MINUS_DST_ALPHA
.. py:attribute:: Context.DST_COLOR
.. py:attribute:: Context.ONE_MINUS_DST_COLOR

Blend Function Shortcuts
------------------------

.. py:attribute:: Context.DEFAULT_BLENDING
.. py:attribute:: Context.ADDITIVE_BLENDING
.. py:attribute:: Context.PREMULTIPLIED_ALPHA

Blend Equations
---------------

Used with :py:attr:`Context.blend_equation`.

.. py:attribute:: Context.FUNC_ADD
.. py:attribute:: Context.FUNC_SUBTRACT
.. py:attribute:: Context.FUNC_REVERSE_SUBTRACT
.. py:attribute:: Context.MIN
.. py:attribute:: Context.MAX

Other Enums
-----------

.. py:attribute:: Context.FIRST_VERTEX_CONVENTION
.. py:attribute:: Context.LAST_VERTEX_CONVENTION

Examples
--------

ModernGL Context
~~~~~~~~~~~~~~~~

.. code-block:: python

    import moderngl
    # create a window
    ctx = moderngl.create_context()
    print(ctx.version_code)

Standalone ModernGL Context
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: python

    import moderngl
    ctx = moderngl.create_standalone_context()
    print(ctx.version_code)

ContextManager
~~~~~~~~~~~~~~

.. rubric:: context_manager.py

.. literalinclude:: ../../examples/context_manager.py
    :linenos:

.. rubric:: example.py

.. code-block:: python
    :linenos:

    from context_manager import ContextManager

    ctx = ContextManager.get_default_context()
    print(ctx.version_code)

.. toctree::
    :maxdepth: 2
