<div align="center">

[![preview](https://github.com/moderngl/moderngl/assets/11232402/b314f7af-0c0a-4b7d-b4f5-857f426454ca)](#readme)

</div>

# ModernGL

ModernGL is a Python wrapper over OpenGL Core. ModernGL simplifies the creation of graphics applications like scientific simulations, games or user interfaces.
Usually, acquiring in-depth knowledge of OpenGL requires a steep learning curve. In contrast, ModernGL is easy to learn and use.
ModernGL is capable of rendering with high performance and quality, with less code written.

```
pip install moderngl
```

- [Documentation](https://moderngl.readthedocs.io/)
- [Examples](https://github.com/moderngl/moderngl/tree/master/examples/#readme)
- [ModernGL on Github](https://github.com/moderngl/moderngl/)
- [ModernGL on PyPI](https://pypi.org/project/ModernGL/)
- [ModernGL Discord Server](https://discord.gg/UEMtW8D)

## Extras

### [glcontext](https://github.com/moderngl/glcontext) - Headless Context Creation

```
pip install moderngl[headless]
```

### [moderngl-window](https://github.com/moderngl/moderngl-window) - Window Creation and Resource Loading

```
pip install moderngl-window
```

## Features

- GPU accelerated high quality graphics
- Rendering modern OpenGL scenes with less headache
- Simpler and faster than PyOpenGL
- Can render without a window
- 100% Pythonic

## Sample usage

```py
>>> import moderngl
>>> ctx = moderngl.get_context()
>>> buf = ctx.buffer(b"Hello World!")  # allocated on the GPU
>>> buf.read()
b'Hello World!'
```

For complete examples please visit the [Examples](https://github.com/moderngl/moderngl/tree/master/examples/#readme).

## Easy to use with Pillow

```py
>>> img = Image.open("texture.jpg").convert("RGB")
>>> ctx.texture(img.size, 3, img.tobytes())
<Texture: 1>
```

## Easy to use with Numpy

```py
>>> ctx.buffer(np.array([0.0, 0.0, 1.0, 1.0], dtype="f4"))
<Buffer: 1>
```

## Compared to PyOpenGL

With PyOpenGL, using the original OpenGL API, you have to write three lines to
achieve a simple task like binding a VBO:

```py
vbo1 = GL.glGenBuffers(1)
GL.glBindBuffer(GL.GL_ARRAY_BUFFER, vbo1)
GL.glBufferData(GL.GL_ARRAY_BUFFER, b"Hello World!", GL.GL_STATIC_DRAW)

vbo2 = GL.glGenBuffers(1)
GL.glBindBuffer(GL.GL_ARRAY_BUFFER, vbo2)
GL.glBufferData(GL.GL_ARRAY_BUFFER, None, GL.GL_DYNAMIC_DRAW)
```

With ModernGL you need just one simple line per VBO to achieve the same results:

```py
vbo1 = ctx.buffer(b"Hello World!")
vbo2 = ctx.buffer(reserve=1024, dynamic=True)
```

## Development

```
git clone git@github.com:moderngl/moderngl.git
cd moderngl
python -m pip install -e .
```

## Notes

ModernGL may be faster than other libraries providing direct OpenGL access.
ModernGL is implemented in C++ and a single render call involving multiple OpenGL functions count as a single Python function call.

ModernGL require **OpenGL 3.3**. Compute Shaders require **OpenGL 4.3**.
Some functionality relies on specific extensions.

ModernGL can be used **anywhere where OpenGL is supported.** ModernGL is also working in a [headless](examples/headless) environment.

ModernGL is responsible for calling the OpenGL API and providing a Pythonic user-friendly API instead.
It is possible to integrate moderngl into any window libraries that support OpenGL.
Consider [moderngl-window](https://github.com/moderngl/moderngl-window) which implements many of them, plus it also helps with resource loading.

ModernGL does not implement the full OpenGL feature set or extensions. You can interact with the ModernGL objects from OpenGL.

## Citation

If you need to cite this repository in academic research:

```txt
@Online{Dombi2020,
  author = {Szabolcs Dombi},
  title = {ModernGL, high performance python bindings for OpenGL 3.3+},
  date = {2020-05-01},
  publisher = {GitHub},
  journal = {GitHub repository},
  howpublished = {\url{https://github.com/moderngl/moderngl}},
  commit = {<insert hash if needed>}
}
```

The commit hash can be found in the [Releases](https://github.com/moderngl/moderngl/releases).

## Community

- [ModernGL Discord Server](https://discord.gg/UEMtW8D)
- [Contributors](https://github.com/moderngl/moderngl/graphs/contributors)
