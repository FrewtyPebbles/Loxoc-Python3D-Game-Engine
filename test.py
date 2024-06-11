from renderer import Vec3, Camera, Mesh, Object, Window, event,\
    Material, Shader, ShaderType
import math

dim = (1280, 720)

camera = Camera(Vec3(0.0,0.0,0.0), *dim, 1000, math.radians(60))
window = Window("Pirate ship", camera, *dim)

ship_material = Material(
    Shader.from_file("./default_vertex.glsl", ShaderType.VERTEX),
    Shader.from_file("./default_fragment.glsl", ShaderType.FRAGMENT)
)

ship = Object(Mesh.from_obj("meshes/pirate_ship/pirate_ship.obj"),
    Vec3(0.0,-70,-300), Vec3(0,0,0), material=ship_material)

render_list = [
    ship
]

vel = 0.0
frict = 0.1
while window.current_event != event.QUIT:
    if window.current_event == event.KEY_RIGHT:
        vel -= 0.3
    if window.current_event == event.KEY_LEFT:
        vel += 0.3
    vel = min(max(vel, -100), 100)
    ship.rotation.y += vel * window.dt

    vel -= math.copysign(frict, vel)

    window.update(render_list)