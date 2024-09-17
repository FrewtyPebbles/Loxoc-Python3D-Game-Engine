import time
from Loxoc import (
    Vec3, Camera, Mesh, Object3D, Window, EVENT_FLAG,
    Material, Shader, ShaderType, EVENT_STATE, Quaternion,
    Texture, Sprite, Object2D, Vec2, PointLight, MeshDict, 
    DirectionalLight, SpotLight, BoxCollider, Matrix4x4 as Mat4,
    Vec4, Font, Text, CubeMap, SkyBox, Emitter, ConvexCollider,
    Model, Sound, RayCollider
)
import math
from copy import copy

def lerp(a: float, b: float, t: float) -> float:
    return (1 - t) * a + t * b

p = print

# The meshes used in this testfile are not provided with the library or source files.

dim = (1280, 720)
focal_length = 10000

camera = Camera(Vec3(0.0,-10,470), Vec3(0.0,0.0,0.0), *dim, focal_length, math.radians(60))
#1
window = Window("Loxoc Engine Test Scene", camera, *dim, False, Vec3(0.1,0.1,0.1))
#2
sky_box = SkyBox(CubeMap(
    "./skyboxes/skybox/right.jpg",
    "./skyboxes/skybox/left.jpg",
    "./skyboxes/skybox/top.jpg",
    "./skyboxes/skybox/bottom.jpg",
    "./skyboxes/skybox/back.jpg",
    "./skyboxes/skybox/front.jpg",
))
#3
window.sky_box = sky_box
#4
# Materials are equivalent to shader programs.
default_material = Material()
#5

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
p(0)
car_meshes = Model.from_file("./meshes/vintage_racing_car/scene.gltf")
# "./meshes/test_anim_mesh/test_anim_mesh.gltf"
# "./meshes/dancing_alien/scene.gltf"
# "./meshes/dancing_crab/scene.gltf"
#6
p(1)
test_anim_model = Mesh.from_file("./meshes/dancing_crab/scene.gltf")
#7
p(2)
spr_doomguy = Sprite("./textures/doomguy.png")
p(3)
doomguy = Object2D(spr_doomguy, camera, scale=Vec2(100,100), depth=-3.0)
p(4)
doomguy2 = Object2D(spr_doomguy, camera, position=Vec2(dim[0]/4,dim[1]/4), scale=Vec2(100,100), depth=-4.0)
p(5)
car = Object3D(car_meshes,
    Vec3(0.0,-10,500), Vec3(0,0,0), Vec3(100,100,100))
p(6)
car2 = Object3D(car_meshes,
    Vec3(300,30,500), Vec3(10,3.57,23.2), Vec3(100,100,100))
p(7)
teapot = Object3D(Mesh.from_file("./meshes/teapot/scene.gltf"),
    Vec3(-100,0,200), Vec3(0,0,0), Vec3(1000,1000,1000))
p(8)
cube = Object3D(Mesh.from_file("./meshes/basic_crate_2/scene.gltf"),
    Vec3(100,0,0), Vec3(0,0,0), Vec3(20,20,20))
p(9)
test_anim_obj = Object3D(test_anim_model,
    Vec3(100,30,60), Vec3(0,0,0), Vec3(100,100,100))
p(10)
pirate_ship = Object3D(Mesh.from_file("./meshes/pirate_ship/pirate_ship.obj"),
    Vec3(-100,0,300), Vec3(0,10,0))

p(11)
test_light = PointLight(Vec3(-100,100,300), 500.0, Vec3(1,1,1), 2.5)
p(12)
test_light2 = PointLight(Vec3(20,100,0), 500.0, Vec3(0,0,2), 3)

dir_light = DirectionalLight(Vec3(math.radians(180),0,0), intensity=2)

spot_light = SpotLight(Vec3(0,0,0), Vec3(0,0,0), intensity=20.0)

font_roboto = Font("./fonts/Roboto/Roboto-Regular.ttf")

text = Text("Hello!", Vec4(0,1,0.5,1), Vec2(dim[0]/2, dim[1]/2), font=font_roboto)

test_emitter = Emitter(
    Vec3(0,0,0),
    Quaternion.from_axis_angle(Vec3(1,0,0), math.radians(-90)),
    Vec2(1.0,1.0),Vec2(10.0,10.0),
    100,
    10.0,
    math.radians(30),
    0,
    0.10, 20.0,
    20, 50,
    Vec4(1,0,0,1),
    Vec4(0.5,0.5,0.5,1)
)

space_ship_mesh = Mesh.from_file("./meshes/space_ship/Space_Ship.gltf")

space_ship = Object3D(space_ship_mesh,
    Vec3(0.0, 100,500), Vec3(0,0,0), Vec3(1,1,1))

