Sampler
=======

.. py:class:: Sampler

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

Create
------

.. py:method:: Context.sampler
   :noindex:

Methods
-------

.. py:method:: Sampler.use
.. py:method:: Sampler.clear
.. py:method:: Sampler.assign
.. py:method:: Sampler.release

Attributes
----------

.. py:attribute:: Sampler.texture
.. py:attribute:: Sampler.repeat_x
.. py:attribute:: Sampler.repeat_y
.. py:attribute:: Sampler.repeat_z
.. py:attribute:: Sampler.filter
.. py:attribute:: Sampler.compare_func
.. py:attribute:: Sampler.anisotropy
.. py:attribute:: Sampler.border_color
.. py:attribute:: Sampler.min_lod
.. py:attribute:: Sampler.max_lod

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
