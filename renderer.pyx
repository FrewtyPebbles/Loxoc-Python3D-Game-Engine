# distutils: language = c++
from cython.parallel cimport prange




cdef class Texture:
    @classmethod
    def from_file(cls, str file_path, TextureWraping wrap = TextureWraping.REPEAT, TextureFiltering filtering = TextureFiltering.LINEAR) -> Texture:
        return Texture_from_file(file_path, wrap, filtering)
        


    def __dealloc__(self):
        del self.c_class

cpdef Texture Texture_from_file(str file_path, TextureWraping wrap, TextureFiltering filtering):
    ret = Texture()
    ret.c_class = new texture(file_path.encode(), wrap, filtering)

    return ret

cdef class Camera:
    def __init__(self, Vec3 position, Vec3 rotation, int view_width, int view_height, float focal_length, float fov) -> None:
        self._position = position
        self._rotation = rotation
        self.c_class = new camera(position.c_class, rotation.c_class, view_width, view_height, focal_length, fov)

    @property
    def position(self):
        return self._position

    @position.setter
    def position(self, Vec3 value):
        self._position = value
        self.c_class.position = value.c_class

    @property
    def rotation(self):
        return self._rotation

    @rotation.setter
    def rotation(self, Vec3 value):
        self._rotation = value
        self.c_class.rotation = value.c_class

    def __dealloc__(self):
        del self.c_class

    cpdef void render(self, list[Object] objects):
        cdef:
            vector[object3d*] vec
            Object obj
        for obj in objects:
            vec.push_back(obj.c_class)
        self.c_class.render(vec)


cdef class Mesh:
    def __dealloc__(self):
        del self.c_class

    @staticmethod
    cdef Mesh from_cpp(mesh* cppinst):
        cdef:
            Mesh ret = Mesh()
            texture* _tex
            list[Texture] diffuse_textures = []
            list[Texture] specular_textures = []
            list[Texture] normals_textures = []
        
        ret.c_class = cppinst

        # register textures on the python heap so it can manage garbage collection for c++ memory
        
        # refcount diffuse
        for _tex in ret.c_class.diffuse_textures:
            tex = Texture()
            tex.c_class = _tex
            diffuse_textures.append(tex)
        
        ret.diffuse_textures = diffuse_textures

        # refcount specular
        for _tex in ret.c_class.specular_textures:
            tex = Texture()
            tex.c_class = _tex
            specular_textures.append(tex)
        
        ret.specular_textures = specular_textures

        # refcount normals
        for _tex in ret.c_class.normals_textures:
            tex = Texture()
            tex.c_class = _tex
            normals_textures.append(tex)
        
        ret.normals_textures = normals_textures

        return ret

    @staticmethod
    def from_file(file_path:str) -> list[Mesh]:
        return mesh_from_file(file_path)

cpdef list[Mesh] mesh_from_file(str file_path):
    cdef:
        mesh* m
        list[Mesh] ret = []
    for m in mesh.from_file(file_path.encode()):
        ret.append(Mesh.from_cpp(m))
    return ret

cdef class Object:
    def __init__(self, list[Mesh] mesh_list, Vec3 position = Vec3(0.0,0.0,0.0),
    Vec3 rotation = Vec3(0.0,0.0,0.0), Vec3 scale = Vec3(1.0, 1.0, 1.0),
    Material material = None) -> None:
        self._position = position
        self._rotation = rotation
        self._scale = scale
        self.meshes = mesh_list
        
        # create the mesh vector for the cppclass
        cdef:
            vector[mesh*] mesh_vec
            Mesh m

        for m in mesh_list:
            mesh_vec.push_back(m.c_class)
        
        if material:
            self.material = material
            self.c_class = new object3d(mesh_vec, position.c_class, rotation.c_class, scale.c_class, self.material.c_class)
        else:
            self.material = Material()
            self.c_class = new object3d(mesh_vec, position.c_class, rotation.c_class, scale.c_class, self.material.c_class)

    @property
    def position(self):
        return self._position

    @position.setter
    def position(self, Vec3 value):
        self._position = value
        self.c_class.position = value.c_class

    @property
    def rotation(self):
        return self._rotation

    @rotation.setter
    def rotation(self, Vec3 value):
        self._rotation = value
        self.c_class.rotation = value.c_class

    @property
    def scale(self):
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
        if isinstance(other, Quaternion):
            return self.vecmul(other)
        else:
            return self.floatmul(other)
        
    cpdef Quaternion quatmul(self, Quaternion other):
        return quat_from_cpp(self.c_class[0] * other.c_class[0])

    cpdef Vec3 vecmul(self, Vec3 other):
        return vec_from_cpp(self.c_class[0] * other.c_class[0])

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
        return vec_from_cpp(self.c_class.cross(other.c_class[0]))

    cpdef float get_magnitude(self):
        return self.c_class[0].get_magnitude()

    cpdef Quaternion get_normalized(self):
        return quat_from_cpp(self.c_class.get_normalized())


    cpdef Vec3 to_euler(self):
        return vec_from_cpp(self.c_class.to_euler())

    @staticmethod
    def from_euler(Vec3 euler_vec) -> Quaternion:
        return quat_from_cpp(quaternion.from_euler(euler_vec.c_class[0]))

    @staticmethod
    def from_axis_angle(Vec3 axis, float angle) -> Quaternion:
        return quat_from_cpp(quaternion.from_axis_angle(axis.c_class[0], angle))

    cpdef void rotate(self, Vec3 axis, float angle):
        self.c_class.rotate(axis.c_class[0], angle)

