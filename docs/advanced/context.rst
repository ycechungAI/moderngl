
.. _context:

Context Creation
================

.. py:currentmodule:: moderngl

.. Note:: From moderngl 5.6 context creation is handled by the glcontext_ package.
          This makes expanding context support easier for users lowering the
          bar for contributions. It also means context creation is no longer
          limited by a moderngl releases.

.. Note:: This page might not list all supported backends as the glcontext_
          project keeps evolving. If using anything outside of the default
          contexts provided per OS, please check the listed backends in
          the glcontext_ project.

Introduction
------------

A context is an object giving moderngl access to opengl instructions
(greatly simplified). How a context is created depends on your
operating system and what kind of platform you want to target.

In the vast majority of cases you'll be using the default context
backend supported by your operating system. This backend will be
automatically selected unless a specific ``backend`` parameter is used.

Default backend per OS

* **Windows**: wgl / opengl32.dll
* **Linux**: x11/glx/libGL
* **OS X**: CGL

These default backends support two modes:

* Detecting an exiting active context possibly created by a window
  library such as glfw, sdl2, pyglet etc.
* Creating a headless context (No visible window)

Detecting an existing active context created by a window library::

    import moderngl
    # Create the window with an OpenGL context (Most window libraries support this)
    ctx = moderngl.create_context()
    # If successful we can now render to the window
    print("Default framebuffer is:", ctx.screen)

A great reference using various window libraries can be found here: 
https://github.com/moderngl/moderngl-window/tree/master/moderngl_window/context

Creating a headless context::

    import moderngl
    # Create the context
    ctx = moderngl.create_context(standalone=True)
    # Create a framebuffer we can render to
    fbo = ctx.simple_framebuffer((100, 100), 4)
    fbo.use()

Require a minimum OpenGL version
--------------------------------

ModernGL only support 3.3+ contexts. By default version 3.3
is passed in as the minimum required version of the context
returned by the backend.

To require a specific version::

    moderngl.create_context(require=430)

This will require OpenGL 4.3. If a lower context version is
returned the context creation will fail.

This attribute can be accessed in :py:attr:`Context.version_code`
and will be updated to contain the actual version code of the
context (If higher than required).

Specifying context backend
--------------------------

A ``backend`` can be passed in for more advanced usage.

For example: Making a headless EGL context on linux::

    ctx = moderngl.create_context(standalone=True, backend='egl')

.. Note:: Each backend supports additional keyword arguments for
          more advanced configuration. This can for example be
          the exact name of the library to load. More information
          in the glcontext_ docs.

Context Sharing
---------------

.. Warning:: Object sharing is an experimental feature

Some context support the ``share`` parameters enabling
object sharing between contexts. This is not needed
if you are attaching to existing context with share mode enabled.
For example if you create two windows with glfw enabling object sharing.

ModernGL objects (such as :py:class:`moderngl.Buffer`, :py:class:`moderngl.Texture`, ..)
has a ``ctx`` property containing the context they were created in.
Still **ModernGL do not check what context is currently active when
accessing these objects.** This means the object can be used
in both contexts when sharing is enabled.

This should in theory work fine with object sharing enabled::

    data1 = numpy.array([1, 2, 3, 4], dtype='u1')
    data2 = numpy.array([4, 3, 2, 1], dtype='u1')

    ctx1 = moderngl.create_context(standalone=True)
    ctx2 = moderngl.create_context(standalone=True, share=True)

    with ctx1 as ctx:
        b1 = ctx.buffer(data1)

    with ctx2 as ctx:
        b2 = ctx.buffer(data2)

    print(b1.glo)  # Displays: 1
    print(b2.glo)  # Displays: 2

    with ctx1:
        print(b1.read())
        print(b2.read())

    with ctx2:
        print(b1.read())
        print(b2.read())

Still, there are some limitations to object sharing. Especially
objects that reference other objects (framebuffer, vertex array object, etc.)

More information for a deeper dive:

* https://www.khronos.org/opengl/wiki/OpenGL_Object#Object_Sharing
* https://www.khronos.org/opengl/wiki/Memory_Model

Context Info
------------

Various information such as limits and driver information can be found in the
:py:attr:`~moderngl.Context.info` property. It can often be useful to know
the vendor and render for the context::

    >>> import moderngl
    >>> ctx = moderngl.create_context(standalone=True, gl_version=(4.6))
    >>> ctx.info["GL_VENDOR"]
    'NVIDIA Corporation'
    >>> ctx.info["GL_RENDERER"] 
    'GeForce RTX 2080 SUPER/PCIe/SSE2'
    >>> ctx.info["GL_VERSION"]  
    '3.3.0 NVIDIA 456.71'

Note that it reports version 3.3 here because ModernGL by default
requests a version 3.3 context (minimum requirement).

.. _glcontext: https://github.com/moderngl/glcontext
