
Getting Started
---------------

To get something rendered, you will need a :py:class:`VertexArray`.

VertexArrays can be created from a :py:class:`Program` object and one or several
:py:class:`Buffer` objects. (In some techniques no buffers are needed, but let's keep
it simple for now)

To create a :py:class:`Program` object, you will need to supply it your shader(s)
as strings. Having a vertex and fragment shader is the most common use case
as this will render something visible to a screen or framebuffer.

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
