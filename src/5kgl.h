#ifndef __5kgl_h

#define __5kgl_h

#ifdef _WIN32
#define alloca(x) _alloca(x)
#else
#include <alloca.h>
#endif

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

/*
 * util.c
 */

#define DEFAULT_LIST_CAPACITY 16

typedef struct {
    void **data;
    int capacity;
    int length;
} list;

list* split_string(char*, char*);

list* create_list();

int list_add(list*, void*);

void list_add_all(list*, list*);

void* list_insert(list*, void*, int);

void* list_get(list*, int);

int list_find(list*, void*);

void* list_iterate(list*, int*, void**);

void* list_remove(list*, int);

int list_remove_element(list*, void*);

void list_clear(list*);

void list_free(list*);

void list_free_keep_elements(list*);

void* read_file(const char*, int*);

typedef struct {
    int *data;
    int size;
    int i;
} ring_buffer;

ring_buffer* make_ring_buffer(int);

void ring_buffer_free(ring_buffer*);

void ring_buffer_add(ring_buffer*, int);

int ring_buffer_get(ring_buffer*, int);

int int_mod(int i, int m);

/*
 * space_math.c
 */

//type definitions for vectors, quaternion, and matrix

typedef struct {
    double x, y;
} vec2;

typedef struct {
    double x, y, z;
} vec3;

typedef struct {
    double x, y, z, w;
} quat;

typedef float mat4[16];

typedef struct {
    int i, j, k;
} ivec3;

// constuctors

vec2* c_vec2(double, double);

vec3* c_vec3(double, double, double);

ivec3* c_ivec3(int, int, int);

quat* c_quat(double, double, double, double);

mat4* c_mat4();

vec2 add_vec2(vec2, vec2);

vec3 add_vec3(vec3, vec3);

void add_vec3_mutable(vec3*, vec3*);

quat add_quat(quat, quat);

// subtraction

vec3 sub(vec3, vec3);

// multiplication

vec2 mult_vec2(vec2, double);

vec2 mut_vec22(vec2, vec2);

vec3 mult_vec3(vec3, double);

void mult_vec3_mutable(vec3*, double);

vec3 mult_vec33(vec3, vec3);

quat mult_quat(quat, double n);

quat mult_quatquat(quat, quat);

// dot products

double dot_vec2(vec2, vec2);

double dot_vec3(vec3, vec3);

double dot_quat(quat, quat);

// norms and vector normalization

double norm2_vec2(vec2);

double norm2_vec3(vec3);

double norm_vec3(vec3);

vec3 normalize(vec3);

void normalize_mutable(vec3*);

double norm2_quat(quat);

// vector math

vec3 cross(vec3, vec3);

vec3 normal_vector(vec3*, vec3*, vec3*);

double vec3_distance(vec3, vec3);

// quaternion math and helper functions

quat quat_from_axis_angle(vec3, double);

quat quat_from_euler_angles(double, double, double);

quat quat_conjugate(quat);

quat quat_mult(quat, quat);

vec3 quat_transform(quat, vec3);

void quat_to_matrix(quat, mat4);

// matrices

void mat4_zero(mat4);

void mat4_id(mat4);

void mat4_mult(mat4, mat4, mat4);

void mat4_translate(mat4, float, float, float);

void mat4_perspective(mat4, float, float, float);

void mat4_look_at(mat4, vec3, vec3, vec3);

// Camera

typedef struct {
    float fov, znear, zfar;
    vec3 from, to, up;
    mat4 v, p, vp;
} Camera;

void cam_update_view(Camera*, vec3*, vec3*, vec3*);

void cam_update_perspective(Camera*, float, float, float);

void cam_get_mvp(mat4, Camera*, mat4);

/*
 * color.c
 */

typedef struct {
    float r, g, b, a;
} color;

color color_interpolate(color, color, float);

color color_from_HSV(float, float, float);

/*
 * mesh.c
 */

enum MeshAttribute {
    VERT = 0,
    TRIS,
    NORM,
    UV,
    COLOR,
    NUMBER_ATTRIBUTES
};

enum ShaderUniformType {
    MATRIX_4FV,
    VECTOR_3FV,
    TEXTURE_2D
};

// shader uniform setter impl

typedef void (*_uniform_setter)(GLuint, int, void*);

