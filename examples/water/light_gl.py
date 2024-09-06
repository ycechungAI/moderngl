# WebGL Water
# https://madebyevan.com/webgl-water/
# Copyright 2011 Evan Wallace
# Released under the MIT license

import re
import moderngl
from typing import Self
from PIL import Image
import numpy as np
import glm
from typing import Iterator


class Matrices:
    def __init__(self, model: glm.mat4 = None, view: glm.mat4 = None, projection: glm.mat4 = None) -> None:
        self._model = model or glm.identity(glm.mat4)
        self.view = view or glm.lookAt(
            (1.0, 1.0, 1.0), (0.0, 0.0, 0.0), (0.0, 1.0, 0.0))
        self.projection = projection or glm.perspective(45.0, 1.0, 0.1, 1000.0)

    @property
    def normal(self) -> glm.mat4:
        return glm.transpose(glm.inverse(self._model))

    @property
    def model_view(self) -> glm.mat4:
        return self.view * self._model

    @property
    def model_view_proj(self) -> glm.mat4:
        return self.projection * self.model_view

    @property
    def eye(self) -> glm.vec3:
        m = np.array(self.model_view).T
        axis_x: np.ndarray = m[0]
        axis_y: np.ndarray = m[1]
        axis_z: np.ndarray = m[2]
        offset: np.ndarray = m[3]
        return glm.vec3(
            -offset.dot(axis_x), -offset.dot(axis_y), -offset.dot(axis_z))

    def get(self, cap_name: str) -> glm.mat4:
        match cap_name:
            case "MVM":
                return self.model_view
            case "MVMI":
                return glm.inverse(self.model_view)
            case "PM":
                return self.projection
            case "PMI":
                return glm.inverse(self.projection)
            case "MVPM":
                return self.model_view_proj
            case "MVPMI":
                return glm.inverse(self.model_view_proj)
            case "NM":
                return self.normal


class MeshData:
    def __init__(self, vertices: np.ndarray, triangles: np.ndarray, coords: np.ndarray = None, normals: np.ndarray = None, color: np.ndarray = None, lines: np.ndarray = None) -> None:
        self.vertices = vertices
        self.triangles = triangles
        self.coords = coords
        self.normals = normals
        self.color = color
        self.lines = lines


class Mesh:
    def __init__(self, data: MeshData, ctx: moderngl.Context = None) -> None:
        ctx = ctx or moderngl.get_context()
        self.vbos = {
            "gl_Vertex": Mesh.layout(ctx, data.vertices, '3f'),
            "gl_TexCoord": Mesh.layout(ctx, data.coords, '2f'),
            "gl_Normal": Mesh.layout(ctx, data.normals, '3f'),
            "gl_Color": Mesh.layout(ctx, data.color, '4f'),
        }
        self.lines_ibo = ctx.buffer(data.lines.astype("i4").tobytes())
        if data.triangles is not None:
            self.triangles_ibo = ctx.buffer(
                data.triangles.astype("i4").tobytes())
        self.vaos: dict[int, tuple[moderngl.VertexArray, int]] = {}

    def layout(ctx: moderngl.Context, array: np.ndarray, t: str) -> bytes:
        if array is not None:
            return (ctx.buffer(array.astype("f4").tobytes()), t)
        return None

    def _layouts(self, prefix: str, used_attributes: set[str]) -> Iterator[tuple]:
        for attr_name in self.vbos:
            if self.vbos[attr_name] is None:
                continue
            prefixed_name = prefix + attr_name
            if prefixed_name in used_attributes:
                yield (*self.vbos[attr_name], prefixed_name)

    def build_vao(self, ctx: moderngl.Context, program: moderngl.Program, layouts: list, mode=moderngl.TRIANGLES):
        if mode == moderngl.TRIANGLES:
            vao = ctx.vertex_array(
                program, layouts, self.triangles_ibo, mode=moderngl.TRIANGLES)
        else:
            vao = ctx.vertex_array(
                program, layouts, self.lines_ibo, mode=moderngl.LINES)
        self.vaos[program.glo] = (vao, mode)
        return vao

    def vertex_array(self, ctx: moderngl.Context, program: moderngl.Program, prefix: str, used_attributes: dict[str, moderngl.Attribute], mode=moderngl.TRIANGLES) -> moderngl.VertexArray:
        vao, vao_mode = self.vaos.get(program.glo, (None, None))
        if vao is not None:
            if vao_mode == mode:
                return vao
            else:
                vao.release()

        layouts = list(self._layouts(prefix, used_attributes))
        vao = self.build_vao(ctx, program, layouts, mode)
        return vao


