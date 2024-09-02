# Loxoc Game Engine

Important: This engine has been renamed from Runespoor to Loxoc.

> A python 3D game engine written in c++ using SDL, Assimp, OpenGL(GLAD), and GLM.

To install:

```
pip install Loxoc
```

Documentation Now Available [here](https://frewtypebbles.github.io/).

Please note:

 - Loxoc is still in development and only dev versions are available right now.  Everything is subject to change, be fixed, and improve.

 - When building from source `pkg-config` must be installed.  In addition to this, `GLM` must be installed and present in your c/c++ package manager's include directory.  Successful attempts to compile have been made on windows with `vcpkg` as the package manager.

## About:

Loxoc is a flexible, straight forward, multi-paradigm game engine that is built from the ground up with developer experience in mind.  Utilizing a game loop system similar to pygame, you are able to abstract elements of your game away from the game loop as much as you'd like.

For a taste of the api check out the [Quick Start Guide](https://frewtypebbles.github.io/quick_start.html#quick-start-page).

![](https://github.com/FrewtyPebbles/Runespoor-Python3D-Game-Engine/blob/main/tests/test_dev29.gif)

# TODO:

 - Add `from_raw` constructor to mesh to make procedural meshes possible.

 - Port over modified openGL api.

# Future Plans:

 - Add Vulkan backend.
