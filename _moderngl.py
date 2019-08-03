import re


class Limits:
    POINT_SIZE_RANGE: tuple
    MAX_VIEWPORT_DIMS: tuple
    POINT_SIZE_GRANULARITY: float
    MAX_TEXTURE_SIZE: int
    SUBPIXEL_BITS: int
    SMOOTH_POINT_SIZE_RANGE: tuple
    SMOOTH_LINE_WIDTH_RANGE: tuple
    ALIASED_LINE_WIDTH_RANGE: tuple
    MAX_3D_TEXTURE_SIZE: int
    MAX_ELEMENTS_VERTICES: int
    MAX_ELEMENTS_INDICES: int
    SMOOTH_POINT_SIZE_GRANULARITY: float
    SMOOTH_LINE_WIDTH_GRANULARITY: float
    MAX_CUBE_MAP_TEXTURE_SIZE: int
    POINT_FADE_THRESHOLD_SIZE: float
    MAX_TEXTURE_LOD_BIAS: int
    MAX_DRAW_BUFFERS: int
    MAX_VERTEX_ATTRIBS: int
    MAX_TEXTURE_IMAGE_UNITS: int
    MAX_FRAGMENT_UNIFORM_COMPONENTS: int
    MAX_VERTEX_UNIFORM_COMPONENTS: int
    MAX_COMBINED_TEXTURE_IMAGE_UNITS: int
    MAX_VERTEX_TEXTURE_IMAGE_UNITS: int
    MAX_VARYING_FLOATS: int
    MAX_CLIP_DISTANCES: int
    MAJOR_VERSION: int
    MINOR_VERSION: int
    MAX_ARRAY_TEXTURE_LAYERS: int
    MIN_PROGRAM_TEXEL_OFFSET: float
    MAX_PROGRAM_TEXEL_OFFSET: float
    MAX_RENDERBUFFER_SIZE: int
    MAX_COLOR_ATTACHMENTS: int
    MAX_SAMPLES: int
    MAX_VARYING_COMPONENTS: int
    MAX_TEXTURE_BUFFER_SIZE: int
    MAX_RECTANGLE_TEXTURE_SIZE: int
    MAX_VERTEX_UNIFORM_BLOCKS: int
    MAX_GEOMETRY_UNIFORM_BLOCKS: int
    MAX_FRAGMENT_UNIFORM_BLOCKS: int
    MAX_COMBINED_UNIFORM_BLOCKS: int
    MAX_UNIFORM_BUFFER_BINDINGS: int
    MAX_UNIFORM_BLOCK_SIZE: int
    MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS: int
    MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS: int
    MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS: int
    UNIFORM_BUFFER_OFFSET_ALIGNMENT: int
    MAX_SERVER_WAIT_TIMEOUT: int
    MAX_GEOMETRY_TEXTURE_IMAGE_UNITS: int
    MAX_GEOMETRY_UNIFORM_COMPONENTS: int
    MAX_GEOMETRY_OUTPUT_VERTICES: int
    MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS: int
    MAX_VERTEX_OUTPUT_COMPONENTS: int
    MAX_GEOMETRY_INPUT_COMPONENTS: int
    MAX_GEOMETRY_OUTPUT_COMPONENTS: int
    MAX_FRAGMENT_INPUT_COMPONENTS: int
    CONTEXT_PROFILE_MASK: int
    MAX_SAMPLE_MASK_WORDS: int
    MAX_COLOR_TEXTURE_SAMPLES: int
    MAX_DEPTH_TEXTURE_SAMPLES: int
    MAX_INTEGER_SAMPLES: int
    MAX_DUAL_SOURCE_DRAW_BUFFERS: int
    VIEWPORT_BOUNDS_RANGE: tuple
    MAX_VERTEX_UNIFORM_VECTORS: int
    MAX_VARYING_VECTORS: int
    MAX_FRAGMENT_UNIFORM_VECTORS: int
    MAX_VIEWPORTS: int
    VIEWPORT_SUBPIXEL_BITS: int
    MIN_MAP_BUFFER_ALIGNMENT: int
    MAX_VERTEX_ATOMIC_COUNTER_BUFFERS: int
    MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS: int
    MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS: int
    MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS: int
    MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS: int
    MAX_COMBINED_ATOMIC_COUNTER_BUFFERS: int
    MAX_VERTEX_ATOMIC_COUNTERS: int
    MAX_TESS_CONTROL_ATOMIC_COUNTERS: int
    MAX_TESS_EVALUATION_ATOMIC_COUNTERS: int
    MAX_GEOMETRY_ATOMIC_COUNTERS: int
    MAX_FRAGMENT_ATOMIC_COUNTERS: int
    MAX_COMBINED_ATOMIC_COUNTERS: int
    MAX_ATOMIC_COUNTER_BUFFER_SIZE: int
    MAX_ATOMIC_COUNTER_BUFFER_BINDINGS: int
    MAX_IMAGE_UNITS: int
    MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS: int
    MAX_IMAGE_SAMPLES: int
    MAX_VERTEX_IMAGE_UNIFORMS: int
    MAX_TESS_CONTROL_IMAGE_UNIFORMS: int
    MAX_TESS_EVALUATION_IMAGE_UNIFORMS: int
    MAX_GEOMETRY_IMAGE_UNIFORMS: int
    MAX_FRAGMENT_IMAGE_UNIFORMS: int
    MAX_COMBINED_IMAGE_UNIFORMS: int
    MAX_COMPUTE_WORK_GROUP_COUNT: tuple
    MAX_COMPUTE_WORK_GROUP_SIZE: tuple
    MAX_ELEMENT_INDEX: int
    MAX_SHADER_STORAGE_BLOCK_SIZE: int
    MAX_COMPUTE_UNIFORM_BLOCKS: int
    MAX_COMPUTE_TEXTURE_IMAGE_UNITS: int
    MAX_COMPUTE_IMAGE_UNIFORMS: int
    MAX_COMPUTE_SHARED_MEMORY_SIZE: int
    MAX_COMPUTE_UNIFORM_COMPONENTS: int
    MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS: int
    MAX_COMPUTE_ATOMIC_COUNTERS: int
    MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS: int
    MAX_COMPUTE_WORK_GROUP_INVOCATIONS: int
    MAX_DEBUG_MESSAGE_LENGTH: int
    MAX_DEBUG_LOGGED_MESSAGES: int
    MAX_DEBUG_GROUP_STACK_DEPTH: int
    MAX_LABEL_LENGTH: int
    MAX_UNIFORM_LOCATIONS: int
    MAX_FRAMEBUFFER_WIDTH: int
    MAX_FRAMEBUFFER_HEIGHT: int
    MAX_FRAMEBUFFER_LAYERS: int
    MAX_FRAMEBUFFER_SAMPLES: int
    MAX_VERTEX_SHADER_STORAGE_BLOCKS: int
    MAX_GEOMETRY_SHADER_STORAGE_BLOCKS: int
    MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS: int
    MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS: int
    MAX_FRAGMENT_SHADER_STORAGE_BLOCKS: int
    MAX_COMPUTE_SHADER_STORAGE_BLOCKS: int
    MAX_COMBINED_SHADER_STORAGE_BLOCKS: int
    MAX_SHADER_STORAGE_BUFFER_BINDINGS: int
    MAX_COMBINED_SHADER_OUTPUT_RESOURCES: int
    MAX_VERTEX_ATTRIB_RELATIVE_OFFSET: int
    MAX_VERTEX_ATTRIB_BINDINGS: int
    MAX_VERTEX_ATTRIB_STRIDE: int
    MAX_CULL_DISTANCES: float
    MAX_COMBINED_CLIP_AND_CULL_DISTANCES: float
    MAX_TEXTURE_MAX_ANISOTROPY: float

    def __init__(self, data):
        for name, value in data:
            setattr(self, name, value)

    def __repr__(self):
        return 'Limits(%r)' % self.json()

    def json(self):
        return self.__dict__.copy()


