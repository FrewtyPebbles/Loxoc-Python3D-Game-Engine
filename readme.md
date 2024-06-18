# Runespoor Python 3D Game Engine

> A python 3D game engine written in c++ using SDL, OpenGL(GLAD), and GLM.

## About:

The Runespoor game engine is a flexible, straight forward, multi-paradigm game engine that is built from the ground up with developer experience in mind.  Utilizing a game loop system similar to pygame, you are able to abstract elements of your game away from the game loop as much as you'd like.

For a taste of the api check out one of the test files:

```py
from renderer import Vec3, Camera, Mesh, Object, Window, EVENT_FLAG,\
    Material, Shader, ShaderType, EVENT_STATE
import math

# The meshes used in this testfile are not provided with the library or source files.

dim = (1280, 720)
focal_length = 5000

camera = Camera(Vec3(0.0,0.0,0.0), Vec3(0.0,0.0,0.0), *dim, focal_length, math.radians(60))
window = Window("FBX Car Test", camera, *dim, False)

# Materials are equivalent to shader programs.
default_material = Material(
    Shader.from_file("./default_vertex.glsl", ShaderType.VERTEX),
    Shader.from_file("./default_fragment.glsl", ShaderType.FRAGMENT)
)

default_material.set_uniform("focal_length", focal_length, "i")
# "i" means the uniform is an integer type

car_meshes = Mesh.from_file("./meshes/fbx_car/svj_PACKED.fbx")

car = Object(car_meshes,
    Vec3(0.0,-100.0,500), Vec3(0,0,0), Vec3(1,1,1), material=default_material)

car2 = Object(car_meshes,
    Vec3(300,0,500), Vec3(10,3.57,23.2), material=default_material)

teapot = Object(Mesh.from_file("./meshes/teapot/teapot.obj"),
    Vec3(-100,0,200), Vec3(10,3.57,23.2), material=default_material)

pirate_ship = Object(Mesh.from_file("./meshes/pirate_ship/pirate_ship.obj"),
    Vec3(-100,-100,300), Vec3(0,10,0), material=default_material)

render_list = [
    car,
    car2,
    teapot,
    pirate_ship
]

vel_yaw = 0.0
vel = 0.0
frict = 0.1
accel = 5
while not window.current_event.check_flag(EVENT_FLAG.QUIT) and window.current_event.get_flag(EVENT_FLAG.KEY_ESCAPE) != EVENT_STATE.PRESSED:
    # Use WASD keys.
    if window.current_event.get_flag(EVENT_FLAG.KEY_d) == EVENT_STATE.PRESSED:
        # ROTATE RIGHT
        vel_yaw -= 0.2
    if window.current_event.get_flag(EVENT_FLAG.KEY_a) == EVENT_STATE.PRESSED:
        # ROTATE LEFT
        vel_yaw += 0.2
    if window.current_event.get_flag(EVENT_FLAG.KEY_s) == EVENT_STATE.PRESSED:
        # BACKWARDS
        vel -= accel
    if window.current_event.get_flag(EVENT_FLAG.KEY_w) == EVENT_STATE.PRESSED:
        # FORWARD
        vel += accel

    # Clamp and rotate, then apply friction.
    vel_yaw = min(max(vel_yaw, -100), 100)
    car.rotation.y += vel_yaw * window.dt
    vel_yaw -= math.copysign(frict, vel_yaw)
    
    # Clamp the velocity.
    vel = min(max(vel, -1000), 1000)
    # Move the car forwards with its forwards vector with a magnitude of `vel`
    car.position += -car.rotation.forward * vel * window.dt # window.dt is deltatime
    # Apply friction.
    vel -= math.copysign(frict, vel)

    # Position the camera
    camera.position = car.position + Vec3(0, 250, -800) # vec3 is offset
    
    # Re-render the scene.
    window.update(render_list)
    # This also refreshes window.current_event.
```

Then when we run it, it looks something like this:

![](https://github.com/FrewtyPebbles/Python3D-Game-Engine/blob/main/tests/car_vroom.gif)

# TODO:

 - Figure out better solution for multimesh assimp imports.  (Current solution groups all meshes from the scene in a python list.)

 - Add mouse events.

 - Add `from_raw` constructor to mesh to make procedural meshes possible.

 - Make render list managed by Window object.

 - Add Sprite object and Object2D object for 2D games/HUD.

 - Add Quaternions.

# Future Plans:

 - Switch to Vulkan.
