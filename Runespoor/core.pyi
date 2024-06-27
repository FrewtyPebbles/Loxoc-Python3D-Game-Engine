from enum import Enum
from typing import Generator

class ShaderType(Enum):
    FRAGMENT: 'ShaderType'
    VERTEX: 'ShaderType'

class Camera:
    """
    This class is the 3d perspective for a `Window`.
    """
    position:Vec3
    rotation:Quaternion
    def __init__(self, position:Vec3, rotation:Vec3, view_width:int, view_height:int, focal_length:float, fov:float) -> None:...

class Mesh:
    """
    This class is used to create/hold onto/load meshes.
    """

    @staticmethod
    def from_file(file_path: str) -> MeshDict:
        """
        Returns all of the meshes from the supplied path to the 3d file.
        """

    @property
    def name(self) -> str:
        """
        Name of the mesh.
        """

class MeshDict:

    def __init__(self, meshes: list[Mesh]) -> None:
        """
        A fast mesh container that can be used like a statically typed dict.
        """

    def insert(self, m: Mesh) -> None:
        """
        Insert a Mesh.  It will use the Mesh name as a key.
        """

    def get(self, name: str) -> list[Mesh]:
        """
        Get a Mesh from the dict by name.
        """

    def remove(self, name: str) -> None:
        """
        Remove a Mesh from the dict by name.
        """

    def __iter__(self) -> Generator[tuple[str, list[Mesh]], None, None]:
        """
        Itterates through the key value pairs of the dict.
        """

    def __getitem__(self, key: str) -> list[Mesh]:
        """
        Get a Mesh from the dict by name.
        """

class Material:
    """
    This is a shader program for an object.  It decides how to render an object.

    A shader program includes both a Vertex and Fragment shader.
    """
    def __init__(self, vertex:Shader = None, fragment:Shader = None) -> None:...

    def set_uniform(self, name:str, value:list[float] | int | float, type:str) ->None:
        """
        Sets the value of a uniform for the shaders in the material.
        """

class Object:
    """
    This class is your game object.
    """
    mesh_data:MeshDict
    material:Material
    position:Vec3
    rotation:Quaternion
    scale:Vec3
    def __init__(self, mesh_data:MeshDict, position:Vec3 = Vec3(0.0,0.0,0.0), rotation:Vec3 = Vec3(0.0,0.0,0.0), scale:Vec3 = Vec3(1.0,1.0,1.0), material:Material | None = None) -> None:...

    @property
    def position(self) -> Vec3:
        """
        The position of the object as a vec3.
        """

    @position.setter
    def position(self, value:Vec3):
        """
        The position of the object as a vec3.
        """

    @property
    def rotation(self) -> Quaternion:
        """
        The rotation of the object as a Quaternion.
        """

    @rotation.setter
    def rotation(self, value: Vec3 | Quaternion):
        """
        The rotation of the object as a Quaternion.  Can assign a Vec3 of EulerAngles to it.
        """

    @property
    def scale(self) -> Vec3:
        """
        The scale of the object as a vec3.
        """

    @scale.setter
    def scale(self, value:Vec3):
        """
        The scale of the object as a vec3.
        """

    def set_uniform(self, name:str, value:list[float] | int | float, type:str) ->None:
        """
        Sets the value of a uniform for the shaders in the object's material.
        """

class Shader:
    def __init__(self, source:str, shader_type:ShaderType) -> None:...

    @classmethod
    def from_file(cls, filepath:str, type:ShaderType) -> Shader:...