class Buffer:
    ctx: 'Context'
    glo: int
    size: int

    def read(self, size, offset, into, write_offset, viewport) -> bytes:
        '''```py
        np.frombuffer(vbo.read(), dtype='f4')
        ```'''
        return bytes()

    def write(self, data, offset):
        '''```py
        vbo.write(array.astype('f4').tobytes())
        ```'''

    def bind(self, layout, stride, divisor, offset) -> tuple:
        '''```py
        vao = ctx.vertex_array(prog, [
            vbo.bind('in_vert', 'in_norm', 'in_text', layout='3f 3f 2f'),
        ])
        ```'''
        return ()

    def assign(self, index) -> tuple:
        '''```py
        scope = ctx.scope(uniform_buffers=[
            ubo.assing(0),
        ])
        ```'''
        return ()


class Framebuffer:
    ctx: 'Context'
    glo: int
    size: tuple
    components: int
    samples: int

    def read(self, size, components, attachment, read_offset, write_offset, into, alignment) -> bytes:
        '''```py
        Image.frombytes(fbo.size, 'RGB', fbo.read())
        ```'''
        return bytes()

    def clear(self, color, depth, attachment, viewport):
        '''```py
        fbo.clear(color=(1.0, 1.0, 1.0))
        ```'''


class Program:
    ctx: 'Context'
    glo: int

    def run(self, x, y, z):
        '''```
        compute = ctx.program(compute_shader='...')
        compute.run(32, 32, 1)
        ```'''

    def __getitem__(self, key) -> object:
        pass

    def __setitem__(self, key, value):
        pass


