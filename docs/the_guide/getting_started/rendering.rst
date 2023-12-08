.. py:currentmodule:: moderngl

Standalone rendering
====================

Standalone (offline) rendering allows you to render without using a window, and is included in ModernGL by default.

To display the rendering result, we use the `Pillow (PIL)`_ library that comes with Python. Let's return the texture from the GPU memory to RAM and call the ``PIL.Image.show()`` method to show it.

.. rubric:: Entire source

.. literalinclude:: first.5.py
    :emphasize-lines: 4,46-
    :linenos:

The result will be something like this:

.. figure:: rendering_result.png
    :alt: Rendering result
    :align: center
    :figclass: align-center

    Rendering result

.. _Pillow (PIL): https://pillow.readthedocs.io/
