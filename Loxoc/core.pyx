# distutils: language = c++
from cython.parallel cimport prange
from libc.math cimport M_PI
from os import path
from cpython.ref cimport Py_INCREF, Py_DECREF
from cython.operator import dereference, preincrement, postincrement
from typing import Generator
from libcpp.pair cimport pair

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
        self.c_class = new camera(position.c_class, self._rotation.c_class, view_width, view_height, focal_length, fov)
        
        

    def __dealloc__(self):
        del self.c_class

    @property
    def position(self) -> Vec3:
        return self._position

    @position.setter
    def position(self, Vec3 value):
        self._position = value
        self.c_class.position = value.c_class

    @property
    def rotation(self) -> Quaternion:
        return self._rotation

    @rotation.setter
    def rotation(self, value: Vec3 | Quaternion):
        if isinstance(value, Vec3):
            self._rotation = value.to_quaternion()
        elif isinstance(value, Quaternion):
            self._rotation = value
        self.c_class.rotation = self._rotation.c_class

    cpdef void render(self, list[Object] objects):
        cdef:
            vector[object3d*] vec
            Object obj
        for obj in objects:
            vec.push_back(obj.c_class)
        self.c_class.render(vec)

cdef class MeshDict:

    def __init__(self, list[Mesh] meshes) -> None:
        cdef:
            Mesh m
            mesh_dict md = mesh_dict()
        for m in meshes:
            md.insert(m.c_class)
        
        self.c_class = new mesh_dict(md)

    def __repr__(self) -> str:
        return "\n".join([
            "MeshDict {",
            *["\t{}: {}".format(m_n, m) for m_n, m in self],
            "}"
        ])

    def __dealloc__(self):
        # Collect the mesh_dict and decrement each mesh's refcount
        cdef:
            pair[string, vector[RC[mesh*]*]] _pair
            RC[mesh*]* m
        for _pair in self.c_class.data:
            for m in _pair.second:
                RC_collect(m)
        del self.c_class

    cpdef void insert(self, Mesh m):
        self.c_class.insert(m.c_class)

    cpdef list[Mesh] get(self, str name):
        cdef:
            RC[mesh*]* m
            list[Mesh] ret = []
        for m in self.c_class.get(name.encode()):
            ret.append(Mesh.from_cpp(m))
        return ret

    cpdef void remove(self, str name):
        self.c_class.remove(name.encode())

    def __iter__(self) -> Generator[(str, list[Mesh]), None, None]:
        cdef:
            pair[string, vector[RC[mesh*]*]] _pair
            RC[mesh*]* m
            Mesh M
            list[Mesh] M_list = []
        for _pair in self.c_class.data:
            for m in _pair.second:
                M = Mesh.from_cpp(m)
                M_list.append(M)
            yield (
                bytes(_pair.first).decode(), 
                M_list
            )

    def __getitem__(self, str key) -> list[Mesh]:
        return self.get(key)

    @staticmethod
    cdef MeshDict from_cpp(mesh_dict cppinst):
        cdef MeshDict ret = MeshDict.__new__(MeshDict)
        ret.c_class = new mesh_dict(cppinst)
        return ret

    @staticmethod
    cdef MeshDict from_cpp_ptr(mesh_dict* cppinst):
        cdef MeshDict ret = MeshDict.__new__(MeshDict)
        ret.c_class = cppinst
        return ret

cdef class Mesh:

    def __repr__(self) -> str:
        return "Mesh {{name: {}}}".format(self.name)

    def __dealloc__(self):
        RC_collect(self.c_class)

    @property
    def name(self) -> str:
        return bytes(self.c_class.data.name).decode()

    @staticmethod
    cdef Mesh from_cpp(RC[mesh*]* cppinst):
        cdef:
            Mesh ret = Mesh.__new__(Mesh)
            RC[texture*]* _tex
            list[Texture] diffuse_textures = []
            list[Texture] specular_textures = []
            list[Texture] normals_textures = []
            Texture tex_diff, tex_spec, tex_norm
        
        ret.c_class = cppinst
        ret.c_class.inc()

        # register textures on the python heap so it can manage garbage collection for c++ memory
        
        # refcount diffuse
        for _tex in ret.c_class.data.diffuse_textures:
            tex_diff = Texture.__new__(Texture)
            tex_diff.c_class = _tex
            tex_diff.c_class.inc()
            diffuse_textures.append(tex_diff)
        
        ret.diffuse_textures = diffuse_textures

        # refcount specular
        for _tex in ret.c_class.data.specular_textures:
            tex_spec = Texture.__new__(Texture)
            tex_spec.c_class = _tex
            tex_diff.c_class.inc()
            specular_textures.append(tex_spec)
        
        ret.specular_textures = specular_textures

        # refcount normals
        for _tex in ret.c_class.data.normals_textures:
            tex_norm = Texture.__new__(Texture)
            tex_norm.c_class = _tex
            tex_diff.c_class.inc()
            normals_textures.append(tex_norm)
        
        ret.normals_textures = normals_textures

        return ret

    @staticmethod
    def from_file(file_path:str) -> MeshDict:
        return mesh_from_file(file_path)

