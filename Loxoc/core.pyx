# distutils: language = c++
from cython.parallel cimport prange
from libc.math cimport M_PI
from os import path
from cpython.ref cimport Py_INCREF, Py_DECREF
from cython.operator import dereference, preincrement, postincrement
from typing import Generator
from libcpp.pair cimport pair
from libcpp.algorithm cimport remove
import math


# IMPORTANT: All data must be written saved within the CPP classes.
# All classes here just wrap CPP class pointers.
# So if data is stored in a python class for purposes other than reference counting,
# the pointer could change causing the class to associate with the wrong pointers.

# ALSO:
# Any classes that are heap allocated by c++ as a child of the c++ function's return type
# need to be wrapped in an RC and handled accordingly. You must increment the RC manually
# with .inc() wherever the counter is incremented and RC_collect(RC) when ever the counter
# is decremented.

cpdef void set_mod_path(str _path):
    c_set_mod_path(path.dirname(_path).encode())

cdef class Texture:
    @classmethod
    def from_file(cls, str file_path, TextureWraping wrap = TextureWraping.REPEAT, TextureFiltering filtering = TextureFiltering.LINEAR) -> Texture:
        return Texture_from_file(file_path, wrap, filtering)
        


    def __dealloc__(self):
        RC_collect(self.c_class)

ctypedef texture* texture_ptr

cpdef Texture Texture_from_file(str file_path, TextureWraping wrap, TextureFiltering filtering):
    ret = Texture()
    ret.c_class = new RC[texture_ptr](new texture(file_path.encode(), wrap, filtering))

    return ret

cdef class Camera:
    def __init__(self, Vec3 position, Vec3 rotation, int view_width, int view_height, float focal_length, float fov) -> None:
        self._position = position
        self._rotation = rotation.to_quaternion()
        self.c_class = new camera(self._position.c_class, self._rotation.c_class, view_width, view_height, focal_length, fov)
        
        

    def __dealloc__(self):
        del self.c_class

    @property
    def view_width(self) -> int:
        return self.c_class.view_width

    @view_width.setter
    def view_width(self, int value):
        self.c_class.view_width = value

    @property
    def view_height(self) -> int:
        return self.c_class.view_height

    @view_height.setter
    def view_height(self, int value):
        self.c_class.view_height = value

    @property
    def focal_length(self) -> float:
        return self.c_class.focal_length

    @focal_length.setter
    def focal_length(self, float value):
        self.c_class.focal_length = value

    @property
    def fov(self) -> float:
        return self.c_class.fov

    @fov.setter
    def fov(self, float value):
        self.c_class.fov = value

    @property
    def deltatime(self) -> double:
        return self.c_class.deltatime[0]

    @property
    def dt(self) -> double:
        return self.c_class.deltatime[0]

    @property
    def time_ns(self) -> int:
        return self.c_class.time_ns[0]

    @property
    def time(self) -> int:
        return self.c_class.time[0]

    # TRANSFORMS

    @property
    def position(self) -> Vec3:
        return self._position

    @position.setter
    def position(self, Vec3 value):
        self._position.c_class[0] = value.c_class[0]

    @property
    def rotation(self) -> Quaternion:
        return self._rotation

    @rotation.setter
    def rotation(self, value: Vec3 | Quaternion):
        cdef:
            Vec3 v
            Quaternion q
        if isinstance(value, Vec3):
            v = value
            self._rotation.c_class[0] = v.to_quaternion().c_class[0]
        elif isinstance(value, Quaternion):
            q = value
            self._rotation.c_class[0] = q.c_class[0]

ctypedef mesh_dict* mesh_dict_ptr
ctypedef RC[mesh*]* rc_mesh
ctypedef RC[mesh_dict*]* rc_mesh_dict
ctypedef mesh* mesh_ptr

cdef class MeshDict:

    def __init__(self, str name, meshes:list[Mesh | MeshDict]) -> None:
        cdef:
            object m
            Mesh _m
            MeshDict _m_d
            mesh_dict md = mesh_dict()
        for m in meshes:
            if isinstance(m, Mesh):
                _m = m
                md.insert(mesh_dict_child(_m.c_class))
            elif isinstance(m, MeshDict):
                _m_d = m
                md.insert(mesh_dict_child(_m_d.c_class))

        md.name = name.encode()
        
        self.c_class = new RC[mesh_dict_ptr](new mesh_dict(md))

    @property
    def name(self) -> str:
        return bytes(self.c_class.data.name).decode()

    @name.setter
    def name(self, str value):
        self.c_class.data.name = value.encode()

    def __repr__(self) -> str:
        return "".join([
            "MeshDict::`{}` {{".format(self.name),
            ", ".join(["\"{}\": {}".format(m_n, m) for m_n, m in self]),
            "}"
        ])

    def __dealloc__(self):
        RC_collect(self.c_class)
        

    def insert(self, m: Mesh | MeshDict) -> None:
        cdef:
            Mesh msh
            MeshDict msh_d
        if isinstance(m, Mesh):
            msh = m
            msh.c_class.inc()
            self.c_class.data.insert(mesh_dict_child(msh.c_class))
        elif isinstance(m, MeshDict):
            msh_d = m
            msh_d.c_class.inc()
            self.c_class.data.insert(mesh_dict_child(msh_d.c_class))

    def get(self, str name) -> Mesh|MeshDict :
        cdef:
            mesh_dict_child m = self.c_class.data.get(name.encode())
            RC[mesh*]* _m
            RC[mesh_dict*]* _m_d
        if holds_alternative[rc_mesh](m):
            _m = get[rc_mesh](m)
            return Mesh.from_cpp(_m)
        elif holds_alternative[rc_mesh_dict](m):
            _m_d = get[rc_mesh_dict](m)
            return MeshDict.from_cpp_ptr(_m_d)

    cpdef void remove(self, str name):
        cdef:
            mesh_dict_child m = self.c_class.data.data[name]
            RC[mesh*]* _m
            RC[mesh_dict*]* _m_d
        self.c_class.data.remove(name.encode())
        if holds_alternative[rc_mesh](m):
            _m = get[rc_mesh](m)
            _m.dec()
        elif holds_alternative[rc_mesh_dict](m):
            _m_d = get[rc_mesh_dict](m)
            _m_d.dec()

    def __iter__(self) -> Generator[(str, Mesh|MeshDict), None, None]:
        cdef:
            pair[string, mesh_dict_child] _pair
            RC[mesh*]* _m
            RC[mesh_dict*]* _m_d
            mesh_dict_child m
        for _pair in self.c_class.data[0]:
            m = _pair.second
            if holds_alternative[rc_mesh](m):
                _m = get[rc_mesh](m)
                yield (
                    bytes(_pair.first).decode(), 
                    Mesh.from_cpp(_m)
                )
            elif holds_alternative[rc_mesh_dict](m):
                _m_d = get[rc_mesh_dict](m)
                yield (
                    bytes(_pair.first).decode(), 
                    MeshDict.from_cpp_ptr(_m_d)
                )

    def __getitem__(self, str key) -> list[Mesh]:
        if self.c_class.data.data.contains(key.encode()):
            return self.get(key)
        else:
            raise KeyError("Key or Mesh name `{}` could not be found in MeshDict::`{}`".format(key, self.name))

    @staticmethod
    cdef MeshDict from_cpp(mesh_dict cppinst):
        cdef MeshDict ret = MeshDict.__new__(MeshDict)
        ret.c_class = new RC[mesh_dict_ptr](new mesh_dict(cppinst))
        return ret

    @staticmethod
    cdef MeshDict from_cpp_ptr(RC[mesh_dict*]* cppinst):
        cdef MeshDict ret = MeshDict.__new__(MeshDict)
        ret.c_class = cppinst
        ret.c_class.inc()
        return ret

cdef class Mesh:

    def __repr__(self) -> str:
        return "Mesh {{name: \"{}\"}}".format(self.name)

    def __dealloc__(self):
        RC_collect(self.c_class)

    @property
    def name(self) -> str:
        return bytes(self.c_class.data.name).decode()

    @staticmethod
    cdef Mesh from_cpp(RC[mesh*]* cppinst):
        cdef:
            Mesh ret = Mesh.__new__(Mesh)
        
        ret.c_class = cppinst
        ret.c_class.inc()

        # register material
        
        ret.material = Material.from_cpp(cppinst.data.mesh_material)

        ret.c_class.data.mesh_material = ret.material.c_class

        return ret

    @staticmethod
    def from_file(str file_path, bint animated = False) -> Model:
        return model_from_file(file_path, animated)

cpdef Model model_from_file(str file_path, bint animated):
    return Model.from_cpp_ptr(mesh.from_file(file_path.encode(), animated))

ctypedef model* model_ptr

cdef class Model:

    def __init__(self, MeshDict mesh_dict, bint animated = False) -> None:
        self._mesh_data = mesh_dict
        self.c_class = new RC[model_ptr](new model(self._mesh_data.c_class, animated))

    @staticmethod
    def from_file(str file_path, bint animated = False) -> Model:
        return model_from_file(file_path, animated)

    @property
    def use_default_material_properties(self) -> bint:
        return self.c_class.data.use_default_material_properties

    @use_default_material_properties.setter
    def use_default_material_properties(self, bint value) -> None:
        self.c_class.data.use_default_material_properties = value

    cpdef void play_animation(self, str animation):
        self.c_class.data.play_animation(animation.encode())

    @property
    def mesh_dict(self) -> MeshDict:
        return self._mesh_data

    @mesh_dict.setter
    def mesh_dict(self, MeshDict value) -> None:
        self._mesh_data.c_class[0] = value.c_class[0]

    @property
    def animated(self) -> bint:
        return self.c_class.data.animated

    @animated.setter
    def animated(self, bint value) -> None:
        self.c_class.data.animated = value

    @staticmethod
    cdef Model from_cpp(model cppinst):
        cdef Model ret = Model.__new__(Model)
        ret._mesh_data = MeshDict.from_cpp_ptr(cppinst.mesh_data)
        ret.c_class = new RC[model_ptr](new model(ret._mesh_data.c_class, cppinst.animated))
        return ret

    @staticmethod
    cdef Model from_cpp_ptr(RC[model*]* cppinst):
        cdef Model ret = Model.__new__(Model)
        ret._mesh_data = MeshDict.from_cpp_ptr(cppinst.data.mesh_data)
        ret.c_class = cppinst
        ret.c_class.inc()
        return ret

    def __dealloc__(self):
        RC_collect(self.c_class)

cdef class Object3D:
    def __init__(self, Model model_data, Vec3 position = Vec3(0.0,0.0,0.0),
    Vec3 rotation = Vec3(0.0,0.0,0.0), Vec3 scale = Vec3(1.0, 1.0, 1.0),
    Collider collider = None, Material material = None) -> None:
        self._position = position
        self._rotation = rotation.to_quaternion()
        self._scale = scale
        self._model_data = model_data
        
        if material:
            self._material = material
            if collider is not None:
                self.c_class = new object3d(self._model_data.c_class, position.c_class, self._rotation.c_class, scale.c_class, self._material.c_class, collider.c_class)
            else:
                self.c_class = new object3d(self._model_data.c_class, position.c_class, self._rotation.c_class, scale.c_class, self._material.c_class)
        else:
            if collider is not None:
                self.c_class = new object3d(self._model_data.c_class, position.c_class, self._rotation.c_class, scale.c_class)
                collider.c_class.inc()
                self.c_class.colliders.push_back(collider.c_class)
            else:
                self.c_class = new object3d(self._model_data.c_class, position.c_class, self._rotation.c_class, scale.c_class)

    @property
    def use_default_material_properties(self) -> bint:
        return self.c_class.model_data.data.use_default_material_properties

    @use_default_material_properties.setter
    def use_default_material_properties(self, bint value) -> None:
        self.c_class.model_data.data.use_default_material_properties = value

    cpdef Matrix4x4 get_model_matrix(self):
        return mat4x4_from_cpp(self.c_class.get_model_matrix())

    cpdef void add_collider(self, Collider collider):
        collider.c_class.inc()
        self.c_class.colliders.push_back(collider.c_class)

    cpdef void remove_collider(self, Collider collider):
        self.c_class.colliders.erase(remove(self.c_class.colliders.begin(), self.c_class.colliders.end(), collider.c_class), self.c_class.colliders.end())
        RC_collect(collider.c_class)

    def check_collision(self, intersection: Vec3 | Object3D) -> bool:
        cdef:
            Vec3 argvec
            Object3D argobj
        if isinstance(intersection, Vec3):
            argvec = intersection
            return self.c_class.check_collision_point(argvec.c_class[0])
        elif isinstance(intersection, Object3D):
            argobj = intersection
            return self.c_class.check_collision_object(argobj.c_class)
        
        return False

    cpdef void play_animation(self, str animation_name):
        self._model_data.play_animation(animation_name)

    @property
    def position(self) -> Vec3:
        return self._position

    @position.setter
    def position(self, Vec3 value):
        self._position.c_class[0] = value.c_class[0]

    @property
    def material(self) -> Vec3:
        return self._material

    @material.setter
    def material(self, Material value) -> None:
        self._material.c_class[0] = value.c_class[0]

    @property
    def model(self) -> Model:
        return self._model_data

    @model.setter
    def model(self, Model value) -> None:
        self._model_data.c_class[0] = value.c_class[0]

    @property
    def rotation(self) -> Quaternion:
        return self._rotation

    @rotation.setter
    def rotation(self, value: Vec3 | Quaternion):
        cdef:
            quaternion q
            Quaternion Q
        if isinstance(value, Vec3):
            Q = value.to_quaternion()
            q = Q.c_class[0]
        elif isinstance(value, Quaternion):
            Q = value
            q = Q.c_class[0]

        self._rotation.c_class[0] = q

    @property
    def scale(self) -> Vec3:
        return self._scale

    @scale.setter
    def scale(self, Vec3 value):
        self._scale.c_class[0] = value.c_class[0]

    def __dealloc__(self):
        cdef RC[collider*]* col
        for col in self.c_class.colliders:
            RC_collect(col)
        del self.c_class


    cpdef void set_uniform(self, str name, value:UniformValueType):
        _set_uniform(self, name, value)


