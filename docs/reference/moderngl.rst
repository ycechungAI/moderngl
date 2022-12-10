.. py:module:: moderngl
.. py:currentmodule:: moderngl

moderngl
========

Attributes
----------

Attributes available in the root ``moderngl`` module.
Some may be listed in their original sub-module,
but they are imported during initialization.

Context Flags
~~~~~~~~~~~~~

Also available in the :py:class:`Context` instance
including mode details.

.. autodata:: moderngl.NOTHING
.. autodata:: moderngl.BLEND
.. autodata:: moderngl.DEPTH_TEST
.. autodata:: moderngl.CULL_FACE
.. autodata:: moderngl.RASTERIZER_DISCARD
.. autodata:: moderngl.PROGRAM_POINT_SIZE

Primitive Modes
~~~~~~~~~~~~~~~

Also available in the :py:class:`Context` instance
including mode details.

.. autodata:: moderngl.POINTS
.. autodata:: moderngl.LINES
.. autodata:: moderngl.LINE_LOOP
.. autodata:: moderngl.LINE_STRIP
.. autodata:: moderngl.TRIANGLES
.. autodata:: moderngl.TRIANGLE_STRIP
.. autodata:: moderngl.TRIANGLE_FAN
.. autodata:: moderngl.LINES_ADJACENCY
.. autodata:: moderngl.LINE_STRIP_ADJACENCY
.. autodata:: moderngl.TRIANGLES_ADJACENCY
.. autodata:: moderngl.TRIANGLE_STRIP_ADJACENCY
.. autodata:: moderngl.PATCHES

Texture Filters
~~~~~~~~~~~~~~~

Also available in the :py:class:`Context` instance
including mode details.

.. autodata:: moderngl.NEAREST
.. autodata:: moderngl.LINEAR
.. autodata:: moderngl.NEAREST_MIPMAP_NEAREST
.. autodata:: moderngl.LINEAR_MIPMAP_NEAREST
.. autodata:: moderngl.NEAREST_MIPMAP_LINEAR
.. autodata:: moderngl.LINEAR_MIPMAP_LINEAR

Blend Functions
~~~~~~~~~~~~~~~

Also available in the :py:class:`Context` instance
including mode details.

.. autodata:: moderngl.ZERO
.. autodata:: moderngl.ONE
.. autodata:: moderngl.SRC_COLOR
.. autodata:: moderngl.ONE_MINUS_SRC_COLOR
.. autodata:: moderngl.SRC_ALPHA
.. autodata:: moderngl.ONE_MINUS_SRC_ALPHA
.. autodata:: moderngl.DST_ALPHA
.. autodata:: moderngl.ONE_MINUS_DST_ALPHA
.. autodata:: moderngl.DST_COLOR
.. autodata:: moderngl.ONE_MINUS_DST_COLOR

Shortcuts
^^^^^^^^^

.. autodata:: moderngl.DEFAULT_BLENDING
.. autodata:: moderngl.ADDITIVE_BLENDING
.. autodata:: moderngl.PREMULTIPLIED_ALPHA

Blend Equations
~~~~~~~~~~~~~~~

Also available in the :py:class:`Context` instance
including mode details.

.. autodata:: moderngl.FUNC_ADD
.. autodata:: moderngl.FUNC_SUBTRACT
.. autodata:: moderngl.FUNC_REVERSE_SUBTRACT
.. autodata:: moderngl.MIN
.. autodata:: moderngl.MAX

Provoking Vertex
~~~~~~~~~~~~~~~~

Also available in the :py:class:`Context` instance
including mode details.

.. autodata:: moderngl.FIRST_VERTEX_CONVENTION
.. autodata:: moderngl.LAST_VERTEX_CONVENTION

Functions
---------

Also see :py:class:`Context`.

.. autofunction:: create_context
    :noindex:
.. autofunction:: create_standalone_context
    :noindex:
.. autofunction:: detect_format
    :noindex:
