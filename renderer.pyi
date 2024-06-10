from enum import Enum

class ShaderType(Enum):
    FRAGMENT: 'ShaderType'
    VERTEX: 'ShaderType'

class Camera:
    """
    This class is the 3d perspective for a `Window`.
    """
    def __init__(self, position:Vec3, view_width:int, view_height:int, focal_length:float, fov:float) -> None:...

class Mesh:
    """
    This class is used to create/hold onto/load meshes.
    """

    @staticmethod
    def from_obj(file_path:str, vertex_shader:Shader = None, fragment_shader:Shader = None) -> Mesh:
        """
        Constructs a `Mesh` from a `.obj` file
        """

class V3Property:
    """
    This is functionally equivalent to a Vec3.
    """

    def get_magnitude(self) -> float:...

    def get_normalized(self) -> Vec3:...

    @property
    def x(self)->float:...
    
    @property
    def y(self)->float:...

    @property
    def z(self)->float:...

    @x.setter
    def x(self, value:float):...
    
    @y.setter
    def y(self, value:float):...

    @z.setter
    def z(self, value:float):...

    def __add__(self, other:Vec3 | float) -> Vec3:...

    def __sub__(self, other:Vec3 | float) -> Vec3:...

    def __mul__(self, other:Vec3 | float) -> Vec3:...

    def __truediv__(self, other:Vec3 | float) -> Vec3:...

    def dot(self, other:Vec3) -> float:...

    def cross(self, other:Vec3) -> Vec3:...

class Object:
    def __init__(self, mesh_instance:Mesh, position:Vec3 = Vec3(0.0,0.0,0.0), rotation:Vec3 = Vec3(0.0,0.0,0.0), scale:Vec3 = Vec3(0.0,0.0,0.0)) -> None:...


    @property
    def position(self) -> V3Property:...

    @position.setter
    def position(self, other:Vec3 | V3Property):...

    @property
    def rotation(self) -> V3Property:...

    @rotation.setter
    def rotation(self, other:Vec3 | V3Property):...

    @property
    def scale(self) -> V3Property:...

    @scale.setter
    def scale(self, other:Vec3 | V3Property):...


    def render(self, camera:Camera):...

class Shader:
    def __init__(self, source:str, shader_type:ShaderType) -> None:...

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


    # OPERATORS

    def __add__(self, other:Vec3 | float) -> Vec3:...

    def __sub__(self, other:Vec3 | float) -> Vec3:...

    def __mul__(self, other:Vec3 | float) -> Vec3:...

    def __truediv__(self, other:Vec3 | float) -> Vec3:...

    def dot(self, other:Vec3) -> float:...

    def cross(self, other:Vec3)->Vec3:...

    def get_magnitude(self)->float:...

    def get_normalized(self)->Vec3:...

class event(Enum):
    """
    Events recieved by the window.
    """
    NOTHING: 'event'
    WINDOW_CLOSE: 'event'
    QUIT: 'event'
    KEY_UP: 'event'
    KEY_DOWN: 'event'
    KEY_RIGHT: 'event'
    KEY_LEFT: 'event'
    KEY_SPACE: 'event'

class Window:
    """
    An application window that can be used as a render medium.
    """

    def __init__(self, title:str, cam:Camera, width:int, height:int) -> None:...
    
    @property
    def current_event(self) -> event:
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