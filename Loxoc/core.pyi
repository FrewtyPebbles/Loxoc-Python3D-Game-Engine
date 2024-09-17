from enum import Enum
from typing import Generator
import math

UniformValueType = int | float | Matrix2x2 | Matrix2x3 | Matrix2x4 | Matrix3x2 | Matrix3x3 | Matrix3x4 | Matrix4x2 | Matrix4x3 | Matrix4x4 | Vec2 | Vec3 | Vec4
"""
The types able to be sent to shaders via Uniforms.

Includes:

`int` , `float` , :class:`Matrix2x2` , :class:`Matrix2x3` , :class:`Matrix2x4` , :class:`Matrix3x2` , :class:`Matrix3x3` , :class:`Matrix3x4` , :class:`Matrix4x2` , :class:`Matrix4x3` , :class:`Matrix4x4` , :class:`Vec2` , :class:`Vec3` , :class:`Vec4`
"""

class ShaderType(Enum):
    """
    The shader type of a :class:`Shader` object.

    .. #pragma: ignore_inheritance
    """
    FRAGMENT: 'ShaderType'
    VERTEX: 'ShaderType'
    GEOMETRY: 'ShaderType'
    COMPUTE: 'ShaderType'

class Camera:
    """
    This class is the 3d perspective for a :class:`Window`.
    """
    
    def __init__(self, position:Vec3, rotation:Vec3, view_width:int, view_height:int, focal_length:float, fov:float) -> None:
        """
        This class is the 3d perspective for a :class:`Window`.
        """

    @property
    def view_width(self) -> int:
        """
        The view width of the camera.
        """

    @view_width.setter
    def view_width(self, value:int) -> None:
        """
        The view width of the camera.
        """

    @property
    def view_height(self) -> int:
        """
        The view height of the camera.
        """

    @view_height.setter
    def view_height(self, value:int) -> None:
        """
        The view height of the camera.
        """

    @property
    def focal_length(self) -> float:
        """
        The focal length of the camera.
        """

    @focal_length.setter
    def focal_length(self, value:float) -> None:
        """
        The focal length of the camera.
        """

    @property
    def fov(self) -> float:
        """
        The field of view of the camera.
        """

    @fov.setter
    def fov(self, value:float) -> None:
        """
        The field of view of the camera.
        """

    @property
    def deltatime(self) -> float:
        """
        The deltatime of the :class:`Window` the camera is attached to.
        """

    @property
    def dt(self) -> float:
        """
        The deltatime of the :class:`Window` the camera is attached to.
        """

    @property
    def time_ns(self) -> int:
        """
        The time in nanoseconds since the creation of the :class:`Window` the camera is attached to.
        """

    @property
    def time(self) -> int:
        """
        The time in seconds since the creation of the :class:`Window` the camera is attached to.
        """

    @property
    def position(self) -> Vec3:
        """
        The current position of the :class:`Camera` as a :class:`Vec3` .
        """

    @position.setter
    def position(self, value: Vec3) -> None:
        """
        Set the current position of the :class:`Camera` as a :class:`Vec3` .
        """

    @property
    def rotation(self) -> Quaternion:
        """
        The current rotation of the :class:`Camera` as a :class:`Quaternion` .
        """

    @rotation.setter
    def rotation(self, value: Vec3 | Quaternion) -> None:
        """
        Set the current rotation of the :class:`Camera` as a :class:`Quaternion` .
        """

class Mesh:
    """
    This class is used to create/load meshes.
    """

    @staticmethod
    def from_file(file_path: str, animated:bool = False) -> Model:
        """
        Returns the :class:`Model` instance created from the provided file.  If the 3D asset contains animations, set `animated` to `True` .
        """

    @property
    def name(self) -> str:
        """
        Name of the mesh.

        :rtype: str
        """

class MeshDict:
    """
    :class:`Loxoc.MeshDict` is a datastructure that acts like a statically typed dictionary storing each :class:`Mesh<Loxoc.Mesh>` by name.
    This is nessicary because 3D asset files can have more than one :class:`Mesh<Loxoc.Mesh>` in them.  If you have a 3D
    asset file with more than one :class:`Mesh<Loxoc.Mesh>` inside of it, you can extract them from their :class:`MeshDict<Loxoc.MeshDict>`
    to new individual :class:`MeshDict<Loxoc.MeshDict>` s and then to :class:`Model<Loxoc.Model>` s to be used in :class:`Object3D<Loxoc.Object3D>` s like so:

    .. code-block:: python

        my_assets: MeshDict = Model.from_file("./assets/models/model_name/model_name.gltf").mesh_dict
        # Import the 3D asset file.
        
        player_md = MeshDict("player_model_mesh", [my_assets["player_model"]])
        # Extract the Mesh into its own group/MeshDict

        player_object = Object3D(Model(player_md), Vec3(0.0, 0.0, 20.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0))
        # Now our model is ready to be used.

    We can extract the :class:`Mesh<Loxoc.Mesh>` s we need from the ``my_assets``  :class:`MeshDict<Loxoc.MeshDict>` by name.
    Hence we use ``my_assets["player_model"]``.  This is assuming your desired :class:`Mesh<Loxoc.Mesh>`
    is at the top level of your imported 3D file/asset's heirarchy.  if it is in a group inside
    the 3D file/asset you imported you could do something like:
    ``my_assets["group_name"]["player_model"]``
    """

    def __init__(self, name: str, meshes: list[Mesh|MeshDict]) -> None:
        """
        A fast :class:`Mesh` container that can be used like a statically typed dict.
        """

    @property
    def name(self):
        """
        The name associated with the :class:`MeshDict` .
        """

    @name.setter
    def name(self, value: str):
        """
        The name associated with the :class:`MeshDict` .
        """

    def insert(self, m: Mesh|MeshDict) -> None:
        """
        Insert a Mesh.  It will use the Mesh name as a key.
        """

    def get(self, name: str) -> Mesh|MeshDict:
        """
        Get a Mesh from the dict by name.
        """

    def remove(self, name: str) -> None:
        """
        Remove a Mesh from the dict by name.
        """

    def __iter__(self) -> Generator[tuple[str, Mesh|MeshDict], None, None]:
        """
        Itterates through the key value pairs of the dict.
        """

    def __getitem__(self, key: str) -> Mesh|MeshDict:
        """
        Get a Mesh from the dict by name.
        """

class Material:
    """
    This decides how to render an Object (:class:`Object3D` or :class:`Object2D`).

    You may supply your own :class:`Shader` s to customize how Objects are rendered.
    """
    def __init__(self, vertex:Shader|None = None, fragment:Shader|None = None, geometry:Shader|None = None, compute:Shader|None = None, animated:bool = False) -> None:...

    def set_uniform(self, name:str, value:UniformValueType) ->None:
        """
        Sets the value of a uniform for the shaders in the material.
        """

class Model:
    """
    Holds all model data for an imported 3D asset file.
    """

    def __init__(self, mesh_dict:MeshDict, animated:bool = False) -> None:
        ...

    @staticmethod
    def from_file( file_path:str, animated:bool = False) -> Model:
        """
        Returns the :class:`Model` instance created from the provided file.  If the 3D asset contains animations, set `animated` to `True` .
        """

    @property
    def use_default_material_properties(self) -> bool:
        """
        Uses the :class:`Material` properties defined by each :class:`Mesh` 's :class:`Material` . This only has an effect if the :class:`Object3D` of interest has an object-level :class:`Material` set (having an object-level :class:`Material` means the :class:`Object3D` 's `Object3D.material` attribute is set.).
        When there is an object-level :class:`Material` set, the default value for `use_default_material_properties` is `False`.
        """

    @use_default_material_properties.setter
    def use_default_material_properties(self, value:bool) -> None:
        """
        Uses the :class:`Material` properties defined by each :class:`Mesh` 's :class:`Material` . This only has an effect if the :class:`Object3D` of interest has an object-level :class:`Material` set (having an object-level :class:`Material` means the :class:`Object3D` 's `Object3D.material` attribute is set.).
        When there is an object-level :class:`Material` set, the default value for `use_default_material_properties` is `False`.
        """

    def play_animation(self, animation:str) -> None:
        """
        Plays the specified animation.
        """
    

    @property
    def mesh_dict(self) -> MeshDict:
        """
        The :class:`Model` 's :class:`MeshDict` .
        """

    @mesh_dict.setter
    def mesh_dict(self, value:MeshDict) -> None:
        """
        The :class:`Model` 's :class:`MeshDict` .
        """

    @property
    def animated(self) -> bool:
        """
        Whether or not the model has animations.  If it does set this to true.
        """

    @animated.setter
    def animated(self, value:bool) -> None:
        """
        Whether or not the model has animations.  If it does set this to true.
        """