ctypedef shader* shader_ptr

cdef class Shader:
    
    def __init__(self, str source, ShaderType shader_type) -> None:
        self.c_class = new RC[shader_ptr](new shader(source.encode(), shader_type))

    def __dealloc__(self):
        RC_collect(self.c_class)

    @classmethod
    def from_file(cls, str filepath, ShaderType type) -> Shader:
        with open(filepath, 'r') as fp:
            src = fp.read()
        return cls(src, type)

    @staticmethod
    cdef Shader from_cpp(RC[shader*]* cppinst):
        cdef:
            Shader ret = Shader.__new__(Shader)
        ret.c_class = cppinst
        ret.c_class.inc()
        return ret

cdef class Quaternion:
    def __init__(self, float w, float x, float y, float z) -> None:
        self.c_class = new quaternion(w,x,y,z)

    def __copy__(self) -> Quaternion:
        return Quaternion.from_quat(self)

    def __repr__(self) -> str:
        return f"<{{{self.w}, {self.x}, {self.y}, {self.z}}}>"

    def __dealloc__(self):
        del self.c_class

    def __neg__(self) -> Quaternion:
        return quat_from_cpp(-self.c_class[0])

    @property
    def up(self) -> Vec3:
        return vec3_from_cpp(self.c_class.get_up())

    @property
    def right(self) -> Vec3:
        return vec3_from_cpp(self.c_class.get_right())

    @property
    def forward(self) -> Vec3:
        return vec3_from_cpp(self.c_class.get_forward())

    @property
    def w(self) -> float:
        return self.c_class.get_w()

    @w.setter
    def w(self, float value):
        self.c_class.set_w(value)

    @property
    def x(self) -> float:
        return self.c_class.get_x()

    @x.setter
    def x(self, float value):
        self.c_class.set_x(value)

    @property
    def y(self) -> float:
        return self.c_class.get_y()

    @y.setter
    def y(self, float value):
        self.c_class.set_y(value)

    @property
    def z(self) -> float:
        return self.c_class.get_z()

    @z.setter
    def z(self, float value):
        self.c_class.set_z(value)

    @property
    def euler_angles(self) -> Vec3:
        return self.to_euler()

    @euler_angles.setter
    def euler_angles(self, value: Vec3 | Quaternion):
        if isinstance(value, Quaternion):
            self.set_euler_quat(value)
        elif isinstance(value, Vec3):
            self.set_euler_vec(value)
    
    cpdef void set_euler_quat(self, Quaternion value):
        self.c_class[0].quat = value.c_class[0].quat

    cpdef void set_euler_vec(self, Vec3 value):
        self.c_class[0].quat = value.c_class.to_quaternion().quat
        

    @property
    def euler_pitch(self) -> float:
        return self.to_euler().x

    def rotate_pitch(self, float value) -> Quaternion:
        self.c_class.rotate(vec3(1.0,0.0,0.0), value)
        return self

    @property
    def euler_yaw(self) -> float:
        return self.to_euler().y

    def rotate_yaw(self, float value) -> Quaternion:
        self.c_class.rotate(vec3(0.0,1.0,0.0), value)
        return self

    @property
    def euler_roll(self) -> float:
        return self.to_euler().z

    def rotate_roll(self, float value) -> Quaternion:
        self.c_class.rotate(vec3(0.0,0.0,1.0), value)
        return self

    # OPERATORS

    def __add__(self, other:Quaternion | float) -> Quaternion:
        if isinstance(other, Quaternion):
            return self.quatadd(other)
        else:
            return self.floatadd(other)

    cpdef Quaternion quatadd(self, Quaternion other):
        return quat_from_cpp(self.c_class[0] + other.c_class[0])

    cpdef Quaternion floatadd(self, float other):
        return quat_from_cpp(self.c_class[0] + other)

    def __sub__(self, other:Quaternion | float) -> Quaternion:
        if isinstance(other, Quaternion):
            return self.quatsub(other)
        else:
            return self.floatsub(other)

    cpdef Quaternion quatsub(self, Quaternion other):
        return quat_from_cpp(self.c_class[0] - other.c_class[0])

    cpdef Quaternion floatsub(self, float other):
        return quat_from_cpp(self.c_class[0] - other)

    def __mul__(self, other:Quaternion | float | Vec3) -> Quaternion | Vec3:
        if isinstance(other, Quaternion):
            return self.quatmul(other)
        elif isinstance(other, Vec3):
            return self.vecmul(other)
        else:
            return self.floatmul(other)
        
    cpdef Quaternion quatmul(self, Quaternion other):
        return quat_from_cpp(self.c_class[0] * other.c_class[0])

    cpdef Vec3 vecmul(self, Vec3 other):
        return vec3_from_cpp(self.c_class[0] * other.c_class[0])

    cpdef Quaternion floatmul(self, float other):
        return quat_from_cpp(self.c_class[0] * other)

    def __truediv__(self, other:Quaternion | float) -> Quaternion:
        if isinstance(other, Quaternion):
            return self.quatdiv(other)
        else:
            return self.floatdiv(other)
        
    cpdef Quaternion quatdiv(self, Quaternion other):
        return quat_from_cpp(self.c_class[0] / other.c_class[0])

    cpdef Quaternion floatdiv(self, float other):
        return quat_from_cpp(self.c_class[0] / other)

    def dot(self, Quaternion other) -> float:
        return self.c_class.dot(other.c_class[0])

    def cross(self, other:Quaternion | Vec3) -> float:
        if isinstance(other, Quaternion):
            return self.quat_cross(other)
        elif isinstance(other, Vec3):
            return self.vec_cross(other)

    cpdef Quaternion quat_cross(self, Quaternion other):
        return quat_from_cpp(self.c_class.cross(other.c_class[0]))

    cpdef Vec3 vec_cross(self, Vec3 other):
        return vec3_from_cpp(self.c_class.cross(other.c_class[0]))

    cpdef float get_magnitude(self):
        return self.c_class[0].get_magnitude()

    cpdef Quaternion get_normalized(self):
        return quat_from_cpp(self.c_class.get_normalized())

    cpdef Quaternion get_conjugate(self):
        return quat_from_cpp(self.c_class.get_conjugate())

    cpdef Quaternion get_inverse(self):
        return quat_from_cpp(self.c_class.get_inverse())
    
    cpdef Quaternion get_reverse(self):
        return quat_from_cpp(self.c_class.get_reverse())


    cpdef Vec3 to_euler(self):
        return vec3_from_cpp(self.c_class.to_euler())

    @staticmethod
    def from_euler(Vec3 euler_vec) -> Quaternion:
        return quat_from_cpp(quaternion.from_euler(euler_vec.c_class[0]))

    @staticmethod
    def from_axis_angle(Vec3 axis, float angle) -> Quaternion:
        return quat_from_cpp(quaternion.from_axis_angle(axis.c_class[0], angle))

    @staticmethod
    def from_unit(Vec3 axis, Vec3 up = None) -> Quaternion:
        up = up if up else Vec3(0.0,1.0,0.0)
        return quat_from_cpp(quaternion.from_unit(axis.c_class[0], up.c_class[0]))


    cpdef void rotate(self, Vec3 axis, float angle):
        self.c_class.rotate(axis.c_class[0], angle)

    @staticmethod
    def from_quat(Quaternion quat) -> Quaternion: # copy method
        cdef Quaternion q = Quaternion.__new__(Quaternion)
        q.c_class = new quaternion(quat.c_class[0])
        return q

    cpdef Quaternion lerp(self, Quaternion other, float ratio):
        return quat_from_cpp(self.c_class.lerp(other.c_class[0], ratio))

    cpdef Quaternion slerp(self, Quaternion other, float ratio):
        return quat_from_cpp(self.c_class.slerp(other.c_class[0], ratio))

cdef Quaternion quat_from_cpp(quaternion cppinst):
    cdef Quaternion ret = Quaternion.__new__(Quaternion)
    ret.c_class = new quaternion(cppinst)
    return ret

cdef class Vec4:
    def __init__(self, float x, float y, float z, float w) -> None:
        self.c_class = new vec4(x,y,z,w)

    cpdef Vec4 lerp(self, Vec4 other, float ratio):
        return vec4_from_cpp(self.c_class.lerp(other.c_class[0], ratio))

    def __copy__(self) -> Vec4:
        return vec4_from_cpp(self.c_class[0])

    def __repr__(self) -> str:
        return f"<{self.x}, {self.y}, {self.z}, {self.w}>"

    def __dealloc__(self):
        del self.c_class

    def __neg__(self) -> Vec4:
        return vec4_from_cpp(-self.c_class[0])

    @property
    def x(self) -> float:
        return self.c_class.get_x()

    @x.setter
    def x(self, float value):
        self.c_class.set_x(value)

    @property
    def y(self) -> float:
        return self.c_class.get_y()

    @y.setter
    def y(self, float value):
        self.c_class.set_y(value)

    @property
    def z(self) -> float:
        return self.c_class.get_z()

    @z.setter
    def z(self, float value):
        self.c_class.set_z(value)

    @property
    def w(self) -> float:
        return self.c_class.get_w()

    @w.setter
    def w(self, float value):
        self.c_class.set_w(value)

    @property
    def up(self) -> Vec4:
        return vec4_from_cpp(self.c_class.get_up())

    @property
    def right(self) -> Vec4:
        return vec4_from_cpp(self.c_class.get_right())

    @property
    def forward(self) -> Vec4:
        return vec4_from_cpp(self.c_class.get_forward())


    # OPERATORS

    def __add__(self, other:Vec4 | float) -> Vec4:
        if isinstance(other, Vec4):
            return self.vecadd(other)
        else:
            return self.floatadd(other)

    cpdef Vec4 vecadd(self, Vec4 other):
        return vec4_from_cpp(self.c_class[0] + other.c_class[0])

    cpdef Vec4 floatadd(self, float other):
        return vec4_from_cpp(self.c_class[0] + other)

    def __sub__(self, other:Vec4 | float) -> Vec4:
        if isinstance(other, Vec4):
            return self.vecsub(other)
        else:
            return self.floatsub(other)

    cpdef Vec4 vecsub(self, Vec4 other):
        return vec4_from_cpp(self.c_class[0] - other.c_class[0])

    cpdef Vec4 floatsub(self, float other):
        return vec4_from_cpp(self.c_class[0] - other)

    def __mul__(self, other:Vec4 | float | Quaternion) -> Vec4:
        if isinstance(other, Vec4):
            return self.vecmul(other)
        elif isinstance(other, Quaternion):
            return self.quatmul(other)
        else:
            return self.floatmul(other)
        
    cpdef Vec4 vecmul(self, Vec4 other):
        return vec4_from_cpp(self.c_class[0] * other.c_class[0])

    cpdef Vec4 quatmul(self, Quaternion other):
        return vec4_from_cpp(self.c_class[0] * other.c_class[0])

    cpdef Vec4 floatmul(self, float other):
        return vec4_from_cpp(self.c_class[0] * other)

    def __truediv__(self, other:Vec4 | float) -> Vec4:
        if isinstance(other, Vec4):
            return self.vecdiv(other)
        else:
            return self.floatdiv(other)
        
    cpdef Vec4 vecdiv(self, Vec4 other):
        return vec4_from_cpp(self.c_class[0] / other.c_class[0])

    cpdef Vec4 floatdiv(self, float other):
        return vec4_from_cpp(self.c_class[0] / other)

    def dot(self, Vec4 other) -> float:
        return self.c_class[0].dot(other.c_class[0])

    cpdef float get_magnitude(self):
        return self.c_class.get_magnitude()

    cpdef Vec4 get_normalized(self):
        return vec4_from_cpp(self.c_class.get_normalized())


    cpdef Vec3 to_vec3(self):
        return vec3_from_cpp(self.c_class.to_vec3())
    cpdef Vec2 to_vec2(self):
        return vec2_from_cpp(self.c_class.to_vec2())

    def outer_product(self, vec: Vec2|Vec3|Vec4) -> Matrix2x4|Matrix3x4|Matrix4x4:
        cdef:
            Vec2 v2
            Vec3 v3
            Vec4 v4
        if isinstance(vec, Vec2):
            v2 = vec
            return mat2x4_from_cpp(self.c_class.outer_product(v2.c_class[0]))
        elif isinstance(vec, Vec3):
            v3 = vec
            return mat3x4_from_cpp(self.c_class.outer_product(v3.c_class[0]))
        elif isinstance(vec, Vec4):
            v4 = vec
            return mat4x4_from_cpp(self.c_class.outer_product(v4.c_class[0]))

    cpdef float distance(self, Vec4 other):
        return self.c_class.distance(other.c_class[0])

