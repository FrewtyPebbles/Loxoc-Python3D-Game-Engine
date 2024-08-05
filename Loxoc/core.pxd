# distutils: language = c++
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp.pair cimport pair

cdef extern from "<variant>" namespace "std" nogil:
    cdef cppclass variant:
        variant& operator=(variant&)

        # value status
        bint valueless_by_exception()
        size_t index()

    cdef struct monostate:
        pass

    cdef T get_if[T](...)

    cdef T get[T](...)

    cdef Z holds_alternative[Z](...)

cdef extern from "../src/util.h" namespace "std" nogil:
    cdef cppclass variant2[T,A]:
        variant2() except *
        variant2(T) except *
        variant2(A) except *
        variant2[T,A]& operator=(variant2[T,A]&)

        # value status
        bint valueless_by_exception()
        size_t index()

    cdef struct monostate:
        pass

    cdef Z get_if[Z](...)

    cdef Z get[Z](...)

    cdef Z holds_alternative[Z](...)

cdef extern from "../src/RC.h":
    cdef cppclass RC[T]:
        RC() except *
        RC(T data) except *
        T inc()
        void dec()
        T data
        int refcount

    cdef void RC_collect[T](RC[T*]* rc)

cdef extern from "../src/util.h":
    cdef void c_set_mod_path(string path)

cpdef void set_mod_path(str path)

cdef extern from "../src/Texture.h":
    cpdef enum class TextureFiltering:
        NEAREST,
        LINEAR

    cpdef enum class TextureWraping:
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER

    cdef cppclass texture:
        texture() except *
        texture(string file_path, TextureWraping wrap, TextureFiltering filtering) except *
        int width, height, number_of_channels
        void bind()

cdef class Texture:
    cdef RC[texture*]* c_class

cpdef Texture Texture_from_file(str file_path, TextureWraping wrap, TextureFiltering filtering)

cdef extern from "../src/Shader.h":
    cpdef enum class ShaderType:
        FRAGMENT,
        VERTEX
    cdef cppclass shader:
        shader() except *
        shader(string source, ShaderType type) except *
        void compile() except *
        const char * source
        ShaderType type
        unsigned int shader_handle
        from_file(string filepath, ShaderType type) except *
 
cdef class Shader:
    cdef RC[shader*]* c_class
    
    @staticmethod
    cdef Shader from_cpp(RC[shader*]* cppinst)



cdef extern from "../src/Material.h":

    ctypedef variant uniform_type
        

    cdef cppclass material:
        material() except *
        material(RC[shader*]* vertex, RC[shader*]* fragment) except *
        void set_uniform(string name, uniform_type value, string type) except *
        void register_uniforms() except *
        void link_shaders() except *
        void use_material() except *
        RC[shader*]* vertex
        RC[shader*]* fragment
        RC[texture*]* diffuse_texture
        RC[texture*]* specular_texture
        RC[texture*]* normals_texture
        string name
        vec3 ambient
        vec3 diffuse
        vec3 specular
        float shine
    
cdef class Material:
    cdef RC[material*]* c_class
    cdef public Shader vertex_shader, fragment_shader
    cdef public Texture diffuse_texture
    cdef public Texture specular_texture
    cdef public Texture normals_texture
    cpdef void set_uniform(self, str name, value:list[float] | int | float, str type)

    @staticmethod
    cdef Material from_cpp(RC[material*]* cppinst)

ctypedef unsigned char uint8

cdef extern from "../src/Tup.h":
    cdef cppclass tup2f:
        tup2f() except *
        tup2f(float[2] values) except *
        float[2] data

    cdef cppclass tup3tup2f:
        tup3tup2f() except *
        tup3tup2f(tup2f[3] values) except *
        tup2f[3] data


    cdef cppclass tup3f:
        tup3f() except *
        tup3f(float[3] values) except *
        float[3] data

    cdef cppclass tup4f:
        tup4f() except *
        tup4f(float[4] values) except *
        float[4] data
    
    cdef cppclass tup2i:
        tup2i() except *
        tup2i(int[2] values) except *
        int[2] data

    cdef cppclass tup3tup2i:
        tup3tup2i() except *
        tup3tup2i(tup2i[3] values) except *
        tup2i[3] data

    cdef cppclass tup3i:
        tup3i() except *
        tup3i(int[3] values) except *
        int[3] data

    cdef cppclass tup3ui:
        tup3ui() except *
        tup3ui(unsigned int[3] values) except *
        unsigned int[3] data

    cdef cppclass tup4i:
        tup4i() except *
        tup4i(int[4] values) except *
        int[4] data

    cdef cppclass tup3ui8:
        tup3i() except *
        tup3i(uint8[3] values) except *
        uint8[3] data