class Object3D:
    """
    This class is your 3D game object.
    """
    def __init__(self, model_data:Model, position:Vec3 = Vec3(0.0,0.0,0.0), rotation:Vec3 = Vec3(0.0,0.0,0.0), scale:Vec3 = Vec3(1.0,1.0,1.0), collider:Collider | None = None, material:Material | None = None) -> None:...

    @property
    def use_default_material_properties(self) -> bool:
        """
        Uses the :class:`Material` properties defined by each :class:`Mesh` 's :class:`Material` . This only has an effect if the :class:`Object3D` of interest has an object-level :class:`Material` set (having an object-level :class:`Material` means the :class:`Object3D` 's `Object3D.material` attribute is set.).
        When there is an object-level :class:`Material` set, the default value for `use_default_material_properties` is `False`.
        """

    @use_default_material_properties.setter
    def use_default_material_properties(self, value:bool) -> None:
        """
        Uses the :class:`Material` properties defined by each :class:`Mesh` 's :class:`Material` . This only has an effect if the :class:`Object3D` of interest has an object-level :class:`Material` set (having an object-level :class:`Material` means the :class:`Object3D` 's `Object3D.material` attribute is set.).
        When there is an object-level :class:`Material` set, the default value for `use_default_material_properties` is `False`.
        """

    @property
    def material(self) -> Vec3:
        """
        The :class:`Material` used to specify how to render the :class:`Object3D`
        """

    @material.setter
    def material(self, value:Material) -> None:
        """
        The :class:`Material` used to specify how to render the :class:`Object3D`
        """

    @property
    def model(self) -> Model:
        """
        The :class:`Model` of the :class:`Object3D`
        """

    @model.setter
    def model(self, value:Model) -> None:
        """
        The :class:`Model` of the :class:`Object3D`
        """

    def add_collider(self, collider:Collider) -> None:
        """
        Adds a :class:`Collider` to the object.
        """

    def remove_collider(self, collider:Collider) -> None:
        """
        Removes a :class:`Collider` from the object.
        """

    def check_collision(self, intersection: Vec3 | Object3D) -> bool:
        """
        Checks for a collision between this :class:`Object3D` and another :class:`Object3D` or :class:`Vec3` .
        """

    def get_model_matrix(self) -> Matrix4x4:
        """
        Returns an instance of the model matrix as a :class:`Matrix4x4` .
        """

    def play_animation(self, animation_name: str) -> None:
        """
        Plays the specified animation by name of the model.
        """

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
        The rotation of the object as a Quaternion.  Can assign a :class:`Vec3` of EulerAngles to it.
        """

    @property
    def scale(self) -> Vec3:
        """
        The scale of the object as a :class:`Vec3`.
        """

    @scale.setter
    def scale(self, value:Vec3):
        """
        The scale of the object as a :class:`Vec3`.
        """

    def set_uniform(self, name:str, value: UniformValueType) ->None:
        """
        Sets the value of a uniform for the :class:`Shader` s in the object's :class:`Material`.
        """

class Shader:
    """
    Used to import shader files (glsl) that can be used in :class:`Material` s.
    """

    def __init__(self, source:str, shader_type:ShaderType) -> None:...

    @classmethod
    def from_file(cls, filepath:str, type:ShaderType) -> Shader:...

class Vec4:
    """
    A 4 float datastructure used to represent positional data, colors, or whatever you may need it for.
    Contains useful linear algebra operators and functions.
    """

    def __init__(self, x:float, y:float, z:float, w:float) -> None:
        ...

    def __copy__(self) -> Quaternion:
        """
        Copies the :class:`Vec4` .
        """

    def __repr__(self) -> str:
        """
        Returns a string representation of the :class:`Vec4` .
        """

    def __neg__(self) -> Vec4:
        """
        Negates the :class:`Vec4` .
        """

    @property
    def x(self) -> float:
        """
        The x component :class:`Vec4` .
        """

    @x.setter
    def x(self, value:float):
        """
        The x component :class:`Vec4` .
        """

    @property
    def y(self) -> float:
        """
        The y component :class:`Vec4` .
        """

    @y.setter
    def y(self, value:float):
        """
        The y component :class:`Vec4` .
        """

    @property
    def z(self) -> float:
        """
        The z component :class:`Vec4` .
        """

    @z.setter
    def z(self, value:float):
        """
        The z component :class:`Vec4` .
        """

    @property
    def w(self) -> float:
        """
        The w component :class:`Vec4` .
        """

    @w.setter
    def w(self, value:float):
        """
        The w component :class:`Vec4` .
        """


    # OPERATORS

    def __add__(self, other:Vec4 | float) -> Vec4:
        """
        Adds the :class:`Vec4` .
        """

    def __sub__(self, other:Vec4 | float) -> Vec4:
        """
        Subtracts the :class:`Vec4` .
        """

    def __mul__(self, other:Vec4 | float | Quaternion) -> Vec4:
        """
        Multiplies the :class:`Vec4` .
        """

    def __truediv__(self, other:Vec4 | float) -> Vec4:
        """
        Divides the two :class:`Vec4` s.
        """

    def dot(self, other:Vec4) -> float:
        """
        Returns the dot product of the two :class:`Vec4` s.
        """

    def get_magnitude(self) -> float:
        """
        Returns the magnitude of the :class:`Vec4` .
        """

    def  get_normalized(self) -> Vec4:
        """
        Returns the normalized :class:`Vec4` .
        """


    def to_vec3(self) -> Vec3:
        """
        Converts the :class:`Vec4` to a :class:`Vec3` using its x, y and z components.
        """

    def to_vec2(self) -> Vec2:
        """
        Converts the :class:`Vec4` to a :class:`Vec2` using its x and y components.
        """

    def outer_product(self, vec: Vec2|Vec3|Vec4) -> Matrix2x4|Matrix3x4|Matrix4x4:
        """
        Calculates the outer product.
        """

    def distance(self, other:Vec4) -> float:
        """
        The distance between two vectors as a `float` .
        """

class Vec3:
    """
    A 3 float datastructure used to represent positional data, colors, or whatever you may need it for.
    Contains useful linear algebra operators and functions.
    """

    def __init__(self, x:float, y:float, z:float) -> None:
        """
        A size 3 vector used for 3D positioning, color, (sometimes)rotation, and more.
        """

    def __repr__(self) -> str:
        """
        Vec3 str representation.
        """

    @property
    def quaternion(self) -> Quaternion:
        """
        Get the :class:`Quaternion` form of the vector.  Can also be assigned to, but not mutated.
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
        The up directional vector from the Euler representation of the :class:`Vec3` .
        """

    @property
    def right(self) -> Vec3:
        """
        The right directional vector from the Euler representation of the :class:`Vec3` .
        """

    @property
    def forward(self) -> Vec3:
        """
        The forward directional vector from the Euler representation of the :class:`Vec3` .
        """


    # OPERATORS

    def __neg__(self) -> Vec3:
        """
        Negate a vector.
        """

    def __add__(self, other:Vec3 | float) -> Vec3:
        """
        Add 2 vectors.
        """

    def __sub__(self, other:Vec3 | float) -> Vec3:
        """
        Subtract 2 vectors.
        """

    def __mul__(self, other:Vec3 | float | Quaternion) -> Vec3:
        """
        Multiply 2 vectors.
        """

    def __truediv__(self, other:Vec3 | float) -> Vec3:
        """
        divide 2 vectors.
        """

    def dot(self, other:Vec3) -> float:
        """
        Performs a dot product operation between two :class:`Vec3` s.
        """

    def cross(self, other:Quaternion | Vec3)->Vec3:
        """
        Performs a cross product operation between two :class:`Vec3` s.
        """

    def get_magnitude(self)->float:
        """
        Returns the magnitude of the vector.
        """

    def get_normalized(self)->Vec3:
        """
        Returns the normalized vector.
        """

    def to_quaternion(self) -> Quaternion:
        """
        Constructs a :class:`Quaternion` from the given Euler Angle :class:`Vec3` (in radians).
        """

    def outer_product(self, vec: Vec2|Vec3|Vec4) -> Matrix2x3|Matrix3x3|Matrix4x3:
        """
        Calculates the outer product.
        """

    def distance(self, other:Vec3) -> float:
        """
        The distance between two vectors as a `float` .
        """


class Vec2:
    """
    A 2 float datastructure used to represent positional data, 2D rotation, or whatever you may need it for.
    Contains useful linear algebra operators and functions.
    """
    def __init__(self, x:float, y:float) -> None:
        """
        A size 2 vector used for 2D positioning and (sometimes)rotation.
        """

    def __repr__(self) -> str:
        """
        :class:`Vec2` str representation.
        """

    def __neg__(self) -> Vec2:...

    def __copy__(self) -> Vec2:...

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
        Calculate the dot product of 2 :class:`Vec2` s.
        """

    def get_magnitude(self) -> float:
        """
        Calculate the :class:`Vec2` 's magnitude.
        """

    def get_normalized(self) -> Vec2:
        """
        Calculate the normalized :class:`Vec2` of the :class:`Vec2` .
        """

    @classmethod
    def from_angle(cls, angle:float) -> Vec2:
        """
        Construct a normalized :class:`Vec2` given an angle.
        """

    def outer_product(self, vec: Vec2|Vec3|Vec4) -> Matrix2x2|Matrix3x2|Matrix4x2:
        """
        Calculates the outer product.
        """

    def distance(self, other:Vec2) -> float:
        """
        The distance between two vectors as a `float` .
        """