class Query:
    ctx: 'Context'
    elapsed: int
    primitives: int
    samples: int

    def __enter__(self):
        pass

    def __exit__(self, *args):
        pass


class Renderbuffer:
    ctx: 'Context'
    glo: int
    size: tuple
    components: int
    samples: int


class Sampler:
    ctx: 'Context'
    glo: int
    texture: 'Texture'
    filter: int
    wrap: int

    def assign(self, index) -> tuple:
        '''```py
        scope = ctx.scope(samplers=[
            sampler.assing(0),
        ])
        ```'''
        return ()


class Scope:
    ctx: 'Context'
    framebuffer: 'Framebuffer'
    enable: int


class Texture:
    ctx: 'Context'
    glo: int
    size: tuple
    components: int
    samples: int

    def read(self, into) -> bytes:
        '''```py
        image = Image.frombytes(texture.size, 'RGB', texture.read(), 'raw', 'RGB', 0, -1)
        ```'''
        return bytes()

    def write(self, data, viewport):
        '''```py
        texture.write(image.tobytes('raw', 'RGB', 0, -1))
        ```'''


class VertexArray:
    ctx: 'Context'
    glo: int
    mode: int
    vertices: int
    instances: int

    index_buffer: 'Buffer'
    output_buffer: 'Buffer'
    indirect_buffer: 'Buffer'

    def bind(self, buffer, location, gltype, size, offset, stride, divisor):
        '''internal method for glVertexAttribPointer'''

    def render(self, condition):
        '''```py
        vao.render()
        ```'''