cdef extern from "../src/Vec3.h":
    cdef cppclass glmquat:
        float w,x,y,z

    cdef cppclass quaternion:
        quaternion() except *
        quaternion(float w, float x, float y, float z) except *
        quaternion(const glmquat& quat) except *
        quaternion(const quaternion& quat) except *
        glmquat quat
        #conversions/alternate constructors:
        vec3 to_euler()
        @staticmethod
        quaternion from_euler(const vec3& euler_vec)
        @staticmethod
        quaternion from_axis_angle(vec3 axis, float angle)

        void rotate(vec3 axis, float angle)

        # getters/setters
        float get_w()
        float get_x()
        float get_y()
        float get_z()
        void set_w(float other)
        void set_x(float other)
        void set_y(float other)
        void set_z(float other)

        vec3 get_up()
        vec3 get_right()
        vec3 get_forward()

        # operators
        quaternion operator+(quaternion& quaternion)
        quaternion operator+(float& quaternion)
        quaternion operator-()
        quaternion operator-(quaternion& other)
        quaternion operator-(float& other)
        quaternion operator*(quaternion& other)
        quaternion operator*(float& other)
        quaternion operator/(quaternion& other)
        quaternion operator/(float& other)
        float dot(quaternion& other)
        quaternion cross(quaternion& other)
        float get_magnitude()
        quaternion get_normalized()
        quaternion lerp(const quaternion& other, float ratio)

        vec3 cross(vec3& other)

        vec3 operator*(vec3& other)

cdef class Quaternion:
    cdef quaternion* c_class

    cpdef void set_euler_quat(self, Quaternion value)

    cpdef void set_euler_vec(self, Vec3 value)

    cpdef Quaternion quatadd(self, Quaternion other)

    cpdef Quaternion floatadd(self, float other)

    cpdef Quaternion quatsub(self, Quaternion other)

    cpdef Quaternion floatsub(self, float other)

    cpdef Quaternion quatmul(self, Quaternion other)

    cpdef Quaternion floatmul(self, float other)

    cpdef Quaternion quatdiv(self, Quaternion other)

    cpdef Quaternion floatdiv(self, float other)
    
    cpdef float get_magnitude(self)

    cpdef Quaternion get_normalized(self)

    cpdef Quaternion quat_cross(self, Quaternion other)

    cpdef Vec3 vec_cross(self, Vec3 other)

    cpdef Vec3 vecmul(self, Vec3 other)

    cpdef Vec3 to_euler(self)

    cpdef void rotate(self, Vec3 axis, float angle)
    
    cpdef Quaternion lerp(self, Quaternion other, float ratio)
    
cdef Quaternion quat_from_cpp(quaternion cppinst)

