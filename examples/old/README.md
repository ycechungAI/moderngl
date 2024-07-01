# ModernGL Examples

Before running examples make sure you install the requirements:

```bash
pip install -r examples/requirements.txt
```

## Running Examples

All examples should use [moderngl-window] to make window creation and resource loading as simple
as possible and ensures all supported platforms and covered.

**Examples should work out of the box on Windows, Linux and OS X.**

## Options

```
optional arguments:
  -h, --help            show this help message and exit
  -wnd {glfw,headless,pygame2,pyglet,pyqt5,pyside2,sdl2,tk}, --window {glfw,headless,pygame2,pyglet,pyqt5,pyside2,sdl2,tk}
                        Name for the window type to use
  -fs, --fullscreen     Open the window in fullscreen mode
  -vs VSYNC, --vsync VSYNC
                        Enable or disable vsync
  -r RESIZABLE, --resizable RESIZABLE
                        Enable/disable window resize
  -s SAMPLES, --samples SAMPLES
                        Specify the desired number of samples to use for
                        multisampling
  -c CURSOR, --cursor CURSOR
                        Enable or disable displaying the mouse cursor
  --size SIZE           Window size
  --size_mult SIZE_MULT
                        Multiplier for the window size making it easy scale
                        the window
```

For example:

```bash
# Run example in fullscreen with 8 x MSAA, vsync and no visible mouse cursor
python 03_alpha_blending.py --fullscreen --samples 8 --cursor off --vsync on

# Run example using pyglet
python 03_alpha_blending.py --window pyglet

# Run example using pygame
python 03_alpha_blending.py --window pygame

# Run example using SDL2
python 03_alpha_blending.py --window sdl2

# Run example using PySide2
python 03_alpha_blending.py --window pyside2

# Run example using PyQt5
python 03_alpha_blending.py --window pyqt5

# Run example using glfw
python 03_alpha_blending.py --window glfw

# Run example using tkinter (no osx support yet)
python 03_alpha_blending.py --window tk
```

## Other Examples

The examples directory also contains a **large amount of examples** contributed
by various people over time. Browse around and see what you can find :)

[moderngl-window] also has various examples.

## Old examples

Also see the [old examples](old-examples).


[moderngl-window]: https://github.com/moderngl/moderngl-window