cdef Quaternion quat_from_cpp(quaternion cppinst):
    return Quaternion(cppinst.quat.w, cppinst.quat.x, cppinst.quat.y, cppinst.quat.z) 

cdef class Vec3:
    def __init__(self, float x, float y, float z) -> None:
        self.c_class = new vec3(x,y,z)

    def __repr__(self) -> str:
        return f"<{self.x}, {self.y}, {self.z}>"

    def __dealloc__(self):
        del self.c_class

    def __neg__(self) -> Vec3:
        return vec_from_cpp(-self.c_class[0])

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
        return vec_from_cpp(self.c_class.get_up())

    @property
    def right(self) -> Vec3:
        return vec_from_cpp(self.c_class.get_right())

    @property
    def forward(self) -> Vec3:
        return vec_from_cpp(self.c_class.get_forward())


    # OPERATORS

    def __add__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecadd(other)
        else:
            return self.floatadd(other)

    cpdef Vec3 vecadd(self, Vec3 other):
        return vec_from_cpp(self.c_class[0] + other.c_class[0])

    cpdef Vec3 floatadd(self, float other):
        return vec_from_cpp(self.c_class[0] + other)

    def __sub__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecsub(other)
        else:
            return self.floatsub(other)

    cpdef Vec3 vecsub(self, Vec3 other):
        return vec_from_cpp(self.c_class[0] - other.c_class[0])

    cpdef Vec3 floatsub(self, float other):
        return vec_from_cpp(self.c_class[0] - other)

    def __mul__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecmul(other)
        if isinstance(other, Quaternion):
            return self.quatmul(other)
        else:
            return self.floatmul(other)
        
    cpdef Vec3 vecmul(self, Vec3 other):
        return vec_from_cpp(self.c_class[0] * other.c_class[0])

    cpdef Vec3 quatmul(self, Quaternion other):
        return vec_from_cpp(self.c_class[0] * other.c_class[0])

    cpdef Vec3 floatmul(self, float other):
        return vec_from_cpp(self.c_class[0] * other)

    def __truediv__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecdiv(other)
        else:
            return self.floatdiv(other)
        
    cpdef Vec3 vecdiv(self, Vec3 other):
        return vec_from_cpp(self.c_class[0] / other.c_class[0])

    cpdef Vec3 floatdiv(self, float other):
        return vec_from_cpp(self.c_class[0] / other)

    def dot(self, Vec3 other) -> float:
        return self.c_class[0].dot(other.c_class[0])

    def cross(self, other:Quaternion | Vec3) -> float:
        if isinstance(other, Quaternion):
            return self.quat_cross(other)
        elif isinstance(other, Vec3):
            return self.vec_cross(other)

    cpdef Quaternion quat_cross(self, Quaternion other):
        return vec_from_cpp(self.c_class.cross(other.c_class[0]))

    cpdef Vec3 vec_cross(self, Vec3 other):
        return vec_from_cpp(self.c_class.cross(other.c_class[0]))

    cpdef float get_magnitude(self):
        return self.c_class[0].get_magnitude()

    cpdef Vec3 get_normalized(self):
        return vec_from_cpp(self.c_class[0].get_normalized())

cdef Vec3 vec_from_cpp(vec3 cppinst):
    return Vec3(cppinst.axis.x, cppinst.axis.y, cppinst.axis.z)
    


cdef class Material:
    def __init__(self, Shader vertex = None, Shader fragment = None) -> None:
        if vertex:
            self.vertex_shader = vertex
        else:
            self.vertex_shader = Shader.from_file("./default_vertex.glsl", ShaderType.VERTEX)

        if fragment:
            self.fragment_shader = fragment
        else:
            self.fragment_shader = Shader.from_file("./default_fragment.glsl", ShaderType.FRAGMENT)

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
    def deltatime(self) -> float:
        return self.c_class.deltatime

    @property
    def dt(self) -> float:
        return self.c_class.deltatime

    def __dealloc__(self):
        del self.c_class

    cpdef void update(self, list[Object] objects):
        cdef:
            vector[object3d*] vec
            Object obj
        for obj in objects:
            vec.push_back(obj.c_class)
        self.c_class.update(vec)

    cpdef void lock_mouse(self, bint lock):
        self.c_class.lock_mouse(lock)

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


