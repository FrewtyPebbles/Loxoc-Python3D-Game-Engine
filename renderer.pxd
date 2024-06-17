# distutils: language = c++
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map


cdef extern from "src/Texture.h":
    cpdef enum class TextureFiltering:
        NEAREST,
        LINEAR

    cpdef enum class TextureWraping:
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER

    cdef cppclass texture:
        texture() except +
        texture(string file_path, TextureWraping wrap, TextureFiltering filtering) except +
        int width, height, number_of_channels
        void bind()

cdef class Texture:
    cdef texture* c_class

cpdef Texture Texture_from_file(str file_path, TextureWraping wrap, TextureFiltering filtering)

cdef extern from "src/Shader.h":
    cpdef enum class ShaderType:
        FRAGMENT,
        VERTEX
    cdef cppclass shader:
        shader() except +
        shader(string source, ShaderType type) except +
        void compile() except +
        const char * source
        ShaderType type
        unsigned int shader_handle
        from_file(string filepath, ShaderType type)
 
cdef class Shader:
    cdef shader* c_class
    

cdef extern from "<variant>" namespace "std" nogil:
    cdef cppclass variant:
        variant& operator=(variant&)

        # value status
        bint valueless_by_exception()
        size_t index()

    cdef struct monostate:
        pass

    cdef T* get_if[T](...)

cdef extern from "src/Material.h":

    ctypedef variant uniform_type
        

    cdef cppclass material:
        material() except +
        material(shader* vertex) except +
        material(shader* vertex, shader* fragment) except +
        void set_uniform(string name, uniform_type value, string type)
        void register_uniforms()
        void link_shaders()
        void use_material()
    
cdef class Material:
    cdef material* c_class
    cdef public Shader vertex_shader, fragment_shader
    cpdef void set_uniform(self, str name, value:list[float] | int | float, str type)


ctypedef unsigned char uint8

cdef extern from "src/Tup.h":
    cdef cppclass tup2f:
        tup2f() except +
        tup2f(float[2] values) except +
        float[2] data

    cdef cppclass tup3tup2f:
        tup3tup2f() except +
        tup3tup2f(tup2f[3] values) except +
        tup2f[3] data


    cdef cppclass tup3f:
        tup3f() except +
        tup3f(float[3] values) except +
        float[3] data

    cdef cppclass tup4f:
        tup4f() except +
        tup4f(float[4] values) except +
        float[4] data
    
    cdef cppclass tup2i:
        tup2i() except +
        tup2i(int[2] values) except +
        int[2] data

    cdef cppclass tup3tup2i:
        tup3tup2i() except +
        tup3tup2i(tup2i[3] values) except +
        tup2i[3] data

    cdef cppclass tup3i:
        tup3i() except +
        tup3i(int[3] values) except +
        int[3] data

    cdef cppclass tup3ui:
        tup3ui() except +
        tup3ui(unsigned int[3] values) except +
        unsigned int[3] data

    cdef cppclass tup4i:
        tup4i() except +
        tup4i(int[4] values) except +
        int[4] data

    cdef cppclass tup3ui8:
        tup3i() except +
        tup3i(uint8[3] values) except +
        uint8[3] data

cdef extern from "src/Vec3.h":
    cdef cppclass glmvec3:
        float x,y,z

    cdef cppclass vec3:
        vec3() except +
        vec3(float x, float y, float z) except +
        vec3(glmvec3& axis) except +
        glmvec3 axis
        float get_x()
        float get_y()
        float get_z()
        void set_x(float other)
        void set_y(float other)
        void set_z(float other)

        vec3 get_up()
        vec3 get_right()
        vec3 get_forward()

        # operators
        vec3 operator+(vec3& other)
        vec3 operator+(float& other)
        vec3 operator-()
        vec3 operator-(vec3& other)
        vec3 operator-(float& other)
        vec3 operator*(vec3& other)
        vec3 operator*(float& other)
        vec3 operator/(vec3& other)
        vec3 operator/(float& other)
        float dot(vec3& other)
        vec3 cross(vec3& other)
        float get_magnitude()
        vec3 get_normalized()