class MeshBuilder(MeshData):
    def __init__(self, vertices: np.ndarray, triangles: np.ndarray, coords: np.ndarray = None, normals: np.ndarray = None, lines: np.ndarray = None) -> None:
        super().__init__(vertices, triangles, coords, normals, None, lines)
        if self.normals is None:
            self.compute_normals()
        if self.lines is None:
            self.compute_wireframe()

    def compute_normals(self):
        self.normals = np.zeros(np.shape(self.vertices))
        pass

    def compute_wireframe(self):
        lines = {}
        for i in range(len(self.triangles)):
            t = self.triangles[i]
            for j in range(3):
                a = t[j]
                b = t[(j+1) % 3]
                l = (min(a, b), max(a, b))
                k = (l[0] << 16) + l[1]
                lines[k] = l
        lines = list(lines.values())
        self.lines = np.array(lines, dtype=np.int32)

    def build(self, ctx: moderngl.Context = None) -> Mesh:
        return Mesh(self, ctx)

    def seg_lines(*points: glm.vec3):
        vertices = list(points)
        num_vertices = len(vertices)
        num_vertices = num_vertices - (num_vertices % 2)
        vertices = np.array(vertices[0:num_vertices])
        lines = np.arange(num_vertices, dtype=np.int32)
        return MeshBuilder(vertices, None, normals=np.zeros(vertices.shape), lines=lines)

    def panel(detail: int = 1, detail_x: int = None, detail_y: int = None) -> Self:
        detail_x = detail_x or detail
        detail_y = detail_y or detail
        vertices = []
        coords = []
        normals = []
        triangles = []
        for y in range(detail_y + 1):
            t = y / detail_y
            for x in range(detail_x + 1):
                s = x / detail_x
                vertices.append((2.0 * s - 1.0, 2.0 * t - 1.0, 0.0))
                coords.append((s, t))
                normals.append((0.0, 0.0, 1.0))
                if x < detail_x and y < detail_y:
                    i = x + y * (detail_y + 1)
                    triangles.append((i, i + 1, i + detail_x + 1))
                    triangles.append(
                        (i + detail_x + 1, i + 1, i + detail_x + 2))
        return MeshBuilder(np.array(vertices), np.array(triangles, dtype=np.int32), np.array(coords), np.array(normals))

    cube_data = [
        [0, 4, 2, 6, -1, 0, 0],
        [1, 3, 5, 7, +1, 0, 0],
        [0, 1, 4, 5, 0, -1, 0],
        [2, 6, 3, 7, 0, +1, 0],
        [0, 2, 1, 3, 0, 0, -1],
        [4, 5, 6, 7, 0, 0, +1]
    ]

    def _pick_octant(i) -> tuple[int, int, int]:
        return ((i & 1) * 2 - 1, (i & 2) - 1, (i & 4) / 2 - 1)

    def cube() -> Self:
        vertices = []
        coords = []
        normals = []
        triangles = []
        for i in range(len(MeshBuilder.cube_data)):
            data = MeshBuilder.cube_data[i]
            v = i * 4
            for j in range(4):
                d = data[j]
                vertices.append(MeshBuilder._pick_octant(d))
                coords.append((j & 1, (j & 2) / 2))
                normals.append((data[4:7]))
            triangles.append((v, v+1, v+2))
            triangles.append((v+2, v+1, v+3))
        return MeshBuilder(np.array(vertices), np.array(triangles, dtype=np.int32), np.array(coords), np.array(normals))

    def _tri(flip: bool, a, b, c):
        return (a, c, b) if flip else (a, b, c)

    def _fix(x: np.ndarray) -> np.ndarray:
        return x + (x - x * x) / 2.0

    def _tri_idx(d: int, i: int, j: int) -> int:
        return i * d + (i - i * i) // 2 + j

    def sphere(detail: int = 6):
        detail_2 = detail * 2
        detail_22 = detail_2 * detail_2
        raw_vertices = []
        raw_abc = []
        vertices = []
        coords = []
        unique_map = {}
        for i in range(detail + 1):
            for j in range(detail - i + 1):
                raw_abc.append((i, j, detail - i - j))
        raw_vertices = MeshBuilder._fix((np.array(raw_abc) / detail))
        raw_normal = np.linalg.norm(raw_vertices, axis=1)
        raw_vertices = raw_vertices / raw_normal[:, np.newaxis]

        triangles = []
        for octant in range(8):
            scale = MeshBuilder._pick_octant(octant)
            flip = scale[0] * scale[1] * scale[2]
            data = []
            for abc, rv in zip(raw_abc, raw_vertices):
                a, b, c = abc
                k = a * scale[0] * detail_22 + b * \
                    scale[1] * detail_2 + c * scale[2]
                if k not in unique_map:
                    unique_map[k] = len(vertices)
                    vertex = rv * np.array(scale)
                    coord = (1 - a, c) if scale[1] > 0 else (c, 1 - a)
                    vertices.append(vertex)
                    coords.append(coord)
                data.append(unique_map[k])

            for i in range(1, detail + 1):
                for j in range(detail - i + 1):
                    a = MeshBuilder._tri_idx(detail + 1, i-1, j)
                    b = MeshBuilder._tri_idx(detail + 1, i, j)
                    triangles.append(MeshBuilder._tri(
                        flip, data[a], data[a + 1], data[b]))
                    if (i + j < detail):
                        triangles.append(MeshBuilder._tri(
                            flip, data[b], data[a + 1], data[b+1]))

        return MeshBuilder(np.array(vertices), np.array(triangles, dtype=np.int32), np.array(coords))