cdef extern from "../src/Matrix.h":
    cdef cppclass mat_ret_deduce[T]:
        pass

    cdef cppclass glmmat2x2:
        pass

    cdef cppclass glmmat2x3:
        pass

    cdef cppclass glmmat2x4:
        pass

    cdef cppclass glmmat3x2:
        pass

    cdef cppclass glmmat3x3:
        pass

    cdef cppclass glmmat3x4:
        pass

    cdef cppclass glmmat4x2:
        pass

    cdef cppclass glmmat4x3:
        pass

    cdef cppclass glmmat4x4:
        pass

    cdef cppclass matrix[glm_mat_type]:
        matrix() except *
        matrix(float value) except *
        matrix(float, float, float, float) except *
        matrix(float, float, float, float, float, float) except *
        matrix(float, float, float, float, float, float, float, float) except *
        matrix(float, float, float, float, float, float, float, float, float) except *
        matrix(float, float, float, float, float, float, float, float, float, float, float, float) except *
        matrix(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float) except *
        matrix(const quaternion& quat) except *
        matrix(quaternion * quat) except *
        matrix(const glm_mat_type& glm_mat) except *
        matrix(const matrix[glm_mat_type]& other) except *

        glm_mat_type mat

        vec3 get_up()

        vec3 get_right()

        vec3 get_forward()

        inline vec4 get_vec4(int index) const
        inline vec3 get_vec3(int index) const
        inline vec2 get_vec2(int index) const

        inline matrix[glm_mat_type] operator-()

        inline matrix[glm_mat_type] operator+(const matrix[glm_mat_type]& other)

        inline matrix[glm_mat_type] operator+(const float& other)

        inline matrix[glm_mat_type] operator-(const matrix[glm_mat_type]& other)

        inline matrix[glm_mat_type] operator-(const float& other)

        inline matrix[glm_mat_type] operator*(const matrix[glm_mat_type]& other)

        inline vec2 operator*(const vec2& other)

        inline vec2 mul_3x2(const vec3& other)

        inline vec2 mul_4x2(const vec4& other)

        inline vec3 operator*(const vec3& other)

        inline vec3 mul_4x3(const vec4& other)

        inline vec3 mul_2x3(const vec2& other)

        inline vec4 operator*(const vec4& other)

        inline matrix[glm_mat_type] operator*(const float& other)

        inline matrix[glm_mat_type] operator/(const matrix[glm_mat_type]& other)

        inline matrix[glm_mat_type] operator/(const float& other)

        inline matrix[glm_mat_type] translate(const vec3& vec)

        inline matrix[glm_mat_type] translate(vec3 * vec)

        inline matrix[glm_mat_type] scale(const vec3& vec)

        inline matrix[glm_mat_type] scale(vec3 * vec)

        inline matrix[glm_mat_type] scale(const vec2& vec)

        inline matrix[glm_mat_type] scale(vec2 * vec)

        inline matrix[glm_mat_type] inverse()

        inline float determinant()

        quaternion to_quaternion()

ctypedef matrix[glmmat2x2] matrix2x2

ctypedef matrix[glmmat2x3] matrix2x3

ctypedef matrix[glmmat2x4] matrix2x4

ctypedef matrix[glmmat3x2] matrix3x2

ctypedef matrix[glmmat3x3] matrix3x3

ctypedef matrix[glmmat3x4] matrix3x4

ctypedef matrix[glmmat4x2] matrix4x2

ctypedef matrix[glmmat4x3] matrix4x3

ctypedef matrix[glmmat4x4] matrix4x4

cdef extern from "../src/Vec4.h":
    cdef cppclass glmvec4:
        float x,y,z,w

    cdef cppclass vec4:
        vec4() except *
        vec4(float x, float y, float z, float w) except *
        vec4(const glmvec4& axis) except *
        vec4(const vec4& rhs) except *
        vec4(const vec3& rhs, float w) except *
        vec4(const vec2& rhs, float z, float w) except *
        glmvec4 axis
        float get_x()
        float get_y()
        float get_z()
        float get_w()
        void set_x(float other)
        void set_y(float other)
        void set_z(float other)
        void set_w(float other)

        vec4 get_up()
        vec4 get_right()
        vec4 get_forward()

        # operators
        vec4 operator+(vec4& other)
        vec4 operator+(float& other)
        vec4 operator-()
        vec4 operator-(vec4& other)
        vec4 operator-(float& other)
        vec4 operator*(vec4& other)
        vec4 operator*(float& other)
        vec4 operator/(vec4& other)
        vec4 operator/(float& other)
        float dot(vec4& other)
        float get_magnitude()
        vec4 get_normalized()
        float distance(vec4& other)

        vec4 operator*(quaternion& other)

        matrix2x4 outer_product(const vec2&)
        matrix3x4 outer_product(const vec3&)
        matrix4x4 outer_product(const vec4&)
        
        vec3 to_vec3()
        vec2 to_vec2()

