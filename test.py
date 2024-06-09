from renderer import Vec3, Camera, Mesh, Object, Window, event
import math

dim = (1280, 720)

camera = Camera(Vec3(0.0,0.0,0.0), *dim, 1000, math.radians(60))
window = Window("Pirate ship", camera, *dim)

ship = Object(Mesh.from_obj("meshes\pirate_ship\pirate_ship.obj"),
    Vec3(0.0,0,-300), Vec3(0,0,0))

render_list = [
    ship
]

while True:
    ship.rotation += 0.01
    
    window.update(render_list)

    if window.current_event == event.QUIT:
        break