class Shader:
    header = """
        uniform mat3 gl_NormalMatrix;
        uniform mat4 gl_ModelViewMatrix;
        uniform mat4 gl_ProjectionMatrix;
        uniform mat4 gl_ModelViewProjectionMatrix;
        uniform mat4 gl_ModelViewMatrixInverse;
        uniform mat4 gl_ProjectionMatrixInverse;
        uniform mat4 gl_ModelViewProjectionMatrixInverse;
    """

    _vert_header = """
        attribute vec4 gl_Vertex;
        attribute vec2 gl_TexCoord;
        attribute vec3 gl_Normal;
        attribute vec4 gl_Color;
        vec4 ftransform() {
            return gl_ModelViewProjectionMatrix * gl_Vertex;
        }
    """
    vert_header = "#version 130\n" + header + _vert_header

    frag_header = """
        #version 130
        precision highp float;
    """ + header

    matrices_name = list(
        map(lambda m: m.groups()[0], re.finditer(r"\b(gl_[^;]*)\b;", header)))
    attributes_name = list(
        map(lambda m: m.groups()[0], re.finditer(r"\b(gl_[^;]*)\b;", _vert_header)))

    def dump_source(emsg: str, vert_shader: str, frag_shader: str):
        if emsg.find("vertex_shader") != -1:
            lines = vert_shader.splitlines()
            for num, line in enumerate(lines):
                print(f"{num+1:4d}", line)
        if emsg.find("fragment_shader") != -1:
            lines = frag_shader.splitlines()
            for num, line in enumerate(lines):
                print(f"{num+1:4d}", line)

    def fix(header: str, source: str, prefix: str) -> str:
        replace = {}
        source = header + re.sub(r"#extension.*\n", '', source)
        all_gl_vars = list(
            map(lambda m: m.groups()[0], re.finditer(r"\b(gl_\w+)\b;", header)))
        for gl_var_name in all_gl_vars:
            if gl_var_name not in replace:
                source = re.sub('\\b' + gl_var_name + '\\b',
                                prefix + gl_var_name, source)
        return source

    def __init__(self, vert_shader: str, frag_shader: str, ctx: moderngl.Context = None, prefix="LIGHTGL") -> None:
        self.ctx = ctx or moderngl.get_context()
        source: str = vert_shader + frag_shader
        self.use_matrices: dict[str, str] = {}
        for name in Shader.matrices_name:
            if source.find(name) != -1:
                # gl_NormalMatrix -> NM
                capitalLetters = re.sub(r"[a-z_]", '', name)
                self.use_matrices[capitalLetters] = prefix + name

        if source.find('ftransform') != -1:
            self.use_matrices['MVPM'] = prefix + "gl_ModelViewProjectionMatrix"

        self.prefix = prefix
        vertex_shader = Shader.fix(Shader.vert_header, vert_shader, prefix)
        fragment_shader = Shader.fix(Shader.frag_header, frag_shader, prefix)
        try:
            self.program = self.ctx.program(vertex_shader, fragment_shader)
        except moderngl.Error as e:
            Shader.dump_source(e.args[0], vertex_shader, fragment_shader)
            print(e)
            exit(-1)
        self.use_attributes = {name: self.program[name] for name in self.program
                               if isinstance(self.program[name], moderngl.Attribute)}

    def draw_mesh(self, mesh: Mesh, matrices: Matrices = None, unifroms: dict = {}, mode=moderngl.TRIANGLES):
        if matrices is not None:
            for m_cap_name in self.use_matrices:
                self.program[self.use_matrices[m_cap_name]].write(
                    matrices.get(m_cap_name))
        for k in unifroms:
            self.program[k] = unifroms[k]
        vao = mesh.vertex_array(self.ctx, self.program,
                                self.prefix, self.use_attributes, mode)
        vao.render(mode)