cdef class Vec4:
    cdef vec4* c_class

    cpdef Vec4 vecadd(self, Vec4 other)

    cpdef Vec4 floatadd(self, float other)

    cpdef Vec4 vecsub(self, Vec4 other)

    cpdef Vec4 floatsub(self, float other)

    cpdef Vec4 vecmul(self, Vec4 other)

    cpdef Vec4 floatmul(self, float other)

    cpdef Vec4 vecdiv(self, Vec4 other)

    cpdef Vec4 floatdiv(self, float other)
    
    cpdef float get_magnitude(self)

    cpdef Vec4 get_normalized(self)

    cpdef Vec4 quatmul(self, Quaternion other)
    
    cpdef Vec3 to_vec3(self)
    cpdef Vec2 to_vec2(self)
    
cdef Vec4 vec4_from_cpp(vec4 cppinst)

cdef extern from "../src/Vec3.h":
    cdef cppclass glmvec3:
        float x,y,z

    cdef cppclass vec3:
        vec3() except *
        vec3(float x, float y, float z) except *
        vec3(const glmvec3& axis) except *
        vec3(const vec3& rhs) except *
        vec3(const vec2& rhs, float z) except *
        vec3(const vec4& rhs) except *
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
        float distance(vec3& other)

        quaternion to_quaternion()

        vec3 cross(quaternion& other)

        vec3 operator*(quaternion& other)

        matrix2x3 outer_product(const vec2&)
        matrix3x3 outer_product(const vec3&)
        matrix4x3 outer_product(const vec4&)
        
        vec4 to_vec4(float w)

        vec2 to_vec2()

cdef class Vec3:
    cdef vec3* c_class

    cpdef void set_quat_quat(self, Quaternion value)

    cpdef void set_quat_vec(self, Vec3 value)

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

    cpdef Quaternion quat_cross(self, Quaternion other)

    cpdef Vec3 vec_cross(self, Vec3 other)

    cpdef Vec3 quatmul(self, Quaternion other)

    cpdef Quaternion to_quaternion(self)
    
cdef Vec3 vec3_from_cpp(vec3 cppinst)


cdef extern from "../src/Vec2.h":
    cdef cppclass glmvec2:
        float x,y

    cdef cppclass vec2:
        vec2() except *
        vec2(float x, float y) except *
        vec2(glmvec2& axis) except *
        vec2(vec2& rhs) except *
        glmvec2 axis
        float get_x()
        float get_y()
        void set_x(float other)
        void set_y(float other)

        # operators
        vec2 operator+(vec2& other)
        vec2 operator+(float& other)
        vec2 operator-()
        vec2 operator-(vec2& other)
        vec2 operator-(float& other)
        vec2 operator*(vec2& other)
        vec2 operator*(float& other)
        vec2 operator/(vec2& other)
        vec2 operator/(float& other)
        float dot(vec2& other)
        float get_magnitude()
        vec2 get_normalized()
        float to_angle()
        @staticmethod
        vec2 from_angle(float angle)

        matrix2x2 outer_product(const vec2&)
        matrix3x2 outer_product(const vec3&)
        matrix4x2 outer_product(const vec4&)


cdef class Vec2:
    cdef vec2* c_class

    cpdef Vec2 vecadd(self, Vec2 other)

    cpdef Vec2 floatadd(self, float other)

    cpdef Vec2 vecsub(self, Vec2 other)

    cpdef Vec2 floatsub(self, float other)

    cpdef Vec2 vecmul(self, Vec2 other)

    cpdef Vec2 floatmul(self, float other)

    cpdef Vec2 vecdiv(self, Vec2 other)

    cpdef Vec2 floatdiv(self, float other)
    
    cpdef float get_magnitude(self)

    cpdef Vec2 get_normalized(self)

    cpdef float to_angle(self)
    

cdef Vec2 vec2_from_cpp(vec2 cppinst)



 