class Vec3:
    def __init__(self, x:float, y:float, z:float) -> None:
        """
        A size 3 vector used for 3D positioning and (sometimes)rotation.
        """

    def __repr__(self) -> str:
        """
        Vec3 str representation.
        """

    @property
    def quaternion(self) -> Quaternion:
        """
        Get the quaternion form of the vector.  Can also be assigned to, but not mutated.
        """

    @quaternion.setter
    def quaternion(self, value: Vec3 | Quaternion):...

    @property
    def x(self)->float:...

    @x.setter
    def x(self, value:float):...

    @property
    def y(self)->float:...

    @y.setter
    def y(self, value:float):...

    @property
    def z(self)->float:...

    @z.setter
    def z(self, value:float):...

    @property
    def up(self) -> Vec3:
        """
        The up directional vector.
        """

    @property
    def right(self) -> Vec3:
        """
        The right directional vector.
        """

    @property
    def forward(self) -> Vec3:
        """
        The forward directional vector.
        """


    # OPERATORS

    def __neg__(self) -> Vec3:...

    def __add__(self, other:Vec3 | float) -> Vec3:...

    def __sub__(self, other:Vec3 | float) -> Vec3:...

    def __mul__(self, other:Vec3 | float | Quaternion) -> Vec3:...

    def __truediv__(self, other:Vec3 | float) -> Vec3:...

    def dot(self, other:Vec3) -> float:...

    def cross(self, other:Quaternion | Vec3)->Vec3:...

    def get_magnitude(self)->float:...

    def get_normalized(self)->Vec3:...

    def to_quaternion(self) -> Quaternion:
        """
        Constructs a Quaternion from the given Euler Angle Vec3 (in radians).
        """


class Vec2:
    def __init__(self, x:float, y:float) -> None:
        """
        A size 2 vector used for 2D positioning and (sometimes)rotation.
        """

    def __repr__(self) -> str:
        """
        Vec2 str representation.
        """

    def __neg__(self) -> Vec2:...


    @property
    def angle(self) -> float:
        """
        The vector converted to an angle in radians.
        """

    @angle.setter
    def angle(self, value: float):
        """
        Setting the angle of the vector in radians
        """

    def to_angle(self) -> float:
        """
        Convert the vector to an angle in radians
        """

    @property
    def x(self) -> float:...

    @x.setter
    def x(self, value:float):...

    @property
    def y(self) -> float:...

    @y.setter
    def y(self, value:float):...


    # OPERATORS

    def __add__(self, other:Vec2 | float) -> Vec2:...

    def __sub__(self, other:Vec2 | float) -> Vec2:...

    def __mul__(self, other: Vec2 | float) -> Vec2:...

    def __truediv__(self, other:Vec2 | float) -> Vec2:...

    def dot(self, other:Vec2) -> float:
        """
        Calculate the dot product of 2 `Vec2`s.
        """

    def get_magnitude(self) -> float:
        """
        Calculate the `Vec2`'s magnitude.
        """

    def get_normalized(self) -> Vec2:
        """
        Calculate the normalized `Vec2` of the `Vec2`.
        """

    @classmethod
    def from_angle(cls, angle:float) -> Vec2:
        """
        Construct a normalized `Vec2` given an angle.
        """


class Window:
    """
    An application window that can be used as a render medium.
    """

    def __init__(self, title:str, cam:Camera, width:int, height:int, fullscreen:bool = False) -> None:...
    
    @property
    def event(self) -> Event:
        """
        This is the most recent event that the window recieved durring `Window.update(self, objects)`
        """

    @property
    def deltatime(self) -> float:
        """
        The current deltatime for the window.
        """

    @property
    def dt(self) -> float:
        """
        The current deltatime for the window.
        """
    
    @property
    def time_ns(self) -> int:
        """
        Time since the launch of the window in nanoseconds.
        """

    @property
    def time(self) -> int:
        """
        Time since the launch of the window in seconds.
        """

    def update(self) -> None:
        """
        Re-renders and refreshes the current_event on the application window.
        """

    def lock_mouse(self, lock:bool) -> None:
        """
        Locks the mouse in the center of the window.
        """

    def add_object(self, obj: Object) -> None:
        """
        Adds the `Object` to the scene.  This ensures that the `Object` is rendered by the camera.
        """

    def remove_object(self, obj: Object) -> None:
        """
        Removes the `Object` from the scene.  Only `Object`s which are in the scene will be rendered by the camera.
        """

    def add_object_list(self, objs: list[Object]) -> None:
        """
        Adds multiple `Object`s to the scene.  This ensures that they are rendered by the camera.
        """

    def remove_object_list(self, objs: list[Object]) -> None:
        """
        Removes multiple `Object`s from the scene.  Only `Object`s which are in the scene will be rendered by the camera.
        """

    def add_object2d(self, obj: Object2D) -> None:
        """
        Adds the `Object2D` to the scene.  This ensures that the `Object2D` is rendered by the camera.
        """

    def remove_object2d(self, obj: Object2D) -> None:
        """
        Removes the `Object2D` from the scene.  Only `Object2D`s which are in the scene will be rendered by the camera.
        """

    def add_object2d_list(self, objs: list[Object2D]) -> None:
        """
        Adds multiple `Object2D`s to the scene.  This ensures that they are rendered by the camera.
        """

    def remove_object2d_list(self, objs: list[Object2D]) -> None:
        """
        Removes multiple `Object2D`s from the scene.  Only `Object2D`s which are in the scene will be rendered by the camera.
        """

