# Runespoor Python 3D Game Engine

This is the a third refactor of my 3d engine, this time with opengl.

This 3D engine is made using proprietary c++ OpenGL and SDL code.

# Tests:

Texture mapping:

![](https://github.com/FrewtyPebbles/Python3D-Game-Engine/blob/main/tests/TextureMapping.gif)

# TODO:

 - Figure out better solution for multimesh assimp imports.  (Current solution groups all meshes from the scene in a python list.)

 - Refactor SDL-based event system (to include more events).

 - Add mouse events.

 - Add `from_raw` constructor to mesh to make procedural meshes possible.

 - Make render list managed by Window object.

 - Add Sprite object and Object2D object for 2D games/HUD.

# Future Plans:

 - Switch to Vulkan.