cdef extern from "../src/Mesh.h":

    cdef enum illum_model:
        CONSTANT_COLOR, 
        DIFFUSE,
        DIFFUSE_AND_SPECULAR
        

    cdef cppclass mesh:
        mesh() except *
        mesh(const mesh& rhs) except *
        mesh(string name, RC[material*]* mesh_material, vector[vec3]* vertexes, vector[vec3]* diffuse_coordinates, vector[vec3]* vertex_normals, vector[tup3ui]* faces, vec3 transform) except *
        @staticmethod
        RC[mesh_dict*]* from_file(string file_path) except *
        string name
        RC[material*]* mesh_material
        vector[vec3]* vertexes
        vector[vec3]* diffuse_coordinates
        vector[vec3]* vertex_normals
        vector[tup3ui]* faces
        vec3 transform
        float radius
        


    ctypedef variant2[RC[mesh*]*, RC[mesh_dict*]*] mesh_dict_child

    cdef cppclass mesh_dict:
        ctypedef map[string, mesh_dict_child].iterator meshmap_iterator
        ctypedef map[string, mesh_dict_child].const_iterator const_meshmap_iterator
        #
        mesh_dict() except *
        mesh_dict(string name, map[string, mesh_dict_child] data) except *
        mesh_dict(mesh_dict& rhs) except *
        void insert(mesh_dict_child m) except *
        mesh_dict_child get(string name) except * 
        void remove(string name) except *
        mesh_dict_child operator[](string name) except *
        meshmap_iterator begin() except *
        const_meshmap_iterator cbegin() except *
        meshmap_iterator end() except *
        const_meshmap_iterator cend() except *
        string name
        map[string, mesh_dict_child] data

cdef class MeshDict:
    cdef RC[mesh_dict*]* c_class
    cpdef void remove(self, str name)

    @staticmethod
    cdef MeshDict from_cpp(mesh_dict cppinst)
    @staticmethod
    cdef MeshDict from_cpp_ptr(RC[mesh_dict*]* cppinst)


cdef class Mesh:
    cdef RC[mesh*]* c_class
    cdef public Material material

    @staticmethod
    cdef Mesh from_cpp(RC[mesh*]* cppinst)
    
cpdef MeshDict mesh_from_file(str file_path) except *

cdef extern from "../src/Object3d.h":
    cdef cppclass object3d:
        object3d() except *
        object3d(RC[mesh_dict*]* mesh, vec3* position, quaternion* rotation, vec3* scale) except *
        object3d(RC[mesh_dict*]* mesh, vec3* position, quaternion* rotation, vec3* scale, RC[material*]* mat) except *
        object3d(RC[mesh_dict*]* mesh, vec3* position, quaternion* rotation, vec3* scale, RC[material*]* mat, RC[collider*]* collider) except *
        RC[mesh_dict*]* mesh_data
        vec3* position
        quaternion* rotation
        vec3* scale
        material* mat
        vector[RC[collider*]*] colliders

        void render(camera& camera, window* window)
        
        vector[vec3] get_translation(vector[vec3] vertexes)

        vector[vec3] get_rotation(vector[vec3] vertexes)

        void set_uniform(string name, uniform_type value, string type)

        inline bint check_collision_point(vec3 point)

        inline bint check_collision_object(object3d* obj)


cdef class Object3D:
    cdef:
        object3d* c_class
        public MeshDict mesh_data
        public Material material
        Vec3 _position, _scale
        Quaternion _rotation
    
    cpdef void set_uniform(self, str name, value:list[float] | int | float, str type)
    cpdef void add_collider(self, Collider collider)
    cpdef void remove_collider(self, Collider collider)

cdef extern from "../src/Camera.h":

    cdef cppclass camera:
        camera() except *
        camera(vec3* position, quaternion* rotation, int view_width, int view_height, float focal_length, float fov) except *
        vec3* position
        quaternion* rotation
        int view_width, view_height
        float focal_length
        float fov



cdef class Camera:
    cdef:
        camera* c_class
        Vec3 _position
        Quaternion _rotation


cdef extern from "../src/PointLight.h":
    cdef cppclass point_light:
        point_light() except *
        point_light(vec3* pos, float rad, vec3* col, float intensity) except *
        vec3* position
        float radius
        vec3* color
        float intensity
        float constant
        float linear
        float quadratic


cdef class PointLight:
    cdef:
        point_light* c_class
        Vec3 _position, _color, _ambient, _diffuse, _specular

cdef extern from "../src/DirectionalLight.h":
    cdef cppclass directional_light:
        directional_light() except *
        directional_light(quaternion* rotation, vec3* color, vec3* ambient, vec3* diffuse, vec3* specular, float intensity) except *
        quaternion* rotation
        vec3* color
        vec3* ambient
        vec3* diffuse
        vec3* specular
        float intensity