class Texture:
    def __init__(self, texture: moderngl.Texture = None, sampler: moderngl.Sampler = None) -> None:
        self.texture: moderngl.Texture = texture
        self.sampler: moderngl.Sampler = sampler
        self.framebuffer: moderngl.Framebuffer = None

    def use(self, location=0):
        if self.sampler:
            self.sampler.use(location)
        else:
            self.texture.use(location)

    def draw_to(self, ctx: moderngl.Context = None):
        if self.framebuffer is None:
            ctx = ctx or moderngl.get_context()
            self.framebuffer = ctx.framebuffer(
                self.texture, ctx.depth_renderbuffer(self.texture.size))
        self.framebuffer.use()

    def swap_with(self, other: Self):
        tmp_texture = self.texture
        tmp_sampler = self.sampler
        tmp_framebuffer = self.framebuffer
        self.texture = other.texture
        self.sampler = other.sampler
        self.framebuffer = other.framebuffer
        other.texture = tmp_texture
        other.sampler = tmp_sampler
        other.framebuffer = tmp_framebuffer


class RawTexture(Texture):
    def __init__(self, size: tuple[int, int], data: bytes = None, pixel_fmt='RGBA', ctx: moderngl.Context = None, repeat=(False, False, False), filter=(moderngl.LINEAR, moderngl.LINEAR), mipmaps=False, dtype="f1") -> None:
        ctx = ctx or moderngl.get_context()
        texture = ctx.texture(size, len(pixel_fmt), data, dtype=dtype)
        sampler = ctx.sampler(
            repeat_x=repeat[0], repeat_y=repeat[1], repeat_z=repeat[2], filter=filter, texture=texture)
        if mipmaps:
            texture.build_mipmaps()
        super().__init__(texture, sampler)


class ImageTexture(RawTexture):
    def __init__(self, image_path: str, pixel_fmt='RGBA', ctx: moderngl.Context = None, repeat=(False, False, False), filter=(moderngl.LINEAR, moderngl.LINEAR), mipmaps=False) -> None:
        img = Image.open(image_path).transpose(
            Image.FLIP_TOP_BOTTOM).convert(pixel_fmt)
        super().__init__(img.size, img.tobytes(),
                         pixel_fmt, ctx, repeat, filter, mipmaps)


def Cubemap(pos_x: str, neg_x: str, pos_y: str, neg_y: str, pos_z: str, neg_z: str, ctx: moderngl.Context = None) -> moderngl.TextureCube:
    ctx = ctx or moderngl.get_context()
    images_path = (pos_x, neg_x, pos_y, neg_y, pos_z, neg_z)
    images: list[Image.Image] = list(map(lambda p: Image.open(p).transpose(
        Image.FLIP_TOP_BOTTOM).convert("RGB"), images_path))
    image_size = images[0].size
    image_len = image_size[0] * image_size[1] * 3
    data = bytearray(image_len * 6)
    for i in range(6):
        data[i * image_len:] = images[i].tobytes()
    return ctx.texture_cube(image_size, 3, data)