class Window:
    """
    An application window for a game.  This class also keeps track of the state of the game engine runtime (ie. what to and not to render along with events and time).
    """

    ambient_light:Vec3
    """
    The ambient or "base" level of light before any lights are added
    """

    def __init__(self, title:str, cam:Camera, width:int, height:int, fullscreen:bool = False, ambient_light:Vec3 = Vec3(1.0, 1.0, 1.0)) -> None:
        """
        Constructs an application window for a game.
        """

    @property
    def fullscreen(self) -> bool:
        """
        Wether to render the window in fullscreen mode (`True`) or not (`False`).
        """

    @fullscreen.setter
    def fullscreen(self, value:bool) -> None:
        """
        Wether to render the window in fullscreen mode (`True`) or not (`False`).
        """

    @property
    def resizeable(self) -> bool:
        """
        This flag determines wether or not the window is resizeable.  The default value is `True`.
        """

    @resizeable.setter
    def resizeable(self, value: bool):
        """
        This flag determines wether or not the window is resizeable.  The default value is `True`.
        """

    @property
    def sky_box(self) -> SkyBox:
        """
        The :class:`Skybox` .
        """

    @sky_box.setter
    def sky_box(self, value:SkyBox):
        """
        The :class:`Skybox` .
        """
    
    @property
    def event(self) -> Event:
        """
        This is the most recent event that the window recieved durring :meth:`Window.update` .
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
        Re-renders and refreshes the :attr:`Window.event` on the application :class:`Window` .
        Should be used in a render/gameloop like so:

            .. code-block:: python

                window = Window(...)

                while ...:
                    
                    # render/gameloop code...

                    window.update()

        Whenever :meth:`Window.update` is called, a new frame is drawn to the :class:`Window` .
        If the program ends, the window will be closed.
        """

    def lock_mouse(self, lock:bool) -> None:
        """
        Locks the mouse in the center of the window.
        """

    # OBJECT

    def add_object(self, obj: Object3D) -> None:
        """
        Adds the :class:`Object3D` to the scene.  This ensures that the :class:`Object3D` is rendered by the camera.
        """

    def remove_object(self, obj: Object3D) -> None:
        """
        Removes the :class:`Object3D`  from the scene.  Only :class:`Object3D` s which are in the scene will be rendered by the camera.
        """

    def add_object_list(self, objs: list[Object3D]) -> None:
        """
        Adds multiple :class:`Object3D` s to the scene.  This ensures that they are rendered by the camera.
        """

    def remove_object_list(self, objs: list[Object3D]) -> None:
        """
        Removes multiple :class:`Object3D` s from the scene.  Only :class:`Object3D` s which are in the scene will be rendered by the camera.
        """

    # OBJECT2D

    def add_object2d(self, obj: Object2D) -> None:
        """
        Adds the :class:`Object2D` to the scene.  This ensures that the :class:`Object2D` is rendered by the camera.
        """

    def remove_object2d(self, obj: Object2D) -> None:
        """
        Removes the :class:`Object2D` from the scene.  Only :class:`Object2D` s which are in the scene will be rendered by the camera.
        """

    def add_object2d_list(self, objs: list[Object2D]) -> None:
        """
        Adds multiple :class:`Object2D` s to the scene.  This ensures that they are rendered by the camera.
        """

    def remove_object2d_list(self, objs: list[Object2D]) -> None:
        """
        Removes multiple :class:`Object2D` s from the scene.  Only :class:`Object2D` s which are in the scene will be rendered by the camera.
        """

    # POINT LIGHT

    def add_point_light(self, obj:PointLight) -> None:
        """
        Adds the :class:`PointLight` to the scene.  This ensures that the :class:`PointLight` is rendered by the camera.
        """

    def remove_point_light(self, obj:PointLight) -> None:
        """
        Removes the :class:`PointLight` from the scene.  Only :class:`PointLight` s which are in the scene will be rendered by the camera.
        """

    def add_point_light_list(self, objs:list[PointLight]) -> None:
        """
        Adds multiple :class:`PointLight` s to the scene.  This ensures that they are rendered by the camera.
        """

    def remove_point_light_list(self, objs:list[PointLight]) -> None:
        """
        Removes multiple :class:`PointLight` s from the scene.  Only :class:`PointLight` s which are in the scene will be rendered by the camera.
        """

    # DIRECTIONAL LIGHT

    def add_directional_light(self, obj:DirectionalLight) -> None:
        """
        Adds the :class:`DirectionalLight` to the scene.  This ensures that the :class:`DirectionalLight` is rendered by the camera.
        """

    def remove_directional_light(self, obj:DirectionalLight) -> None:
        """
        Removes the :class:`DirectionalLight` from the scene.  Only :class:`DirectionalLight` s which are in the scene will be rendered by the camera.
        """

    def add_directional_light_list(self, objs:list[DirectionalLight]) -> None:
        """
        Adds multiple :class:`DirectionalLight` s to the scene.  This ensures that they are rendered by the camera.
        """

    def remove_directional_light_list(self, objs:list[DirectionalLight]) -> None:
        """
        Removes multiple :class:`DirectionalLight` s from the scene.  Only :class:`DirectionalLight` s which are in the scene will be rendered by the camera.
        """

    # SPOT LIGHT

    def add_spot_light(self, obj:SpotLight) -> None:
        """
        Adds the :class:`SpotLight` to the scene.  This ensures that the :class:`SpotLight` is rendered by the camera.
        """

    def remove_spot_light(self, obj:SpotLight) -> None:
        """
        Removes the :class:`SpotLight` from the scene.  Only :class:`SpotLight` s which are in the scene will be rendered by the camera.
        """

    def add_spot_light_list(self, objs:list[SpotLight]) -> None:
        """
        Adds multiple :class:`SpotLight` s to the scene.  This ensures that they are rendered by the camera.
        """

    def remove_spot_light_list(self, objs:list[SpotLight]) -> None:
        """
        Removes multiple :class:`SpotLight` s from the scene.  Only :class:`SpotLight` s which are in the scene will be rendered by the camera.
        """

    # TEXT

    def add_text(self, obj:Text) -> None:
        """
        Adds the :class:`Text` to the scene.  This ensures that the :class:`Text` is rendered by the camera.
        """

    def remove_text(self, obj:Text) -> None:
        """
        Removes the :class:`Text` from the scene.  Only :class:`Text` s which are in the scene will be rendered by the camera.
        """

    def add_text_list(self, objs:list[Text]) -> None:
        """
        Adds multiple :class:`Text` s to the scene.  This ensures that they are rendered by the camera.
        """

    def remove_text_list(self, objs:list[Text]) -> None:
        """
        Removes multiple :class:`Text` s from the scene.  Only :class:`Text` s which are in the scene will be rendered by the camera.
        """

    # EMITTER

    def add_emitter(self, obj:Emitter) -> None:
        """
        Adds the :class:`Emitter` to the scene.  This ensures that the :class:`Emitter` is rendered by the camera.
        """

    def remove_emitter(self, obj:Emitter) -> None:
        """
        Removes the :class:`Emitter` from the scene.  Only :class:`Emitter` s which are in the scene will be rendered by the camera.
        """

    def add_emitter_list(self, objs:list[Emitter]) -> None:
        """
        Adds multiple :class:`Emitter` s to the scene.  This ensures that they are rendered by the camera.
        """

    def remove_emitter_list(self, objs:list[Emitter]) -> None:
        """
        Removes multiple :class:`Emitter` s from the scene.  Only :class:`Emitter` s which are in the scene will be rendered by the camera.
        """

class EVENT_FLAG(Enum):
    """
    An IO or engine event flag.

    .. #pragma: ignore_inheritance
    """
    # WINDOW MANAGEMENT
    WINDOW_MINIMIZE: 'EVENT_FLAG'
    """
    Flagged when the window minimize button is clicked.
    """
    WINDOW_CLOSE: 'EVENT_FLAG'
    """
    Flagged when the window close button is clicked.
    """
    QUIT: 'EVENT_FLAG'
    """
    Flagged when the window close button is clicked.
    """

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

    # MORE WINDOW EVENTS
    WINDOW_RESIZE: 'EVENT_FLAG'

class EVENT_STATE(Enum):
    """
    The state of an IO or engine event flag.

    .. #pragma: ignore_inheritance
    """

    NONE: 'EVENT_STATE'
    PRESSED: 'EVENT_STATE'
    RELEASED: 'EVENT_STATE'

class MOUSE_EVENT_TYPE(Enum):
    """
    The type of a mouse event.

    .. #pragma: ignore_inheritance
    """
    BUTTON_DOWN: 'MOUSE_EVENT_TYPE'
    BUTTON_UP: 'MOUSE_EVENT_TYPE'
    NONE: 'MOUSE_EVENT_TYPE'

class MOUSE_BUTTON(Enum):
    """
    The button of a mouse event.

    .. #pragma: ignore_inheritance
    """
    LEFT: 'MOUSE_BUTTON'
    RIGHT: 'MOUSE_BUTTON'
    MIDDLE: 'MOUSE_BUTTON'

class MOUSE_WHEEL_DIRECTION(Enum):
    """
    The wheel direction of a mouse wheel event.

    .. #pragma: ignore_inheritance
    """
    FLIPPED: 'MOUSE_WHEEL_DIRECTION'
    NORMAL: 'MOUSE_WHEEL_DIRECTION'

class MouseWheel:
    """
    The mousewheel data for a mouse wheel event.
    """
    int_x:int
    "The x value of the mouse wheel with integer precision."
    int_y:int
    "The y value of the mouse wheel with integer precision."
    x:float
    "The x value of the mouse wheel with full floating point precision."
    y:float
    "The y value of the mouse wheel with full floating point precision."
    direction:MOUSE_WHEEL_DIRECTION
    "The direction of the mouse wheel."