cdef Vec4 vec4_from_cpp(vec4 cppinst):
    cdef Vec4 ret = Vec4.__new__(Vec4)
    ret.c_class = new vec4(cppinst)
    return ret

cdef class Vec3:
    def __init__(self, float x, float y, float z) -> None:
        self.c_class = new vec3(x,y,z)
    
    cpdef Vec3 lerp(self, Vec3 other, float ratio):
        return vec3_from_cpp(self.c_class.lerp(other.c_class[0], ratio))

    def __copy__(self) -> Vec3:
        return vec3_from_cpp(self.c_class[0])

    def __repr__(self) -> str:
        return f"<{self.x}, {self.y}, {self.z}>"

    def __dealloc__(self):
        del self.c_class

    def __neg__(self) -> Vec3:
        return vec3_from_cpp(-self.c_class[0])

    @property
    def quaternion(self) -> Quaternion:
        return self.to_quaternion()

    @quaternion.setter
    def quaternion(self, value: Vec3 | Quaternion):
        if isinstance(value, Quaternion):
            self.set_quat_quat(value)
        elif isinstance(value, Vec3):
            self.set_quat_vec(value)

    cpdef void set_quat_quat(self, Quaternion value):
        self.c_class[0] = value.c_class.to_euler()

    cpdef void set_quat_vec(self, Vec3 value):
        self.c_class[0] = value.c_class[0]

    @property
    def x(self) -> float:
        return self.c_class.get_x()

    @x.setter
    def x(self, float value):
        self.c_class.set_x(value)

    @property
    def y(self) -> float:
        return self.c_class.get_y()

    @y.setter
    def y(self, float value):
        self.c_class.set_y(value)

    @property
    def z(self) -> float:
        return self.c_class.get_z()

    @z.setter
    def z(self, float value):
        self.c_class.set_z(value)

    @property
    def up(self) -> Vec3:
        return vec3_from_cpp(self.c_class.get_up())

    @property
    def right(self) -> Vec3:
        return vec3_from_cpp(self.c_class.get_right())

    @property
    def forward(self) -> Vec3:
        return vec3_from_cpp(self.c_class.get_forward())


    # OPERATORS

    def __add__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecadd(other)
        else:
            return self.floatadd(other)

    cpdef Vec3 vecadd(self, Vec3 other):
        return vec3_from_cpp(self.c_class[0] + other.c_class[0])

    cpdef Vec3 floatadd(self, float other):
        return vec3_from_cpp(self.c_class[0] + other)

    def __sub__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecsub(other)
        else:
            return self.floatsub(other)

    cpdef Vec3 vecsub(self, Vec3 other):
        return vec3_from_cpp(self.c_class[0] - other.c_class[0])

    cpdef Vec3 floatsub(self, float other):
        return vec3_from_cpp(self.c_class[0] - other)

    def __mul__(self, other:Vec3 | float | Quaternion) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecmul(other)
        elif isinstance(other, Quaternion):
            return self.quatmul(other)
        else:
            return self.floatmul(other)
        
    cpdef Vec3 vecmul(self, Vec3 other):
        return vec3_from_cpp(self.c_class[0] * other.c_class[0])

    cpdef Vec3 quatmul(self, Quaternion other):
        return vec3_from_cpp(self.c_class[0] * other.c_class[0])

    cpdef Vec3 floatmul(self, float other):
        return vec3_from_cpp(self.c_class[0] * other)

    def __truediv__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecdiv(other)
        else:
            return self.floatdiv(other)
        
    cpdef Vec3 vecdiv(self, Vec3 other):
        return vec3_from_cpp(self.c_class[0] / other.c_class[0])

    cpdef Vec3 floatdiv(self, float other):
        return vec3_from_cpp(self.c_class[0] / other)

    def dot(self, Vec3 other) -> float:
        return self.c_class[0].dot(other.c_class[0])

    def cross(self, other:Quaternion | Vec3) -> float:
        if isinstance(other, Quaternion):
            return self.quat_cross(other)
        elif isinstance(other, Vec3):
            return self.vec_cross(other)

    cpdef Quaternion quat_cross(self, Quaternion other):
        return vec3_from_cpp(self.c_class.cross(other.c_class[0]))

    cpdef Vec3 vec_cross(self, Vec3 other):
        return vec3_from_cpp(self.c_class.cross(other.c_class[0]))

    cpdef float get_magnitude(self):
        return self.c_class.get_magnitude()

    cpdef Vec3 get_normalized(self):
        return vec3_from_cpp(self.c_class.get_normalized())

    cpdef Quaternion to_quaternion(self):
        return quat_from_cpp(self.c_class.to_quaternion())

    def outer_product(self, vec: Vec2|Vec3|Vec4) -> Matrix2x3|Matrix3x3|Matrix4x3:
        cdef:
            Vec2 v2
            Vec3 v3
            Vec4 v4
        if isinstance(vec, Vec2):
            v2 = vec
            return mat2x3_from_cpp(self.c_class.outer_product(v2.c_class[0]))
        elif isinstance(vec, Vec3):
            v3 = vec
            return mat3x3_from_cpp(self.c_class.outer_product(v3.c_class[0]))
        elif isinstance(vec, Vec4):
            v4 = vec
            return mat4x3_from_cpp(self.c_class.outer_product(v4.c_class[0]))
    
    cpdef float distance(self, Vec3 other):
        return self.c_class.distance(other.c_class[0])

cdef Vec3 vec3_from_cpp(vec3 cppinst):
    cdef Vec3 ret = Vec3.__new__(Vec3)
    ret.c_class = new vec3(cppinst)
    return ret
    
cdef class Vec2:
    def __init__(self, float x, float y) -> None:
        self.c_class = new vec2(x,y)

    cpdef Vec2 lerp(self, Vec2 other, float ratio):
        return vec2_from_cpp(self.c_class.lerp(other.c_class[0], ratio))

    def __repr__(self) -> str:
        return f"<{self.x}, {self.y}>"

    def __dealloc__(self):
        del self.c_class

    def __neg__(self) -> Vec2:
        return vec2_from_cpp(-self.c_class[0])

    def __copy__(self) -> Vec2:
        return vec2_from_cpp(self.c_class[0])


    @property
    def angle(self) -> float:
        return self.to_angle()

    @angle.setter
    def angle(self, float value):
        self.c_class[0] = vec2.from_angle(value) * self.c_class.get_magnitude()

    cpdef float to_angle(self):
        return self.c_class.to_angle()

    @property
    def x(self) -> float:
        return self.c_class.get_x()

    @x.setter
    def x(self, float value):
        self.c_class.set_x(value)

    @property
    def y(self) -> float:
        return self.c_class.get_y()

    @y.setter
    def y(self, float value):
        self.c_class.set_y(value)

    # OPERATORS

    def __add__(self, other:Vec2 | float) -> Vec2:
        if isinstance(other, Vec2):
            return self.vecadd(other)
        else:
            return self.floatadd(other)

    cpdef Vec2 vecadd(self, Vec2 other):
        return vec2_from_cpp(self.c_class[0] + other.c_class[0])

    cpdef Vec2 floatadd(self, float other):
        return vec2_from_cpp(self.c_class[0] + other)

    def __sub__(self, other:Vec2 | float) -> Vec2:
        if isinstance(other, Vec2):
            return self.vecsub(other)
        else:
            return self.floatsub(other)

    cpdef Vec2 vecsub(self, Vec2 other):
        return vec2_from_cpp(self.c_class[0] - other.c_class[0])

    cpdef Vec2 floatsub(self, float other):
        return vec2_from_cpp(self.c_class[0] - other)

    def __mul__(self, other: Vec2 | float) -> Vec2:
        if isinstance(other, Vec2):
            return self.vecmul(other)
        else:
            return self.floatmul(other)
        
    cpdef Vec2 vecmul(self, Vec2 other):
        return vec2_from_cpp(self.c_class[0] * other.c_class[0])


    cpdef Vec2 floatmul(self, float other):
        return vec2_from_cpp(self.c_class[0] * other)

    def __truediv__(self, other:Vec2 | float) -> Vec2:
        if isinstance(other, Vec2):
            return self.vecdiv(other)
        else:
            return self.floatdiv(other)
        
    cpdef Vec2 vecdiv(self, Vec2 other):
        return vec2_from_cpp(self.c_class[0] / other.c_class[0])

    cpdef Vec2 floatdiv(self, float other):
        return vec2_from_cpp(self.c_class[0] / other)

    def dot(self, Vec2 other) -> float:
        return self.c_class.dot(other.c_class[0])

    cpdef float get_magnitude(self):
        return self.c_class.get_magnitude()

    cpdef Vec2 get_normalized(self):
        cdef Vec2 ret = vec2_from_cpp(self.c_class.get_normalized())
        return ret

    @classmethod
    def from_angle(cls, float angle) -> Vec2:
        return vec2_from_cpp(vec2.from_angle(angle))

    def outer_product(self, vec: Vec2|Vec3|Vec4) -> Matrix2x2|Matrix3x2|Matrix4x2:
        cdef:
            Vec2 v2
            Vec3 v3
            Vec4 v4
        if isinstance(vec, Vec2):
            v2 = vec
            return mat2x2_from_cpp(self.c_class.outer_product(v2.c_class[0]))
        elif isinstance(vec, Vec3):
            v3 = vec
            return mat3x2_from_cpp(self.c_class.outer_product(v3.c_class[0]))
        elif isinstance(vec, Vec4):
            v4 = vec
            return mat4x2_from_cpp(self.c_class.outer_product(v4.c_class[0]))

    cpdef float distance(self, Vec2 other):
        return self.c_class.distance(other.c_class[0])

cdef Vec2 vec2_from_cpp(vec2 cppinst):
    cdef:
        Vec2 ret = Vec2.__new__(Vec2)
        vec2* v = new vec2(cppinst)
    ret.c_class = v
    return ret

ctypedef material* material_ptr