class Context:
    screen: 'Framebuffer'
    limits: 'Limits'

    BLEND: int
    DEPTH_TEST: int
    CULL_FACE: int

    CLAMP_TO_EDGE_X: int
    CLAMP_TO_EDGE_Y: int
    CLAMP_TO_EDGE_Z: int
    REPEAT_X: int
    REPEAT_Y: int
    REPEAT_Z: int
    MIRRORED_REPEAT_X: int
    MIRRORED_REPEAT_Y: int
    MIRRORED_REPEAT_Z: int
    MIRROR_CLAMP_TO_EDGE_X: int
    MIRROR_CLAMP_TO_EDGE_Y: int
    MIRROR_CLAMP_TO_EDGE_Z: int
    CLAMP_TO_BORDER_X: int
    CLAMP_TO_BORDER_Y: int
    CLAMP_TO_BORDER_Z: int

    ZERO: int
    ONE: int
    SRC_COLOR: int
    SRC_ALPHA: int
    DST_COLOR: int
    DST_ALPHA: int
    ONE_MINUS_SRC_COLOR: int
    ONE_MINUS_SRC_ALPHA: int
    ONE_MINUS_DST_COLOR: int
    ONE_MINUS_DST_ALPHA: int

    FIRST_VERTEX_CONVENTION: int
    LAST_VERTEX_CONVENTION: int

    NEAREST: int
    LINEAR: int
    NEAREST_MIPMAP_NEAREST: int
    LINEAR_MIPMAP_NEAREST: int
    NEAREST_MIPMAP_LINEAR: int
    LINEAR_MIPMAP_LINEAR: int

    POINTS: int
    LINES: int
    LINE_LOOP: int
    LINE_STRIP: int
    TRIANGLES: int
    TRIANGLE_STRIP: int
    TRIANGLE_FAN: int
    LINES_ADJACENCY: int
    LINE_STRIP_ADJACENCY: int
    TRIANGLES_ADJACENCY: int
    TRIANGLE_STRIP_ADJACENCY: int
    PATCHES: int

    def buffer(self, data, reserve, readable, writable, local) -> 'Buffer':
        '''```py
        vbo = ctx.buffer(moderngl.pack([
            0.0, 0.0,
            1.0, 1.0,
        ]))
        ```'''
        return Buffer()

    def framebuffer(self, color_attachments, depth_attachment) -> 'Framebuffer':
        '''```py
        fbo = ctx.framebuffer(
            ctx.renderbuffer(size),
            ctx.depth_renderbuffer(size),
        )
        ```'''
        return Framebuffer()

    def program(self, vertex_shader, fragment_shader, geometry_shader, tess_control_shader, tess_evaluation_shader, compute_shader, varyings) -> 'Program':
        '''```py
        prog = ctx.program(
            vertex_shader=\'''
                #version 330
                in vec2 in_vert;
                void main() {
                    gl_Position = vec4(in_vert, 0.0, 1.0);
                }
            \''',
            fragment_shader=\'''
                #version 330
                out vec4 f_color;
                void main() {
                    f_color = vec4(0.3, 0.5, 1.0, 1.0);
                }
            \''',
        )
        ```'''
        return Program()

    def query(self, time_elapsed, primitives_generated, samples_passed, any_samples_passed) -> 'Query':
        '''```py
        query = ctx.query(samples_passed=True)
        ```'''
        return Query()

    def renderbuffer(self, size, components, samples, dtype) -> 'Renderbuffer':
        '''```py
        rbo = ctx.renderbuffer(size)
        ```'''
        return Renderbuffer()

    def sampler(self, texture, filter, wrap, compare_func, anisotropy, lod_range, lod_bias, border) -> 'Sampler':
        '''```py
        sampler = ctx.sampler(texture, filter=ctx.NEAREST)
        ```'''
        return Sampler()

    def scope(self, enable, framebuffer, samplers, uniform_buffers, storage_buffers, line_width, point_size, viewport) -> 'Scope':
        '''```py
        scope = ctx.scope(ctx.DEPTH_TEST, framebuffer=fbo, samplers=[
            sampler.assing(0),
        ])
        ```'''
        return Scope()

    def texture(self, size, components, data, samples, levels, dtype) -> 'Texture':
        '''```py
        texture = ctx.texture((width, height), components=3)
        ```'''
        return Texture()

    def texture_array(self, size, components, data, samples, levels, dtype) -> 'Texture':
        '''```py
        texture = ctx.texture_array((width, height, length), components=3)
        ```'''
        return Texture()

    def texture_from(self, image) -> 'Texture':
        '''```py
        image = Image.open('image.png')
        texture = ctx.texture_from(image)
        ```'''
        return Texture()

    def texture_array_from(self, images) -> 'Texture':
        '''```py
        images = [Image.open('image1.png'), Image.open('image2.png')]
        texture = ctx.texture_array_from(images)
        ```'''
        return Texture()

    def vertex_array(self, program, bindings, mode) -> 'VertexArray':
        '''```py
        vao = ctx.vertex_array(prog, [
            vbo.bind('in_vert'),
        ])
        ```'''
        return VertexArray()

    def objects(self) -> list:
        '''the list of non released objects.'''
        return []

    def release(self, obj):
        '''release objects'''