class MouseDevice:
    """
    The data for a mouse device populated durring a mouse event.
    """
    id:int
    """
    The id of the mouse device.
    """
    timestamp:int
    """
    The timestamp of the mouse event.
    """
    x:int
    """
    The x position of the mouse in relation to the window.
    """
    y:int
    """
    The y position of the mouse in relation to the window.
    """
    rel_x:int
    """
    The x position of the mouse relative to its last position.  Also can be thought of as the mouse x delta.
    """
    rel_y:int
    """
    The y position of the mouse relative to its last position.  Also can be thought of as the mouse y delta.
    """
    clicks:int
    """
    The ammount of clicks recorded.
    """
    type:MOUSE_EVENT_TYPE
    """
    The type of mouse event.
    """
    state:EVENT_STATE
    """
    The state of the mouse event.
    """
    button:MOUSE_BUTTON
    """
    The button pressed durring the mouse event.
    """
    wheel:MouseWheel
    """
    The mouse wheel data for mouse wheel events.
    """

class Event:
    """
    This non-constructable class contains all information pertaining to :class:`Window` events.  
    This class can be accessed/read via :class:`Window` :class:`.event<Event>` .

    To check if an :class:`EVENT_FLAG` has been triggered you can use :class:`Window` :class:`.event.check_flag(...)<Event.check_flag>`:

        .. code-block:: python

            window = Window(...)

            while not window.event.check_flag(EVENT_FLAG.QUIT):
                
                # some game loop code...

                window.update()
        
    This can be useful for events where you only need to chjeck if they have been triggered.  Such as quitting the game when you close the window.

    To check the :class:`EVENT_STATE` of an :class:`EVENT_FLAG` you can use :class:`Window` :class:`.event.get_flag(...)<Event.get_flag>`:

        .. code-block:: python

            window = Window(...)

            while not window.event.check_flag(EVENT_FLAG.QUIT):
                
                if window.event.get_flag(EVENT_FLAG.KEY_UP) == EVENT_STATE.PRESSED:
                    # do some stuff when up is pressed...

                window.update()

    This can be useful for getting the :class:`EVENT_STATE` of specific :class:`EVENT_FLAG` s.  
    It is recommended to do this before attempting to access data from the :class:`Event` that is related to the :class:`EVENT_FLAG` and its :class:`EVENT_STATE` .

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
    """
    A *"4 dimensional"* rotation arround a developer defined :class:`Vec3` axis.

    In simple terms, a Quaternion is just an angle (in radians) rotation arround some 3D vector.  
    For example if we wanted to rotate arround the Euler y axis of an :class:`Object3D` 
    or along "yaw" we would just rotate arroud the :class:`Object3D` :class:`.rotation.up<Quaternion.up>` vector:

        .. code-block:: python

            obj = Object3D(...)

            yaw_rotation = math.radians(90)

            quat = Quaternion.from_axis_angle(obj.rotation.up, yaw_rotation)

    It is recommended to construct Quaternions with :meth:`Quaternion.from_axis_angle`.

    Read more about Quaternions `here`_.

    .. _here: https://en.wikipedia.org/wiki/Quaternion
    """

    def __init__(self, w:float, x:float, y:float, z:float) -> None:
        """
        For all of your 4-dimensional rotation needs.
        """

    def __repr__(self) -> str:
        """
        The string representation of the Quaternion.
        """

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
        The Euler Angle Vec3 form of the Quaternion.  Can also be assigned to, but not mutated.
        
        Use :meth:`Quaternion.rotate_pitch`, :meth:`Quaternion.rotate_yaw`, and :meth:`Quaternion.rotate_roll` for mutating Euler angle rotation.
        """

    @euler_angles.setter
    def euler_angles(self, value:Vec3 | Quaternion):
        """        
        Set the rotation to a Euler angle Vec3 in radians.  This is also not mutable.
        """
    
    @property
    def euler_pitch(self) -> float:
        """
        Rotation about the x axis.
        """

    def rotate_pitch(self, value:float) -> Quaternion:
        """
        Rotates the Euler pitch (x) axis by the provided value.
        """

    @property
    def euler_yaw(self) -> float:
        """
        Rotation about the y axis.
        """

    def rotate_yaw(self, value:float) -> Quaternion:
        """
        Rotates the Euler yaw (y) axis by the provided value.
        """

    @property
    def euler_roll(self) -> float:
        """
        Rotation about the z axis.
        """

    def rotate_roll(self, value:float) -> Quaternion:
        """
        Rotates the Euler roll (z) axis by the provided value.
        """

    # OPERATORS

    def __add__(self, other:Quaternion | float) -> Quaternion:...

    def __sub__(self, other:Quaternion | float) -> Quaternion:...

    def __mul__(self, other:Quaternion | float | Vec3) -> Quaternion | Vec3:...

    def __truediv__(self, other:Quaternion | float) -> Quaternion:...

    def dot(self, other:Quaternion) -> float:
        """
        The dot product of 2 :class:`Quaternion` s.
        """

    def cross(self, other:Quaternion | Vec3) -> float:
        """
        The cross product of the :class:`Quaternion` and another compatible structure.
        """

    def get_magnitude(self) -> float:
        """
        Returns the magnitude of the :class:`Quaternion` .
        """

    def get_normalized(self) -> Quaternion:
        """
        Returns the normalized :class:`Quaternion` .
        """

    def to_euler(self) -> Vec3:
        """
        Converts the :class:`Quaternion` to a Euler Angle :class:`Vec3` (in radians).
        """

    def get_conjugate(self) -> Quaternion:
        """
        The conjugate of the :class:`Quaternion` .
        """

    def get_inverse(self) -> Quaternion:
        """
        The inverse of the :class:`Quaternion` .
        """
    
    def get_reverse(self) -> Quaternion:
        """
        The 180 degree reverse of the :class:`Quaternion` .
        """

    @staticmethod
    def from_euler(euler_vec: Vec3) -> Quaternion:
        """
        Converts the provided Euler Angle :class:`Vec3` (in radians) to a :class:`Quaternion` .
        """

    @staticmethod
    def from_axis_angle(axis:Vec3, angle:float) -> Quaternion:
        """
        Constructs a :class:`Quaternion` with a rotation of ``angle`` arround ``axis``. 

        :arg Vec3 axis: A Euler Angle :class:`Vec3` (in radians).

        :arg float angle: An angle in radians of which to rotate arround the ``axis``.
        """

    def rotate(self, axis:Vec3, angle:float) -> None:
        """
        Mutably rotates the :class:`Quaternion` by `angle` arround `axis`.

        :arg Vec3 axis: A Euler Angle :class:`Vec3` (in radians).

        :arg float angle: An angle in radians of which to rotate arround the ``axis``.
        """

    @staticmethod
    def from_quat(quat:Quaternion) -> Quaternion:
        """
        Used to construct a copy of a :class:`Quaternion` .
        """

    @staticmethod
    def from_unit(axis:Vec3, up:Vec3 | None = Vec3(0.0, 1.0, 0.0)) -> Quaternion:
        """
        Takes in a unit :class:`Vec3` direction and returns a :class:`Quaternion` rotated in that direction from the global forward `Vec3(0.0, 0.0, 1.0)` .
        """

    def lerp(self, other: Quaternion, ratio: float) -> Quaternion:
        """
        Returns a lerped :class:`Quaternion` between two :class:`Quaternion` by the provided ratio.
        """

    def slerp(self, other: Quaternion, ratio: float) -> Quaternion:
        """
        Returns a slerped :class:`Quaternion` between two :class:`Quaternion` by the provided ratio.
        """

class TextureFiltering(Enum):
    """
    The texture filtering setting for a :class:`Texture` .

    .. #pragma: ignore_inheritance
    """
    NEAREST: 'TextureFiltering'
    LINEAR: 'TextureFiltering'

class TextureWraping(Enum):
    """
    The texture wrapping setting for a :class:`Texture` .

    .. #pragma: ignore_inheritance
    """
    REPEAT: 'TextureWraping'
    MIRRORED_REPEAT: 'TextureWraping'
    CLAMP_TO_EDGE: 'TextureWraping'
    CLAMP_TO_BORDER: 'TextureWraping'

class Texture:
    """
    A texture for a :class:`Mesh` or :class:`Sprite` .
    """
    @classmethod
    def from_file(cls, file_path:str, wrap:TextureWraping = TextureWraping.REPEAT, filtering:TextureFiltering = TextureFiltering.LINEAR) -> Texture:
        """
        Create a :class:`Texture` from the specified file.
        """

