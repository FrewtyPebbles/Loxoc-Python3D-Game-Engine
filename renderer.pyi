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

    def __init__(self, title:str, cam:Camera, width:int, height:int, fullscreen:bool = False) -> None:...
    
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