Context
=======

.. py:module:: moderngl
.. py:currentmodule:: moderngl

.. autoclass:: moderngl.Context

Create
------

.. autofunction:: moderngl.create_context(require=None) -> Context
.. autofunction:: moderngl.create_standalone_context(require=None) -> Context

ModernGL Objects
----------------

.. automethod:: Context.program(vertex_shader, fragment_shader=None, geometry_shader=None, tess_control_shader=None, tess_evaluation_shader=None, varyings=()) -> Program
.. automethod:: Context.simple_vertex_array(program, buffer, *attributes, index_buffer=None, index_element_size=4) -> VertexArray
.. automethod:: Context.vertex_array(program, content, index_buffer=None, index_element_size=4, skip_errors=False) -> VertexArray
.. automethod:: Context.buffer(data=None, reserve=0, dynamic=False) -> Buffer
.. automethod:: Context.texture(size, components, data=None, samples=0, alignment=1, dtype='f1') -> Texture
.. automethod:: Context.depth_texture(size, data=None, samples=0, alignment=4) -> Texture
.. automethod:: Context.texture3d(size, components, data=None, alignment=1, dtype='f1') -> Texture3D
.. automethod:: Context.texture_array(size, components, data=None, alignment=1, dtype='f1') -> TextureArray
.. automethod:: Context.texture_cube(size, components, data=None, alignment=1, dtype='f1') -> TextureCube
.. automethod:: Context.simple_framebuffer(size, components=4, samples=0, dtype='f1') -> Framebuffer
.. automethod:: Context.framebuffer(color_attachments=(), depth_attachment=None) -> Framebuffer
.. automethod:: Context.renderbuffer(size, components=4, samples=0, dtype='f1') -> Renderbuffer
.. automethod:: Context.depth_renderbuffer(size, samples=0) -> Renderbuffer
.. automethod:: Context.scope(framebuffer=None, enable_only=None, textures=(), uniform_buffers=(), storage_buffers=(), samplers=()) -> Scope
.. automethod:: Context.query(samples=False, any_samples=False, time=False, primitives=False) -> Query
.. automethod:: Context.compute_shader(source) -> ComputeShader
.. automethod:: Context.sampler(repeat_x=True, repeat_y=True, repeat_z=True, filter=None, anisotropy=1.0, compare_func='?', border_color=None, min_lod=-1000.0, max_lod=1000.0) -> Sampler
.. automethod:: Context.clear_samplers(start=0, end=-1)

Methods
-------

.. automethod:: Context.clear(red=0.0, green=0.0, blue=0.0, alpha=0.0, depth=1.0, viewport=None)
.. automethod:: Context.enable_only(flags)
.. automethod:: Context.enable(flags)
.. automethod:: Context.disable(flags)
.. automethod:: Context.finish()
.. automethod:: Context.copy_buffer(dst, src, size=-1, read_offset=0, write_offset=0)
.. automethod:: Context.copy_framebuffer(dst, src)
.. automethod:: Context.detect_framebuffer(glo=None) -> Framebuffer

Attributes
----------

.. autoattribute:: Context.line_width
.. autoattribute:: Context.point_size
.. autoattribute:: Context.depth_func
.. autoattribute:: Context.blend_func
.. autoattribute:: Context.blend_equation
.. autoattribute:: Context.viewport
.. autoattribute:: Context.version_code
.. autoattribute:: Context.screen
.. autoattribute:: Context.fbo
.. autoattribute:: Context.front_face
.. autoattribute:: Context.wireframe
.. autoattribute:: Context.max_samples
.. autoattribute:: Context.max_integer_samples
.. autoattribute:: Context.max_texture_units
.. autoattribute:: Context.default_texture_unit
.. autoattribute:: Context.max_anisotropy
.. autoattribute:: Context.multisample
.. autoattribute:: Context.patch_vertices
.. autoattribute:: Context.provoking_vertex
.. autoattribute:: Context.error
.. autoattribute:: Context.info
.. autoattribute:: Context.extra

Examples
--------

ModernGL Context
^^^^^^^^^^^^^^^^

.. code-block:: python

    import moderngl
    # create a window
    ctx = moderngl.create_context()
    print(ctx.version_code)

Standalone ModernGL Context
^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: python

    import moderngl
    ctx = moderngl.create_standalone_context()
    print(ctx.version_code)

ContextManager
^^^^^^^^^^^^^^

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
