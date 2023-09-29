Sampler
=======

.. py:class:: Sampler

    Returned by :py:meth:`Context.sampler`

    A Sampler Object is an OpenGL Object that stores the sampling parameters for a Texture access inside of a shader.

    When a sampler object is bound to a texture image unit,
    the internal sampling parameters for a texture bound to the same image unit are all ignored.
    Instead, the sampling parameters are taken from this sampler object.

    Unlike textures, a samplers state can also be changed freely be at any time
    without the sampler object being bound/in use.

    Samplers are bound to a texture unit and not a texture itself. Be careful with leaving
    samplers bound to texture units as it can cause texture incompleteness issues
    (the texture bind is ignored).

    Sampler bindings do clear automatically between every frame so a texture unit
    need at least one bind/use per frame.

Methods
-------

.. py:method:: Sampler.use(location: int = 0) -> None:

    Bind the sampler to a texture unit.

    :param int location: The texture unit

.. py:method:: Sampler.clear(location: int = 0) -> None:

    Clear the sampler binding on a texture unit.

    :param int location: The texture unit

.. py:method:: Sampler.assign(index: int) -> tuple

    Helper method for assigning samplers to scopes.

    Example::

        s1 = ctx.sampler(...)
        s2 = ctx.sampler(...)
        ctx.scope(samplers=(s1.assign(0), s1.assign(1)), ...)mpler

.. py:method:: Sampler.release

Attributes
----------

.. py:attribute:: Sampler.texture
    :type: Texture

    The texture object to sample.

.. py:attribute:: Sampler.repeat_x
    :type: bool

    The x repeat flag for the sampler (Default ``True``).

    Example::

        # Enable texture repeat (GL_REPEAT)
        sampler.repeat_x = True

        # Disable texture repeat (GL_CLAMP_TO_EDGE)
        sampler.repeat_x = False

.. py:attribute:: Sampler.repeat_y
    :type: bool

    The y repeat flag for the sampler (Default ``True``).

    Example::

        # Enable texture repeat (GL_REPEAT)
        sampler.repeat_y = True

        # Disable texture repeat (GL_CLAMP_TO_EDGE)
        sampler.repeat_y = False

.. py:attribute:: Sampler.repeat_z
    :type: bool

    The z repeat flag for the sampler (Default ``True``).

    Example::

        # Enable texture repeat (GL_REPEAT)
        sampler.repeat_z = True

        # Disable texture repeat (GL_CLAMP_TO_EDGE)
        sampler.repeat_z = False

.. py:attribute:: Sampler.filter
    :type: tuple

    The minification and magnification filter for the sampler.

    (Default ``(moderngl.LINEAR. moderngl.LINEAR)``)

    Example::

        sampler.filter == (moderngl.NEAREST, moderngl.NEAREST)
        sampler.filter == (moderngl.LINEAR_MIPMAP_LINEAR, moderngl.LINEAR)
        sampler.filter == (moderngl.NEAREST_MIPMAP_LINEAR, moderngl.NEAREST)
        sampler.filter == (moderngl.LINEAR_MIPMAP_NEAREST, moderngl.NEAREST)

.. py:attribute:: Sampler.compare_func
    :type: tuple

    The compare function for a depth textures (Default ``'?'``).

    By default samplers don't have depth comparison mode enabled.
    This means that depth texture values can be read as a ``sampler2D``
    using ``texture()`` in a GLSL shader by default.

    When setting this property to a valid compare mode, ``GL_TEXTURE_COMPARE_MODE``
    is set to ``GL_COMPARE_REF_TO_TEXTURE`` so that texture lookup
    functions in GLSL will return a depth comparison result instead
    of the actual depth value.

    Accepted compare functions::

        .compare_func = ''    # Disale depth comparison completely
        sampler.compare_func = '<='  # GL_LEQUAL
        sampler.compare_func = '<'   # GL_LESS
        sampler.compare_func = '>='  # GL_GEQUAL
        sampler.compare_func = '>'   # GL_GREATER
        sampler.compare_func = '=='  # GL_EQUAL
        sampler.compare_func = '!='  # GL_NOTEQUAL
        sampler.compare_func = '0'   # GL_NEVER
        sampler.compare_func = '1'   # GL_ALWAYS

.. py:attribute:: Sampler.anisotropy
    :type: float

    Number of samples for anisotropic filtering (Default ``1.0``).

    The value will be clamped in range ``1.0`` and ``ctx.max_anisotropy``.

    Any value greater than 1.0 counts as a use of anisotropic filtering::

        # Disable anisotropic filtering
        sampler.anisotropy = 1.0

        # Enable anisotropic filtering suggesting 16 samples as a maximum
        sampler.anisotropy = 16.0

.. py:attribute:: Sampler.border_color
    :type: tuple

    The (r, g, b, a) color for the texture border (Default ``(0.0, 0.0, 0.0, 0.0)``).

    When setting this value the ``repeat_`` values are overridden setting the texture wrap to return
    the border color when outside [0, 1] range.

    Example::

        # Red border color
        sampler.border_color = (1.0, 0.0, 0.0, 0.0)

.. py:attribute:: Sampler.min_lod
    :type: float

    Minimum level-of-detail parameter (Default ``-1000.0``).

    This floating-point value limits the selection of highest resolution mipmap (lowest mipmap level)

.. py:attribute:: Sampler.max_lod
    :type: float

    Minimum level-of-detail parameter (Default ``1000.0``).

    This floating-point value limits the selection of the lowest resolution mipmap (highest mipmap level)

.. py:attribute:: Sampler.ctx
    :type: Context

    The context this object belongs to

.. py:attribute:: Sampler.glo
    :type: int

    The internal OpenGL object.
    This values is provided for interoperability and debug purposes only.

.. py:attribute:: Sampler.extra
    :type: Any

    User defined data.