class Sprite:
    """
    The image asset which is used when rendering an :class:`Object2D` .  Serves a purpose similar to how :class:`Mesh` is used with :class:`Object3D` but for :class:`Object2D` .
    """
    
    texture:Texture
    """
    The :class:`Texture` that is drawn when the :class:`Sprite` is rendered.
    """

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
    """
    An 2 Dimensional Object which is rendered infront of all :class:`Object3D` s on the screen.  Good for GUI, HUD interfaces, or anything else 2D.
    """
    def __init__(self, sprite: Sprite, camera:Camera, position:Vec2 = Vec2(0.0,0.0), depth:float = 0.0,
    rotation:float = 0.0, scale:Vec2 = Vec2(1.0, 1.0),
    material:Material = None) -> None:
        """
        :class:`Object2D` s are rendered infront of all objects rendered in 3D space (:class:`Object3D`).
        This class is good for 2D games, GUIs, game heads up displays or anything you might need to render something in 2D space for.
        """

    @property
    def position(self) -> Vec2:
        """
        The position of the :class:`Object2D` on the screen as a :class:`Vec2`.
        """

    @position.setter
    def position(self, value: Vec2) -> None:...

    @property
    def depth(self) -> float:
        """
        The depth layer of the :class:`Sprite` when rendered, or "z" position.  :class:`Object2D` s with a higher depth are rendered underneath :class:`Object2D` s with a lower depth.
        """

    @depth.setter
    def depth(self, value: float):
        """
        The depth layer of the :class:`Sprite` when rendered, or "z" position.  :class:`Object2D` s with a higher depth are rendered underneath :class:`Object2D` s with a lower depth.
        """

    @property
    def rotation(self) -> float:
        """
        The rotation of the :class:`Object2D` on the screen as a :class:`Vec2`.
        """

    @rotation.setter
    def rotation(self, value: Vec2 | float) -> None:...

    @property
    def scale(self) -> Vec2:
        """
        The scale of the :class:`Object2D` on the screen as a :class:`Vec2`.
        """

    @scale.setter
    def scale(self, value:Vec2) -> None:...

    @property
    def untransformed_dimensions(self) -> Vec2:
        """
        The screen/camera coordinate dimensions of the :class:`Sprite` before scale is applied.
        """

    @property
    def dimensions(self) -> Vec2:
        """
        The true screen/camera coordinate dimensions of the :class:`Sprite` .
        """

    @property
    def width(self) -> float:
        """
        The true screen/camera coordinate height of the :class:`Sprite` .
        """

    @property
    def height(self) -> float:
        """
        The true screen/camera coordinate height of the :class:`Sprite` .
        """

    def set_uniform(self, name:str, value:UniformValueType) ->None:
        """
        Sets the value of a uniform for the shaders in the :class:`Object2D` 's :class:`Material`.
        """

class PointLight:
    """
    A Object that emits light from a :class:`Vec3` point in 3D space.
    """
    def __init__(self, position:Vec3, radius:float, color:Vec3, intensity:float) -> None:
        """
        A Light that radiates out from its position by `radius`
        """

    @property
    def position(self) -> Vec3:
        """
        The :class:`Vec3` position of the light.
        """

    @position.setter
    def position(self, value:Vec3) -> None:
        """
        The :class:`Vec3` position of the light.
        """

    @property
    def color(self) -> Vec3:
        """
        The :class:`Vec3` color of the light in rgb.
        """

    @color.setter
    def color(self, value:Vec3) -> None:
        """
        The :class:`Vec3` color of the light in rgb.
        """

    @property
    def intensity(self) -> float:
        """
        The intensity of the light.
        """

    @intensity.setter
    def intensity(self, value:float):
        """
        The intensity of the light.
        """

    @property
    def radius(self) -> float:
        """
        The radius of the light
        """

    @radius.setter
    def radius(self, value:float):
        """
        The radius of the light
        """


class DirectionalLight:
    def __init__(self, rotation:Vec3 = None, color:Vec3 = None, ambient:Vec3 = None,
    diffuse:Vec3 = None, specular:Vec3 = None, intensity:float = 1.0) -> None:
        """
        A light that shines on everything globally from a specified angle.
        """
    
    @property
    def rotation(self) -> Quaternion:
        """
        The :class:`Quaternion` rotation of the :class:`DirectionalLight`
        """

    @rotation.setter
    def rotation(self, value:Quaternion):
        """
        The :class:`Quaternion` rotation of the :class:`DirectionalLight`
        """

    @property
    def ambient(self) -> Vec3:
        """
        The :class:`Vec3` ambient property of the :class:`DirectionalLight`
        """

    @ambient.setter
    def ambient(self, value:Vec3):
        """
        The :class:`Vec3` ambient property of the :class:`DirectionalLight`
        """

    @property
    def diffuse(self) -> Vec3:
        """
        The :class:`Vec3` diffuse property of the :class:`DirectionalLight`
        """

    @diffuse.setter
    def diffuse(self, value:Vec3):
        """
        The :class:`Vec3` diffuse property of the :class:`DirectionalLight`
        """

    @property
    def specular(self) -> Vec3:
        """
        The :class:`Vec3` specular property of the :class:`DirectionalLight`
        """

    @specular.setter
    def specular(self, value:Vec3):
        """
        The :class:`Vec3` specular property of the :class:`DirectionalLight`
        """

    @property
    def intensity(self) -> float:
        """
        The intensity property of the :class:`DirectionalLight`
        """

    @intensity.setter
    def intensity(self, value:float):
        """
        The intensity property of the :class:`DirectionalLight`
        """

    @property
    def color(self) -> Vec3:
        """
        The :class:`Vec3` color property of the :class:`DirectionalLight`
        """

    @color.setter
    def color(self, value:Vec3):
        """
        The :class:`Vec3` color property of the :class:`DirectionalLight`
        """


class SpotLight:
    def __init__(self, position:Vec3, rotation:Vec3 = None, color:Vec3 = None, cutoff:float = 12.5, outer_cutoff:float = 17.5, intensity:float = 1.0, reach:float = 100.0, cookie:Texture = None) -> None:
        """
        A spot-light object.
        """

    @property
    def position(self) -> Vec3:
        """
        The :class:`Vec3` position of the light.
        """

    @position.setter
    def position(self, value:Vec3) -> None:
        """
        The :class:`Vec3` position of the light.
        """

    @property
    def rotation(self) -> Quaternion:
        """
        The direction of the spotlight as a :class:`Quaternion` .
        """

    @rotation.setter
    def rotation(self, value:Quaternion):
        """
        The direction of the spotlight as a :class:`Quaternion` .
        """

    @property
    def cookie(self) -> Texture:
        """
        The :class:`Texture` of the light cookie the spotlight will cast if provided.
        """

    @cookie.setter
    def cookie(self, value: Texture):
        """
        The :class:`Texture` of the light cookie the spotlight will cast if provided.
        """

    @property
    def color(self) -> Vec3:
        """
        The :class:`Vec3` color of the light.
        """

    @color.setter
    def color(self, value: Vec3):
        """
        The :class:`Vec3` color of the light.
        """

    @property
    def intensity(self) -> float:
        """
        The intensity of the light.
        """

    @intensity.setter
    def intensity(self, value: float):
        """
        The intensity of the light.
        """

    @property
    def cutoff(self) -> float:
        """
        the cutoff of the light cookie as an angle from the center.
        """

    @cutoff.setter
    def cutoff(self, value: float):
        """
        the cutoff of the light cookie as an angle from the center.
        """

    @property
    def outer_cutoff(self) -> float:
        """
        the cutoff of the light cookie smoothing as an angle from the center.
        """

    @outer_cutoff.setter
    def outer_cutoff(self, value: float):
        """
        the cutoff of the light cookie smoothing as an angle from the center.
        """

    @property
    def reach(self) -> float:
        """
        The distance the light is able to travel.
        """

    @reach.setter
    def reach(self, value:float):
        """
        The distance the light is able to travel.
        """

class Collider:
    """
    The Collider base class.
    """
    def check_collision(self, intersection: Vec3 | Collider | Object3D) -> bool:
        """
        Checks for a collision between this :class:`Collider`  and another :class:`Collider` , :class:`Object3D` or :class:`Vec3` .
        """

    @property
    def offset(self) -> Vec3:
        """
        The :class:`Vec3` offset of the collider.
        """

    @offset.setter
    def offset(self, value: Vec3) -> None:
        """
        The :class:`Vec3` offset of the collider.
        """

    @property
    def rotation(self) -> Quaternion:
        """
        The :class:`Quaternion` rotation of the collider.
        """

    @rotation.setter
    def rotation(self, value: Quaternion) -> None:
        """
        The :class:`Quaternion` rotation of the collider.
        """

    @property
    def scale(self) -> Vec3:
        """
        The :class:`Vec3` scale of the collider.
        """

    @scale.setter
    def scale(self, value: Vec3) -> None:
        """
        The :class:`Vec3` scale of the collider.
        """

    @property
    def show(self):
        """
        Makes the collider visible.  Good for debugging purposes.
        """

    @show.setter
    def show(self, value: bool):
        """
        Makes the collider visible.  Good for debugging purposes.
        """
    
    def dbg_render(self, cam: Camera) -> None:
        """
        Call this function when you wish to show a collider's collision mesh despite the collider not having an owner.  (This api is temporary until future game engine systems are implemented.)
        """

class BoxCollider(Collider):
    """
    An Oriented Box Collider.
    """
    def __init__(self, object: Object3D, offset: Vec3 = Vec3(0,0,0), rotation: Vec3 | Quaternion = Vec3(0,0,0), scale: Vec3 = Vec3(1.0,1.0,1.0)) -> None:
        ...

    @classmethod
    def from_bounds(cls, upper_bound: Vec3 = Vec3(10, 10, 10), lower_bound: Vec3 = Vec3(-10, -10, -10), offset: Vec3 = Vec3(0,0,0), rotation: Vec3 | Quaternion = Vec3(0,0,0), scale: Vec3 = Vec3(1.0,1.0,1.0)) -> BoxCollider:
        """
        Constructs an :class:`BoxCollider` from the provided bounds.
        """

