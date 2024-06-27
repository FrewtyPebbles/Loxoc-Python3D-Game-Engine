# distutils: language = c++
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map

cdef extern from "../src/RC.h":
    cdef cppclass RC[T]:
        RC() except +
        RC(T data) except +
        T inc()
        void dec()
        T data

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
        texture() except +
        texture(string file_path, TextureWraping wrap, TextureFiltering filtering) except +
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

cdef extern from "../src/Material.h":

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

cdef extern from "../src/Tup.h":
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

cdef extern from "../src/Vec3.h":
    cdef cppclass glmquat:
        float w,x,y,z

    cdef cppclass quaternion:
        quaternion() except +
        quaternion(float w, float x, float y, float z) except +
        quaternion(const glmquat& quat) except +
        quaternion(const quaternion& quat) except +
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
    
cdef Quaternion quat_from_cpp(quaternion cppinst)

cdef extern from "../src/Vec3.h":
    cdef cppclass glmvec3:
        float x,y,z

    cdef cppclass vec3:
        vec3() except +
        vec3(float x, float y, float z) except +
        vec3(const glmvec3& axis) except +
        vec3(const vec3& rhs) except +
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

        quaternion to_quaternion()

        vec3 cross(quaternion& other)

        vec3 operator*(quaternion& other)

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
        vec2() except +
        vec2(float x, float y) except +
        vec2(glmvec2& axis) except +
        vec2(vec2& rhs) except +
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

    cdef cppclass mesh_material:
        obj_material() except +
        tup3f ambient, diffuse, specular, emissive_coeficient
        float specular_exponent, optical_density, transparency
        illum_model illumination_model
        string ambient_tex_file, diffuse_tex_file, specular_highlight_file
        unsigned char* ambient_texture
        unsigned char* diffuse_texture
        unsigned char* specular_highlight_texture
        

    cdef cppclass mesh:
        mesh() except +
        mesh(const mesh& rhs) except +
        mesh(vector[mesh_material*] materials, vector[vec3]* vertexes, vector[vec3]* diffuse_coordinates, vector[vec3]* vertex_normals, vector[tup3ui]* faces, vec3 transform, vector[RC[texture*]*] diffuse_textures, vector[RC[texture*]*] specular_textures, vector[RC[texture*]*] normals_textures) except +
        @staticmethod
        mesh_dict from_file(string file_path) except +
        string name
        vector[mesh_material*] materials
        vector[vec3]* vertexes
        vector[vec3]* diffuse_coordinates
        vector[vec3]* vertex_normals
        vector[tup3ui]* faces
        vec3 transform
        vector[RC[texture*]*] diffuse_textures
        vector[RC[texture*]*] specular_textures
        vector[RC[texture*]*] normals_textures

    

    cdef cppclass mesh_dict:
        ctypedef map[string, vector[RC[mesh*]*]].iterator meshmap_iterator
        ctypedef map[string, vector[RC[mesh*]*]].const_iterator const_meshmap_iterator
        
        mesh_dict() except +
        mesh_dict(map[string, vector[RC[mesh*]*]] data) except +
        mesh_dict(mesh_dict& rhs) except +
        void insert(RC[mesh*]* m)
        vector[RC[mesh*]*] get(string name)
        void remove(string name)
        vector[RC[mesh*]*] operator[](string name)
        meshmap_iterator begin() noexcept
        const_meshmap_iterator cbegin() noexcept
        meshmap_iterator end() noexcept
        const_meshmap_iterator cend()

        map[string, vector[RC[mesh*]*]] data

cdef class MeshDict:
    cdef mesh_dict* c_class
    cpdef void insert(self, Mesh m)
    cpdef list[Mesh] get(self, str name)
    cpdef void remove(self, str name)

    @staticmethod
    cdef MeshDict from_cpp(mesh_dict cppinst)
    @staticmethod
    cdef MeshDict from_cpp_ptr(mesh_dict* cppinst)


cdef class Mesh:
    cdef RC[mesh*]* c_class
    cdef public list[Texture] diffuse_textures
    cdef public list[Texture] specular_textures
    cdef public list[Texture] normals_textures

    @staticmethod
    cdef Mesh from_cpp(RC[mesh*]* cppinst)
    
cpdef MeshDict mesh_from_file(str file_path)

cdef extern from "../src/Object.h":
    cdef cppclass object3d:
        # alias for cpp class object since name object is reserved by python
        object3d() except +
        object3d(mesh_dict* mesh, vec3* position, quaternion* rotation, vec3* scale) except +
        object3d(mesh_dict* mesh, vec3* position, quaternion* rotation, vec3* scale, material* mat) except +
        mesh_dict* mesh_data
        vec3* position
        quaternion* rotation
        vec3* scale
        material* mat

        void render(camera& camera)
        
        vector[vec3] get_translation(vector[vec3] vertexes)

        vector[vec3] get_rotation(vector[vec3] vertexes)

        void set_uniform(string name, uniform_type value, string type)


cdef class Object:
    cdef:
        object3d* c_class
        public MeshDict mesh_data
        public Material material
        Vec3 _position, _scale
        Quaternion _rotation
    
    cpdef void render(self, Camera camera)

    cpdef void set_uniform(self, str name, value:list[float] | int | float, str type)

cdef extern from "../src/Camera.h":

    cdef cppclass camera:
        camera() except +
        camera(vec3* position, quaternion* rotation, int view_width, int view_height, float focal_length, float fov) except +
        void render(vector[object3d*] objects)
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

    cpdef void render(self, list[Object] objects)



cdef extern from "../src/Window.h":
    cdef cppclass window:
        window() except +
        window(string title, camera* cam, int width, int height, bint fullscreen) except +
        camera* cam
        string title
        int width, height
        void update()
        void lock_mouse(bint lock)

        void add_object(object3d* obj)
        void remove_object(object3d* obj)
        void add_object_list(vector[object3d*] objs)
        void remove_object_list(vector[object3d*] objs)

        void add_object2d(object2d* obj)
        void remove_object2d(object2d* obj)
        void add_object2d_list(vector[object2d*] objs)
        void remove_object2d_list(vector[object2d*] objs)

        event current_event
        double deltatime
        bint fullscreen
        long long time_ns
        long long time

cdef class Window:
    cdef window* c_class

    cpdef void update(self)

    cpdef void add_object(self, Object obj)
    cpdef void remove_object(self, Object obj)
    cpdef void add_object_list(self, list[Object] objs)
    cpdef void remove_object_list(self, list[Object] objs)

    cpdef void add_object2d(self, Object2D obj)
    cpdef void remove_object2d(self, Object2D obj)
    cpdef void add_object2d_list(self, list[Object2D] objs)
    cpdef void remove_object2d_list(self, list[Object2D] objs)

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
        mouse_wheel() except +
        int int_x, int_y
        float x, y
        MOUSE_WHEEL_DIRECTION direction

    cdef cppclass mouse_device:
        mouse_device() except +
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
        event() except +
        EVENT_STATE get_flag(EVENT_FLAG _event)
        bint check_flag(EVENT_FLAG _event)
        mouse_device get_mouse() except +
        mouse_device get_mouse(unsigned char id) except +
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
        sprite() except +
        sprite(RC[texture*]* tex) except +

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
        object2d() except +
        object2d(sprite* spr, vec2* position, float rotation, vec2* scale, material* mat) except +
        
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
