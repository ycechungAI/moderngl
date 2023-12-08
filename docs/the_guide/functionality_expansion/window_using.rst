Rendering to a window
=====================

.. py:currentmodule:: moderngl

By default, ModernGL does not have a window, but the `moderngl-window`_ module allows you to use one. The installation is as follows::

    pip install moderngl-window

`moderngl-window`_ uses `pyglet`_ as its default backend. It is installed automatically along with `moderngl-window`_. However, its use is limited to the supported functionality in ``moderngl_window.WindowConfig``.

.. rubric:: Entire source

.. literalinclude:: window_using_example.py
    :emphasize-lines: 1,4-12,57,62,64
    :linenos:

You can read the full usage of `moderngl-window`_ in its `documentation`_.

.. _moderngl-window: https://github.com/moderngl/moderngl-window
.. _documentation: https://moderngl-window.readthedocs.io/
.. _pyglet: https://github.com/pyglet/pyglet