cdef class DirectionalLight:
    cdef:
        directional_light* c_class
        Quaternion _rotation
        Vec3 _color, _ambient, _diffuse, _specular

cdef extern from "../src/SpotLight.h":
    cdef cppclass spot_light:
        spot_light() except *
        spot_light(vec3* position, quaternion*  rotation, vec3* color, float cutOff, float outerCutOff, float intensity, float reach, bint use_cookie) except *
        spot_light(vec3* position, quaternion*  rotation, vec3* color, float cutOff, float outerCutOff, float intensity, float reach, bint use_cookie, RC[texture*]* cookie) except *
        vec3* position
        quaternion* rotation
        vec3* color
        float cutOff
        float outerCutOff
        float intensity
        float reach
        float constant
        float linear
        float quadratic
        bint use_cookie
        RC[texture*]* cookie


cdef class SpotLight:
    cdef:
        spot_light* c_class
        Quaternion _rotation
        Vec3 _position, _color
        Texture _cookie

cdef extern from "../src/Window.h":
    cdef cppclass window:
        window() except *
        window(string title, camera* cam, int width, int height, bint fullscreen, vec3 * ambient_light) except *
        camera* cam
        string title
        int width, height
        void update() except *
        void lock_mouse(bint lock) except *

        void add_object(object3d* obj)
        void remove_object(object3d* obj)
        void add_object_list(vector[object3d*] objs)
        void remove_object_list(vector[object3d*] objs)

        void add_object2d(object2d* obj)
        void remove_object2d(object2d* obj)
        void add_object2d_list(vector[object2d*] objs)
        void remove_object2d_list(vector[object2d*] objs)

        void add_point_light(point_light* obj)
        void remove_point_light(point_light* obj)
        void add_point_light_list(vector[point_light*] objs)
        void remove_point_light_list(vector[point_light*] objs)

        void add_directional_light(directional_light* obj)
        void remove_directional_light(directional_light* obj)
        void add_directional_light_list(vector[directional_light*] objs)
        void remove_directional_light_list(vector[directional_light*] objs)
        
        void add_spot_light(spot_light* obj)
        void remove_spot_light(spot_light* obj)
        void add_spot_light_list(vector[spot_light*] objs)
        void remove_spot_light_list(vector[spot_light*] objs)

        event current_event
        double deltatime
        bint fullscreen
        long long time_ns
        long long time
        vec3 * ambient_light

cdef class Window:
    cdef:
        window* c_class
        Vec3 _ambient_light
    

    cpdef void update(self)

    cpdef void add_object(self, Object3D obj)
    cpdef void remove_object(self, Object3D obj)
    cpdef void add_object_list(self, list[Object3D] objs)
    cpdef void remove_object_list(self, list[Object3D] objs)

    cpdef void add_object2d(self, Object2D obj)
    cpdef void remove_object2d(self, Object2D obj)
    cpdef void add_object2d_list(self, list[Object2D] objs)
    cpdef void remove_object2d_list(self, list[Object2D] objs)

    cpdef void add_point_light(self, PointLight obj)
    cpdef void remove_point_light(self, PointLight obj)
    cpdef void add_point_light_list(self, list[PointLight] objs)
    cpdef void remove_point_light_list(self, list[PointLight] objs)

    cpdef void add_directional_light(self, DirectionalLight obj)
    cpdef void remove_directional_light(self, DirectionalLight obj)
    cpdef void add_directional_light_list(self, list[DirectionalLight] objs)
    cpdef void remove_directional_light_list(self, list[DirectionalLight] objs)

    cpdef void add_spot_light(self, SpotLight obj)
    cpdef void remove_spot_light(self, SpotLight obj)
    cpdef void add_spot_light_list(self, list[SpotLight] objs)
    cpdef void remove_spot_light_list(self, list[SpotLight] objs)

    cpdef void lock_mouse(self, bint lock)


