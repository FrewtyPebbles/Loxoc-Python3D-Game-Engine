import time
from Loxoc import (
    Vec3, Camera, Mesh, Object3D, Window, EVENT_FLAG,
    Material, Shader, ShaderType, EVENT_STATE, Quaternion,
    Texture, Sprite, Object2D, Vec2, PointLight, MeshDict
)
import math
from copy import copy

# The meshes used in this testfile are not provided with the library or source files.

dim = (1280, 720)
focal_length = 10000

camera = Camera(Vec3(0.0,-10,470), Vec3(0.0,0.0,0.0), *dim, focal_length, math.radians(60))
window = Window("Loxoc Engine Test Scene", camera, *dim, False, Vec3(0.1,0.1,0.1))

# Materials are equivalent to shader programs.
default_material = Material()


# # If you wanted to create a new shader program, normally you would do something like this:
#
# default_material = Material(
#   Shader.from_file("path/to/vertexshader.glsl", ShaderType.FRAGMENT),
#   Shader.from_file("path/to/fragmentshader.glsl", ShaderType.VERTEX)
# )
# 
# # Then to set a uniform:
#
# default_material.set_uniform("uniform_name", some_value, "i")
#
# # The third argument is a magic string that represents the type.  in this case the type is an integer.

car_meshes = Mesh.from_file("./meshes/vintage_racing_car/scene.gltf")



spr_doomguy = Sprite("./textures/doomguy.png")

doomguy = Object2D(spr_doomguy, scale=Vec2(0.3, 0.3))

car = Object3D(car_meshes,
    Vec3(0.0,-10,500), Vec3(0,0,0), Vec3(100,100,100))

car2 = Object3D(car_meshes,
    Vec3(300,30,500), Vec3(10,3.57,23.2), Vec3(100,100,100))

teapot = Object3D(Mesh.from_file("./meshes/teapot/scene.gltf"),
    Vec3(-100,0,200), Vec3(0,0,0), Vec3(1000,1000,1000))

cube = Object3D(Mesh.from_file("./meshes/basic_crate_2/scene.gltf"),
    Vec3(100,0,0), Vec3(0,0,0), Vec3(20,20,20))

pirate_ship = Object3D(Mesh.from_file("./meshes/pirate_ship/pirate_ship.obj"),
    Vec3(-100,0,300), Vec3(0,10,0))

test_light = PointLight(Vec3(-100,100,300), 500.0, Vec3(1,1,1) * 2.5)
test_light2 = PointLight(Vec3(20,100,0), 500.0, Vec3(0,0,2) * 3)

window.add_object_list([
    car,
    car2,
    teapot,
    cube,
    pirate_ship
])

window.add_object2d_list([
    doomguy
])

window.add_point_light_list([
    test_light,
    test_light2
])

window.lock_mouse(True)

vel_yaw = 0.0
vel = 0.0
frict = 0.1
accel = 5
cam_dist = 300.0
magic_turn_dampener = 4
mouse_sensitivity = 10
while not window.event.check_flag(EVENT_FLAG.QUIT) and window.event.get_flag(EVENT_FLAG.KEY_ESCAPE) != EVENT_STATE.PRESSED:
    if window.dt > 0:
        print(f"FRAMERATE: {1.0/window.dt:.1f} fps")
    else:
        print("FRAMERATE: inf fps")
    
    doomguy.position.x = math.sin(window.time_ns/1000000000)
    doomguy.position.y = math.cos(window.time_ns/1000000000)
    
    # Use WASD keys.
    if window.event.get_flag(EVENT_FLAG.KEY_d) == EVENT_STATE.PRESSED:
        # ROTATE RIGHT
        vel_yaw -= 0.3
    if window.event.get_flag(EVENT_FLAG.KEY_a) == EVENT_STATE.PRESSED:
        # ROTATE LEFT
        vel_yaw += 0.3
    if window.event.get_flag(EVENT_FLAG.KEY_s) == EVENT_STATE.PRESSED:
        # BACKWARDS
        vel -= accel
    if window.event.get_flag(EVENT_FLAG.KEY_w) == EVENT_STATE.PRESSED:
        # FORWARD
        vel += accel
    # apply a quaternion rotation arround the vector vec3(1,1,0)
    teapot.rotation = Quaternion.from_axis_angle(Vec3(1,1,0), math.radians(window.time_ns/10000000))

    # test_light.position = car.position
    # test_light.position.y += 200
    
    # Clamp and rotate, then apply friction.
    vel_yaw = min(max(vel_yaw, -100), 100) if abs(vel_yaw) > frict else 0
    car.rotation.rotate_yaw(vel_yaw/magic_turn_dampener * window.dt)
    vel_yaw -= math.copysign(frict, vel_yaw)
    
    # Clamp the velocity.
    vel = min(max(vel, -1000), 1000) if abs(vel) > frict else 0
    # Move the car forwards with its forwards vector with a magnitude of `vel` and apply friction
    # The right vector is the forward vector for this mesh because it is rotated 90 degrees by default.
    car.position += -car.rotation.forward * vel * window.dt # window.dt is deltatime
    vel -= math.copysign(frict, vel)
    
    mouse_scrolling = window.event.check_flag(EVENT_FLAG.MOUSE_WHEEL)

    cam_dist -= 100 * mouse_scrolling*window.event.mouse.wheel.y * window.dt

    
    cam_rot = copy(camera.rotation)

    mouse_moving = window.event.check_flag(EVENT_FLAG.MOUSE_MOTION)
    
    cam_rot.rotate(cam_rot.right, -mouse_moving*math.radians(window.event.mouse.rel_y * 10) * window.dt)
    if abs(cam_rot.up.y) > 0.4:
        camera.rotation = cam_rot
    camera.rotation.rotate(car.rotation.up, mouse_moving*math.radians(window.event.mouse.rel_x * 10) * window.dt)
    


    camera.position = car.position - camera.rotation.forward * cam_dist

    # Re-render the scene.
    window.update()
    # This also refreshes window.current_event.
    
    
