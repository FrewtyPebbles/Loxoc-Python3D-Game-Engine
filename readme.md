# 3D Projections in Python

This is the a third refactor of my 3d engine with opengl.

As for the future of this project, I hope to turn it into a game engine and general 3d rendering tool.

Here are some rendering tests using this library:

successfull application window test with keyboard control input:

![](https://github.com/FrewtyPebbles/python-c---rasterizer-library/blob/main/tests/sdl_3d_engine.gif)

pirate ship sailing arround teapot:

![](https://github.com/FrewtyPebbles/python-c---rasterizer-library/blob/main/tests/boat_and_teapot.gif)

Distance shaded texture map:

![](https://github.com/FrewtyPebbles/python-c---rasterizer-library/blob/main/tests/distance_shaded_texture_mapped.gif)

Check out **tests** to see more tests.

# TODO:

 - Add `Material` class for wrapping shader programs and their uniforms.  replace shader pointer in object with Material pointer.

 - Move shader linking function to material constructor.

 - Make more flexible setup system that supports github actions.

 - Make event system more robust.  eg: Allow for multiple key-presses at once.
