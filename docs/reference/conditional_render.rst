ConditionalRender
=================

.. py:class:: ConditionalRender

    Available in :py:attr:`Query.crender`

    This class represents a ConditionalRender object.

    ConditionalRender objects can only be accessed from :py:class:`Query` objects.

Examples
--------

.. code-block:: python

    query = ctx.query(any_samples=True)

    with query:
        vao1.render()

    with query.crender:
        print('This will always get printed')
        vao2.render()  # But this will be rendered only if vao1 has passing samples.