class Uniform:
    def __init__(self, prog, name):
        self.prog = prog
        self.name = name

    @property
    def value(self):
        raise NotImplementedError()

    @value.setter
    def value(self, val):
        self.prog[self.name] = val

    def write(self, val):
        self.prog[self.name] = val


class GLSLError(Exception):
    pass


FORMAT = {
    'f1': (1, 0x101401),
    'f2': (2, 0x140B),
    'f4': (4, 0x1406),
    'f': (4, 0x1406),
    'i1': (1, 0x11400),
    'i2': (2, 0x11402),
    'i4': (4, 0x11404),
    'i': (4, 0x11404),
    'u1': (1, 0x11401),
    'u2': (2, 0x11403),
    'u4': (4, 0x11405),
    'u': (4, 0x11405),
    'nf1': (1, 0x101401),
    'nf2': (2, 0x10140B),
    'nf4': (4, 0x101406),
    'nf': (4, 0x101406),
    'ni1': (1, 0x101400),
    'ni2': (2, 0x101402),
    'ni4': (4, 0x101404),
    'ni': (4, 0x101404),
    'nu1': (1, 0x101401),
    'nu2': (2, 0x101403),
    'nu4': (4, 0x101405),
    'nu': (4, 0x101405),
}

GLTYPE_INFO = {
    0x1406: (1, 1, 0x1406), # 'f'),
    0x8B50: (2, 1, 0x1406), # 'f'),
    0x8B51: (3, 1, 0x1406), # 'f'),
    0x8B52: (4, 1, 0x1406), # 'f'),
    0x8B5A: (2, 2, 0x1406), # 'f'),
    0x8B5B: (3, 3, 0x1406), # 'f'),
    0x8B5C: (4, 4, 0x1406), # 'f'),
    0x8B65: (3, 2, 0x1406), # 'f'),
    0x8B66: (4, 2, 0x1406), # 'f'),
    0x8B67: (2, 3, 0x1406), # 'f'),
    0x8B68: (4, 3, 0x1406), # 'f'),
    0x8B69: (2, 4, 0x1406), # 'f'),
    0x8B6A: (3, 4, 0x1406), # 'f'),
    0x1404: (1, 1, 0x11404), # 'i'),
    0x8B53: (2, 1, 0x11404), # 'i'),
    0x8B54: (3, 1, 0x11404), # 'i'),
    0x8B55: (4, 1, 0x11404), # 'i'),
    0x1405: (1, 1, 0x11405), # 'u'),
    0x8DC6: (2, 1, 0x11405), # 'u'),
    0x8DC7: (3, 1, 0x11405), # 'u'),
    0x8DC8: (4, 1, 0x11405), # 'u'),
    0x8B56: (1, 1, 0x18B56), # 'p'),
    0x8B57: (2, 1, 0x18B56), # 'p'),
    0x8B58: (3, 1, 0x18B56), # 'p'),
    0x8B59: (4, 1, 0x18B56), # 'p'),
}

FLOAT_TYPES = {0x1406, 0x8B50, 0x8B51, 0x8B52, 0x8B5A, 0x8B5B, 0x8B5C, 0x8B65, 0x8B66, 0x8B67, 0x8B68, 0x8B69, 0x8B6A}


def texture_from(ctx, image):
    width, height = image.size
    components = len(image.mode)
    data = image.tobytes('raw', image.mode, 0, -1)
    return ctx.texture((width, height), components, data)