cdef class Material:
    def __init__(self, Shader vertex = None, Shader fragment = None, Shader geometry = None, Shader compute = None, bint animated = False) -> None:
        if vertex:
            self._vertex_shader = vertex
        else:
            self._vertex_shader = Shader.from_file(path.join(path.dirname(__file__), "default_vertex_animated.glsl" if animated else "default_vertex.glsl"), ShaderType.VERTEX)

        if fragment:
            self._fragment_shader = fragment
        else:
            self._fragment_shader = Shader.from_file(path.join(path.dirname(__file__), "default_fragment.glsl"), ShaderType.FRAGMENT)

        if geometry and not compute:
            self._geometry_shader = geometry
            self.c_class = new RC[material_ptr](new material(self._vertex_shader.c_class, self._fragment_shader.c_class, self._geometry_shader.c_class))
        elif compute and not geometry:
            self._compute_shader = compute
            self.c_class = new RC[material_ptr](new material(self._vertex_shader.c_class, self._fragment_shader.c_class, NULL, self._compute_shader.c_class))
        elif compute and geometry:
            self._geometry_shader = geometry
            self._compute_shader = compute
            self.c_class = new RC[material_ptr](new material(self._vertex_shader.c_class, self._fragment_shader.c_class, self._geometry_shader.c_class, self._compute_shader.c_class))
        else:
            self.c_class = new RC[material_ptr](new material(self._vertex_shader.c_class, self._fragment_shader.c_class))
        
    def __dealloc__(self):
        RC_collect(self.c_class)

    @property
    def diffuse_texture(self) -> Texture:
        return self._diffuse_texture

    @diffuse_texture.setter
    def diffuse_texture(self, Texture value):
        self._diffuse_texture = value
        self.c_class.data.diffuse_texture = self._diffuse_texture.c_class

    @property
    def specular_texture(self) -> Texture:
        return self._specular_texture

    @specular_texture.setter
    def specular_texture(self, Texture value):
        self._specular_texture = value
        self.c_class.data.specular_texture = self._specular_texture.c_class

    @property
    def normals_texture(self) -> Texture:
        return self._normals_texture

    @normals_texture.setter
    def normals_texture(self, Texture value):
        self._normals_texture = value
        self.c_class.data.normals_texture = self._normals_texture.c_class
    
    cpdef void set_uniform(self, str name, value:UniformValueType):
        _set_uniform(self, name, value)

    @staticmethod
    cdef Material from_cpp(RC[material*]* cppinst):
        cdef:
            Material ret = Material.__new__(Material)

        ret.c_class = cppinst
        ret.c_class.inc()

        # register material
        
        ret._vertex_shader = Shader.from_cpp(cppinst.data.vertex)
        ret._fragment_shader = Shader.from_cpp(cppinst.data.fragment)

        ret.c_class.data.vertex = ret._vertex_shader.c_class
        ret.c_class.data.fragment = ret._fragment_shader.c_class

        return ret
        


cdef class Window:

    def __init__(self, str title, Camera cam, int width, int height, bint fullscreen = False, Vec3 ambient_light = Vec3(1.0, 1.0, 1.0)) -> None:
        self._ambient_light = ambient_light
        self.c_class = new window(title.encode(), cam.c_class, width, height, fullscreen, self._ambient_light.c_class)
    
    @property
    def fullscreen(self) -> bint:
        return self.c_class.fullscreen

    @fullscreen.setter
    def fullscreen(self, bint value):
        self.c_class.set_fullscreen(value)

    @property
    def sky_box(self) -> SkyBox:
        return self._sky_box

    @sky_box.setter
    def sky_box(self, SkyBox value):
        self._sky_box = value
        self.c_class.sky_box = self._sky_box.c_class

    @property
    def ambient_light(self) -> Vec3:
        return self._ambient_light

    @ambient_light.setter
    def ambient_light(self, Vec3 value):
        self._ambient_light.c_class[0] = value.c_class[0]

    @property
    def resizeable(self) -> bint:
        return self.c_class.resizeable

    @resizeable.setter
    def resizeable(self, bint value):
        self.c_class.resizeable = value

    @property
    def event(self) -> Event:
        ret = Event()
        ret.c_class = self.c_class.current_event
        return ret

    @property
    def deltatime(self) -> double:
        return self.c_class.deltatime

    @property
    def dt(self) -> double:
        return self.c_class.deltatime

    @property
    def time_ns(self) -> int:
        return self.c_class.time_ns

    @property
    def time(self) -> int:
        return self.c_class.time

    def __dealloc__(self):
        del self.c_class

    cpdef void update(self):
        self.c_class.update()

    cpdef void lock_mouse(self, bint lock):
        self.c_class.lock_mouse(lock)

    cpdef void add_object(self, Object3D obj):
        Py_INCREF(obj)
        self.c_class.add_object(obj.c_class)

    cpdef void remove_object(self, Object3D obj):
        self.c_class.remove_object(obj.c_class)
        Py_DECREF(obj)

    cpdef void add_object_list(self, list[Object3D] objs):
        cdef:
            Object3D obj

        for obj in objs:
            self.add_object(obj)

    cpdef void remove_object_list(self, list[Object3D] objs):
        cdef:
            Object3D obj

        for obj in objs:
            self.remove_object(obj)

    cpdef void add_object2d(self, Object2D obj):
        Py_INCREF(obj)
        self.c_class.add_object2d(obj.c_class)

    cpdef void remove_object2d(self, Object2D obj):
        self.c_class.remove_object2d(obj.c_class)
        Py_DECREF(obj)

    cpdef void add_object2d_list(self, list[Object2D] objs):
        cdef:
            Object2D obj

        for obj in objs:
            self.add_object2d(obj)

    cpdef void remove_object2d_list(self, list[Object2D] objs):
        cdef:
            Object2D obj

        for obj in objs:
            self.remove_object2d(obj)

    # POINT LIGHTS

    cpdef void add_point_light(self, PointLight obj):
        Py_INCREF(obj)
        self.c_class.add_point_light(obj.c_class)

    cpdef void remove_point_light(self, PointLight obj):
        self.c_class.remove_point_light(obj.c_class)
        Py_DECREF(obj)

    cpdef void add_point_light_list(self, list[PointLight] objs):
        cdef:
            PointLight obj

        for obj in objs:
            self.add_point_light(obj)

    cpdef void remove_point_light_list(self, list[PointLight] objs):
        cdef:
            PointLight obj

        for obj in objs:
            self.remove_point_light(obj)

    # DIRECTIONAL LIGHTS

    cpdef void add_directional_light(self, DirectionalLight obj):
        Py_INCREF(obj)
        self.c_class.add_directional_light(obj.c_class)

    cpdef void remove_directional_light(self, DirectionalLight obj):
        self.c_class.remove_directional_light(obj.c_class)
        Py_DECREF(obj)

    cpdef void add_directional_light_list(self, list[DirectionalLight] objs):
        cdef:
            DirectionalLight obj

        for obj in objs:
            self.add_directional_light(obj)

    cpdef void remove_directional_light_list(self, list[DirectionalLight] objs):
        cdef:
            DirectionalLight obj

        for obj in objs:
            self.remove_directional_light(obj)

    # SPOT LIGHTS

    cpdef void add_spot_light(self, SpotLight obj):
        Py_INCREF(obj)
        self.c_class.add_spot_light(obj.c_class)

    cpdef void remove_spot_light(self, SpotLight obj):
        self.c_class.remove_spot_light(obj.c_class)
        Py_DECREF(obj)

    cpdef void add_spot_light_list(self, list[SpotLight] objs):
        cdef:
            SpotLight obj

        for obj in objs:
            self.add_spot_light(obj)

    cpdef void remove_spot_light_list(self, list[SpotLight] objs):
        cdef:
            SpotLight obj

        for obj in objs:
            self.remove_spot_light(obj)

    # TEXTS

    cpdef void add_text(self, Text obj):
        Py_INCREF(obj)
        self.c_class.add_text(obj.c_class)

    cpdef void remove_text(self, Text obj):
        self.c_class.remove_text(obj.c_class)
        Py_DECREF(obj)

    cpdef void add_text_list(self, list[Text] objs):
        cdef:
            Text obj

        for obj in objs:
            self.add_text(obj)

    cpdef void remove_text_list(self, list[Text] objs):
        cdef:
            Text obj

        for obj in objs:
            self.remove_text(obj)

    # EMITTER

    cpdef void add_emitter(self, Emitter obj):
        Py_INCREF(obj)
        self.c_class.add_emitter(obj.c_class)

    cpdef void remove_emitter(self, Emitter obj):
        self.c_class.remove_emitter(obj.c_class)
        Py_DECREF(obj)

    cpdef void add_emitter_list(self, list[Emitter] objs):
        cdef:
            Emitter obj

        for obj in objs:
            self.add_emitter(obj)

    cpdef void remove_emitter_list(self, list[Emitter] objs):
        cdef:
            Emitter obj

        for obj in objs:
            self.remove_emitter(obj)

cdef class MouseDevice:
    pass

cdef class MouseWheel:
    pass


cdef class Event:
    cpdef EVENT_STATE get_flag(self, EVENT_FLAG _event):
        return self.c_class.get_flag(_event)

    cpdef bint check_flag(self, EVENT_FLAG _event):
        return self.c_class.check_flag(_event)

    @property
    def mouse(self) -> MouseDevice:
        return self.get_mouse(self.c_class.current_mouse_id)

    cpdef MouseDevice get_mouse(self, int id):
        cdef mouse_device cmdev = self.c_class.get_mouse(id)
        cdef MouseDevice mdev = MouseDevice()
        mdev.button = cmdev.button
        mdev.clicks = cmdev.clicks
        mdev.id = cmdev.id
        mdev.timestamp = cmdev.timestamp
        mdev.x = cmdev.x
        mdev.y = cmdev.y
        mdev.rel_x = cmdev.rel_x
        mdev.rel_y = cmdev.rel_y
        mdev.type = cmdev.type
        mdev.state = cmdev.state
        mdev.wheel = MouseWheel()
        mdev.wheel.direction = cmdev.wheel.direction
        mdev.wheel.x = cmdev.wheel.x
        mdev.wheel.y = cmdev.wheel.y
        mdev.wheel.int_x = cmdev.wheel.int_x
        mdev.wheel.int_y = cmdev.wheel.int_y
        return mdev


cdef class Sprite:
    def __init__(self, str file_path) -> None:
        self.texture = Texture.from_file(file_path, TextureWraping.CLAMP_TO_EDGE, TextureFiltering.LINEAR)
        self.c_class = new sprite(self.texture.c_class)
        
    @classmethod
    def from_texture(cls, Texture tex) -> Sprite:
        return sprite_from_texture(tex)


    def __dealloc__(self):
        del self.c_class

cpdef Sprite sprite_from_texture(Texture tex):
    cdef:
        Sprite ret = Sprite.__new__(Sprite)
    ret.c_class = new sprite(tex.c_class)
    ret.texture = tex
    ret.c_class.tex = tex.c_class
    return ret

ctypedef object2d* obj2d_ptr

cdef class Object2D:
    def __init__(self, Sprite sprite, Camera camera, Vec2 position = Vec2(0.0,0.0), float depth = 0.0,
    float rotation = 0.0, Vec2 scale = Vec2(1.0, 1.0),
    Material material = None) -> None:
        self._position = position
        self._scale = scale
        self.sprite = sprite
        self._camera = camera
        
        
        if material:
            self.material = material
            
        else:
            self.material = Material(
                Shader.from_file(path.join(path.dirname(__file__), "default_vertex_2D.glsl"), ShaderType.VERTEX),
                Shader.from_file(path.join(path.dirname(__file__), "default_fragment_2D.glsl"), ShaderType.FRAGMENT)
            )
        
        self.c_class = new object2d(sprite.c_class, self._camera.c_class, position.c_class, rotation, scale.c_class, self.material.c_class, depth)

    @property
    def position(self) -> Vec2:
        return self._position

    @position.setter
    def position(self, Vec2 value):
        self._position.c_class[0] = value.c_class[0]

    @property
    def depth(self) -> float:
        return self.c_class.depth

    @depth.setter
    def depth(self, float value):
        self.c_class.depth = value

    @property
    def rotation(self) -> float:
        return self.c_class.rotation

    @rotation.setter
    def rotation(self, value: Vec2 | float):
        if isinstance(value, Vec2):
            self.c_class.rotation = value.to_angle()
        elif isinstance(value, float):
            self.c_class.rotation = value

    @property
    def scale(self) -> Vec2:
        return self._scale

    @scale.setter
    def scale(self, Vec2 value):
        self._scale.c_class[0] = value.c_class[0]

    @property
    def untransformed_dimensions(self) -> Vec2:
        return vec2_from_cpp(self.c_class.unscaled_dim)

    @property
    def dimensions(self) -> Vec2:
        return vec2_from_cpp(self.c_class.get_scaled_dimensions())

    @property
    def width(self) -> float:
        return self.c_class.get_scaled_dimensions().axis.x

    @property
    def height(self) -> float:
        return self.c_class.get_scaled_dimensions().axis.y

    def __dealloc__(self):
        del self.c_class

    

    cpdef void set_uniform(self, str name, value:UniformValueType):
        _set_uniform(self, name, value)