// type definitions

typedef struct {
    GLuint handle;
    int count;
    _uniform_setter func;
    int hints;
} uniform_data;

typedef struct {
    list *attr[NUMBER_ATTRIBUTES];
    GLuint vbo[NUMBER_ATTRIBUTES];
} Mesh;

typedef struct {
    GLuint vert, frag, prog;
    GLuint handles[NUMBER_ATTRIBUTES];
    GLuint draw_mode;
    list *unif;
} Shader;

// shader construction

GLuint make_buffer(GLenum, const void*, GLsizei);

Shader* make_shader(char*, char*);

void free_shader(Shader*);

void map_shader_attrib(Shader*, int, char*);

uniform_data* map_shader_uniform(Shader*, int, char*, int);

// rendering

void draw_mesh(Shader*, Mesh*, list*);

// mesh construction, buffer packing and VBO busing

typedef GLuint (*_attrib_buffer_maker)(Mesh*);

void pack_vec3(list*, float[]);

void pack_vec2(list*, float[]);

void pack_ivec3(list*, short[]);

void pack_color(list*, float[]);

GLuint make_vert_buffer(Mesh*);

GLuint make_norm_buffer(Mesh*);

GLuint make_tri_buffer(Mesh*);

GLuint make_color_buffer(Mesh*);

GLuint make_uv_buffer(Mesh*);

Mesh* make_mesh();

void free_mesh(Mesh*);

void mesh_make_vbo(Mesh*);

void mesh_translate(Mesh*, vec3);

void mesh_scale(Mesh*, double n);

int mesh_add_point(Mesh*, vec3*);

int mesh_add_tri(Mesh*, ivec3*);

int mesh_add_normal(Mesh*, vec3*);

int mesh_add_uv(Mesh*, vec2*);

int mesh_add_color(Mesh*, color*);

void mesh_build_triangle(Mesh*, vec3*, vec3*, vec3*);

void mesh_build_quad(Mesh*, vec3*, vec3*, vec3*, vec3*);

Mesh* mesh_build_cube();

Mesh* mesh_build_plane();

void mesh_make_normals(Mesh*);

Mesh* simplify_mesh(Mesh*);

/*
 * game
 */

typedef struct {
    void   *data;
    void* (*create)  ();
    void  (*step)    (void*, double);
    void  (*draw)    (void*);
    void  (*destroy) (void*);
} Actor;

Actor* make_actor(void* (*) (), void (*) (void*, double), void (*) (void*), void (*) (void*));

void add_actor(Actor*);

list* set_actors(list*);

list* get_actors();

void start_game();

void step_scene(double);

void draw_scene();

void end_scene();

void end_game();

/*
 * render.c
 */

void print_gl_log(GLuint, PFNGLGETSHADERIVPROC, PFNGLGETSHADERINFOLOGPROC);

void check_gl_error();

GLuint _make_shader(GLenum, const char*);

GLuint _make_program(GLuint, GLuint);

void resize_callback(GLFWwindow*, int, int);

float get_aspect_ratio();

void add_key_callback(void (*) (GLFWwindow*, int, int, int, int));

void remove_key_callback(void (*) (GLFWwindow*, int, int, int, int));

void get_cursor_position(double*, double*);

GLFWwindow* make_window(int, int, char*);

void start_main_loop();

// extra util stuff

list* read_lines(const char*);

Mesh* read_obj(const char*);

Mesh* read_raw(const char*);

GLuint load_bmp(const char*);

/*
 * physics.c
 */

enum HitboxType {
    RECT = 0,
    SPHERE,
    PLANE,
    HITBOX_NUM
};

typedef struct {
    int type;
    void *hitbox;
} Hitbox;

typedef struct {
    Mesh *mesh;
    Shader *shader;
    list *uniforms;
    Hitbox *hitbox;
} Object;

struct rect_hitbox {
    vec3 pos;
    vec3 dim;
};

struct sphere_hitbox {
    vec3 pos;
    double r;
};

vec3 collide(struct sphere_hitbox, Hitbox);

vec3 collide_sphere_rect(struct sphere_hitbox, struct rect_hitbox);

vec3 collide_sphere_sphere(struct sphere_hitbox, struct sphere_hitbox);

#endif
