from renderer import Vec3, Camera, Mesh, Object, Window, event,\
    Material, Shader, ShaderType
import math

dim = (1280, 720)
focal_length = 2000

camera = Camera(Vec3(0.0,100.0,0.0), Vec3(0.0,0.0,0.0), *dim, focal_length, math.radians(60))
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
    Vec3(0.0,-100.0,-500), Vec3(0,0,0), Vec3(1,1,1), material=default_material)

car2 = Object(car_meshes,
    Vec3(300,0,-500), Vec3(10,3.57,23.2), material=default_material)

teapot = Object(Mesh.from_file("./meshes/teapot/teapot.obj"),
    Vec3(-100,0,-200), Vec3(10,3.57,23.2), material=default_material)

pirate_ship = Object(Mesh.from_file("./meshes/pirate_ship/pirate_ship.obj"),
    Vec3(-100,-100,-300), Vec3(0,10,0), material=default_material)

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
while window.current_event != event.QUIT:
    if window.current_event == event.KEY_RIGHT:
        vel_yaw -= 0.3
    if window.current_event == event.KEY_LEFT:
        vel_yaw += 0.3

    vel_yaw = min(max(vel_yaw, -100), 100)

    car.rotation.y += vel_yaw * window.dt

    vel_yaw -= math.copysign(frict, vel_yaw)

    if window.current_event == event.KEY_DOWN:
        vel -= accel
    if window.current_event == event.KEY_UP:
        vel += accel

    vel = min(max(vel, -1000), 1000)

    car.position += -car.rotation.forward * vel * window.dt

    vel -= math.copysign(frict, vel)

    window.update(render_list)