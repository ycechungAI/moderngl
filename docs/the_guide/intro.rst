A short introduction
====================

.. py:module:: moderngl
.. py:currentmodule:: moderngl

.. rubric:: What you will need?

To get something rendered, you will need a :py:class:`VertexArray`.

VertexArrays can be created from a :py:class:`Program` object and several
:py:class:`Buffer` objects.

To create a :py:class:`Program` object, you will need some :py:class:`Shader`
objects.

Once you have your :py:class:`Program` object, you can fill a
:py:class:`Buffer` with your data,
then pass them to :py:class:`VertexArray`, then call
:py:meth:`VertexArray.render`.

All of the objects above can only be created from a :py:class:`Context` object.

.. rubric:: Here is our checklist:

1. Install ModernGL.
2. Create a Context.
3. Create a Program object.
4. Create a VertexArray object.

Proceed to the :doc:`next step <install>`.