cpdef MeshDict mesh_from_file(str file_path):
    return MeshDict.from_cpp(mesh.from_file(file_path.encode()))

cdef class Object:
    def __init__(self, MeshDict mesh_data, Vec3 position = Vec3(0.0,0.0,0.0),
    Vec3 rotation = Vec3(0.0,0.0,0.0), Vec3 scale = Vec3(1.0, 1.0, 1.0),
    Material material = None) -> None:
        self._position = position
        self._rotation = rotation.to_quaternion()
        self._scale = scale
        self.mesh_data = mesh_data
        
        # create the mesh vector for the cppclass
        cdef:
            Mesh m
            str name
        
        if material:
            self.material = material
            self.c_class = new object3d(mesh_data.c_class, position.c_class, self._rotation.c_class, scale.c_class, self.material.c_class)
        else:
            self.material = Material()
            self.c_class = new object3d(mesh_data.c_class, position.c_class, self._rotation.c_class, scale.c_class, self.material.c_class)

    @property
    def position(self) -> Vec3:
        return self._position

    @position.setter
    def position(self, Vec3 value):
        self._position = value
        self.c_class.position = value.c_class

    @property
    def rotation(self) -> Quaternion:
        return self._rotation

    @rotation.setter
    def rotation(self, value: Vec3 | Quaternion):
        if isinstance(value, Vec3):
            self._rotation = value.to_quaternion()
        elif isinstance(value, Quaternion):
            self._rotation = value
        self.c_class.rotation = self._rotation.c_class

    @property
    def scale(self) -> Vec3:
        return self._scale

    @scale.setter
    def scale(self, Vec3 value):
        self._scale = value
        self.c_class.scale = value.c_class

    def __dealloc__(self):
        del self.c_class


    cpdef void render(self, Camera camera):
        self.c_class.render(camera.c_class[0])

    cpdef void set_uniform(self, str name, value:list[float] | int | float, str type):
        cdef:
            vector[float] uni_vec
            uniform_type valu

        if isinstance(value, float):
            valu = <float>value
            self.c_class.set_uniform(name, valu, type)
        elif isinstance(value, int):
            valu = <int>value
            self.c_class.set_uniform(name.encode(), valu, type.encode())
        else:
            for val in value:
                uni_vec.push_back(val)
            valu = uni_vec
            self.c_class.set_uniform(name, valu, type)




cdef class Shader:
    
    def __init__(self, str source, ShaderType shader_type) -> None:
        self.c_class = new shader(source.encode(), shader_type)

    def __dealloc__(self):
        del self.c_class

    @classmethod
    def from_file(cls, str filepath, ShaderType type) -> Shader:
        with open(filepath, 'r') as fp:
            src = fp.read()
        return cls(src, type)

cdef class Quaternion:
    def __init__(self, float w, float x, float y, float z) -> None:
        self.c_class = new quaternion(w,x,y,z)

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

    def rotate_pitch(self, float value):
        self.c_class.rotate(vec3(1.0,0.0,0.0), value)

    @property
    def euler_yaw(self) -> float:
        return self.to_euler().y

    def rotate_yaw(self, float value):
        self.c_class.rotate(vec3(0.0,1.0,0.0), value)

    @property
    def euler_roll(self) -> float:
        return self.to_euler().z

    def rotate_roll(self, float value):
        self.c_class.rotate(vec3(0.0,0.0,1.0), value)

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

    def __mul__(self, other:Quaternion | float | Vec3) -> Quaternion:
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


    cpdef Vec3 to_euler(self):
        return vec3_from_cpp(self.c_class.to_euler())

    @staticmethod
    def from_euler(Vec3 euler_vec) -> Quaternion:
        return quat_from_cpp(quaternion.from_euler(euler_vec.c_class[0]))

    @staticmethod
    def from_axis_angle(Vec3 axis, float angle) -> Quaternion:
        return quat_from_cpp(quaternion.from_axis_angle(axis.c_class[0], angle))

    cpdef void rotate(self, Vec3 axis, float angle):
        self.c_class.rotate(axis.c_class[0], angle)

    @staticmethod
    def from_quat(Quaternion quat) -> Quaternion: # copy method
        cdef Quaternion q = Quaternion.__new__(Quaternion)
        q.c_class = new quaternion(quat.c_class[0])
        return q

