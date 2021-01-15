Install
=======

From PyPI (pip)
---------------

ModernGL is available on PyPI for Windows, OS X and Linux as pre-built
wheels. No complication is needed unless you are setting up a
development environment.

.. code-block:: sh

    $ pip install moderngl

Verify that the package is working:

.. code:: sh

    $ python -m moderngl
    moderngl 5.6.0
    --------------
    vendor: NVIDIA Corporation
    renderer: GeForce RTX 2080 SUPER/PCIe/SSE2
    version: 3.3.0 NVIDIA 441.87
    python: 3.7.6 (tags/v3.7.6:43364a7ae0, Dec 19 2019, 00:42:30) [MSC v.1916 64 bit (AMD64)]
    platform: win32
    code: 330

.. Note:: If you experience issues it's probably related to context creation.
          More configuration might be needed to run moderngl in some cases.
          This is especially true on linux running without X. See the context section.

Development environment
-----------------------

Ideally you want to fork the repository first.

.. code-block:: sh

    # .. or clone for your fork
    git clone https://github.com/moderngl/moderngl.git
    cd moderngl

Building on various platforms:

* On Windows you need visual c++ build tools installed:
  https://visualstudio.microsoft.com/visual-cpp-build-tools/
* On OS X you need X Code installed + command line tools
  (``xcode-select --install``)
* Building on linux should pretty much work out of the box
* To compile moderngl: ``python setup.py build_ext --inplace``

Package and dev dependencies:

* Install ``requirements.txt``, ``tests/requirements.txt`` and ``docs/requirements.txt``
* Install the package in editable mode: ``pip install -e .``