test_sound = Sound(window, "./sound/pop.wav", False)

music = Sound(window, "./sound/Vanguard.wav", True)
music.play(volume=0.5)

window.add_text_list([
    text
])

window.add_object_list([
    test_anim_obj,
    space_ship,
    car,
    car2,
    teapot,
    cube,
    pirate_ship
])

window.add_object2d_list([
    doomguy,
    doomguy2,
])

window.add_point_light_list([
    test_light,
    test_light2
])

window.add_directional_light_list([
    dir_light
])

window.add_spot_light_list([
    spot_light
])


window.add_emitter_list([
    test_emitter
])

test_emitter.start()

window.lock_mouse(True)

print("START COLLIDERS")
pirate_ship_collider = BoxCollider(pirate_ship, Vec3(30,0,0), Vec3(0,math.radians(45),0), Vec3(2,1,1))
pirate_ship_collider.show = True
pirate_ship.add_collider(pirate_ship_collider)


random_raycast_collider = RayCollider(Vec3(0,0,0), Quaternion(0,0,0,0))

print("END COLLIDERS")

# space_ship_collider = ConvexCollider(space_ship)
# space_ship.add_collider(space_ship_collider)

# teapot_collider = ConvexCollider(teapot)
# teapot.add_collider(teapot_collider)

car_collider = BoxCollider(car)
car_collider.show = True
car.add_collider(car_collider)

vel_yaw = 0.0
vel = 0.0
frict = 0.1
accel = 5
cam_dist = 300.0
magic_turn_dampener = 4
mouse_sensitivity = 10
while not window.event.check_flag(EVENT_FLAG.QUIT) and window.event.get_flag(EVENT_FLAG.KEY_ESCAPE) != EVENT_STATE.PRESSED:
    # if window.dt > 0:
    #     print(f"FRAMERATE: {1.0/window.dt:.1f} fps")
    # else:
    #     print("FRAMERATE: inf fps")
    
    if (rh := random_raycast_collider.get_collision(pirate_ship)).hit and window.event.get_flag(EVENT_FLAG.KEY_SPACE) == EVENT_STATE.PRESSED:
        
        mat = Mat4.from_identity(1.0)
        mat2 = Mat4.from_quaternion(Quaternion.from_euler(Vec3(0, math.radians(30 * window.dt), 0)))
        mat *= mat2
        pirate_ship.rotation = (Mat4.from_quaternion(pirate_ship.rotation) * mat).to_quaternion()
        test_emitter.position = rh.position
        test_emitter.direction = Quaternion.from_unit(rh.normal)

    doomguy.position.x = dim[0]/4 * math.sin(window.time_ns/1000000000) + dim[0]/2
    doomguy.position.y = dim[1]/4 * math.cos(window.time_ns/1000000000) + dim[1]/2
    
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

    if window.event.get_flag(EVENT_FLAG.KEY_f) == EVENT_STATE.PRESSED:
        # FULLSCREEN
        window.fullscreen = not window.fullscreen

    if window.event.get_flag(EVENT_FLAG.KEY_g) == EVENT_STATE.PRESSED:
        range_3d = 1000
        # "ArmatureAction" or "mixamo.com" or "Dance"
        relative_position = test_anim_obj.position - camera.position
        proj = camera.rotation.right.dot(relative_position)
        panning = max(min(proj / range_3d, 1.0), -1.0)
        vol = max(min(1.0, 1-camera.position.distance(test_anim_obj.position)/range_3d), 0.0)
        test_sound.play(volume = vol, panning=panning)
        test_anim_obj.play_animation("Dance")
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
    car.position -= car.rotation.forward * vel * window.dt # window.dt is deltatime
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

    spot_light.position = car.position + Vec3(0,60,0)
    spot_light.rotation = car.rotation

    random_raycast_collider.origin = car.position + Vec3(0,100,0)

    random_raycast_collider.direction = Quaternion.from_unit(-car.rotation.forward)

    text.text = repr(camera.position)

    # SPRITE DIMENSIONS TEST
    dt = window.deltatime
    mouse = window.event.mouse

    if mouse.state == EVENT_STATE.PRESSED:
        print(doomguy2.width, ", ", mouse.x)
        if doomguy2.position.x - doomguy2.width/2 < mouse.x < doomguy2.position.x + doomguy2.width/2 and \
        doomguy2.position.y - doomguy2.height/2 < mouse.y < doomguy2.position.y + doomguy2.height/2:#doomguy2.position.distance(Vec2(mouse.x, camera.view_height - mouse.y)) < 30:
            print("CLICKED DOOM GUY")

    # Re-render the scene.
    window.update()
    # This also refreshes window.current_event.
    