class ConvexCollider(Collider):
    """
    A convex hull collider.
    """

    def __init__(self, object:Object3D, offset: Vec3 = Vec3(0,0,0), rotation: Vec3 | Quaternion = Vec3(0,0,0), scale: Vec3 = Vec3(1.0,1.0,1.0)) -> None:
        ...

    @classmethod
    def from_mesh(cls, msh:Mesh, offset: Vec3 = Vec3(0,0,0), rotation: Vec3 | Quaternion = Vec3(0,0,0), scale: Vec3 = Vec3(1.0,1.0,1.0)) -> BoxCollider:
        """
        Creates a :class:`ConvexCollider` from the provided :class:`Mesh` .
        """

    @classmethod
    def from_mesh_dict(cls, msh_dict:MeshDict, offset: Vec3 = Vec3(0,0,0), rotation: Vec3 | Quaternion = Vec3(0,0,0), scale: Vec3 = Vec3(1.0,1.0,1.0)) -> BoxCollider:
        """
        Creates a :class:`ConvexCollider` from the provided :class:`MeshDict` .
        """

class RayCollider(Collider):
    """
    A raycast collider that takes in an :class:`Vec3` origin and :class:`Quaternion` direction.
    """

    def __init__(self, origin:Vec3, direction:Quaternion) -> None:
        ...

    def get_collision(self, intersection: Collider | Object3D) -> RayHit:
        """
        Checks for a collision on the `RayCollider` .  Upon colliding this function returns an instance of :class:`RayHit` .
        """

    @property
    def rotation(self) -> Quaternion:
        """
        The :class:`Quaternion` direction of the `RayCollider` .
        """

    @rotation.setter
    def rotation(self, value:Quaternion) -> None:
        """
        The :class:`Quaternion` direction of the `RayCollider` .
        """

    @property
    def direction(self) -> Quaternion:
        """
        The :class:`Quaternion` direction of the `RayCollider` .
        """

    @direction.setter
    def direction(self, value:Quaternion) -> None:
        """
        The :class:`Quaternion` direction of the `RayCollider` .
        """

    @property
    def offset(self) -> Vec3:
        """
        The :class:`Vec3` origin of the `RayCollider` .
        """

    @offset.setter
    def offset(self, value:Vec3) -> None:
        """
        The :class:`Vec3` origin of the `RayCollider` .
        """

    @property
    def origin(self) -> Vec3:
        """
        The :class:`Vec3` origin of the `RayCollider` .
        """

    @origin.setter
    def origin(self, value:Vec3) -> None:
        """
        The :class:`Vec3` origin of the `RayCollider` .
        """

class RayHit:
    """
    Returned by :attr:`RayCollider.get_collision` .  This class contains information about the :class:`RayCollider` collision.
    """

    @property
    def hit(self) -> bool:
        """
        `True` if the :class:`RayCollider` has hit its target, `False` if not.
        """
    
    @property
    def has_normal(self) -> bool:
        """
        Wether the `RayHit` 's collided surface has a normal.
        """

    @property
    def has_distance(self) -> bool:
        """
        Wether the `RayHit` has recorded the distance between the collision point and its origin.
        """

    @property
    def position(self) -> Vec3:
        """
        The :class:`Vec3` position of the `RayHit` .
        """

    @property
    def normal(self) -> Vec3:
        """
        The :class:`Vec3` normal of the surface the :class:`RayCollider` has collided with.
        """

    @property
    def distance(self) -> float:
        """
        The distance between the :class:`Vec3` origin and the :class:`Vec3` position of the `RayHit` .
        """

# MATRICES ------------------------------------------------------------------------------------

# MAT4x4

class Matrix4x4:
    """
    A 4 by 4 matrix.
    """

    def __init__(self, x0: float, y0: float, z0: float, w0:float, x1:float,  y1:float,  z1:float,  w1:float,  x2:float,  y2:float,  z2:float,  w2:float,  x3:float,  y3:float,  z3:float,  w3:float) -> None:
        ...

    @staticmethod
    def from_identity(value:float) -> Matrix4x4:
        """
        Creates a identity matrix with `value`.
        """

    @staticmethod
    def from_ortho( left:float, right:float, bottom:float, top:float,  zNear:float | None = None, zFar:float | None = None) -> Matrix4x4:
        """
        Creates an orthographic projection :class:`Matrix4x4` with the specified bounds.
        """

    @staticmethod
    def look_at(eye:Vec3, center:Vec3, up:Vec3) -> Matrix4x4:
        """
        Creates a look at :class:`Matrix4x4` from the specified parameters.
        """

    @staticmethod
    def from_perspective( fovy:float, aspect:float, near:float, far:float) -> Matrix4x4:
        """
        Creates a perspective projection :class:`Matrix4x4` with the specified bounds.
        """

    @staticmethod
    def from_quaternion(quat: Quaternion) -> Matrix4x4:
        """
        Converts a :class:`Quaternion` into a :class:`Matrix4x4` .
        """

    def to_quaternion(self) -> Quaternion:
        """
        Converts the :class:`Matrix4x4` into a :class:`Quaternion` .
        """

    def get_up(self) -> Vec3:
        """
        Returns the up :class:`Vec3` of the :class:`Matrix4x4` , assuming that it is being used as a transform.
        """
    
    def get_right(self) -> Vec3:
        """
        Returns the right :class:`Vec3` of the :class:`Matrix4x4` , assuming that it is being used as a transform.
        """

    def get_forward(self) -> Vec3:
        """
        Returns the forward :class:`Vec3` of the :class:`Matrix4x4` , assuming that it is being used as a transform.
        """

    def inverse(self) -> Matrix4x4:
        """
        Returns the inverse of the :class:`Matrix4x4` .
        """

    def transpose(self) -> Matrix4x4:
        """
        Transposes the matrix.
        """

    def determinant(self) -> float:
        """
        Returns the determinant of the :class:`Matrix4x4` .
        """

    def __getitem__(self, index: int) -> Vec4:
        """
        Returns `index` th column of the :class:`Matrix4x4` as a :class:`Vec4` .
        """
    
    def __neg__(self) -> Matrix4x4:
        """
        Negates the :class:`Matrix4x4` .
        """

    def __sub__(self, other:Matrix4x4|float) -> Matrix4x4:
        """
        Subtracts the :class:`Matrix4x4` .
        """

    def __add__(self, other:Matrix4x4|float) -> Matrix4x4:
        """
        Adds the :class:`Matrix4x4` .
        """

    def __mul__(self, other:Matrix4x4|Vec4|float) -> Matrix4x4|Vec4:
        """
        Multiplies the :class:`Matrix4x4` .
        """

    # end vec mul

    def __truediv__(self, other:Matrix4x4|float) -> Matrix4x4:
        """
        Divides the :class:`Matrix4x4` .
        """

# MAT3x4

class Matrix3x4:
    """
    A 3 by 4 matrix.
    """

    def __init__(self, x0:float, y0, z0:float, w0:float, x1:float, y1:float, z1:float, w1:float, x2:float, y2:float, z2:float, w2:float) -> None:
        ...

    @staticmethod
    def from_identity(value:float) -> Matrix3x4:
        """
        Creates a identity matrix with `value`.
        """

    def __getitem__(self, index:int) -> Vec4:
        """
        Returns `index` th column of the :class:`Matrix3x4` as a :class:`Vec4` .
        """
    
    def __neg__(self) -> Matrix3x4:
        """
        Negates the :class:`Matrix3x4` .
        """

    def __sub__(self, other:Matrix3x4|float) -> Matrix3x4:
        """
        Subtracts the :class:`Matrix3x4` .
        """

    def __add__(self,  other:Matrix3x4|float) -> Matrix3x4:
        """
        Adds the :class:`Matrix3x4` .
        """

    def __mul__(self, other: float|Vec4) -> Matrix3x4|Vec4:
        """
        Multiplies the :class:`Matrix3x4` .
        """

    def __truediv__(self, other:float) -> Matrix3x4:
        """
        Divides the :class:`Matrix3x4` by a scalar .
        """

    def transpose(self) -> Matrix4x3:
        """
        Transposes the matrix.
        """

# MAT2x4

class Matrix2x4:
    """
    A 2 by 4 matrix.
    """

    def __init__(self, x0:float, y0:float, z0:float, w0:float, x1:float, y1:float, z1:float, w1:float) -> None:
        ...

    @staticmethod
    def from_identity(value:float) -> Matrix2x4:
        """
        Creates a identity matrix with `value`.
        """

    def __getitem__(self, index:int) -> Vec4:
        """
        Returns `index` th column of the :class:`Matrix2x4` as a :class:`Vec4` .
        """
    
    def __neg__(self) -> Matrix2x4:
        """
        Negates the :class:`Matrix2x4` .
        """

    def __sub__(self, other:Matrix2x4|float) -> Matrix2x4:
        """
        Subtracts the :class:`Matrix2x4` .
        """

    def __add__(self,  other:Matrix2x4|float) -> Matrix2x4:
        """
        Adds the :class:`Matrix2x4` .
        """

    def __mul__(self, other: float|Vec4) -> Matrix2x4|Vec4:
        """
        Multiplies the :class:`Matrix2x4` .
        """

    def __truediv__(self, other:float) -> Matrix2x4:
        """
        Divides the :class:`Matrix2x4` by a scalar .
        """

    def transpose(self) -> Matrix4x2:
        """
        Transposes the matrix.
        """

# MAT3x3

