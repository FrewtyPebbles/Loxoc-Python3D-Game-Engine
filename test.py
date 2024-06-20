from renderer import (
    Vec3, Camera, Mesh, Object, Window, EVENT_FLAG,
    Material, Shader, ShaderType, EVENT_STATE, Quaternion
)
import math

# The meshes used in this testfile are not provided with the library or source files.

dim = (1280, 720)
focal_length = 10000

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
    Vec3(-100,0,200), Vec3(0,0,0), material=default_material)

pirate_ship = Object(Mesh.from_file("./meshes/pirate_ship/pirate_ship.obj"),
    Vec3(-100,-100,300), Vec3(0,10,0), material=default_material)

render_list = [
    car,
    car2,
    teapot,
    pirate_ship
]

window.lock_mouse(True)

vel_yaw = 0.0
vel = 0.0
frict = 0.1
accel = 5
cam_dist = 300.0
magic_turn_dampener = 4
mouse_sensitivity = 10
counter = 0
counter_speed = 1
while not window.event.check_flag(EVENT_FLAG.QUIT) and window.event.get_flag(EVENT_FLAG.KEY_ESCAPE) != EVENT_STATE.PRESSED:
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
    teapot.rotation = Quaternion.from_axis_angle(teapot.rotation.up.get_normalized(), math.radians(counter)).to_euler()
    # Clamp and rotate, then apply friction.
    vel_yaw = min(max(vel_yaw, -100), 100) if abs(vel_yaw) > frict else 0
    car.rotation.y += vel_yaw/magic_turn_dampener * window.dt
    vel_yaw -= math.copysign(frict, vel_yaw)
    
    # Clamp the velocity.
    vel = min(max(vel, -1000), 1000) if abs(vel) > frict else 0
    # Move the car forwards with its forwards vector with a magnitude of `vel` and apply friction
    car.position += -car.rotation.forward * vel * window.dt # window.dt is deltatime
    vel -= math.copysign(frict, vel)
    
    # Rotate the camera
    if window.event.check_flag(EVENT_FLAG.MOUSE_MOTION):
        camera.rotation.yaw += math.radians(window.event.mouse.rel_x * mouse_sensitivity * window.dt)

        camera.rotation.pitch -= math.radians(window.event.mouse.rel_y * mouse_sensitivity * window.dt)
    
    # zoom in and out
    if window.event.check_flag(EVENT_FLAG.MOUSE_WHEEL):
        cam_dist -= window.event.mouse.wheel.y * 10

    # Position the camera behind the car based on its forward vector
    camera.position = car.position - (camera.rotation.euler_angles.forward * cam_dist)

    # Attract camera to car direction:
    camera.rotation.yaw += vel_yaw/magic_turn_dampener * window.dt if vel_yaw/magic_turn_dampener * window.dt > 1 else 0
    print(camera.rotation)
    # Re-render the scene.
    window.update(render_list)
    # This also refreshes window.current_event.
    counter += counter_speed
