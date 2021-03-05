"""
Simple test script for gc

* Leak then mgl object fails on creation?
* If object doesn't have ctx attribute it's not valid?
"""
import logging
import time
from ctypes import c_long

import numpy as np
import moderngl

logging.basicConfig(level=logging.DEBUG)

ctx = moderngl.create_context(standalone=True, require=450)
ctx.gc_mode = "auto"


def ref_count():
    """Simple refcount demo"""
    var1 = [1, 2]
    print(c_long.from_address(id(var1)).value)


def test_texture():
    texture = ctx.texture((1000, 1000), 4, data=np.random.randint(0, 255, 1000 * 1000 * 4, dtype="u1"))


def test_texture_array():
    texture = ctx.texture_array((100, 100, 100), 4, data=np.random.randint(0, 255, 100 * 100 * 100 * 4, dtype="u1"))


def test_texture_array_failure():
    texture = ctx.texture_array((100, 100, 10), 4, data=np.random.randint(0, 255, 100 * 100 * 100 * 4, dtype="u1"))


def test_texture_array_mass_create():
    for _ in range(10_000):
        texture = ctx.texture_array((100, 100, 100), 4, data=np.random.randint(0, 255, 100 * 100 * 100 * 4, dtype="u1"))


def test_texture_mass_create():
    for _ in range(100):
        texture = ctx.texture((1000, 1000), 4, data=np.random.randint(0, 255, 1000 * 1000 * 4, dtype="u1"))


def test_texture_3d():
    texture = ctx.texture3d((100, 100, 100), 4, data=np.random.randint(0, 255, 100 * 100 * 100 * 4, dtype="u1"))


def test_texture_3d_mass_create():
    for _ in range(1_000):
        texture = ctx.texture3d((100, 100, 100), 4, data=np.random.randint(0, 255, 100 * 100 * 100 * 4, dtype="u1"))


def test_framebuffer():
    texture = ctx.texture((100, 100), 4)
    fbo = ctx.framebuffer(color_attachments=[texture])


def test_buffer():
    buffer = ctx.buffer(data=np.random.randint(0, 255, 1024 * 1024 * 1024, dtype="u1"))
    buffer.release()


def test_buffer_creation_failed():
    buffer = ctx.buffer(data=None)


def test_buffer_mass_create():
    for i in range(1000):
        b = ctx.buffer(data=np.random.randint(0, 255, 1024 * 1024, dtype="u1"))
        time.sleep(0.1)


def test_query_mass_create():
    for _ in range(1_000_000):
        q = ctx.query(samples=True, any_samples=True, time=True, primitives=True)
        # print(c_long.from_address(id(q.mglo)))
        time.sleep(0.001)

prog_src = {
        "vertex_shader": """
        #version 330
        in vec2 in_pos;
        void main() {
            gl_Position = vec4(in_pos, 0.0, 1.0);
        }
        """,
        "fragment_shader": """
        #version 330
        out vec4 color;
        void main() {
            color = vec4(1.0);
        }
        """
}


def test_vertex_array():
    buffer = ctx.buffer(reserve=1024)
    prog = ctx.program(**prog_src)
    vao = ctx.vertex_array(prog, [(buffer, "2f", "in_pos")])


def test_vertex_array_creation_failure():
    try:
        buffer = ctx.buffer(reserve=1024)
        prog = ctx.program(**prog_src)
        vao = ctx.vertex_array(prog, [(buffer, "2f", "in_pos_invalid")])
    except moderngl.error.Error:
        pass


def test_vertex_array_mass_create():
    # print(c_long.from_address(id(prog)).value)
    for i in range(1_000):
        prog = ctx.program(**prog_src)
        vbo = ctx.buffer(reserve=1_000_000)
        ibo = ctx.buffer(reserve=1_000_000)
        vao = ctx.vertex_array(prog, [(vbo, "2f", "in_pos")], index_buffer=ibo)

    time.sleep(4)


def test_compute_shader():
    compute_shader = ctx.compute_shader('''
        #version 430
        layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
        void main() {
        }
    ''')

def test_compute_shader_fail():
    compute_shader = ctx.compute_shader('''
        #version 430
        layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
        void main() {
        }
    ''')


def test_compute_shader_mass_create():
    for i in range(100_000):
        test_compute_shader()
        print(i)
        time.sleep(0.001)


# ref_count()
# test_texture()

# test_texture_array()
# test_texture_array_failure()
# test_texture_array_mass_create()

# test_texture_3d()
# test_texture_3d_mass_create()
# test_texture_mass_create()
# test_framebuffer()
# test_buffer()
# test_buffer_creation_failed()
# test_buffer_mass_create()
# test_query_mass_create()
# test_vertex_array()
# test_vertex_array_creation_failure()
# test_vertex_array_mass_create()

# test_compute_shader()
# test_compute_shader_fail()
test_compute_shader_mass_create()