class Matrix3x3:
    """
    A 3 by 3 matrix.
    """

    def __init__(self, x0:float, y0:float, z0:float, x1:float, y1:float, z1:float, x2:float, y2:float, z2:float) -> None:
        ...

    @staticmethod
    def from_identity(value:float) -> Matrix3x3:
        """
        Creates a identity matrix with `value`.
        """

    def to_quaternion(self) -> Quaternion:
        """
        Converts the :class:`Matrix3x3` to a Quaternion.
        """

    def get_up(self) -> Vec3:
        """
        Gets the up :class:`Vec3` of the :class:`Matrix3x3` as a transform matrix.
        """
    
    def get_right(self) -> Vec3:
        """
        Gets the right :class:`Vec3` of the :class:`Matrix3x3` as a transform matrix.
        """

    def get_forward(self) -> Vec3:
        """
        Gets the forward :class:`Vec3` of the :class:`Matrix3x3` as a transform matrix.
        """

    def inverse(self) -> Matrix3x3:
        """
        Inverses the :class:`Matrix3x3` .
        """

    def determinant(self) -> float:
        """
        Returns the determinant of the :class:`Matrix3x3` .
        """

    def __getitem__(self, index:int) -> Vec3:
        """
        Returns `index` th column of the :class:`Matrix3x3` as a :class:`Vec3` .
        """
    
    def __neg__(self) -> Matrix3x3:
        """
        Negates the :class:`Matrix3x3` .
        """

    def __sub__(self, other:Matrix3x3|float) -> Matrix3x3:
        """
        Subtracts the :class:`Matrix3x3` .
        """

    def __add__(self,  other:Matrix3x3|float) -> Matrix3x3:
        """
        Adds the :class:`Matrix3x3` .
        """

    def __mul__(self, other: Matrix3x3|float|Vec3) -> Matrix3x3|Vec3:
        """
        Multiplies the :class:`Matrix3x3` .
        """

    def __truediv__(self, other:Matrix3x3|float) -> Matrix3x3:
        """
        Divides the :class:`Matrix3x3` .
        """

    def transpose(self) -> Matrix3x3:
        """
        Transposes the matrix.
        """

# MAT4x3

class Matrix4x3:
    """
    A 4 by 3 matrix.
    """

    def __init__(self, x0:float, y0:float, z0:float, x1:float, y1:float, z1:float, x2:float, y2:float, z2:float, x3:float, y3:float, z3:float) -> None:
        ...

    @staticmethod
    def from_identity(value:float) -> Matrix4x3:
        """
        Creates a identity matrix with `value`.
        """

    def __getitem__(self, index:int) -> Vec3:
        """
        Returns `index` th column of the :class:`Matrix4x3` as a :class:`Vec3` .
        """
    
    def __neg__(self) -> Matrix4x3:
        """
        Negates the :class:`Matrix4x3` .
        """

    def __sub__(self, other:Matrix4x3|float) -> Matrix4x3:
        """
        Subtracts the :class:`Matrix4x3` .
        """

    def __add__(self,  other:Matrix4x3|float) -> Matrix4x3:
        """
        Adds the :class:`Matrix4x3` .
        """

    def __mul__(self, other: float|Vec4) -> Matrix4x3|Vec3:
        """
        Multiplies the :class:`Matrix4x3` .
        """

    # end vec mul

    def __truediv__(self, other:float) -> Matrix4x3:
        """
        Divides the :class:`Matrix4x3` by a scalar.
        """

    def transpose(self) -> Matrix3x4:
        """
        Transposes the matrix.
        """

# MAT2x3

class Matrix2x3:
    """
    A 2 by 3 matrix.
    """

    def __init__(self, x0:float, y0:float, z0:float, x1:float, y1:float, z1:float) -> None:
        ...

    @staticmethod
    def from_identity(value:float) -> Matrix2x3:
        """
        Creates a identity matrix with `value`.
        """

    def __getitem__(self, index:int) -> Vec3:
        """
        Returns `index` th column of the :class:`Matrix4x3` as a :class:`Vec3` .
        """
    
    def __neg__(self) -> Matrix2x3:
        """
        Negates the :class:`Matrix2x3` .
        """

    def __sub__(self, other:Matrix2x3|float) -> Matrix2x3:
        """
        Subtracts the :class:`Matrix2x3` .
        """

    def __add__(self,  other:Matrix2x3|float) -> Matrix2x3:
        """
        Adds the :class:`Matrix2x3` .
        """

    def __mul__(self, other: float|Vec2|Vec3) -> Matrix2x3|Vec3:
        """
        Multiplies the :class:`Matrix2x3` .
        """

    # end vec mul

    def __truediv__(self, other:float) -> Matrix2x3:
        """
        Divides the :class:`Matrix2x3` by a scalar.
        """

    def transpose(self) -> Matrix3x2:
        """
        Transposes the matrix.
        """

# MAT2x2

class Matrix2x2:
    """
    A 2 by 2 matrix.
    """

    def __init__(self, x0:float, y0:float, x1:float, y1:float) -> None:
        ...

    @staticmethod
    def from_identity(value:float) -> Matrix2x2:
        """
        Creates a identity matrix with `value`.
        """

    def inverse(self) -> Matrix2x2:
        """
        Inverses the :class:`Matrix2x2` .
        """

    def determinant(self) -> float:
        """
        Returns the determinant of the :class:`Matrix2x2` .
        """

    def __getitem__(self, index:int) -> Vec2:
        """
        Returns `index` th column of the :class:`Matrix2x2` as a :class:`Vec2` .
        """
    
    def __neg__(self) -> Matrix2x2:
        """
        Negates the :class:`Matrix2x2` .
        """

    def __sub__(self, other:Matrix2x2|float) -> Matrix2x2:
        """
        Subtracts the :class:`Matrix2x2` .
        """

    def __add__(self,  other:Matrix2x2|float) -> Matrix2x2:
        """
        Adds the :class:`Matrix2x2` .
        """

    def __mul__(self, other: Matrix2x2|float|Vec2) -> Matrix2x2|Vec2:
        """
        Multiplies the :class:`Matrix2x2` .
        """

    def __truediv__(self, other:Matrix2x2|float) -> Matrix2x2:
        """
        Divides the :class:`Matrix2x2` .
        """

    def transpose(self) -> Matrix2x2:
        """
        Transposes the matrix.
        """

# MAT3x2

class Matrix3x2:
    """
    A 3 by 2 matrix.
    """

    def __init__(self, x0:float, y0:float, x1:float, y1:float, x2:float, y2:float) -> None:
        ...

    @staticmethod
    def from_identity(value:float) -> Matrix3x2:
        """
        Creates a identity matrix with `value`.
        """

    def __getitem__(self, index:int) -> Vec2:
        """
        Returns `index` th column of the :class:`Matrix3x2` as a :class:`Vec2` .
        """
    
    def __neg__(self) -> Matrix3x2:
        """
        Negates the :class:`Matrix3x2` .
        """

    def __sub__(self, other:Matrix3x2|float) -> Matrix3x2:
        """
        Subtracts the :class:`Matrix3x2`.
        """

    def __add__(self,  other:Matrix3x2|float) -> Matrix3x2:
        """
        Adds the :class:`Matrix3x2` .
        """

    def __mul__(self, other: float|Vec3) -> Matrix3x2|Vec2:
        """
        Multiplies the :class:`Matrix3x2` .
        """

    # end vec mul

    def __truediv__(self, other:float) -> Matrix3x2:
        """
        Divides the :class:`Matrix3x2` by a scalar.
        """
    
    def transpose(self) -> Matrix2x3:
        """
        Transposes the matrix.
        """

# MAT4x2

class Matrix4x2:
    """
    A 4 by 2 matrix.
    """

    def __init__(self, x0:float, y0:float, x1:float, y1:float, x2:float, y2:float, x3:float, y3:float) -> None:
        ...

    @staticmethod
    def from_identity(value:float) -> Matrix4x2:
        """
        Creates a identity matrix with `value`.
        """

    def __getitem__(self, index:int) -> Vec2:
        """
        Returns `index` th column of the :class:`Matrix4x2` as a :class:`Vec2` .
        """
    
    def __neg__(self) -> Matrix4x2:
        """
        Negates the :class:`Matrix4x2` .
        """

    def __sub__(self, other:Matrix4x2|float) -> Matrix4x2:
        """
        Subtracts the :class:`Matrix4x2` .
        """

    def __add__(self,  other:Matrix4x2|float) -> Matrix4x2:
        """
        Adds the :class:`Matrix4x2` .
        """

    def __mul__(self, other: float|Vec4) -> Matrix4x2|Vec2:
        """
        Multiplies the :class:`Matrix4x2` .
        """

    def __truediv__(self, other:float) -> Matrix4x2:
        """
        Divides the :class:`Matrix4x2` by a scalar.
        """

    def transpose(self) -> Matrix2x4:
        """
        Transposes the matrix.
        """


class Font:
    """
    Renders the specified Font to the screen.  Usable with :class:`Text` .
    """
    def __init__(self, font_path:str, font_size:int = 48) -> None:
        ...