cdef class PointLight:
    def __init__(self, Vec3 position, float radius, Vec3 color, float intensity = 1.0) -> None:
        self._position = position
        self._color = color
        self.c_class = new point_light(self._position.c_class, radius, self._color.c_class, intensity)
    
    @property
    def position(self) -> Vec3:
        return self._position

    @position.setter
    def position(self, Vec3 value):
        self._position.c_class[0] = value.c_class[0]

    @property
    def intensity(self) -> float:
        return self.c_class.intensity

    @intensity.setter
    def intensity(self, float value):
        self.c_class.intensity = value

    @property
    def radius(self) -> float:
        return self.c_class.radius

    @radius.setter
    def radius(self, float value):
        self.c_class.radius = value

    @property
    def color(self) -> Vec3:
        return self._color

    @color.setter
    def color(self, Vec3 value):
        self._color.c_class[0] = value.c_class[0]

    def __dealloc__(self):
        del self.c_class

cdef class DirectionalLight:
    def __init__(self, Vec3 rotation = None, Vec3 color = None, Vec3 ambient = None,
    Vec3 diffuse = None, Vec3 specular = None, float intensity = 1.0) -> None:
        self._rotation = rotation.to_quaternion() if rotation else Vec3(0.0,0.0,0.0).to_quaternion()
        self._color = color if color else Vec3(0.333,0.333,0.333)
        self._ambient = ambient if ambient else Vec3(0.0,0.0,0.0)
        self._diffuse = diffuse if diffuse else Vec3(0.0,0.0,0.0)
        self._specular = specular if specular else Vec3(0.0,0.0,0.0)
        self.c_class = new directional_light(self._rotation.c_class, self._color.c_class, self._ambient.c_class, self._diffuse.c_class, self._specular.c_class, intensity)
    
    @property
    def rotation(self) -> Quaternion:
        return self._rotation

    @rotation.setter
    def rotation(self, Quaternion value):
        self._rotation.c_class[0] = value.c_class[0]

    @property
    def ambient(self) -> Vec3:
        return self._ambient

    @ambient.setter
    def ambient(self, Vec3 value):
        self._ambient.c_class[0] = value.c_class[0]

    @property
    def diffuse(self) -> Vec3:
        return self._diffuse

    @diffuse.setter
    def diffuse(self, Vec3 value):
        self._diffuse.c_class[0] = value.c_class[0]

    @property
    def specular(self) -> Vec3:
        return self._specular

    @specular.setter
    def specular(self, Vec3 value):
        self._specular.c_class[0] = value.c_class[0]

    @property
    def intensity(self) -> float:
        return self.c_class.intensity

    @intensity.setter
    def intensity(self, float value):
        self.c_class.intensity = value

    @property
    def color(self) -> Vec3:
        return self._color

    @color.setter
    def color(self, Vec3 value):
        self._color.c_class[0] = value.c_class[0]

    def __dealloc__(self):
        del self.c_class


cdef class SpotLight:
    def __init__(self, Vec3 position, Vec3 rotation = None, Vec3 color = None, float cutoff = 0.2181662, float outer_cutoff = 0.3054326, float intensity = 1.0, float reach = 100.0, Texture cookie = None) -> None:
        self._position = position
        self._rotation = rotation.to_quaternion() if rotation else Vec3(0.0,0.0,0.0).to_quaternion()
        self._color = color if color else Vec3(0.333,0.333,0.333)
        self._cookie = cookie
        if cookie:
            self.c_class = new spot_light(self._position.c_class, self._rotation.c_class, self._color.c_class, cutoff, outer_cutoff, intensity, reach, True, self._cookie.c_class)
        else:
            self.c_class = new spot_light(self._position.c_class, self._rotation.c_class, self._color.c_class, cutoff, outer_cutoff, intensity, reach, False)
    
    @property
    def rotation(self) -> Quaternion:
        return self._rotation

    @rotation.setter
    def rotation(self, Quaternion value):
        self._rotation.c_class[0] = value.c_class[0]

    @property
    def cookie(self) -> Texture:
        return self._cookie

    @cookie.setter
    def cookie(self, Texture value):
        self._cookie.c_class[0] = value.c_class[0]

    @property
    def position(self) -> Vec3:
        return self._position

    @position.setter
    def position(self, Vec3 value):
        self._position.c_class[0] = value.c_class[0]

    @property
    def color(self) -> Vec3:
        return self._color

    @color.setter
    def color(self, Vec3 value):
        self._color.c_class[0] = value.c_class[0]

    @property
    def intensity(self) -> float:
        return self.c_class.intensity

    @intensity.setter
    def intensity(self, float value):
        self.c_class.intensity = value

    @property
    def cutoff(self) -> float:
        return self.c_class.cutOff

    @cutoff.setter
    def cutoff(self, float value):
        self.c_class.cutOff = value

    @property
    def outer_cutoff(self) -> float:
        return self.c_class.outerCutOff

    @outer_cutoff.setter
    def outer_cutoff(self, float value):
        self.c_class.outerCutOff = value

    @property
    def reach(self) -> float:
        return self.c_class.reach

    @reach.setter
    def reach(self, float value):
        self.c_class.reach = value

    def __dealloc__(self):
        del self.c_class

cdef class Collider:
    def check_collision(self, intersection: Vec3 | Collider | Object3D) -> bool:
        cdef:
            Vec3 argvec
            Collider argcol
            Object3D argobj
        if isinstance(intersection, Vec3):
            argvec = intersection
            return self.c_class.data.check_collision(argvec.c_class[0])
        elif isinstance(intersection, Collider):
            argcol = intersection
            return self.c_class.data.check_collision(argcol.c_class.data)
        elif isinstance(intersection, Object3D):
            argobj = intersection
            return self.c_class.data.check_collision(argobj.c_class)
        
        return False

    @property
    def show(self):
        return self.c_class.data.show_collider

    @show.setter
    def show(self, bint value):
        self.c_class.data.show_collider = value

    @property
    def rotation(self) -> Quaternion:
        return self._rotation

    @rotation.setter
    def rotation(self, Quaternion value) -> None:
        self._rotation.c_class[0] = value.c_class[0]

    @property
    def scale(self) -> Vec3:
        return self._scale

    @scale.setter
    def scale(self, Vec3 value) -> None:
        self._scale.c_class[0] = value.c_class[0]

    @property
    def offset(self) -> Vec3:
        return self._offset

    @offset.setter
    def offset(self, Vec3 value) -> None:
        self._offset.c_class[0] = value.c_class[0]

    cpdef void dbg_render(self, Camera cam):
        self.c_class.data.dbg_render(cam.c_class[0])

ctypedef collider* collider_ptr

cdef class BoxCollider(Collider):
    def __init__(self, Object3D object, Vec3 offset = Vec3(0,0,0), rotation: Vec3 | Quaternion = Vec3(0,0,0), Vec3 scale = Vec3(1.0,1.0,1.0)) -> None:
        self._offset = offset
        self._scale = scale
        if isinstance(rotation, Vec3):
            self._rotation = rotation.to_quaternion()
        elif isinstance(rotation, Quaternion):
            self._rotation = rotation

        self.c_class = new RC[collider_ptr](new collider_box(object.c_class, self._offset.c_class, self._rotation.c_class, self._scale.c_class))

    @classmethod
    def from_bounds(cls, Vec3 upper_bound = Vec3(10.0,10.0,10.0), Vec3 lower_bound = Vec3(-10.0,-10.0,-10.0), Vec3 offset = Vec3(0,0,0), rotation: Vec3 | Quaternion = Vec3(0,0,0), Vec3 scale = Vec3(1.0,1.0,1.0)) -> BoxCollider:
        cdef:
            BoxCollider ret = BoxCollider.__new__(BoxCollider)
        ret._offset = offset
        ret._scale = scale
        if isinstance(rotation, Vec3):
            ret._rotation = rotation.to_quaternion()
        elif isinstance(rotation, Quaternion):
            ret._rotation = rotation
        ret.c_class = new RC[collider_ptr](new collider_box(upper_bound.c_class[0], lower_bound.c_class[0], ret._offset.c_class, ret._rotation.c_class, ret._scale.c_class))
        return ret

    def __dealloc__(self):
        RC_collect(self.c_class)

cdef class ConvexCollider(Collider):
    def __init__(self, Object3D object, Vec3 offset = Vec3(0,0,0), rotation: Vec3 | Quaternion = Vec3(0,0,0), Vec3 scale = Vec3(1.0,1.0,1.0)) -> None:
        self._offset = offset
        self._scale = scale
        if isinstance(rotation, Vec3):
            self._rotation = rotation.to_quaternion()
        elif isinstance(rotation, Quaternion):
            self._rotation = rotation
        self.c_class = new RC[collider_ptr](new collider_convex(object.c_class, self._offset.c_class, self._rotation.c_class, self._scale.c_class))

    @classmethod
    def from_mesh(cls, Mesh msh, Vec3 offset = Vec3(0,0,0), rotation: Vec3 | Quaternion = Vec3(0,0,0), Vec3 scale = Vec3(1.0,1.0,1.0)) -> BoxCollider:
        cdef:
            BoxCollider ret = BoxCollider.__new__(BoxCollider)
        ret._offset = offset
        ret._scale = scale
        if isinstance(rotation, Vec3):
            ret._rotation = rotation.to_quaternion()
        elif isinstance(rotation, Quaternion):
            ret._rotation = rotation
        ret.c_class = new RC[collider_ptr](new collider_convex(msh.c_class, ret._offset.c_class, ret._rotation.c_class, ret._scale.c_class))
        return ret

    @classmethod
    def from_mesh_dict(cls, MeshDict msh_dict, Vec3 offset = Vec3(0,0,0), rotation: Vec3 | Quaternion = Vec3(0,0,0), Vec3 scale = Vec3(1.0,1.0,1.0)) -> BoxCollider:
        cdef:
            BoxCollider ret = BoxCollider.__new__(BoxCollider)
        ret._offset = offset
        ret._scale = scale
        if isinstance(rotation, Vec3):
            ret._rotation = rotation.to_quaternion()
        elif isinstance(rotation, Quaternion):
            ret._rotation = rotation
        ret.c_class = new RC[collider_ptr](new collider_convex(msh_dict.c_class, ret._offset.c_class, ret._rotation.c_class, ret._scale.c_class))
        return ret

    def __dealloc__(self):
        RC_collect(self.c_class)

cdef class RayCollider(Collider):

    def __init__(self, Vec3 origin, Quaternion direction) -> None:
        self._origin = origin
        self._direction = direction
        self.c_class = new RC[collider_ptr](new collider_ray(self._origin.c_class, self._direction.c_class))

    def get_collision(self, intersection: Collider | Object3D) -> RayHit:
        cdef:
            Collider argcol
            Object3D argobj
        if isinstance(intersection, Collider):
            argcol = intersection
            return RayHit.from_cpp((<collider_ray*>self.c_class.data).get_collision(argcol.c_class.data))
        elif isinstance(intersection, Object3D):
            argobj = intersection
            return RayHit.from_cpp((<collider_ray*>self.c_class.data).get_collision(argobj.c_class))
        
        return RayHit.from_cpp(ray_hit(False))

    @property
    def rotation(self) -> Quaternion:
        return self._direction

    @rotation.setter
    def rotation(self, Quaternion value) -> None:
        self._direction.c_class[0] = value.c_class[0]

    @property
    def direction(self) -> Quaternion:
        return self._direction

    @direction.setter
    def direction(self, Quaternion value) -> None:
        self._direction.c_class[0] = value.c_class[0]

    @property
    def offset(self) -> Vec3:
        return self._origin

    @offset.setter
    def offset(self, Vec3 value) -> None:
        self._origin.c_class[0] = value.c_class[0]

    @property
    def origin(self) -> Vec3:
        return self._origin

    @origin.setter
    def origin(self, Vec3 value) -> None:
        self._origin.c_class[0] = value.c_class[0]

    def __dealloc__(self):
        RC_collect(self.c_class)