cdef extern from "../src/Event.h":
    cpdef enum class EVENT_FLAG:

        # WINDOW MANAGEMENT
        WINDOW_MINIMIZE,
        WINDOW_CLOSE,
        QUIT,

        # KEYS
        KEY_UP,
        KEY_DOWN,
        KEY_RIGHT,
        KEY_LEFT,
        KEY_SPACE,
        KEY_a,
        KEY_b,
        KEY_c,
        KEY_d,
        KEY_e,
        KEY_f,
        KEY_g,
        KEY_h,
        KEY_i,
        KEY_j,
        KEY_k,
        KEY_l,
        KEY_m,
        KEY_n,
        KEY_o,
        KEY_p,
        KEY_q,
        KEY_r,
        KEY_s,
        KEY_t,
        KEY_u,
        KEY_v,
        KEY_w,
        KEY_x,
        KEY_y,
        KEY_z,
        KEY_LSHIFT,
        KEY_RSHIFT,
        KEY_LCTRL,
        KEY_LALT,
        KEY_ESCAPE,
        KEY_COMMA,
        KEY_PERIOD,
        KEY_FORWARDSLASH,
        KEY_BACKSLASH,
        KEY_OPEN_BRACKET,
        KEY_CLOSE_BRACKET,
        KEY_SEMICOLON,
        KEY_QUOTE,
        KEY_ENTER,
        KEY_BACKSPACE,
        KEY_TAB,
        KEY_BACKTICK,
        KEY_DASH,
        KEY_EQUALS,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_0,
        KEY_RCTRL,
        KEY_RALT,
        
        # Mouse events
        MOUSE_BUTTON_DOWN,
        MOUSE_BUTTON_UP,
        MOUSE_WHEEL,
        MOUSE_MOTION

    cpdef enum class EVENT_STATE:
        NONE,
        PRESSED,
        RELEASED

    cpdef enum class MOUSE_EVENT_TYPE:
        BUTTON_DOWN,
        BUTTON_UP,
        NONE

    cpdef enum class MOUSE_BUTTON:
        LEFT,
        RIGHT,
        MIDDLE

    cpdef enum class MOUSE_WHEEL_DIRECTION:
        FLIPPED,
        NORMAL

    cdef cppclass mouse_wheel:
        mouse_wheel() except *
        int int_x, int_y
        float x, y
        MOUSE_WHEEL_DIRECTION direction

    cdef cppclass mouse_device:
        mouse_device() except *
        unsigned int id
        unsigned int timestamp
        unsigned int x, y, rel_x, rel_y
        unsigned int clicks
        MOUSE_EVENT_TYPE type
        window* window
        EVENT_STATE state
        MOUSE_BUTTON button
        mouse_wheel wheel

    cdef cppclass event:
        event() except *
        EVENT_STATE get_flag(EVENT_FLAG _event)
        bint check_flag(EVENT_FLAG _event)
        mouse_device get_mouse() except *
        mouse_device get_mouse(unsigned char id) except *
        vector[mouse_device] mice
        unsigned char current_mouse_id



cdef class Event:
    cdef event c_class

    cpdef EVENT_STATE get_flag(self, EVENT_FLAG _event)

    cpdef bint check_flag(self, EVENT_FLAG _event)

    cpdef MouseDevice get_mouse(self, int id)

cdef class MouseDevice:
    cdef:
        public unsigned int id
        public unsigned int timestamp
        public int x, y, rel_x, rel_y
        public unsigned int clicks
        public MOUSE_EVENT_TYPE type
        public EVENT_STATE state
        public MOUSE_BUTTON button
        public MouseWheel wheel

cdef class MouseWheel:
    cdef:
        public int int_x, int_y
        public float x, y
        public MOUSE_WHEEL_DIRECTION direction


cdef extern from "../src/Sprite.h":
    cdef cppclass sprite:
        sprite() except *
        sprite(RC[texture*]* tex) except *

        RC[texture*]* tex
        vec2[4] quad
        unsigned int gl_VAO, gl_VBO, gl_EBO

cdef class Sprite:
    cdef:
        sprite* c_class
        public Texture texture

cpdef Sprite sprite_from_texture(Texture tex)

cdef extern from "../src/Object2d.h":
    cdef cppclass object2d:
        object2d() except *
        object2d(sprite* spr, vec2* position, float rotation, vec2* scale, RC[material*]* mat) except *
        
        sprite* spr
        vec2* position
        float rotation
        vec2* scale
        material* mat

        void set_uniform(string name, uniform_type value, string type)