cdef Quaternion quat_from_cpp(quaternion cppinst):
    cdef Quaternion ret = Quaternion.__new__(Quaternion)
    ret.c_class = new quaternion(cppinst)
    return ret

cdef class Vec3:
    def __init__(self, float x, float y, float z) -> None:
        self.c_class = new vec3(x,y,z)

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

cdef Vec3 vec3_from_cpp(vec3 cppinst):
    cdef Vec3 ret = Vec3.__new__(Vec3)
    ret.c_class = new vec3(cppinst)
    return ret
    
cdef class Vec2:
    def __init__(self, float x, float y) -> None:
        self.c_class = new vec2(x,y)

    def __repr__(self) -> str:
        return f"<{self.x}, {self.y}>"

    def __dealloc__(self):
        del self.c_class

    def __neg__(self) -> Vec2:
        return vec2_from_cpp(-self.c_class[0])


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

cdef Vec2 vec2_from_cpp(vec2 cppinst):
    cdef:
        Vec2 ret = Vec2.__new__(Vec2)
        vec2* v = new vec2(cppinst)
    ret.c_class = v
    return ret

cdef class Material:
    def __init__(self, Shader vertex = None, Shader fragment = None) -> None:
        if vertex:
            self.vertex_shader = vertex
        else:
            self.vertex_shader = Shader.from_file(path.join(path.dirname(__file__), "default_vertex.glsl"), ShaderType.VERTEX)

        if fragment:
            self.fragment_shader = fragment
        else:
            self.fragment_shader = Shader.from_file(path.join(path.dirname(__file__), "default_fragment.glsl"), ShaderType.FRAGMENT)

        self.c_class = new material(self.vertex_shader.c_class, self.fragment_shader.c_class)
        
    def __dealloc__(self):
        del self.c_class
    
    cpdef void set_uniform(self, str name, value:list[float] | int | float, str type):
        cdef:
            vector[float] uni_vec
            uniform_type valu

        if isinstance(value, float):
            valu = <float>value
            self.c_class.set_uniform(name, valu, type)
        elif isinstance(value, int):
            valu = <int>value
            self.c_class.set_uniform(name.encode(), valu, type.encode())
        else:
            for val in value:
                uni_vec.push_back(val)
            valu = uni_vec
            self.c_class.set_uniform(name, valu, type)


cdef class Window:

    def __init__(self, str title, Camera cam, int width, int height, bint fullscreen = False) -> None:
        self.c_class = new window(title.encode(), cam.c_class, width, height, fullscreen)
    
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

    cpdef void add_object(self, Object obj):
        Py_INCREF(obj)
        self.c_class.add_object(obj.c_class)

    cpdef void remove_object(self, Object obj):
        self.c_class.remove_object(obj.c_class)
        Py_DECREF(obj)

    cpdef void add_object_list(self, list[Object] objs):
        cdef:
            Object obj

        for obj in objs:
            self.add_object(obj)

    cpdef void remove_object_list(self, list[Object] objs):
        cdef:
            Object obj

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

cdef class Object2D:
    def __init__(self, Sprite sprite, Vec2 position = Vec2(0.0,0.0),
    float rotation = 0.0, Vec2 scale = Vec2(1.0, 1.0),
    Material material = None) -> None:
        self._position = position
        self._scale = scale
        self.sprite = sprite

        
        
        if material:
            self.material = material
            self.c_class = new object2d(sprite.c_class, position.c_class, rotation, scale.c_class, self.material.c_class)
        else:
            self.material = Material(
                Shader.from_file(path.join(path.dirname(__file__), "default_vertex_2D.glsl"), ShaderType.VERTEX),
                Shader.from_file(path.join(path.dirname(__file__), "default_fragment_2D.glsl"), ShaderType.FRAGMENT)
            )
            self.c_class = new object2d(sprite.c_class, position.c_class, rotation, scale.c_class, self.material.c_class)

    @property
    def position(self) -> Vec2:
        return self._position

    @position.setter
    def position(self, Vec2 value):
        self._position = value
        self.c_class.position = value.c_class

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
        self._scale = value
        self.c_class.scale = value.c_class

    def __dealloc__(self):
        del self.c_class


    cpdef void set_uniform(self, str name, value:list[float] | int | float, str type):
        cdef:
            vector[float] uni_vec
            uniform_type valu

        if isinstance(value, float):
            valu = <float>value
            self.c_class.set_uniform(name.encode(), valu, type.encode())
        elif isinstance(value, int):
            valu = <int>value
            self.c_class.set_uniform(name.encode(), valu, type.encode())
        else:
            for val in value:
                uni_vec.push_back(val)
            valu = uni_vec
            self.c_class.set_uniform(name.encode(), valu, type.encode())