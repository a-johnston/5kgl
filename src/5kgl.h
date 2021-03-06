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
 * vector.c
 */

typedef struct {
    void *data;
    int element_size;
    int capacity;
    int length;
    void *fof;
} Vector;

Vector *vector_create(int element_size);

void vector_free(Vector *vector);

void vector_clear(Vector *vector);

int vector_add(Vector *vector, void *e);

int vector_add_p(Vector *vector, void *p);

void vector_add_all(Vector *vector, Vector *toAdd);

void vector_insert(Vector *vector, void *e, int i);

void vector_remove(Vector *vector, int i);

void vector_set(Vector *vector, void *e, int i);

void *vector_get(Vector *vector, int i);

void *vector_get_p(Vector *vector, int i);

/*
 * util.c
 */

Vector* split_string(char*, char*);

int hash_string(char*);

double clamp(double, double, double);

void* read_file(const char*, int*);

int int_mod(int i, int m);

/*
 * space_math.c
 */

//type definitions for vectors, quaternion, and matrix

typedef struct {
    float x, y;
} vec2;

typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    float x, y, z, w;
} quat;

typedef float mat4[16];

typedef struct {
    int i, j, k;
} ivec3;

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
    Vector *attr[NUMBER_ATTRIBUTES];
    GLuint vbo[NUMBER_ATTRIBUTES];
} Mesh;

typedef struct {
    GLuint vert, frag, prog;
    GLuint handles[NUMBER_ATTRIBUTES];
    GLuint draw_mode;
    Vector *unif;
} Shader;

// shader construction

GLuint make_buffer(GLenum, const void*, GLsizei);

Shader* make_shader(char*, char*);

void free_shader(Shader*);

void map_shader_attrib(Shader*, int, char*);

uniform_data* map_shader_uniform(Shader*, int, char*, int);

// rendering

void draw_mesh(Shader*, Mesh*, Vector*);

// mesh construction, buffer packing and VBO busing

Mesh* make_mesh();

void free_mesh(Mesh*);

void mesh_make_vbo(Mesh*);

void mesh_translate(Mesh*, vec3);

void mesh_scale(Mesh*, double n);

int mesh_add(Mesh*, int, void*);

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

void set_main_camera(Camera*);

Camera* get_main_camera();

Actor make_actor(void* (*) (), void (*) (void*, double), void (*) (void*), void (*) (void*));

void add_actor(Actor*);

void set_actors(Vector*);

Vector *get_actors();

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

void add_key_callback(GLFWkeyfun);

void remove_key_callback(GLFWkeyfun);

void get_cursor_position(double*, double*);

void get_cursor_delta(double*, double*);

GLFWwindow* make_window(int, int, char*);

void start_main_loop();

// extra util stuff

Vector* read_lines(const char*);

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
    MESH,
    HITBOX_NUM
};

typedef struct {
    int type;
    void *hitbox;
} Hitbox;

typedef struct {
    Mesh *mesh;
    Shader *shader;
    Vector *uniforms;
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