cdef class Object2D:
    cdef:
        object2d* c_class
        public Sprite sprite
        public Material material
        Vec2 _position, _scale
    
    cpdef void set_uniform(self, str name, value:list[float] | int | float, str type)

cpdef Sprite sprite_from_texture(Texture tex)


cdef extern from "../src/Colliders.h":
    cdef cppclass collider:
        collider() except *
        bint check_collision(vec3 intersection)
        bint check_collision(collider* intersection)
        pair[float, float] minmax_vertex_SAT(const vec3 & axis)
        bint check_SAT(vec3 axis, collider *other)
        object3d* owner

    cdef cppclass collider_box(collider):
        collider_box() except *
        collider_box(object3d* owner) except *
        collider_box(vec3 upper_bounds, vec3 lower_bounds) except *
        bint check_collision(vec3 intersection)
        bint check_collision(collider* other)
        bint check_collision(collider_box* collider)
        pair[float, float] minmax_vertex_SAT(const vec3 & axis)
        vec3 upper_bounds
        vec3 lower_bounds
        vec3[8] bounds
        

cdef class Collider:
    cdef:
        RC[collider*]* c_class


cdef class BoxCollider(Collider):
    pass

# MAT TYPES ------------------------------------------------------------------------------------

# 4x4

cdef class Matrix4x4:

    cdef matrix[glmmat4x4]* c_class

    cpdef Matrix4x4 inverse(self)

    cpdef float determinant(self)

    cpdef Quaternion to_quaternion(self)

    cpdef Vec3 get_up(self)
    
    cpdef Vec3 get_right(self)

    cpdef Vec3 get_forward(self)


cdef Matrix4x4 mat4x4_from_cpp(matrix[glmmat4x4] cppinst)

# 3x4

cdef class Matrix3x4:

    cdef matrix[glmmat3x4]* c_class

cdef Matrix3x4 mat3x4_from_cpp(matrix[glmmat3x4] cppinst)

# 2x4

cdef class Matrix2x4:

    cdef matrix[glmmat2x4]* c_class

cdef Matrix2x4 mat2x4_from_cpp(matrix[glmmat2x4] cppinst)

# 3x3

cdef class Matrix3x3:

    cdef matrix[glmmat3x3]* c_class

    cpdef Matrix3x3 inverse(self)

    cpdef float determinant(self)

    cpdef Quaternion to_quaternion(self)

    cpdef Vec3 get_up(self)
    
    cpdef Vec3 get_right(self)

    cpdef Vec3 get_forward(self)


cdef Matrix3x3 mat3x3_from_cpp(matrix[glmmat3x3] cppinst)

# 4x3

cdef class Matrix4x3:

    cdef matrix[glmmat4x3]* c_class

cdef Matrix4x3 mat4x3_from_cpp(matrix[glmmat4x3] cppinst)

# 2x3

cdef class Matrix2x3:

    cdef matrix[glmmat2x3]* c_class

cdef Matrix2x3 mat2x3_from_cpp(matrix[glmmat2x3] cppinst)

# 2x2

cdef class Matrix2x2:

    cdef matrix[glmmat2x2]* c_class

    cpdef Matrix2x2 inverse(self)

    cpdef float determinant(self)

cdef Matrix2x2 mat2x2_from_cpp(matrix[glmmat2x2] cppinst)

# 3x2

cdef class Matrix3x2:

    cdef matrix[glmmat3x2]* c_class

cdef Matrix3x2 mat3x2_from_cpp(matrix[glmmat3x2] cppinst)

# 4x2

cdef class Matrix4x2:

    cdef matrix[glmmat4x2]* c_class

cdef Matrix4x2 mat4x2_from_cpp(matrix[glmmat4x2] cppinst)

cdef void _set_uniform(obj: Object2D|Object3D, str name, value:list[float] | int | float, str type)

cdef void _set_uniform_helper2d(object2d* obj, str name, uniform_type value, str type)

cdef void _set_uniform_helper3d(object3d* obj, str name, uniform_type value, str type)