class EVENT_FLAG(Enum):

    # WINDOW MANAGEMENT
    WINDOW_MINIMIZE: 'EVENT_FLAG'
    WINDOW_CLOSE: 'EVENT_FLAG'
    QUIT: 'EVENT_FLAG'

    # KEYS
    KEY_UP: 'EVENT_FLAG'
    KEY_DOWN: 'EVENT_FLAG'
    KEY_RIGHT: 'EVENT_FLAG'
    KEY_LEFT: 'EVENT_FLAG'
    KEY_SPACE: 'EVENT_FLAG'
    KEY_a: 'EVENT_FLAG'
    KEY_b: 'EVENT_FLAG'
    KEY_c: 'EVENT_FLAG'
    KEY_d: 'EVENT_FLAG'
    KEY_e: 'EVENT_FLAG'
    KEY_f: 'EVENT_FLAG'
    KEY_g: 'EVENT_FLAG'
    KEY_h: 'EVENT_FLAG'
    KEY_i: 'EVENT_FLAG'
    KEY_j: 'EVENT_FLAG'
    KEY_k: 'EVENT_FLAG'
    KEY_l: 'EVENT_FLAG'
    KEY_m: 'EVENT_FLAG'
    KEY_n: 'EVENT_FLAG'
    KEY_o: 'EVENT_FLAG'
    KEY_p: 'EVENT_FLAG'
    KEY_q: 'EVENT_FLAG'
    KEY_r: 'EVENT_FLAG'
    KEY_s: 'EVENT_FLAG'
    KEY_t: 'EVENT_FLAG'
    KEY_u: 'EVENT_FLAG'
    KEY_v: 'EVENT_FLAG'
    KEY_w: 'EVENT_FLAG'
    KEY_x: 'EVENT_FLAG'
    KEY_y: 'EVENT_FLAG'
    KEY_z: 'EVENT_FLAG'
    KEY_LSHIFT: 'EVENT_FLAG'
    KEY_RSHIFT: 'EVENT_FLAG'
    KEY_LCTRL: 'EVENT_FLAG'
    KEY_LALT: 'EVENT_FLAG'
    KEY_ESCAPE: 'EVENT_FLAG'
    KEY_COMMA: 'EVENT_FLAG'
    KEY_PERIOD: 'EVENT_FLAG'
    KEY_FORWARDSLASH: 'EVENT_FLAG'
    KEY_BACKSLASH: 'EVENT_FLAG'
    KEY_OPEN_BRACKET: 'EVENT_FLAG'
    KEY_CLOSE_BRACKET: 'EVENT_FLAG'
    KEY_SEMICOLON: 'EVENT_FLAG'
    KEY_QUOTE: 'EVENT_FLAG'
    KEY_ENTER: 'EVENT_FLAG'
    KEY_BACKSPACE: 'EVENT_FLAG'
    KEY_TAB: 'EVENT_FLAG'
    KEY_BACKTICK: 'EVENT_FLAG'
    KEY_DASH: 'EVENT_FLAG'
    KEY_EQUALS: 'EVENT_FLAG'
    KEY_1: 'EVENT_FLAG'
    KEY_2: 'EVENT_FLAG'
    KEY_3: 'EVENT_FLAG'
    KEY_4: 'EVENT_FLAG'
    KEY_5: 'EVENT_FLAG'
    KEY_6: 'EVENT_FLAG'
    KEY_7: 'EVENT_FLAG'
    KEY_8: 'EVENT_FLAG'
    KEY_9: 'EVENT_FLAG'
    KEY_0: 'EVENT_FLAG'
    KEY_RCTRL: 'EVENT_FLAG'
    KEY_RALT: 'EVENT_FLAG'
    
    # Mouse Events
    MOUSE_BUTTON_DOWN: 'EVENT_FLAG'
    MOUSE_BUTTON_UP: 'EVENT_FLAG'
    MOUSE_WHEEL: 'EVENT_FLAG'
    MOUSE_MOTION: 'EVENT_FLAG'