class Text:
    """
    Renders the specified Text to the screen with the specified :class:`Font` .
    """
    def __init__(self, text_string:str, color:Vec4, position:Vec2, scale:Vec2 = Vec2(1.0, 1.0), rotation:float = 0, font:Font|None = None, material:Material|None = None) -> None:
        ...

    @property
    def rotation(self) -> float:
        """
        The rotation of the text.
        """

    @rotation.setter
    def rotation(self, value:float):
        """
        The rotation of the text.
        """

    @property
    def position(self) -> Vec2:
        """
        The :class:`Vec2` position of the text.
        """

    @position.setter
    def position(self, value:Vec2):
        """
        The :class:`Vec2` position of the text.
        """

    @property
    def scale(self) -> Vec2:
        """
        The :class:`Vec2` scale of the text.
        """

    @scale.setter
    def scale(self, value:Vec2):
        """
        The :class:`Vec2` scale of the text.
        """

    @property
    def color(self) -> Vec4:
        """
        The :class:`Vec4` color of the text.
        """

    @color.setter
    def color(self, value: Vec4):
        """
        The :class:`Vec4` color of the text.
        """

    @property
    def material(self) -> Material:
        """
        The :class:`Material` of the text.
        """

    @material.setter
    def material(self, value:Material):
        """
        The :class:`Material` of the text.
        """

    @property
    def font(self) -> Font:
        """
        The :class:`Font` of the text.
        """

    @font.setter
    def font(self, value:Font):
        """
        The :class:`Font` of the text.
        """

    @property
    def text(self) -> str:
        """
        The content of the text object.
        """

    @text.setter
    def text(self, value:str):
        """
        The content of the text object.
        """

class CubeMap:
    """
    A cubemap.  Can be used to create a :class:`SkyBox` .
    """
    def __init__(self, right_path:str, left_path:str, top_path:str, bottom_path:str, back_path:str, front_path:str) -> None:
        ...

class SkyBox:
    """
    A :class:`CubeMap` skybox. Can be applied to :attr:`Window.sky_box` .
    """

    def __init__(self, cube_map:CubeMap, mat:Material | None = None) -> None:
        ...

    @property
    def material(self) -> Material:
        """
        The :class:`Material` used to render the :class:`SkyBox` .
        """

    @material.setter
    def material(self, value:Material):
        """
        The :class:`Material` used to render the :class:`SkyBox` .
        """

    @property
    def cubemap(self) -> CubeMap:
        """
        The :class:`CubeMap` used in rendering the :class:`SkyBox` .
        """

    @cubemap.setter
    def cubemap(self, value:CubeMap):
        """
        The :class:`CubeMap` used in rendering the :class:`SkyBox` .
        """

class Emitter:
    """
    Emits particles.
    """

    def __init__(self, position:Vec3, direction:Quaternion, scale_min:Vec2 = Vec2(1.0, 1.0), scale_max:Vec2 = Vec2(1.0, 1.0), rate:int = 50, decay_rate:float = 1.0, spread:float = math.radians(30), velocity_decay:float = 1.0, start_velocity_min:float = 1.0, start_velocity_max:float = 1.0, start_lifetime_min:float = 10.0, start_lifetime_max:float = 10.0, color_min:Vec4 = Vec4(1.0,1.0,1.0,1.0), color_max:Vec4 = Vec4(1.0,1.0,1.0,1.0), material:Material | None = None) -> None:
        ...
        

    def start(self) -> None:
        """
        Starts emitting particles.
        """

    def stop(self) -> None:
        """
        Stops emitting particles.
        """

    # MATERIAL

    @property
    def material(self) -> Material:
        """
        The :class:`Material` for the particle.
        """

    @material.setter
    def material(self, value:Material):
        """
        The :class:`Material` for the particle.
        """

    # POSITION

    @property
    def position(self) -> Vec3:
        """
        The :class:`Vec3` position of the emitter.
        """

    @position.setter
    def position(self, value:Vec3) -> None:
        """
        The :class:`Vec3` position of the emitter.
        """

    # DIRECTION

    @property
    def direction(self) -> Quaternion:
        """
        The :class:`Quaternion` direction of the emitter.
        """

    @direction.setter
    def direction(self, value:Quaternion) -> None:
        """
        The :class:`Quaternion` direction of the emitter.
        """

    # COLOR

    @property
    def color_min(self) -> Vec4:
        """
        The :class:`Vec4` minimum color of the particles.  The color of the particle will be randomly selected between `Emitter.color_min` and `Emitter.color_max` .
        """

    @color_min.setter
    def color_min(self, value:Vec4) -> None:
        """
        The :class:`Vec4` minimum color of the particles.  The color of the particle will be randomly selected between `Emitter.color_min` and `Emitter.color_max` .
        """

    @property
    def color_max(self) -> Vec4:
        """
        The :class:`Vec4` maximum color of the particles.  The color of the particle will be randomly selected between `Emitter.color_min` and `Emitter.color_max` .
        """

    @color_max.setter
    def color_max(self, value:Vec4) -> None:
        """
        The :class:`Vec4` maximum color of the particles.  The color of the particle will be randomly selected between `Emitter.color_min` and `Emitter.color_max` .
        """

    # SCALE

    @property
    def scale_min(self) -> Vec2:
        """
        The :class:`Vec2` minimum scale of the particles.  The scale of the particle will be randomly selected between `Emitter.scale_min` and `Emitter.scale_max` .
        """

    @scale_min.setter
    def scale_min(self, value:Vec2) -> None:
        """
        The :class:`Vec2` minimum scale of the particles.  The scale of the particle will be randomly selected between `Emitter.scale_min` and `Emitter.scale_max` .
        """

    @property
    def scale_max(self) -> Vec2:
        """
        The :class:`Vec2` maximum scale of the particles.  The scale of the particle will be randomly selected between `Emitter.scale_min` and `Emitter.scale_max` .
        """

    @scale_max.setter
    def scale_max(self, value:Vec2) -> None:
        """
        The :class:`Vec2` maximum scale of the particles.  The scale of the particle will be randomly selected between `Emitter.scale_min` and `Emitter.scale_max` .
        """

    # rate
    
    @property
    def rate(self) -> int:
        """
        The ammount of particles present at any one time.
        """

    @rate.setter
    def rate(self, value:int) -> None:
        """
        The ammount of particles present at any one time.
        """

    # decay_rate
    
    @property
    def decay_rate(self) -> float:
        """
        The speed of decay, or how much a particle's life decreases each second.  When a particle's life reaches 0, it disappears.
        """

    @decay_rate.setter
    def decay_rate(self, value:float) -> None:
        """
        The speed of decay, or how much a particle's life decreases each second.  When a particle's life reaches 0, it disappears.
        """

    # spread
    
    @property
    def spread(self) -> float:
        """
        The radius of the cone of particle spread.
        """

    @spread.setter
    def spread(self, value:float) -> None:
        """
        The radius of the cone of particle spread.
        """

    # velocity_decay
    
    @property
    def velocity_decay(self) -> float:
        """
        The ammount a particle's velocity decreases over time.
        """

    @velocity_decay.setter
    def velocity_decay(self, value:float) -> None:
        """
        The ammount a particle's velocity decreases over time.
        """

    # start_velocity_min, start_velocity_max
    
    @property
    def start_velocity_min(self) -> float:
        """
        The minimum starting velocity of a particle.    The start velocity of a particle will be randomly selected between `Emitter.start_velocity_min` and `Emitter.start_velocity_max` .
        """

    @start_velocity_min.setter
    def start_velocity_min(self, value:float) -> None:
        """
        The minimum starting velocity of a particle.    The start velocity of a particle will be randomly selected between `Emitter.start_velocity_min` and `Emitter.start_velocity_max` .
        """

    @property
    def start_velocity_max(self) -> float:
        """
        The maximum starting velocity of a particle.    The start velocity of a particle will be randomly selected between `Emitter.start_velocity_min` and `Emitter.start_velocity_max` .
        """

    @start_velocity_max.setter
    def start_velocity_max(self, value:float) -> None:
        """
        The maximum starting velocity of a particle.    The start velocity of a particle will be randomly selected between `Emitter.start_velocity_min` and `Emitter.start_velocity_max` .
        """
    
    # start_lifetime_min, start_lifetime_max

    @property
    def start_lifetime_min(self) -> float:
        """
        The minimum starting life value of a particle.    The start life value of a particle will be randomly selected between `Emitter.start_lifetime_min` and `Emitter.start_lifetime_max` .
        """

    @start_lifetime_min.setter
    def start_lifetime_min(self, value:float) -> None:
        """
        The minimum starting life value of a particle.    The start life value of a particle will be randomly selected between `Emitter.start_lifetime_min` and `Emitter.start_lifetime_max` .
        """

    @property
    def start_lifetime_max(self) -> float:
        """
        The maximum starting life value of a particle.    The start life value of a particle will be randomly selected between `Emitter.start_lifetime_min` and `Emitter.start_lifetime_max` .
        """

    @start_lifetime_max.setter
    def start_lifetime_max(self, value:float) -> None:
        """
        The maximum starting life value of a particle.    The start life value of a particle will be randomly selected between `Emitter.start_lifetime_min` and `Emitter.start_lifetime_max` .
        """

class Sound:
    """
    A sound asset.  This can also be used to play sounds directly.
    """
    def __init__(self, win:Window, source:str, loop:bool) -> None:
        ...

    def play(self, volume:float = 1.0, panning:float = 0.0, pitch:float = 1.0, position: Vec3 | None = None) -> None:
        """
        Plays the sound globally.  If a :class:`Vec3` `position` is specified, the panning and volume will be balanced to play from that position relative to the camera.
        It will play with the same panning and volume until the sound is finished.  For dynamic positional audio see :class:`Sound3D` .
        """

    def stop(self) -> None:
        """
        Stops the sound if it is playing.
        """