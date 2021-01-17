
The Lifecycle of a ModernGL Object
==================================

.. Note:: Future version of ModernGL might support different GC models.
          It's an area currently being explored.

Releasing Objects
-----------------

Objects in moderngl don't automatically release the OpenGL resources it allocated.
Each type has a ``release()`` method that needs to be called to properly clean
up everything::

    # Create a texture
    texture = ctx.texture((10, 10), 4)

    # Properly release the opengl resources
    texture.release()

    # Ensure we don't keep the object around
    texture = None

This comes as a surprise for most people, but there are a number of reasons moderngl
have chosen this approach. Unless you are doing headless rendering we don't even
"own" the context itself. It's the window library creating the context for us and
we simply detect it. We don't really know exactly when this context is destroyed.
There are also other more complicated situations such as contexts with shared
resources.

You can create your own ``__del__`` methods in wrappers if needed, but keep in mind
that moderngl types cannot be extended. They only have an ``extra`` attribute
that can contain anything.

Detecting Released Objects
--------------------------

If you for some reason need to detect if a resource was released it can be done
by checking the type of the internal moderngl object (``.mglo`` property)::

    >> import moderngl
    >> ctx = moderngl.create_standalone_context()
    >> buffer = ctx.buffer(reserve=1024)
    >> type(buffer.mglo)
    <class 'mgl.Buffer'>
    >> buffer.release()
    >> type(buffer.mglo)
    <class 'mgl.InvalidObject'>
    >> type(buffer.mglo) == moderngl.mgl.InvalidObject
    True