def texture_array_from(ctx, images):
    length = len(images)
    width, height = images[0].size
    components = len(images[0].mode)
    data = b''.join(img.tobytes('raw', img.mode, 0, -1) for img in images)
    return ctx.texture_array((width, height, length), components, data)


def texture_cube_from(ctx, images):
    assert len(images) == 6
    width, height = images[0].size
    components = len(images[0].mode)
    data = b''.join(img.tobytes('raw', img.mode, 0, -1) for img in images)
    return ctx.texture_cube((width, height), components, data)


def texture_cube_array_from(ctx, images):
    assert len(images) % 6 == 0
    width, height = images[0].size
    components = len(images[0].mode)
    data = b''.join(img.tobytes('raw', img.mode, 0, -1) for img in images)
    return ctx.texture_cube((width, height), components, data)


def texture3d_from(ctx, images):
    length = len(images)
    width, height = images[0].size
    components = len(images[0].mode)
    data = b''.join(img.tobytes('raw', img.mode, 0, -1) for img in images)
    return ctx.texture3d((width, height, length), components, data)


def bind_attributes(vao, bindings):
    max_vertices = None
    max_instances = None
    attr_info = vao.program.attributes
    for buffer, attribs, layout, offset, stride, divisor in bindings:
        total_size = 0
        binds = []

        if layout:
            assert re.match(r'^\d*n?[fiu][124]?( \d*n?[fiu][124]?)*$', layout)
            layout = [(int(x), y) for x, y in re.findall(r'(\d*)(n?[fiu][124]?)', layout)]
            assert len(layout) == len(attribs)

            for attr, (items, fmt) in zip(attribs, layout):
                location, attrib_type, alen = attr_info[attr]
                _, rows, _ = GLTYPE_INFO.get(attrib_type, (1, 1, 0x11405))
                cols = items // rows
                size, bind_type = FORMAT[fmt]
                total_size += alen * rows * cols * size
                for i in range(alen * rows):
                    binds.append((location + i, bind_type, cols, offset))
                    offset += cols * size

        else:
            for attr in attribs:
                location, attrib_type, alen = attr_info[attr]
                cols, rows, bind_type = GLTYPE_INFO.get(attrib_type, (1, 1, 0x11405))
                total_size += alen * rows * cols * 4
                for i in range(alen * rows):
                    binds.append((location + i, bind_type, cols, offset))
                    offset += cols * 4

        stride = max(stride, total_size)

        if divisor:
            if max_instances is None or max_instances < buffer.size // stride:
                max_instances = buffer.size // stride
        else:
            if max_vertices is None or max_vertices < buffer.size // stride:
                max_vertices = buffer.size // stride

        for location, typ, cols, offset in binds:
            vao.bind(buffer, location, typ, cols, offset, stride, divisor)

    vao.vertices = max_vertices
    if max_instances is not None:
        vao.instances = max_instances


def serialize_uniform(prog, uniform, value):
    import numpy as np
    if prog.uniforms[uniform][1] in FLOAT_TYPES:
        prog[uniform] = np.array([value], 'f4').tobytes()
    else:
        prog[uniform] = np.array([value], 'i4').tobytes()


def strsize(reserve):
    x, y = re.match(r'(\d+)([KM]?B)?', reserve).groups()
    return int(x) * {'B': 1, 'KB': 1024, 'MB': 1024 * 1024}[y]


def compiler_error(name, source, log):
    import textwrap
    def number_source(source):
        lines = textwrap.dedent(source).split('\n')
        fmt = ' %%%dd | %%s' % len(str(len(lines)))
        return '\n'.join(fmt % (i + 1, line) for i, line in enumerate(lines))
    message = 'compiler error (%s)\n\n%s\n\n%s' % (name, number_source(source), log)
    raise GLSLError(message)


def linker_error(log):
    raise GLSLError('linker error\n\n' + log)