class EVENT_STATE(Enum):
    NONE: 'EVENT_STATE'
    PRESSED: 'EVENT_STATE'
    RELEASED: 'EVENT_STATE'

class EVENT_STATE(Enum):
    NONE: 'EVENT_STATE'
    PRESSED: 'EVENT_STATE'
    RELEASED: 'EVENT_STATE'

class MOUSE_EVENT_TYPE(Enum):
    BUTTON_DOWN: 'MOUSE_EVENT_TYPE'
    BUTTON_UP: 'MOUSE_EVENT_TYPE'
    NONE: 'MOUSE_EVENT_TYPE'

class MOUSE_BUTTON(Enum):
    LEFT: 'MOUSE_BUTTON'
    RIGHT: 'MOUSE_BUTTON'
    MIDDLE: 'MOUSE_BUTTON'

class MOUSE_WHEEL_DIRECTION(Enum):
    FLIPPED: 'MOUSE_WHEEL_DIRECTION'
    NORMAL: 'MOUSE_WHEEL_DIRECTION'

class MouseWheel:
    int_x:int
    int_y:int
    x:float
    y:float
    direction:MOUSE_WHEEL_DIRECTION

class MouseDevice:
    id:int
    timestamp:int
    x:int
    y:int
    rel_x:int
    rel_y:int
    clicks:int
    type:MOUSE_EVENT_TYPE
    state:EVENT_STATE
    button:MOUSE_BUTTON
    wheel:MouseWheel

class Event:
    """
    This class contains all information pertaining to the window events.
    """
    def get_flag(self, _event: EVENT_FLAG) -> EVENT_STATE:
        """
        Checks if the provided event flag is occuring.
        """

    def check_flag(self, _event: EVENT_FLAG) -> bool:
        """
        Checks if the provided event flag is occuring.
        """

    @property
    def mouse(self) -> MouseDevice:
        """
        Gets the current mouse device. Contains current mouse events.
        """

    def get_mouse(self, id:int) -> MouseDevice:
        """
        Gets the mouse device with the provided id.  Contains current mouse events.
        """

