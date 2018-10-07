from .context import Context, create_context, extensions, hwinfo, glprocs, release
from .attribute import Attribute
from .buffer import Buffer
from .compute_shader import ComputeShader
from .error import Error
from .framebuffer import Framebuffer
from .limits import Limits
from .program import Program
from .query import Query
from .renderbuffer import Renderbuffer
from .sampler import Sampler
from .scope import Scope
from .texture_array import TextureArray
from .texture_cube_array import TextureCubeArray
from .texture_cube import TextureCube
from .texture import Texture
from .uniform import Uniform
from .vertex_array import VertexArray
from .mgl import initialize

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

initialize()