cdef class Vec3:
    cdef vec3* c_class


    cpdef Vec3 vecadd(self, Vec3 other)

    cpdef Vec3 floatadd(self, float other)

    cpdef Vec3 vecsub(self, Vec3 other)

    cpdef Vec3 floatsub(self, float other)

    cpdef Vec3 vecmul(self, Vec3 other)

    cpdef Vec3 floatmul(self, float other)

    cpdef Vec3 vecdiv(self, Vec3 other)

    cpdef Vec3 floatdiv(self, float other)
    
    cpdef float get_magnitude(self)

    cpdef Vec3 get_normalized(self)

    cpdef Vec3 cross(self, Vec3 other)
    
cdef Vec3 vec_from_cpp(vec3 cppinst)


cdef extern from "opencv2/core.hpp" namespace "cv":
    cdef cppclass Mat:
        Mat() except +
        void create(int, int, int) except +
        void* data
        int rows
        int cols

 

cdef extern from "src/Mesh.h":
    cdef enum illum_model:
        CONSTANT_COLOR, 
        DIFFUSE,
        DIFFUSE_AND_SPECULAR

    cdef cppclass mesh_material:
        obj_material() except +
        tup3f ambient, diffuse, specular, emissive_coeficient
        float specular_exponent, optical_density, transparency
        illum_model illumination_model
        string ambient_tex_file, diffuse_tex_file, specular_highlight_file
        Mat ambient_texture, diffuse_texture, specular_highlight_texture
        


    cdef cppclass mesh:
        mesh() except +
        mesh(vector[mesh_material*] materials, vector[vec3]* vertexes, vector[vec3]* diffuse_coordinates, vector[vec3]* vertex_normals, vector[tup3ui]* faces, vec3 transform, vector[texture*] diffuse_textures, vector[texture*] specular_textures, vector[texture*] normals_textures) except +
        @staticmethod
        vector[mesh*] from_file(string file_path) except +
        vector[mesh_material*] materials
        vector[vec3]* vertexes
        vector[vec3]* diffuse_coordinates
        vector[vec3]* vertex_normals
        vector[tup3ui]* faces
        vec3 transform
        vector[texture*] diffuse_textures
        vector[texture*] specular_textures
        vector[texture*] normals_textures

cdef class Mesh:
    cdef mesh* c_class
    cdef public list[Texture] diffuse_textures
    cdef public list[Texture] specular_textures
    cdef public list[Texture] normals_textures

    @staticmethod
    cdef Mesh from_cpp(mesh* cppinst)
    
cpdef list[Mesh] mesh_from_file(str file_path)

cdef extern from "src/Object.h":
    cdef cppclass object3d:
        # alias for cpp class object since name object is reserved by python
        object3d() except +
        object3d(vector[mesh*] mesh, vec3* position, vec3* rotation, vec3* scale) except +
        object3d(vector[mesh*] mesh, vec3* position, vec3* rotation, vec3* scale, material* mat) except +
        vector[mesh*] mesh_data
        vec3* position
        vec3* rotation
        vec3* scale
        material* mat

        void render(camera& camera)
        
        vector[vec3] get_translation(vector[vec3] vertexes)

        vector[vec3] get_rotation(vector[vec3] vertexes)

        void set_uniform(string name, uniform_type value, string type)


cdef class Object:
    cdef object3d* c_class
    cdef public list[Mesh] meshes
    cdef public Material material
    cdef Vec3 _position, _rotation, _scale
    
    cpdef void render(self, Camera camera)

    cpdef void set_uniform(self, str name, value:list[float] | int | float, str type)

cdef extern from "src/Camera.h":

    cdef cppclass camera:
        camera() except +
        camera(vec3* position, vec3* rotation, int view_width, int view_height, float focal_length, float fov) except +
        void render(vector[object3d*] objects)
        vec3* position
        vec3* rotation
        int view_width, view_height
        float focal_length
        float fov



cdef class Camera:
    cdef camera* c_class
    cdef Vec3 _position, _rotation

    cpdef void render(self, list[Object] objects)



cdef extern from "src/Window.h":
    cpdef enum class event:
        NOTHING,
        WINDOW_CLOSE,
        QUIT,
        KEY_UP,
        KEY_DOWN,
        KEY_RIGHT,
        KEY_LEFT,
        KEY_SPACE

    cdef cppclass window:
        window() except +
        window(string title, camera* cam, int width, int height, bint fullscreen) except +
        camera* cam
        string title
        int width, height
        void update(vector[object3d*] objects)
        event current_event
        float deltatime
        bint fullscreen

cdef class Window:
    cdef window* c_class

    cpdef void update(self, list[Object] objects)