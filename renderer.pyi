from enum import Enum

class ShaderType(Enum):
    FRAGMENT: 'ShaderType'
    VERTEX: 'ShaderType'

class Camera:
    """
    This class is the 3d perspective for a `Window`.
    """
    position:Vec3
    rotation:Vec3
    def __init__(self, position:Vec3, rotation:Vec3, view_width:int, view_height:int, focal_length:float, fov:float) -> None:...

class Mesh:
    """
    This class is used to create/hold onto/load meshes.
    """

    @staticmethod
    def from_file(file_path:str) -> list[Mesh]:
        """
        Returns all of the meshes from the supplied path to the 3d file.
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
    meshes:list[Mesh]
    material:Material
    position:Vec3
    rotation:Vec3
    scale:Vec3
    def __init__(self, mesh_list:list[Mesh], position:Vec3 = Vec3(0.0,0.0,0.0), rotation:Vec3 = Vec3(0.0,0.0,0.0), scale:Vec3 = Vec3(0.0,0.0,0.0), material:Material | None = None) -> None:...




    def render(self, camera:Camera):...

    def set_uniform(self, name:str, value:list[float] | int | float, type:str) ->None:
        """
        Sets the value of a uniform for the shaders in the object's material.
        """

class Shader:
    def __init__(self, source:str, shader_type:ShaderType) -> None:...

    def from_file(cls, filepath:str, type:ShaderType) -> Shader:...

class Vec3:
    def __init__(self, x:float, y:float, z:float) -> None:...

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

    def update(self, objects:list[Object]):
        """
        Re-renders and refreshes the current_event on the application window.
        """

    def lock_mouse(self, lock:bool):
        """
        Locks the mouse in the center of the window.
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
        For all of your 4-dimensional rotation needs
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

    def rotate(self, axis:Vec3, angle:float):
        """
        Mutably rotates the Quaternion by `angle` arround `axis`.
        `axis` is a Euler Angle Vec3 (in radians).
        """
