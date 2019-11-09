from typing import Tuple, Union

from .program_members import (Attribute, Subroutine, Uniform, UniformBlock,
                              Varying)

__all__ = ['Program', 'detect_format']


class Program:
    '''
        A Program object represents fully processed executable code
        in the OpenGL Shading Language, for one or more Shader stages.

        In ModernGL, a Program object can be assigned to :py:class:`VertexArray` objects.
        The VertexArray object  is capable of binding the Program object once the
        :py:meth:`VertexArray.render` or :py:meth:`VertexArray.transform` is called.

        Program objects has no method called ``use()``, VertexArrays encapsulate this mechanism.

        A Program object cannot be instantiated directly, it requires a context.
        Use :py:meth:`Context.program` to create one.
    '''

    __slots__ = ['mglo', '_members', '_subroutines', '_geom', '_glo', 'ctx', 'extra']

    def __init__(self):
        self.mglo = None
        self._members = {}
        self._subroutines = None
        self._geom = (None, None, None)
        self._glo = None
        self.ctx = None
        self.extra = None  #: Any - Attribute for storing user defined objects
        raise TypeError()

    def __repr__(self):
        return '<Program: %d>' % self._glo

    def __eq__(self, other):
        """Compares two programs opengl names

        Example::

            # True if the internal opengl name is the same
            program_1 == program_2

        """
        return type(self) is type(other) and self.mglo is other.mglo

    def __getitem__(self, key) -> Union[Uniform, UniformBlock, Subroutine, Attribute, Varying]:
        """Get a member such as uniforms, uniform blocks, subroutines,
        attributes and varyings by name.

        Example::

            # Get a uniform
            uniform = program['color]

            # Set a vec4 uniform
            uniform.value = 1.0, 1.0, 1.0, 1.0
        """
        return self._members[key]

    def __iter__(self):
        """Yields the internal members such as uniforms, uniform blocks etc.

        Example::

            for name, member in program:
                print(name, member)
        """
        yield from self._members

    @property
    def geometry_input(self) -> int:
        '''
            int: The geometry input primitive.
            The GeometryShader's input primitive if the GeometryShader exists.
            The geometry input primitive will be used for validation.
        '''

        return self._geom[0]

    @property
    def geometry_output(self) -> int:
        '''
            int: The geometry output primitive.
            The GeometryShader's output primitive if the GeometryShader exists.
        '''

        return self._geom[1]

    @property
    def geometry_vertices(self) -> int:
        '''
            int: The maximum number of vertices that
            the geometry shader will output.
        '''

        return self._geom[2]

    @property
    def subroutines(self) -> Tuple[str, ...]:
        '''
            tuple: The subroutine uniforms.
        '''

        return self._subroutines

    @property
    def glo(self) -> int:
        '''
            int: The internal OpenGL object.
            This values is provided for debug purposes only.
        '''

        return self._glo

    def get(self, key, default) -> Union[Uniform, UniformBlock, Subroutine, Attribute, Varying]:
        '''
            Returns a Uniform, UniformBlock, Subroutine, Attribute or Varying.

            Args:
                default: This is the value to be returned in case key does not exist.

            Returns:
                :py:class:`Uniform`, :py:class:`UniformBlock`, :py:class:`Subroutine`,
                :py:class:`Attribute` or :py:class:`Varying`
        '''

        return self._members.get(key, default)

    def release(self) -> None:
        '''
            Release the ModernGL object.
        '''

        self.mglo.release()


def detect_format(program, attributes) -> str:
    '''
        Detect format for vertex attributes.
        The format returned does not contain padding.

        Args:
            program (Program): The program.
            attributes (list): A list of attribute names.

        Returns:
            str
    '''

    def fmt(attr):
        '''
            For internal use only.
        '''

        return attr.array_length * attr.dimension, attr.shape

    return ' '.join('%d%s' % fmt(program[a]) for a in attributes)
