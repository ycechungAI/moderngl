ComputeShader
=============

.. py:module:: moderngl
.. py:currentmodule:: moderngl

.. autoclass:: moderngl.ComputeShader

Create
------

.. automethod:: Context.compute_shader(source: str) -> ComputeShader
    :noindex:

Methods
-------

.. automethod:: ComputeShader.run(group_x: int = 1, group_y: int = 1, group_z: int = 1)
.. automethod:: ComputeShader.get(key: str, default: Any) -> Union[Uniform, UniformBlock, Subroutine, Attribute, Varying]
.. automethod:: ComputeShader.release()
.. automethod:: ComputeShader.__eq__(other: Any)
.. automethod:: ComputeShader.__getitem__(key: str) -> Union[Uniform, UniformBlock, Subroutine, Attribute, Varying]
.. automethod:: ComputeShader.__setitem__(key: str, value: Any)
.. automethod:: ComputeShader.__iter__() -> Generator[str, NoneType, NoneType]

Attributes
----------

.. autoattribute:: ComputeShader.glo
.. autoattribute:: ComputeShader.mglo
.. autoattribute:: ComputeShader.extra
.. autoattribute:: ComputeShader.ctx

.. toctree::
    :maxdepth: 2
