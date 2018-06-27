import importlib
import io
import re
import textwrap
from typing import Any, Dict, Tuple, Union

NOTHING = 0
BLEND = 1
DEPTH_TEST = 2
CULL_FACE = 4
RASTERIZER_DISCARD = 8

POINTS = 0x0000
LINES = 0x0001
LINE_LOOP = 0x0002
LINE_STRIP = 0x0003
TRIANGLES = 0x0004
TRIANGLE_STRIP = 0x0005
TRIANGLE_FAN = 0x0006
LINES_ADJACENCY = 0x000A
LINE_STRIP_ADJACENCY = 0x000B
TRIANGLES_ADJACENCY = 0x000C
TRIANGLE_STRIP_ADJACENCY = 0x000D
PATCHES = 0x000E

NEAREST = 0x2600
LINEAR = 0x2601
NEAREST_MIPMAP_NEAREST = 0x2700
LINEAR_MIPMAP_NEAREST = 0x2701
NEAREST_MIPMAP_LINEAR = 0x2702
LINEAR_MIPMAP_LINEAR = 0x2703


class Error(Exception):
    pass


class Buffer:
    __slots__ = ['__mglo', 'size', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.size = None  # type: int
        self.extra = None  # type: Any

    def write(self, data, offset=0) -> None:
        self.__mglo.write(data, offset)

    def read(self, size=-1, offset=0, dtype=None) -> bytes:
        return self.__mglo.read(size, offset, dtype)

    def read_into(self, buffer, size=-1, offset=0, write_offset=0) -> None:
        write_end = write_offset + (self.size if size < 0 else size)
        memoryview(buffer)[write_offset : write_end] = self.__mglo.map(size, offset, True, False, None)
        self.__mglo.unmap()

    def map(self, size=-1, offset=0, readable=False, writable=False, dtype=None) -> None:
        return self.__mglo.map(size, offset, readable, writable, dtype)

    def unmap(self) -> None:
        self.__mglo.unmap()

    def clear(self) -> None:
        self.__mglo.clear()

    def bind_to_uniform_block(self, binding=0, offset=0, size=-1) -> None:
        self.__mglo.bind_to_uniform_block(binding, offset, size)

    def bind_to_storage_buffer(self, binding=0, offset=0, size=-1) -> None:
        self.__mglo.bind_to_storage_buffer(binding, offset, size)

    def orphan(self):  # TODO: deprecate
        pass


class Framebuffer:
    __slots__ = ['__mglo', '__viewport', 'size', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.__viewport = None  # type: Tuple[int, int, int, int]
        self.size = None  # type: Tuple[int, int]
        self.extra = None  # type: Any

    @property
    def viewport(self) -> Tuple[int, int, int, int]:
        return self.__viewport

    @viewport.setter
    def viewport(self, value):
        self.__mglo.viewport = value

    def clear_depth(self, value=1.0):
        self.__mglo.clear(value, -1)

    def clear(self, color, attachment=0):
        self.__mglo.clear(color, attachment)

    def read(self, viewport=None, components=3, attachment=0, alignment=1, dtype='f1', np=False):
        return self.__mglo.read(viewport, components, attachment, alignment, dtype, np)

    def read_into(self, buffer, viewport=None, components=3, attachment=0, alignment=1):
        return self.__mglo.read_into(buffer, viewport, components, attachment, alignment)

    def use(self):
        self.__mglo.use()


class Attribute:
    __slots__ = ['__type', 'location', 'cols', 'rows', 'size', 'shape']


class Uniform:
    __slots__ = ['__type', 'location', 'cols', 'rows', 'size', 'shape']


class DeprecatedUniform:
    def __init__(self, program, name):
        self.program = program
        self.name = name

    @property
    def value(self):
        raise NotImplementedError()

    @value.setter
    def value(self, value):
        self.program[self.name] = value

    def write(self, value):
        self.program[self.name] = value


class Program:
    __slots__ = ['__mglo', 'attributes', 'uniforms', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.attributes = None  # type: Dict[str, Attribute]
        self.uniforms = None  # type: Dict[str, Uniform]
        self.extra = None  # type: Any

    def __getitem__(self, key) -> DeprecatedUniform:
        return DeprecatedUniform(self, key)

    def __setitem__(self, key, value):
        self.__mglo.uniform(self.uniforms.get(key), value)


class Renderbuffer:
    __slots__ = ['__mglo', 'size', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.size = None  # type: Tuple[int, int]
        self.extra = None  # type: Any


class Texture:
    __slots__ = ['__mglo', '__level', '__layer', 'size', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.__level = None  # type: int
        self.__layer = None  # type: int
        self.size = None  # type: Union[Tuple[int, int], Tuple[int, int, int]]
        self.extra = None  # type: Any

    @property
    def swizzle(self):
        raise NotImplementedError()

    @swizzle.setter
    def swizzle(self, value):
        self.__mglo.swizzle = value

    @property
    def filter(self):
        raise NotImplementedError()

    @filter.setter
    def filter(self, value):
        self.__mglo.filter = value

    def level(self, level) -> 'Texture':
        return self.__mglo.level(level)

    def layer(self, layer) -> 'Texture':
        return self.__mglo.layer(layer)

    def write(self, data, viewport=None, alignment=1) -> None:
        self.__mglo.write(data, viewport, alignment, self.__level)

    def read(self, alignment=1, np=False) -> None:
        return self.__mglo.read(self.__level, self.__layer, alignment, np)

    def use(self, location=0) -> None:
        self.__mglo.use(location)

    def build_mipmaps(self, base=0, max=-1) -> None:
        self.__mglo.build_mipmaps(base, max)


class TextureArray:
    __slots__ = ['__mglo', '__level', '__index', 'size', 'length', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.__level = None  # type: int
        self.__index = None  # type: int
        self.size = None  # type: Tuple[int, int]
        self.length = None  # type: int
        self.extra = None  # type: Any

    def __getitem__(self, key) -> 'TextureArray':
        return self.__mglo.index(key, self.__level)

    def level(self, level) -> 'TextureArray':
        return self.__mglo.level(self.__level)


class TextureCube:
    __slots__ = ['__mglo', 'size', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.size = None  # type: Tuple[int, int]
        self.extra = None  # type: Any

    def level(self) -> 'Level':
        return None


class VertexArray:
    __slots__ = ['__mglo', '__ibo', 'program', 'mode', 'vertices', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.__ibo = None  # type: Buffer
        self.program = None  # type: Program
        self.mode = None  # type: int
        self.vertices = None  # type: int
        self.extra = None  # type: Any

    def render(self, mode=None, vertices=-1, first=0, instances=1):
        self.__mglo.render(mode, vertices, first, instances)

    def transform(self, buffer, mode=None, vertices=-1, first=0, instances=1, flush=True):
        self.__mglo.transform(buffer, mode, vertices, first, instances, flush)

    @property
    def index_buffer(self):
        return self.__ibo

    @index_buffer.setter
    def index_buffer(self, value):
        self.__mglo.ibo = value


class Limits:
    __slots__ = [
        'point_size_range',
        'smooth_line_width_range',
        'aliased_line_width_range',
        'point_fade_threshold_size',
        'point_size_granularity',
        'smooth_line_width_granularity',
        'min_program_texel_offset',
        'max_program_texel_offset',
        'minor_version',
        'major_version',
        'sample_buffers',
        'subpixel_bits',
        'context_profile_mask',
        'uniform_buffer_offset_alignment',
        'max_viewport_dims',
        'max_3d_texture_size',
        'max_array_texture_layers',
        'max_clip_distances',
        'max_color_attachments',
        'max_color_texture_samples',
        'max_combined_fragment_uniform_components',
        'max_combined_geometry_uniform_components',
        'max_combined_texture_image_units',
        'max_combined_uniform_blocks',
        'max_combined_vertex_uniform_components',
        'max_cube_map_texture_size',
        'max_depth_texture_samples',
        'max_draw_buffers',
        'max_dual_source_draw_buffers',
        'max_elements_indices',
        'max_elements_vertices',
        'max_fragment_input_components',
        'max_fragment_uniform_components',
        'max_fragment_uniform_vectors',
        'max_fragment_uniform_blocks',
        'max_geometry_input_components',
        'max_geometry_output_components',
        'max_geometry_texture_image_units',
        'max_geometry_uniform_blocks',
        'max_geometry_uniform_components',
        'max_integer_samples',
        'max_samples',
        'max_rectangle_texture_size',
        'max_renderbuffer_size',
        'max_sample_mask_words',
        'max_texture_buffer_size',
        'max_texture_image_units',
        'max_texture_lod_bias',
        'max_texture_size',
        'max_uniform_buffer_bindings',
        'max_uniform_block_size',
        'max_varying_components',
        'max_varying_vectors',
        'max_varying_floats',
        'max_vertex_attribs',
        'max_vertex_texture_image_units',
        'max_vertex_uniform_components',
        'max_vertex_uniform_vectors',
        'max_vertex_output_components',
        'max_vertex_uniform_blocks',
        'max_vertex_attrib_relative_offset',
        'max_vertex_attrib_bindings',
        'max_server_wait_timeout',
        'viewport_bounds_range',
        'viewport_subpixel_bits',
        'max_viewports',
        'min_map_buffer_alignment',
        'max_combined_atomic_counters',
        'max_fragment_atomic_counters',
        'max_geometry_atomic_counters',
        'max_tess_control_atomic_counters',
        'max_tess_evaluation_atomic_counters',
        'max_vertex_atomic_counters',
        'max_shader_storage_buffer_bindings',
        'max_combined_shader_storage_blocks',
        'max_vertex_shader_storage_blocks',
        'max_fragment_shader_storage_blocks',
        'max_geometry_shader_storage_blocks',
        'max_tess_evaluation_shader_storage_blocks',
        'max_tess_control_shader_storage_blocks',
        'max_compute_shader_storage_blocks',
        'max_compute_uniform_components',
        'max_compute_atomic_counters',
        'max_compute_atomic_counter_buffers',
        'max_compute_work_group_invocations',
        'max_compute_uniform_blocks',
        'max_compute_texture_image_units',
        'max_combined_compute_uniform_components',
        'max_framebuffer_width',
        'max_framebuffer_height',
        'max_framebuffer_layers',
        'max_framebuffer_samples',
        'max_uniform_locations',
        'max_element_index',
        'max_shader_storage_block_size',
    ]

    def __str__(self):
        res = ''
        for slot in Limits.__slots__:
            if hasattr(self, slot):
                res += '%s = %r\n' % (slot, getattr(self, slot))
        return res


class Query:
    __slots__ = ['__mglo', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any

    def __enter__(self):
        self.__mglo.begin()
        return self

    def __exit__(self, *args):
        self.__mglo.end()

    @property
    def samples(self) -> int:
        return self.__mglo.samples

    @property
    def primitives(self) -> int:
        return self.__mglo.primitives

    @property
    def elapsed(self) -> int:
        return self.__mglo.elapsed


class Scope:
    __slots__ = ['__mglo', 'extra']


class Context:
    __slots__ = ['__mglo', '__framebuffers', 'limits', 'screen', 'fbo', 'extra']

    def __init__(self):
        self.__mglo = None  # type: Any
        self.__framebuffers = None  # type: Dict[int, Framebuffer]
        self.limits = None  # type: Limits
        self.screen = None  # type: Framebuffer
        self.fbo = None  # type: Framebuffer
        self.extra = None  # type: Any

    def __enter__(self):
        self.__mglo.enter()

    def __exit__(self, *args):
        self.__mglo.exit()

    @property
    def viewport(self) -> Tuple[int, int, int, int]:
        return self.fbo.viewport

    @viewport.setter
    def viewport(self, value):
        self.fbo.viewport = value

    @property
    def line_width(self) -> float:
        raise NotImplementedError()

    @line_width.setter
    def line_width(self, value):
        self.__mglo.line_width = value

    @property
    def point_size(self) -> float:
        raise NotImplementedError()

    @point_size.setter
    def point_size(self, value):
        self.__mglo.point_size = value

    def clear(self, red=0.0, green=0.0, blue=0.0, alpha=0.0, depth=1.0, viewport=None) -> None:
        self.fbo.clear((red, green, blue, alpha))
        self.fbo.clear_depth(depth)

    def buffer(self, data=None, reserve=0, readable=True, writable=True, local=False) -> Buffer:
        return self.__mglo.buffer(data, reserve, readable, writable, local)

    def framebuffer(self, *attachments) -> Framebuffer:
        return self.__mglo.framebuffer(attachments)

    def simple_framebuffer(self, size, components=4, samples=0, depth=True) -> Framebuffer:
        attachments = [
            self.renderbuffer(size, components=components, samples=samples)
        ]
        if depth:
            attachments.append(self.depth_renderbuffer(size, samples=samples))
        return self.__mglo.framebuffer(attachments)

    def program(self, vertex_shader, fragment_shader=None, geometry_shader=None,
        tess_control_shader=None, tess_evaluation_shader=None, varyings=()) -> Program:
        return self.__mglo.program(
            vertex_shader,
            fragment_shader,
            geometry_shader,
            tess_control_shader,
            tess_evaluation_shader,
            varyings,
        )

    def renderbuffer(self, size, components=4, samples=0, dtype='f4') -> Renderbuffer:
        return self.__mglo.renderbuffer(size, components, samples, dtype)

    def depth_renderbuffer(self, size, samples=0) -> Renderbuffer:
        return self.__mglo.renderbuffer(size, -1, samples, 'f4')

    def texture(self, size, components=3, data=None, levels=-1, samples=0, aligment=1, dtype='f1') -> Texture:
        return self.__mglo.texture(size, components, data, levels, samples, aligment, dtype)

    def texture_array(self) -> TextureArray:
        return self.__mglo.texture_array()

    def texture_cube(self) -> TextureCube:
        return self.__mglo.texture_cube()

    def vertex_array(self, program, content, index_buffer=None) -> VertexArray:
        return self.__mglo.vertex_array(program, content, index_buffer)

    def simple_vertex_array(self, program, buffer, *attributes, index_buffer=None) -> VertexArray:
        content = [(buffer, detect_format(program, attributes)) + attributes]
        return self.__mglo.vertex_array(program, content, index_buffer)

    def detect_framebuffer(self, fbo=None) -> Framebuffer:
        return self.__mglo.detect_framebuffer(fbo)

    def copy_buffer(self, dst, src, size=-1, read_offset=0, write_offset=0) -> None:
        self.__mglo.copy_buffer(dst, src, size, read_offset, write_offset)

    def enable(self, flags) -> None:
        self.__mglo.enable(flags)

    def enable_only(self, flags) -> None:  # TODO: deprecate
        self.__mglo.enable(flags)

    def make_current(self) -> None:
        self.__mglo.enter()


def create_context(standalone=False, debug=False) -> Context:
    mgl = importlib.import_module('moderngl.mgl_dbg' if debug else 'moderngl.mgl')
    return mgl.create_context(standalone)


def create_standalone_context() -> Context:  # TODO: deprecate
    return create_context(standalone=True)


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

        return attr.size * attr.rows * attr.cols, chr(attr.shape)

    return ' '.join('%d%s' % fmt(program.attributes[a]) for a in attributes)


def _split_format(fmt):
    res = []
    divisor = 0
    stride = 0
    nodes = fmt.replace('/', '  /').split()
    if nodes[-1].startswith('/'):
        match = re.match(r'^/(?:(\d*)i|([vr]))$', nodes[-1])
        if not match:
            raise Error('%r is not a valid divisor' % nodes[-1])
        a, b = match.groups()
        divisor = int(a) if a else 1 if not b else 0 if b == 'v' else 0x7fffffff
        nodes = nodes[:-1]
    for node in nodes:
        match = re.match(r'^(\d*)([fiux])([1248]?)$', node)
        if not match:
            raise Error('%r is not a valid format node' % node)
        a, b, c = match.groups()
        a = int(a) if a else 1
        c = int(c) if c else 1 if b == 'x' else 4
        res.append((a, b, c))
        stride += a * c
    return res, divisor, stride


def _number_source(source):
    lines = textwrap.dedent(source).split('\n')
    fmt = ' %%%dd | %%s' % len(str(len(lines)))
    return '\n'.join(fmt % (i + 1, line) for i, line in enumerate(lines))


def _compiler_error(name, source, info):
    message = 'compiler error (%s)\n\n%s\n\n%s' % (name, _number_source(source), info)
    raise Error(message)


def _linker_error(vertex_shader, fragment_shader, geometry_shader, tess_control_shader, tess_evaluation_shader, info):
    raise Error(info)