class Quaternion:
    def __init__(self, w:float, x:float, y:float, z:float) -> None:
        """
        For all of your 4-dimensional rotation needs.
        """

    def __repr__(self) -> str:
        ...

    def __neg__(self) -> Quaternion:
        ...

    @property
    def w(self) -> float:...

    @w.setter
    def w(self, value:float):...

    @property
    def x(self) -> float:...

    @x.setter
    def x(self, value:float):...

    @property
    def y(self) -> float:...

    @y.setter
    def y(self, value:float):...

    @property
    def z(self) -> float:...

    @z.setter
    def z(self, value:float):...

    @property
    def up(self) -> Vec3:
        """
        The up directional vector.
        """

    @property
    def right(self) -> Vec3:
        """
        The right directional vector.
        """

    @property
    def forward(self) -> Vec3:
        """
        The forward directional vector.
        """

    @property
    def euler_angles(self) -> Vec3:
        """
        ### Read:

        Get the Euler Angle Vec3 form of the Quaternion.  Can also be assigned to, but not mutated.
        
        Use `pitch`, `yaw`, and `roll` for mutating rotation.
        """

    @euler_angles.setter
    def euler_angles(self, value:Vec3 | Quaternion):
        """
        ### Write:
        
        Set the rotation to a Euler angle Vec3 in radians.  This is also not mutable.
        """
    
    @property
    def euler_pitch(self) -> float:
        """
        Rotation about the x axis.
        """

    def rotate_pitch(self, value:float) -> None:
        """
        Rotates the Euler pitch (x) axis by the provided value.
        """

    @property
    def euler_yaw(self) -> float:
        """
        Rotation about the y axis.
        """

    def rotate_yaw(self, value:float) -> None:
        """
        Rotates the Euler yaw (y) axis by the provided value.
        """

    @property
    def euler_roll(self) -> float:
        """
        Rotation about the z axis.
        """

    def rotate_roll(self, value:float) -> None:
        """
        Rotates the Euler roll (z) axis by the provided value.
        """

    # OPERATORS

    def __add__(self, other:Quaternion | float) -> Quaternion:...

    def __sub__(self, other:Quaternion | float) -> Quaternion:...

    def __mul__(self, other:Quaternion | float | Vec3) -> Quaternion:...

    def __truediv__(self, other:Quaternion | float) -> Quaternion:...

    def dot(self, other:Quaternion) -> float:...

    def cross(self, other:Quaternion | Vec3) -> float:...

    def get_magnitude(self) -> float:...

    def get_normalized(self) -> Quaternion:...

    def to_euler(self) -> Vec3:
        """
        Converts the Quaternion to a Euler Angle Vec3 (in radians).
        """

    @staticmethod
    def from_euler(euler_vec: Vec3) -> Quaternion:
        """
        Converts the provided Euler Angle Vec3 (in radians) to a Quaternion.
        """

    @staticmethod
    def from_axis_angle(axis:Vec3, angle:float) -> Quaternion:
        """
        Constructs a Quaternion with a rotation of `angle` arround `axis`. 
        `axis` is a Euler Angle Vec3 (in radians).
        """

    def rotate(self, axis:Vec3, angle:float) -> None:
        """
        Mutably rotates the Quaternion by `angle` arround `axis`.
        `axis` is a Euler Angle Vec3 (in radians).
        """

    @staticmethod
    def from_quat(quat:Quaternion) -> Quaternion:
        """
        Used to construct a copy of a Quaternion.
        """

class TextureFiltering(Enum):
        NEAREST: 'TextureFiltering'
        LINEAR: 'TextureFiltering'

class TextureWraping(Enum):
    REPEAT: 'TextureWraping'
    MIRRORED_REPEAT: 'TextureWraping'
    CLAMP_TO_EDGE: 'TextureWraping'
    CLAMP_TO_BORDER: 'TextureWraping'

class Texture:
    @classmethod
    def from_file(cls, file_path:str, wrap:TextureWraping = TextureWraping.REPEAT, filtering:TextureFiltering = TextureFiltering.LINEAR) -> Texture:
        """
        Create a texture from the specified file.
        """

class Sprite:
    texture:Texture
    def __init__(self, file_path:str) -> None:
        """
        Creates a sprite of the supplied texture.
        """
        
    @classmethod
    def from_texture(cls, tex: Texture) -> Sprite:
        """
        Creates a sprite of the supplied texture.
        """


class Object2D:
    def __init__(self, sprite: Sprite, position:Vec2 = Vec2(0.0,0.0),
    rotation:float = 0.0, scale:Vec2 = Vec2(1.0, 1.0),
    material:Material = None) -> None:
        """
        `Object2D`s are rendered infront of all objects rendered in 3D space.
        This class is good for 2D games, GUIs, game heads up displays or anything you might need to render something in 2D space for.
        """

    @property
    def position(self) -> Vec2:
        """
        The position of the `Object2D` on the screen as a `Vec2`.
        """

    @position.setter
    def position(self, value: Vec2) -> None:...

    @property
    def rotation(self) -> float:
        """
        The rotation of the `Object2D` on the screen as a `Vec2`.
        """

    @rotation.setter
    def rotation(self, value: Vec2 | float) -> None:...

    @property
    def scale(self) -> Vec2:
        """
        The scale of the `Object2D` on the screen as a `Vec2`.
        """

    @scale.setter
    def scale(self, value:Vec2) -> None:...


    def set_uniform(self, name:str, value:list[float] | int | float, type:str) -> None:
        """
        Sets the value of a uniform for the shaders in the object2d's material.
        """