cdef class RayHit:
    @staticmethod
    cdef RayHit from_cpp(ray_hit hit):
        cdef:
            RayHit ret = RayHit.__new__(RayHit)
        ret.c_class = new ray_hit(hit)
        return ret

    @property
    def hit(self) -> bool:
        return self.c_class.hit
    
    @property
    def has_normal(self) -> bool:
        return self.c_class.has_normal

    @property
    def has_distance(self) -> bool:
        return self.c_class.has_distance

    @property
    def position(self) -> Vec3:
        return vec3_from_cpp(self.c_class.position)

    @property
    def normal(self) -> Vec3:
        return vec3_from_cpp(self.c_class.normal)

    @property
    def distance(self) -> float:
        return self.c_class.distance

    def __dealloc__(self):
        del self.c_class


# MATRICES ------------------------------------------------------------------------------------

# MAT4x4

cdef class Matrix4x4:

    def __init__(self, float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3) -> None:
        self.c_class = new matrix[glmmat4x4](x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3)

    @staticmethod
    def from_identity(float value) -> Matrix4x4:
        cdef Matrix4x4 ret = Matrix4x4.__new__(Matrix4x4)
        ret.c_class = new matrix[glmmat4x4](value)
        return ret

    @staticmethod
    def from_ortho(float left, float right, float bottom, float top, zNear:float | None = None, zFar:float | None = None) -> Matrix4x4:
        cdef Matrix4x4 ret = Matrix4x4.__new__(Matrix4x4)
        if zNear != None and zFar != None:
            ret.c_class = new matrix[glmmat4x4](matrix[glmmat4x4].from_ortho(left, right, bottom, top, zNear, zFar))
        else:
            ret.c_class = new matrix[glmmat4x4](matrix[glmmat4x4].from_ortho(left, right, bottom, top))
        return ret

    @staticmethod
    def look_at(Vec3 eye, Vec3 center, Vec3 up) -> Matrix4x4:
        cdef Matrix4x4 ret = Matrix4x4.__new__(Matrix4x4)
        ret.c_class = new matrix[glmmat4x4](matrix[glmmat4x4].look_at(eye.c_class[0], center.c_class[0], up.c_class[0]))
        return ret

    @staticmethod
    def from_perspective(float fovy, float aspect, float near, float far) -> Matrix4x4:
        cdef Matrix4x4 ret = Matrix4x4.__new__(Matrix4x4)
        ret.c_class = new matrix[glmmat4x4](matrix[glmmat4x4].from_perspective(fovy, aspect, near, far))
        return ret

    @staticmethod
    def from_quaternion(Quaternion quat) -> Matrix4x4:
        cdef Matrix4x4 ret = Matrix4x4.__new__(Matrix4x4)
        ret.c_class = new matrix[glmmat4x4](quat.c_class)
        return ret

    cpdef Quaternion to_quaternion(self):
        return quat_from_cpp(self.c_class.to_quaternion())

    cpdef Vec3 get_up(self):
        return vec3_from_cpp(self.c_class.get_up())
    
    cpdef Vec3 get_right(self):
        return vec3_from_cpp(self.c_class.get_right())

    cpdef Vec3 get_forward(self):
        return vec3_from_cpp(self.c_class.get_forward())

    cpdef Matrix4x4 inverse(self):
        return mat4x4_from_cpp(self.c_class.inverse())

    cpdef Matrix4x4 transpose(self):
        return mat4x4_from_cpp(self.c_class.transpose())

    cpdef float determinant(self):
        return self.c_class.determinant()

    def __dealloc__(self):
        del self.c_class

    def __getitem__(self, int index) -> Vec4:
        return vec4_from_cpp(self.c_class.get_vec4(index))
    
    def __neg__(self) -> Matrix4x4:
        return mat4x4_from_cpp(-self.c_class[0])

    def __sub__(self, other:Matrix4x4|float) -> Matrix4x4:
        cdef:
            Matrix4x4 o1
            float o2
        if isinstance(other, Matrix4x4):
            o1 = other
            return mat4x4_from_cpp(self.c_class[0] - o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat4x4_from_cpp(self.c_class[0] - o2)

    def __add__(self,  other:Matrix4x4|float) -> Matrix4x4:
        cdef:
            Matrix4x4 o1
            float o2
        if isinstance(other, Matrix4x4):
            o1 = other
            return mat4x4_from_cpp(self.c_class[0] + o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat4x4_from_cpp(self.c_class[0] + o2)

    def __mul__(self, other: Matrix4x4|float|Vec4) -> Matrix4x4|Vec4:
        cdef:
            Matrix4x4 o1
            float o2
            Vec4 o3
        if isinstance(other, Matrix4x4):
            o1 = other
            return mat4x4_from_cpp(self.c_class[0] * o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat4x4_from_cpp(self.c_class[0] * o2)
        elif isinstance(other, Vec4):
            o3 = other
            return vec4_from_cpp(self.c_class[0] * o3.c_class[0])

    def __truediv__(self, other:Matrix4x4|float) -> Matrix4x4:
        cdef:
            Matrix4x4 o1
            float o2
        if isinstance(other, Matrix4x4):
            o1 = other
            return mat4x4_from_cpp(self.c_class[0] / o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat4x4_from_cpp(self.c_class[0] / o2)

cdef Matrix4x4 mat4x4_from_cpp(matrix[glmmat4x4] cppinst):
    cdef Matrix4x4 ret = Matrix4x4.__new__(Matrix4x4)
    ret.c_class = new matrix[glmmat4x4](cppinst)
    return ret



# MAT3x4

cdef class Matrix3x4:

    def __init__(self, float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) -> None:
        self.c_class = new matrix[glmmat3x4](x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2)

    @staticmethod
    def from_identity(float value) -> Matrix3x4:
        cdef Matrix3x4 ret = Matrix3x4.__new__(Matrix3x4)
        ret.c_class = new matrix[glmmat3x4](value)
        return ret

    cpdef Matrix4x3 transpose(self):
        return mat4x3_from_cpp(matrix[glmmat4x3](self.c_class.transpose3x4()))

    def __dealloc__(self):
        del self.c_class

    def __getitem__(self, int index) -> Vec4:
        return vec4_from_cpp(self.c_class.get_vec4(index))
    
    def __neg__(self) -> Matrix3x4:
        return mat3x4_from_cpp(-self.c_class[0])

    def __sub__(self, other:Matrix3x4|float) -> Matrix3x4:
        cdef:
            Matrix3x4 o1
            float o2
        if isinstance(other, Matrix3x4):
            o1 = other
            return mat3x4_from_cpp(self.c_class[0] - o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat3x4_from_cpp(self.c_class[0] - o2)

    def __add__(self,  other:Matrix3x4|float) -> Matrix3x4:
        cdef:
            Matrix3x4 o1
            float o2
        if isinstance(other, Matrix3x4):
            o1 = other
            return mat3x4_from_cpp(self.c_class[0] + o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat3x4_from_cpp(self.c_class[0] + o2)

    def __mul__(self, other: float|Vec4) -> Matrix3x4|Vec4:
        cdef:
            float o2
            Vec4 o3
        if isinstance(other, float):
            o2 = other
            return mat3x4_from_cpp(self.c_class[0] * o2)
        elif isinstance(other, Vec4):
            o3 = other
            return vec4_from_cpp(self.c_class[0] * o3.c_class[0])

    # end vec mul

    def __truediv__(self, float other) -> Matrix3x4:
        return mat3x4_from_cpp(self.c_class[0] / other)

cdef Matrix3x4 mat3x4_from_cpp(matrix[glmmat3x4] cppinst):
    cdef Matrix3x4 ret = Matrix3x4.__new__(Matrix3x4)
    ret.c_class = new matrix[glmmat3x4](cppinst)
    return ret

# MAT2x4

cdef class Matrix2x4:

    def __init__(self, float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1) -> None:
        self.c_class = new matrix[glmmat2x4](x0, y0, z0, w0, x1, y1, z1, w1)

    cpdef Matrix4x2 transpose(self):
        return mat4x2_from_cpp(matrix[glmmat4x2](self.c_class.transpose2x4()))

    @staticmethod
    def from_identity(float value) -> Matrix2x4:
        cdef Matrix2x4 ret = Matrix2x4.__new__(Matrix2x4)
        ret.c_class = new matrix[glmmat2x4](value)
        return ret

    def __dealloc__(self):
        del self.c_class

    def __getitem__(self, int index) -> Vec4:
        return vec4_from_cpp(self.c_class.get_vec4(index))
    
    def __neg__(self) -> Matrix2x4:
        return mat2x4_from_cpp(-self.c_class[0])

    def __sub__(self, other:Matrix2x4|float) -> Matrix2x4:
        cdef:
            Matrix2x4 o1
            float o2
        if isinstance(other, Matrix2x4):
            o1 = other
            return mat2x4_from_cpp(self.c_class[0] - o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat2x4_from_cpp(self.c_class[0] - o2)

    def __add__(self,  other:Matrix2x4|float) -> Matrix2x4:
        cdef:
            Matrix2x4 o1
            float o2
        if isinstance(other, Matrix2x4):
            o1 = other
            return mat2x4_from_cpp(self.c_class[0] + o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat2x4_from_cpp(self.c_class[0] + o2)

    def __mul__(self, other: float|Vec4) -> Matrix2x4|Vec4:
        cdef:
            float o2
            Vec4 o3
        if isinstance(other, float):
            o2 = other
            return mat2x4_from_cpp(self.c_class[0] * o2)
        elif isinstance(other, Vec4):
            o3 = other
            return vec4_from_cpp(self.c_class[0] * o3.c_class[0])

    def __truediv__(self, float other) -> Matrix2x4:
        return mat2x4_from_cpp(self.c_class[0] / other)

cdef Matrix2x4 mat2x4_from_cpp(matrix[glmmat2x4] cppinst):
    cdef Matrix2x4 ret = Matrix2x4.__new__(Matrix2x4)
    ret.c_class = new matrix[glmmat2x4](cppinst)
    return ret

# MAT3x3

cdef class Matrix3x3:

    def __init__(self, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2) -> None:
        self.c_class = new matrix[glmmat3x3](x0, y0, z0, x1, y1, z1, x2, y2, z2)

    @staticmethod
    def from_identity(float value) -> Matrix3x3:
        cdef Matrix3x3 ret = Matrix3x3.__new__(Matrix3x3)
        ret.c_class = new matrix[glmmat3x3](value)
        return ret

    cpdef Quaternion to_quaternion(self):
        return quat_from_cpp(self.c_class.to_quaternion())

    cpdef Vec3 get_up(self):
        return vec3_from_cpp(self.c_class.get_up())
    
    cpdef Vec3 get_right(self):
        return vec3_from_cpp(self.c_class.get_right())

    cpdef Vec3 get_forward(self):
        return vec3_from_cpp(self.c_class.get_forward())

    cpdef Matrix3x3 inverse(self):
        return mat3x3_from_cpp(self.c_class.inverse())

    cpdef Matrix3x3 transpose(self):
        return mat3x3_from_cpp(self.c_class.transpose())

    cpdef float determinant(self):
        return self.c_class.determinant()

    def __dealloc__(self):
        del self.c_class

    def __getitem__(self, int index) -> Vec3:
        return vec3_from_cpp(self.c_class.get_vec3(index))
    
    def __neg__(self) -> Matrix3x3:
        return mat3x3_from_cpp(-self.c_class[0])

    def __sub__(self, other:Matrix3x3|float) -> Matrix3x3:
        cdef:
            Matrix3x3 o1
            float o2
        if isinstance(other, Matrix3x3):
            o1 = other
            return mat3x3_from_cpp(self.c_class[0] - o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat3x3_from_cpp(self.c_class[0] - o2)

    def __add__(self,  other:Matrix3x3|float) -> Matrix3x3:
        cdef:
            Matrix3x3 o1
            float o2
        if isinstance(other, Matrix3x3):
            o1 = other
            return mat3x3_from_cpp(self.c_class[0] + o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat3x3_from_cpp(self.c_class[0] + o2)

    def __mul__(self, other: Matrix3x3|float|Vec3) -> Matrix3x3|Vec3:
        cdef:
            Matrix3x3 o1
            float o2
            Vec3 o3
        if isinstance(other, Matrix3x3):
            o1 = other
            return mat3x3_from_cpp(self.c_class[0] * o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat3x3_from_cpp(self.c_class[0] * o2)
        elif isinstance(other, Vec3):
            o3 = other
            return vec3_from_cpp(self.c_class[0] * o3.c_class[0])

    def __truediv__(self, other:Matrix3x3|float) -> Matrix3x3:
        cdef:
            Matrix3x3 o1
            float o2
        if isinstance(other, Matrix3x3):
            o1 = other
            return mat3x3_from_cpp(self.c_class[0] / o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat3x3_from_cpp(self.c_class[0] / o2)

cdef Matrix3x3 mat3x3_from_cpp(matrix[glmmat3x3] cppinst):
    cdef Matrix3x3 ret = Matrix3x3.__new__(Matrix3x3)
    ret.c_class = new matrix[glmmat3x3](cppinst)
    return ret

# MAT4x3

cdef class Matrix4x3:

    def __init__(self, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) -> None:
        self.c_class = new matrix[glmmat4x3](x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3)

    cpdef Matrix3x4 transpose(self):
        return mat3x4_from_cpp(matrix[glmmat3x4](self.c_class.transpose4x3()))

    @staticmethod
    def from_identity(float value) -> Matrix4x3:
        cdef Matrix4x3 ret = Matrix4x3.__new__(Matrix4x3)
        ret.c_class = new matrix[glmmat4x3](value)
        return ret

    def __dealloc__(self):
        del self.c_class

    def __getitem__(self, int index) -> Vec3:
        return vec3_from_cpp(self.c_class.get_vec3(index))
    
    def __neg__(self) -> Matrix4x3:
        return mat4x3_from_cpp(-self.c_class[0])

    def __sub__(self, other:Matrix4x3|float) -> Matrix4x3:
        cdef:
            Matrix4x3 o1
            float o2
        if isinstance(other, Matrix4x3):
            o1 = other
            return mat4x3_from_cpp(self.c_class[0] - o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat4x3_from_cpp(self.c_class[0] - o2)

    def __add__(self,  other:Matrix4x3|float) -> Matrix4x3:
        cdef:
            Matrix4x3 o1
            float o2
        if isinstance(other, Matrix4x3):
            o1 = other
            return mat4x3_from_cpp(self.c_class[0] + o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat4x3_from_cpp(self.c_class[0] + o2)

    def __mul__(self, other: float|Vec4) -> Matrix4x3|Vec3:
        cdef:
            float o2
            Vec4 o3
        if isinstance(other, float):
            o2 = other
            return mat4x3_from_cpp(self.c_class[0] * o2)
        elif isinstance(other, Vec4):
            o3 = other
            return vec3_from_cpp(self.c_class.mul_4x3(o3.c_class[0]))

    def __truediv__(self, float other) -> Matrix4x3:
        return mat4x3_from_cpp(self.c_class[0] / other)

cdef Matrix4x3 mat4x3_from_cpp(matrix[glmmat4x3] cppinst):
    cdef Matrix4x3 ret = Matrix4x3.__new__(Matrix4x3)
    ret.c_class = new matrix[glmmat4x3](cppinst)
    return ret

# MAT2x3

cdef class Matrix2x3:

    def __init__(self, float x0, float y0, float z0, float x1, float y1, float z1) -> None:
        self.c_class = new matrix[glmmat2x3](x0, y0, z0, x1, y1, z1)

    cpdef Matrix3x2 transpose(self):
        return mat3x2_from_cpp(matrix[glmmat3x2](self.c_class.transpose2x3()))

    @staticmethod
    def from_identity(float value) -> Matrix2x3:
        cdef Matrix2x3 ret = Matrix2x3.__new__(Matrix2x3)
        ret.c_class = new matrix[glmmat2x3](value)
        return ret

    def __dealloc__(self):
        del self.c_class

    def __getitem__(self, int index) -> Vec3:
        return vec3_from_cpp(self.c_class.get_vec3(index))
    
    def __neg__(self) -> Matrix2x3:
        return mat2x3_from_cpp(-self.c_class[0])

    def __sub__(self, other:Matrix2x3|float) -> Matrix2x3:
        cdef:
            Matrix2x3 o1
            float o2
        if isinstance(other, Matrix2x3):
            o1 = other
            return mat2x3_from_cpp(self.c_class[0] - o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat2x3_from_cpp(self.c_class[0] - o2)

    def __add__(self,  other:Matrix2x3|float) -> Matrix2x3:
        cdef:
            Matrix2x3 o1
            float o2
        if isinstance(other, Matrix2x3):
            o1 = other
            return mat2x3_from_cpp(self.c_class[0] + o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat2x3_from_cpp(self.c_class[0] + o2)

    def __mul__(self, other: float|Vec2|Vec3) -> Matrix2x3|Vec3:
        cdef:
            float o2
            Vec2 o3
            Vec3 o4
        if isinstance(other, float):
            o2 = other
            return mat2x3_from_cpp(self.c_class[0] * o2)
        elif isinstance(other, Vec2):
            o3 = other
            return vec3_from_cpp(self.c_class.mul_2x3(o3.c_class[0]))
        elif isinstance(other, Vec3):
            o4 = other
            return vec3_from_cpp(self.c_class[0] * o4.c_class[0])

    def __truediv__(self, float other) -> Matrix2x3:
        return mat2x3_from_cpp(self.c_class[0] / other)

cdef Matrix2x3 mat2x3_from_cpp(matrix[glmmat2x3] cppinst):
    cdef Matrix2x3 ret = Matrix2x3.__new__(Matrix2x3)
    ret.c_class = new matrix[glmmat2x3](cppinst)
    return ret

# MAT2x2

cdef class Matrix2x2:

    def __init__(self, float x0, float y0, float x1, float y1) -> None:
        self.c_class = new matrix[glmmat2x2](x0, y0, x1, y1)

    @staticmethod
    def from_identity(float value) -> Matrix2x2:
        cdef Matrix2x2 ret = Matrix2x2.__new__(Matrix2x2)
        ret.c_class = new matrix[glmmat2x2](value)
        return ret

    cpdef Matrix2x2 inverse(self):
        return mat2x2_from_cpp(self.c_class.inverse())

    cpdef Matrix2x2 transpose(self):
        return mat2x2_from_cpp(self.c_class.transpose())

    cpdef float determinant(self):
        return self.c_class.determinant()

    def __dealloc__(self):
        del self.c_class

    def __getitem__(self, int index) -> Vec2:
        return vec2_from_cpp(self.c_class.get_vec2(index))
    
    def __neg__(self) -> Matrix2x2:
        return mat2x2_from_cpp(-self.c_class[0])

    def __sub__(self, other:Matrix2x2|float) -> Matrix2x2:
        cdef:
            Matrix2x2 o1
            float o2
        if isinstance(other, Matrix2x2):
            o1 = other
            return mat2x2_from_cpp(self.c_class[0] - o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat2x2_from_cpp(self.c_class[0] - o2)

    def __add__(self,  other:Matrix2x2|float) -> Matrix2x2:
        cdef:
            Matrix2x2 o1
            float o2
        if isinstance(other, Matrix2x2):
            o1 = other
            return mat2x2_from_cpp(self.c_class[0] + o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat2x2_from_cpp(self.c_class[0] + o2)

    def __mul__(self, other: Matrix2x2|float|Vec2) -> Matrix2x2|Vec2:
        cdef:
            Matrix2x2 o1
            float o2
            Vec2 o3
        if isinstance(other, Matrix2x2):
            o1 = other
            return mat2x2_from_cpp(self.c_class[0] * o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat2x2_from_cpp(self.c_class[0] * o2)
        elif isinstance(other, Vec2):
            o3 = other
            return vec2_from_cpp(self.c_class[0] * o3.c_class[0])

    def __truediv__(self, other:Matrix2x2|float) -> Matrix2x2:
        cdef:
            Matrix2x2 o1
            float o2
        if isinstance(other, Matrix2x2):
            o1 = other
            return mat2x2_from_cpp(self.c_class[0] / o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat2x2_from_cpp(self.c_class[0] / o2)

cdef Matrix2x2 mat2x2_from_cpp(matrix[glmmat2x2] cppinst):
    cdef Matrix2x2 ret = Matrix2x2.__new__(Matrix2x2)
    ret.c_class = new matrix[glmmat2x2](cppinst)
    return ret

# MAT3x2

cdef class Matrix3x2:

    def __init__(self, float x0, float y0, float x1, float y1, float x2, float y2) -> None:
        self.c_class = new matrix[glmmat3x2](x0, y0, x1, y1, x2, y2)

    cpdef Matrix2x3 transpose(self):
        return mat2x3_from_cpp(matrix[glmmat2x3](self.c_class.transpose3x2()))

    @staticmethod
    def from_identity(float value) -> Matrix3x2:
        cdef Matrix3x2 ret = Matrix3x2.__new__(Matrix3x2)
        ret.c_class = new matrix[glmmat3x2](value)
        return ret

    def __dealloc__(self):
        del self.c_class

    def __getitem__(self, int index) -> Vec2:
        return vec2_from_cpp(self.c_class.get_vec2(index))
    
    def __neg__(self) -> Matrix3x2:
        return mat3x2_from_cpp(-self.c_class[0])

    def __sub__(self, other:Matrix3x2|float) -> Matrix3x2:
        cdef:
            Matrix3x2 o1
            float o2
        if isinstance(other, Matrix3x2):
            o1 = other
            return mat3x2_from_cpp(self.c_class[0] - o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat3x2_from_cpp(self.c_class[0] - o2)

    def __add__(self,  other:Matrix3x2|float) -> Matrix3x2:
        cdef:
            Matrix3x2 o1
            float o2
        if isinstance(other, Matrix3x2):
            o1 = other
            return mat3x2_from_cpp(self.c_class[0] + o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat3x2_from_cpp(self.c_class[0] + o2)

    def __mul__(self, other: float|Vec3) -> Matrix3x2|Vec2:
        cdef:
            float o2
            Vec3 o3
        if isinstance(other, float):
            o2 = other
            return mat3x2_from_cpp(self.c_class[0] * o2)
        elif isinstance(other, Vec3):
            o3 = other
            return vec2_from_cpp(self.c_class.mul_3x2(o3.c_class[0]))

    def __truediv__(self, float other) -> Matrix3x2:
        return mat3x2_from_cpp(self.c_class[0] / other)

cdef Matrix3x2 mat3x2_from_cpp(matrix[glmmat3x2] cppinst):
    cdef Matrix3x2 ret = Matrix3x2.__new__(Matrix3x2)
    ret.c_class = new matrix[glmmat3x2](cppinst)
    return ret

# MAT4x2

cdef class Matrix4x2:

    def __init__(self, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) -> None:
        self.c_class = new matrix[glmmat4x2](x0, y0, x1, y1, x2, y2, x3, y3)

    cpdef Matrix2x4 transpose(self):
        return mat2x4_from_cpp(matrix[glmmat2x4](self.c_class.transpose4x2()))

    @staticmethod
    def from_identity(float value) -> Matrix4x2:
        cdef Matrix4x2 ret = Matrix4x2.__new__(Matrix4x2)
        ret.c_class = new matrix[glmmat4x2](value)
        return ret

    def __dealloc__(self):
        del self.c_class

    def __getitem__(self, int index) -> Vec2:
        return vec2_from_cpp(self.c_class.get_vec2(index))
    
    def __neg__(self) -> Matrix4x2:
        return mat4x2_from_cpp(-self.c_class[0])

    def __sub__(self, other:Matrix4x2|float) -> Matrix4x2:
        cdef:
            Matrix4x2 o1
            float o2
        if isinstance(other, Matrix4x2):
            o1 = other
            return mat4x2_from_cpp(self.c_class[0] - o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat4x2_from_cpp(self.c_class[0] - o2)

    def __add__(self,  other:Matrix4x2|float) -> Matrix4x2:
        cdef:
            Matrix4x2 o1
            float o2
        if isinstance(other, Matrix4x2):
            o1 = other
            return mat4x2_from_cpp(self.c_class[0] + o1.c_class[0])
        elif isinstance(other, float):
            o2 = other
            return mat4x2_from_cpp(self.c_class[0] + o2)

    def __mul__(self, other: float|Vec4) -> Matrix4x2|Vec2:
        cdef:
            float o2
            Vec4 o3
        if isinstance(other, float):
            o2 = other
            return mat4x2_from_cpp(self.c_class[0] * o2)
        elif isinstance(other, Vec4):
            o3 = other
            return vec2_from_cpp(self.c_class.mul_4x2(o3.c_class[0]))

    def __truediv__(self, float other) -> Matrix4x2:
        return mat4x2_from_cpp(self.c_class[0] / other)

cdef Matrix4x2 mat4x2_from_cpp(matrix[glmmat4x2] cppinst):
    cdef Matrix4x2 ret = Matrix4x2.__new__(Matrix4x2)
    ret.c_class = new matrix[glmmat4x2](cppinst)
    return ret

cdef void _set_uniform(obj: Object2D|Object3D|Material, str name, value:UniformValueType):
    # Eventually move cdefs into their own functions so you are not allocating all of these at once.
    cdef:
        uniform_type valu
        Object3D o3
        Object2D o2
        Material mt
        # Uniform types:
        Matrix2x2 m2x2
        Matrix2x3 m2x3
        Matrix2x4 m2x4
        
        Matrix3x2 m3x2
        Matrix3x3 m3x3
        Matrix3x4 m3x4

        Matrix4x2 m4x2
        Matrix4x3 m4x3
        Matrix4x4 m4x4

        Vec2 v2
        Vec3 v3
        Vec4 v4

    if isinstance(value, float):
        valu = <float>value
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)

    elif isinstance(value, int):
        valu = <int>value
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)

    elif isinstance(value, Matrix2x2):
        m2x2 = <Matrix2x2>value
        valu = m2x2.c_class[0]
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)

    elif isinstance(value, Matrix2x3):
        m2x3 = <Matrix2x3>value
        valu = m2x3.c_class[0]
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)

    elif isinstance(value, Matrix2x4):
        m2x4 = <Matrix2x4>value
        valu = m2x4.c_class[0]
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)

    elif isinstance(value, Matrix3x2):
        m3x2 = <Matrix3x2>value
        valu = m3x2.c_class[0]
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)

    elif isinstance(value, Matrix3x3):
        m3x3 = <Matrix3x3>value
        valu = m3x3.c_class[0]
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)

    elif isinstance(value, Matrix3x4):
        m3x4 = <Matrix3x4>value
        valu = m3x4.c_class[0]
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)

    elif isinstance(value, Matrix4x2):
        m4x2 = <Matrix4x2>value
        valu = m4x2.c_class[0]
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)

    elif isinstance(value, Matrix4x3):
        m4x3 = <Matrix4x3>value
        valu = m4x3.c_class[0]
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)
        
    elif isinstance(value, Matrix4x4):
        m4x4 = <Matrix4x4>value
        valu = m4x4.c_class[0]
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)
    
    elif isinstance(value, Vec2):
        v2 = <Vec2>value
        valu = v2.c_class[0]
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)

    elif isinstance(value, Vec3):
        v3 = <Vec3>value
        valu = v3.c_class[0]
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)

    elif isinstance(value, Vec4):
        v4 = <Vec4>value
        valu = v4.c_class[0]
        if isinstance(obj, Object2D):
            o2 = obj
            _set_uniform_helper2d(o2.c_class, name, valu)
        elif isinstance(obj, Object3D):
            o3 = obj
            _set_uniform_helper3d(o3.c_class, name, valu)
        elif isinstance(obj, Material):
            mt = obj
            _set_uniform_helpermaterial(mt.c_class.data, name, valu)

cdef void _set_uniform_helper2d(object2d* obj, str name, uniform_type value):
    obj.set_uniform(name, value)

cdef void _set_uniform_helper3d(object3d* obj, str name, uniform_type value):
    obj.set_uniform(name, value)

cdef void _set_uniform_helpermaterial(material* obj, str name, uniform_type value):
    obj.set_uniform(name, value)

# Fonts/Text

cdef class Font:
    def __init__(self, str font_path, int font_size = 48) -> None:
        self.c_class = new font(font_path.encode(), font_size)

    def __dealloc__(self):
        del self.c_class

cdef class Text:
    def __init__(self, str text_string, Vec4 color, Vec2 position, Vec2 scale = Vec2(1.0, 1.0), float rotation = 0, Font font = None, Material material = None) -> None:
        self._position = position
        self._scale = scale
        self._font = font
        self._color = color
        self._material = material if material else Material(Shader.from_file(path.join(path.dirname(__file__), "default_vertex_text.glsl"), ShaderType.VERTEX), Shader.from_file(path.join(path.dirname(__file__), "default_fragment_text.glsl"), ShaderType.FRAGMENT))
        self.c_class = new text(text_string.encode(), self._font.c_class, self._color.c_class, self._position.c_class, self._scale.c_class, rotation, self._material.c_class)

    @property
    def rotation(self) -> float:
        return self.c_class.rotation

    @rotation.setter
    def rotation(self, float value):
        self.c_class.rotation = value

    @property
    def position(self) -> Vec2:
        return self._position

    @position.setter
    def position(self, Vec2 value):
        self._position.c_class[0] = value.c_class[0]

    @property
    def scale(self) -> Vec2:
        return self._scale

    @scale.setter
    def scale(self, Vec2 value):
        self._scale.c_class[0] = value.c_class[0]

    @property
    def color(self) -> Vec4:
        return self._color

    @color.setter
    def color(self, Vec4 value):
        self._color.c_class[0] = value.c_class[0]

    @property
    def material(self) -> Material:
        return self._material

    @material.setter
    def material(self, Material value):
        self._material.c_class.data[0] = value.c_class.data[0]

    @property
    def font(self) -> Font:
        return self._font

    @font.setter
    def font(self, Font value):
        self._font.c_class[0] = value.c_class[0]

    @property
    def text(self) -> str:
        return self.c_class.render_text

    @text.setter
    def text(self, value:str):
        self.c_class.render_text = value.encode()

    def __dealloc__(self):
        del self.c_class


cdef class CubeMap:
    def __init__(self, str right_path, str left_path, str top_path, str bottom_path, str back_path, str front_path) -> None:
        self.c_class = new cubemap(right_path.encode(), left_path.encode(), top_path.encode(), bottom_path.encode(), back_path.encode(), front_path.encode())
    
    def __dealloc__(self):
        del self.c_class

cdef class SkyBox:
    def __init__(self, CubeMap cube_map, Material mat = None) -> None:
        self._cubemap = cube_map
        self._material = mat if mat else Material(Shader.from_file(path.join(path.dirname(__file__), "default_vertex_skybox.glsl"), ShaderType.VERTEX), Shader.from_file(path.join(path.dirname(__file__), "default_fragment_skybox.glsl"), ShaderType.FRAGMENT))
        self.c_class = new skybox(self._cubemap.c_class, self._material.c_class)

    @property
    def material(self) -> Material:
        return self._material

    @material.setter
    def material(self, Material value):
        self._material.c_class.data[0] = value.c_class.data[0]

    @property
    def cubemap(self) -> CubeMap:
        return self._cubemap

    @cubemap.setter
    def cubemap(self, CubeMap value):
        self._cubemap.c_class[0] = value.c_class[0]
    
    def __dealloc__(self):
        del self.c_class

cdef class Emitter:

    def __init__(self, Vec3 position, Quaternion direction, Vec2 scale_min = Vec2(1.0, 1.0), Vec2 scale_max = Vec2(1.0, 1.0), int rate = 50, float decay_rate = 1.0, float spread = math.radians(30), float velocity_decay = 1.0, float start_velocity_min = 1.0, float start_velocity_max = 1.0, float start_lifetime_min = 10.0, float start_lifetime_max = 10.0, Vec4 color_min = Vec4(1.0,1.0,1.0,1.0), Vec4 color_max = Vec4(1.0,1.0,1.0,1.0), Material material = None) -> None:
        self._position = position
        self._direction = direction
        self._color_min = color_min
        self._color_max = color_max
        self._scale_min = scale_min
        self._scale_max = scale_max
        self._material = material if material else Material(
            Shader.from_file(path.join(path.dirname(__file__), "default_vertex_particle.glsl"), ShaderType.VERTEX),
            Shader.from_file(path.join(path.dirname(__file__), "default_fragment_particle.glsl"), ShaderType.FRAGMENT),
            Shader.from_file(path.join(path.dirname(__file__), "default_geometry_particle.glsl"), ShaderType.GEOMETRY)
        )
        self._material.diffuse_texture = Texture.from_file(path.join(path.dirname(__file__), "default_particle.png"))

        self.c_class = new emitter(
            self._position.c_class,
            self._direction.c_class,
            self._scale_min.c_class,
            self._scale_max.c_class,
            rate, decay_rate, spread,
            velocity_decay, start_velocity_min,
            start_velocity_max, start_lifetime_min,
            start_lifetime_max, 
            self._color_min.c_class, self._color_max.c_class,
            self._material.c_class
        )

    def __dealloc__(self):
        del self.c_class

    # MATERIAL

    @property
    def material(self) -> Material:
        return self._material

    @material.setter
    def material(self, Material value):
        self._material.c_class.data[0] = value.c_class.data[0]

    # POSITION

    @property
    def position(self) -> Vec3:
        return self._position

    @position.setter
    def position(self, Vec3 value) -> None:
        self._position.c_class[0] = value.c_class[0]

    # DIRECTION

    @property
    def direction(self) -> Quaternion:
        return self._direction

    @direction.setter
    def direction(self, Quaternion value) -> None:
        self._direction.c_class[0] = value.c_class[0]

    # COLOR

    @property
    def color_min(self) -> Vec4:
        return self._color_min

    @color_min.setter
    def color_min(self, Vec4 value) -> None:
        self._color_min.c_class[0] = value.c_class[0]

    @property
    def color_max(self) -> Vec4:
        return self._color_max

    @color_max.setter
    def color_max(self, Vec4 value) -> None:
        self._color_max.c_class[0] = value.c_class[0]

    # SCALE

    @property
    def scale_min(self) -> Vec2:
        return self._scale_min

    @scale_min.setter
    def scale_min(self, Vec2 value) -> None:
        self._scale_min.c_class[0] = value.c_class[0]

    @property
    def scale_max(self) -> Vec2:
        return self._scale_max

    @scale_max.setter
    def scale_max(self, Vec2 value) -> None:
        self._scale_max.c_class[0] = value.c_class[0]

    # rate
    
    @property
    def rate(self) -> int:
        return self.c_class.rate

    @rate.setter
    def rate(self, int value) -> None:
        self.c_class.rate = value

    # decay_rate
    
    @property
    def decay_rate(self) -> float:
        return self.c_class.decay_rate

    @decay_rate.setter
    def decay_rate(self, float value) -> None:
        self.c_class.decay_rate = value

    # spread
    
    @property
    def spread(self) -> float:
        return self.c_class.spread

    @spread.setter
    def spread(self, float value) -> None:
        self.c_class.spread = value

    # velocity_decay
    
    @property
    def velocity_decay(self) -> float:
        return self.c_class.velocity_decay

    @velocity_decay.setter
    def velocity_decay(self, float value) -> None:
        self.c_class.velocity_decay = value

    # start_velocity_min, start_velocity_max
    
    @property
    def start_velocity_min(self) -> float:
        return self.c_class.start_velocity_min

    @start_velocity_min.setter
    def start_velocity_min(self, float value) -> None:
        self.c_class.start_velocity_min = value

    @property
    def start_velocity_max(self) -> float:
        return self.c_class.start_velocity_max

    @start_velocity_max.setter
    def start_velocity_max(self, float value) -> None:
        self.c_class.start_velocity_max = value
    
    # start_lifetime_min, start_lifetime_max

    @property
    def start_lifetime_min(self) -> float:
        return self.c_class.start_lifetime_min

    @start_lifetime_min.setter
    def start_lifetime_min(self, float value) -> None:
        self.c_class.start_lifetime_min = value

    @property
    def start_lifetime_max(self) -> float:
        return self.c_class.start_lifetime_max

    @start_lifetime_max.setter
    def start_lifetime_max(self, float value) -> None:
        self.c_class.start_lifetime_max = value

    cpdef void start(self):
        self.c_class.start()

    cpdef void stop(self):
        self.c_class.stop()


cdef class Sound:
    def __init__(self, Window win, str source, bint loop) -> None:
        self.c_class = new sound(win.c_class, source.encode(), loop)

    def __dealloc__(self):
        del self.c_class

    def play(self, float volume = 1.0, float panning = 0.0, float pitch = 1.0, Vec3 position = None) -> None:
        if position:
            self.c_class.play(position.c_class[0], volume, panning, pitch)
        else:
            self.c_class.play(volume, panning, pitch)

    cpdef void stop(self):
        self.c_class.stop()