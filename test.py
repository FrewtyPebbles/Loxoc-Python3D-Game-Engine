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

def vec2_angle(xy1:tuple[float, float], xy2:tuple[float, float]):
    y = xy2[1] - xy1[1]
    x = xy2[0] - xy1[0]
    return math.atan2(y,x)

def circle(x:float, radius:float, x_orig:float, y_orig:float):
    inner_sqrt = radius**2-(x-x_orig)**2

    return y_orig + math.sqrt(inner_sqrt)

vel_yaw = 0.0
vel = 0.0
frict = 0.1
accel = 5
cam_dist = 10.0
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
    camera.position = car.position + car.rotation.forward * 300 + Vec3(0, 250, 0) # vec3 is offset

    if window.event.check_flag(EVENT_FLAG.MOUSE_MOTION):
        print(window.event.mouse.rel_x, window.event.mouse.rel_y)
        camera.rotation.x += math.radians(window.event.mouse.rel_y) if abs(window.event.mouse.rel_y) > 1 else 0
        mxdelta = math.radians(window.event.mouse.rel_x) if abs(window.event.mouse.rel_x) > 1 else 0
        camera.position.x = circle(car.position.x + mxdelta, cam_dist, car.position.x, car.position.y)
        
    
    camera.rotation.y = vec2_angle((camera.position.z, camera.position.x), (car.position.z, car.position.x))

    # Re-render the scene.
    window.update(render_list)
    # This also refreshes window.current_event.
