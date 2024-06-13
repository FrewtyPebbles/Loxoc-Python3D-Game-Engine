# distutils: language = c++
from cython.parallel cimport prange




cdef class Camera:
    def __init__(self, Vec3 position, int view_width, int view_height, float focal_length, float fov) -> None:
        self.c_class = new camera(&position.c_class, view_width, view_height, focal_length, fov)

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
        cdef Mesh ret = Mesh()
        ret.c_class = cppinst
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


cdef class V3Property:
    # THIS NEEDS TO HAVE ALL OF THE SAME METHODS AND PROPERTIES AS Vec3
    @staticmethod
    cdef V3Property init(vec3* ptr):
        ret = V3Property()
        ret.c_class = ptr
        return ret

    cpdef float get_magnitude(self):
        return self.c_class.get_magnitude()

    cpdef Vec3 get_normalized(self):
        return vec_from_cpp(self.c_class.get_normalized())

    @property
    def x(self):
        return self.c_class[0].axis.x
    
    @property
    def y(self):
        return self.c_class[0].axis.y

    @property
    def z(self):
        return self.c_class[0].axis.z

    @x.setter
    def x(self, float value):
        self.c_class[0].axis.x = value
    
    @y.setter
    def y(self, float value):
        self.c_class[0].axis.y = value

    @z.setter
    def z(self, float value):
        self.c_class[0].axis.z = value

    def __add__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecadd(other)
        else:
            return self.floatadd(other)

    cpdef Vec3 vecadd(self, Vec3 other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s + other.c_class)

    cpdef Vec3 floatadd(self, float other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s + other)

    def __sub__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecsub(other)
        else:
            return self.floatsub(other)

    cpdef Vec3 vecsub(self, Vec3 other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s - other.c_class)

    cpdef Vec3 floatsub(self, float other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s - other)

    def __mul__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecmul(other)
        else:
            return self.floatmul(other)
        
    cpdef Vec3 vecmul(self, Vec3 other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s * other.c_class)

    cpdef Vec3 floatmul(self, float other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s * other)

    def __truediv__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecdiv(other)
        else:
            return self.floatdiv(other)
        
    cpdef Vec3 vecdiv(self, Vec3 other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s / other.c_class)

    cpdef Vec3 floatdiv(self, float other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s / other)

    def dot(self, Vec3 other) -> float:
        return self.c_class[0].dot(other.c_class)

    def cross(self, Vec3 other) -> Vec3:
        return vec_from_cpp(self.c_class[0].cross(other.c_class))

cdef class Object:
    def __init__(self, list[Mesh] mesh_list, Vec3 position = Vec3(0.0,0.0,0.0),
    Vec3 rotation = Vec3(0.0,0.0,0.0), Vec3 scale = Vec3(1.0, 1.0, 1.0),
    Material material = None) -> None:
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

    def __dealloc__(self):
        del self.c_class


    @property
    def position(self) -> V3Property:
        return V3Property.init(&self.c_class.position)

    @position.setter
    def position(self, other:Vec3 | V3Property):
        if isinstance(other, Vec3):
            self.set_position_vec(other)
        elif isinstance(other, V3Property):
            self.set_position_prop(other)

    cpdef void set_position_vec(self, Vec3 other):
        cdef vec3 o = other.c_class
        self.c_class.position = o

    cpdef void set_position_prop(self, V3Property other):
        cdef vec3 o = other.c_class[0]
        self.c_class.position = o

    @property
    def rotation(self) -> V3Property:
        return V3Property.init(&self.c_class.rotation)

    @rotation.setter
    def rotation(self, other:Vec3 | V3Property):
        if isinstance(other, Vec3):
            self.set_rotation_vec(other)
        elif isinstance(other, V3Property):
            self.set_rotation_prop(other)

    cpdef void set_rotation_vec(self, Vec3 other):
        cdef vec3 o = other.c_class
        self.c_class.rotation = o

    cpdef void set_rotation_prop(self, V3Property other):
        cdef vec3 o = other.c_class[0]
        self.c_class.rotation = o

    @property
    def scale(self) -> V3Property:
        return V3Property.init(&self.c_class.scale)

    @scale.setter
    def scale(self, other:Vec3 | V3Property):
        if isinstance(other, Vec3):
            self.set_scale_vec(other)
        elif isinstance(other, V3Property):
            self.set_scale_prop(other)

    cpdef void set_scale_vec(self, Vec3 other):
        cdef vec3 o = other.c_class
        self.c_class.scale = o

    cpdef void set_scale_prop(self, V3Property other):
        cdef vec3 o = other.c_class[0]
        self.c_class.scale = o


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


cdef class Vec3:
    def __init__(self, float x, float y, float z) -> None:
        self.c_class = vec3(x,y,z)

    @property
    def x(self):
        return self.c_class.get_x()

    @x.setter
    def x(self, float value):
        self.c_class.set_x(value)

    @property
    def y(self):
        return self.c_class.get_y()

    @y.setter
    def y(self, float value):
        self.c_class.set_y(value)

    @property
    def z(self):
        return self.c_class.get_z()

    @z.setter
    def z(self, float value):
        self.c_class.set_z(value)


    # OPERATORS

    def __add__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecadd(other)
        else:
            return self.floatadd(other)

    cpdef Vec3 vecadd(self, Vec3 other):
        return vec_from_cpp(self.c_class + other.c_class)

    cpdef Vec3 floatadd(self, float other):
        return vec_from_cpp(self.c_class + other)

    def __sub__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecsub(other)
        else:
            return self.floatsub(other)

    cpdef Vec3 vecsub(self, Vec3 other):
        return vec_from_cpp(self.c_class - other.c_class)

    cpdef Vec3 floatsub(self, float other):
        return vec_from_cpp(self.c_class - other)

    def __mul__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecmul(other)
        else:
            return self.floatmul(other)
        
    cpdef Vec3 vecmul(self, Vec3 other):
        return vec_from_cpp(self.c_class * other.c_class)

    cpdef Vec3 floatmul(self, float other):
        return vec_from_cpp(self.c_class * other)

    def __truediv__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecdiv(other)
        else:
            return self.floatdiv(other)
        
    cpdef Vec3 vecdiv(self, Vec3 other):
        return vec_from_cpp(self.c_class / other.c_class)

    cpdef Vec3 floatdiv(self, float other):
        return vec_from_cpp(self.c_class / other)

    def dot(self, Vec3 other) -> float:
        return self.c_class.dot(other.c_class)

    cpdef Vec3 cross(self, Vec3 other):
        return vec_from_cpp(self.c_class.cross(other.c_class))

    cpdef float get_magnitude(self):
        return self.c_class.get_magnitude()

    cpdef Vec3 get_normalized(self):
        return vec_from_cpp(self.c_class.get_normalized())

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

    def __init__(self, str title, Camera cam, int width, int height) -> None:
        self.c_class = new window(title.encode(), cam.c_class, width, height)
    
    @property
    def current_event(self):
        return self.c_class.current_event

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
