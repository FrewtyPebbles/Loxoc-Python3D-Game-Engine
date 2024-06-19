import time
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

window.lock_mouse(True)


# helper functions

def vec3x2_angle(v1:Vec3, v2:Vec3) -> list[float, float]:
    new_v = (v2-v1).get_normalized()
    yaw = math.atan2(new_v.z, new_v.x)
    pitch = math.asin(-new_v.y)
    return [pitch, yaw]

vel_yaw = 0.0
vel = 0.0
frict = 0.05
accel = 5
cam_dist = 300.0
while not window.event.check_flag(EVENT_FLAG.QUIT) and window.event.get_flag(EVENT_FLAG.KEY_ESCAPE) != EVENT_STATE.PRESSED:
    # Use WASD keys.
    if window.event.get_flag(EVENT_FLAG.KEY_d) == EVENT_STATE.PRESSED:
        # ROTATE RIGHT
        vel_yaw -= 0.2
    if window.event.get_flag(EVENT_FLAG.KEY_a) == EVENT_STATE.PRESSED:
        # ROTATE LEFT
        vel_yaw += 0.2
    if window.event.get_flag(EVENT_FLAG.KEY_s) == EVENT_STATE.PRESSED:
        # BACKWARDS
        vel -= accel
    if window.event.get_flag(EVENT_FLAG.KEY_w) == EVENT_STATE.PRESSED:
        # FORWARD
        vel += accel

    # Clamp and rotate, then apply friction.
    vel_yaw = min(max(vel_yaw, -100), 100) if abs(vel_yaw) > 0.05 else 0
    car.rotation.y += vel_yaw * window.dt
    vel_yaw -= math.copysign(frict, vel_yaw)
    
    # Clamp the velocity.
    vel = min(max(vel, -1000), 1000) if abs(vel) > 0.05 else 0
    # Move the car forwards with its forwards vector with a magnitude of `vel`
    car.position += -car.rotation.forward * vel * window.dt # window.dt is deltatime
    

    # Position the camera
    
    print()
    cam_angle = vec3x2_angle(camera.position, car.position)
    camera.rotation.x = cam_angle[0]
    camera.rotation.y = cam_angle[1]
    if window.event.check_flag(EVENT_FLAG.MOUSE_MOTION):
        camera.rotation.y += math.radians(window.event.mouse.rel_x*5)

        camera.rotation.x -= math.radians(window.event.mouse.rel_y*5)
    
    if window.event.check_flag(EVENT_FLAG.MOUSE_WHEEL):
        cam_dist -= window.event.mouse.wheel.y * 10

    camera.position = car.position - (camera.rotation.forward * cam_dist)
    # Re-render the scene.
    window.update(render_list)
    # This also refreshes window.current_event.
