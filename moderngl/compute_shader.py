from typing import Tuple, Union

from .program_members import Attribute, Uniform, UniformBlock, Varying

__all__ = ['ComputeShader']


class ComputeShader:
    '''
        A Compute Shader is a Shader Stage that is used entirely for computing arbitrary information.
        While it can do rendering, it is generally used for tasks not directly related to drawing.
    '''

    __slots__ = ['mglo', '_members', '_glo', 'ctx', 'extra']

    def __init__(self):
        self.mglo = None
        self._members = {}
        self._glo = None
        self.ctx = None
        self.extra = None  #: Any - Attribute for storing user defined objects
        raise TypeError()

    def __repr__(self):
        return '<ComputeShader: %d>' % self.glo

    def __eq__(self, other):
        return type(self) is type(other) and self.mglo is other.mglo

    def __getitem__(self, key) -> Union[Uniform, UniformBlock, Attribute, Varying]:
        return self._members[key]

    def __iter__(self):
        yield from self._members

    @property
    def source(self) -> str:
        '''
            str: The source code of the compute shader.
        '''

        return self.mglo.source

    @property
    def glo(self) -> int:
        '''
            int: The internal OpenGL object.
            This values is provided for debug purposes only.
        '''

        return self._glo

    def run(self, group_x=1, group_y=1, group_z=1) -> None:
        '''
            Run the compute shader.

            Args:
                group_x (int): The number of work groups to be launched in the X dimension.
                group_y (int): The number of work groups to be launched in the Y dimension.
                group_z (int): The number of work groups to be launched in the Z dimension.
        '''

        return self.mglo.run(group_x, group_y, group_z)

    def get(self, key, default) -> Union[Uniform, UniformBlock, Attribute, Varying]:
        '''
            Returns a Uniform, UniformBlock, Attribute or Varying.

            Args:
                default: This is the value to be returned in case key does not exist.

            Returns:
                :py:class:`Uniform`, :py:class:`UniformBlock`,
                :py:class:`Attribute` or :py:class:`Varying`
        '''

        return self._members.get(key, default)

    def release(self) -> None:
        '''
            Release the ModernGL object.
        '''
