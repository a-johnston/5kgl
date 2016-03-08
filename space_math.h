#ifndef __SPACE_MATH

#define __SPACE_MATH

#include <math.h>
#include <string.h>

#include "render.h"

double deg_to_rad = M_PI / 180.0;

/*
 * Types for 2D and 3D space operations
 */

typedef struct {
    double x, y;
} vec2;

typedef struct {
    double x, y, z;
} vec3;

typedef struct {
    int i, j, k;
} ivec3;

typedef struct {
    double x, y, z, w;
} quat;

typedef float mat4[16];

/*
 * Constructors
 */

vec2* c_vec2(double x, double y) {
    vec2* v = (vec2*) malloc(sizeof(vec2));
    *v = (vec2) { x, y };
    return v;
}

vec3* c_vec3(double x, double y, double z) {
    vec3* v = (vec3*) malloc(sizeof(vec3));
    //*v = (vec3) { x, y, z };
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}

quat* c_quat(double x, double y, double z, double w) {
    quat *q = (quat*) malloc(sizeof(quat));
    *q = (quat) { x, y, z, w };
    return q;
}

mat4* c_mat4() {
    mat4* m = (mat4*) malloc(sizeof(mat4));
    return m;
}

/*
 * Standard addition
 */

vec2 add_vec2(vec2 a, vec2 b) {
    return (vec2) {
        a.x + b.x,
        a.y + b.y
    };
}

vec3 add_vec3(vec3 a, vec3 b) {
    return (vec3) {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

void add_vec3_mutable(vec3 *a, vec3 *b) {
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
}

quat add_quat(quat a, quat b) {
    return (quat) {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w
    };
}

vec3 sub(vec3 a, vec3 b) {
    return (vec3) {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

/*
 * Scaling
 */

vec2 mult_vec2(vec2 a, double n) {
    return (vec2) { a.x * n, a.y * n };
}

vec2 mult_vec22(vec2 a, vec2 b) {
    return (vec2) { a.x * b.x, a.y * b.y };
}

vec3 mult_vec3(vec3 a, double n) {
    return (vec3) { a.x * n, a.y * n, a.z * n };
}

void mult_vec3_mutable(vec3 *a, double n) {
    a->x *= n;
    a->y *= n;
    a->z *= n;
}

vec3 mult_vec33(vec3 a, vec3 b) {
    return (vec3) { a.x * b.x, a.y * b.y, a.z * b.z };
}

quat mult_quat(quat a, double n) {
    return (quat) { a.x * n, a.y * n, a.z * n, a.w * n };
}

quat mult_quatquat(quat a, quat b) {
    return (quat) { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

/*
 * Dot products
 */

double dot_vec2(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}


double dot_vec3(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double dot_quat(quat a, quat b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

/*
 * Norms and normalization
 */

double norm2_vec2(vec2 a) {
    return a.x * a.x + a.y * a.y;
}

double norm2_vec3(vec3 a) {
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

double norm_vec3(vec3 a) {
    return sqrt(norm2_vec3(a));
}

vec3 normalize(vec3 a) {
    double n = norm_vec3(a);
    return n == 0 ? a : mult_vec3(a, 1.0 / n);
}

void normalize_mutable(vec3 *a) {
    double n = norm_vec3(*a);
    mult_vec3_mutable(a, 1.0 / n);
}

double norm2_quat(quat a) {
    return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
}

/*
 * 3D vector math
 */

vec3 cross(vec3 a, vec3 b) {
    return (vec3) {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

vec3 normal_vector(vec3 *a, vec3 *b, vec3 *c) {
    return normalize(cross(sub(*b, *a), sub(*c, *a)));
}

/*
 * Quaternion math and helpers
 */

quat quat_from_axis_angle(vec3 axis, double angle) {
    angle *= deg_to_rad / 2.0;
    double sina = sin(angle);
    return (quat) {
        axis.x * sina,
        axis.y * sina,
        axis.z * sina,
        cos(angle)
    };
}

quat quat_from_euler_angles(double yaw, double pitch, double roll) {
    yaw   *= deg_to_rad / 2.0;
    pitch *= deg_to_rad / 2.0;
    roll  *= deg_to_rad / 2.0;

    double shy = sin(yaw);
    double chy = cos(yaw);
    double shp = sin(pitch);
    double chp = cos(pitch);
    double shr = sin(roll);
    double chr = cos(roll);
    double chy_shp = chy * shp;
    double shy_chp = shy * chp;
    double chy_chp = chy * chp;
    double shy_shp = shy * shp;

    return (quat) {
        (chy_shp * chr) + (shy_chp * shr),
        (shy_chp * chr) - (chy_shp * shr),
        (chy_chp * shr) - (shy_shp * chr),
        (chy_chp * chr) + (shy_shp * shr)
    };
}

quat quat_conjugate(quat q) {
    return (quat) { -q.x, -q.y, -q.z, q.w };
}

quat quat_mult(quat a, quat b) {
    return (quat) {
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
        a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
    };
}

vec3 quat_transform(quat q, vec3 v) {
    quat qv = (quat) { v.x, v.y, v.z, 0.0f };
    qv = quat_mult(q, quat_mult(qv, quat_conjugate(q)));
    return (vec3) { q.x, q.y, q.z };
}

void quat_to_matrix(quat q, mat4 m) {
    m[0]  = (float) (1 - 2 * (q.y * q.y + q.z * q.z));
    m[1]  = (float)     (2 * (q.x * q.y + q.z * q.w));
    m[2]  = (float)     (2 * (q.x * q.z - q.y * q.w));
    m[3]  = 0.0f;

    m[4]  = (float)     (2 * (q.x * q.y - q.z * q.w));
    m[5]  = (float) (1 - 2 * (q.x * q.x + q.z * q.z));
    m[6]  = (float)     (2 * (q.y * q.z + q.x * q.w));
    m[7]  = 0.0f;

    m[8]  = (float)     (2 * (q.x * q.z + q.y * q.w));
    m[9]  = (float)     (2 * (q.y * q.z - q.x * q.w));
    m[10] = (float) (1 - 2 * (q.x * q.x + q.y * q.y));
    m[11] = 0.0f;

    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
}

/*
 * Matrices
 */

void mat4_zero(mat4 m) {
    memset(m, 0, sizeof(mat4));
}

void mat4_id(mat4 dest) {
    mat4_zero(dest);
    dest[0]  = 1;
    dest[5]  = 1;
    dest[10] = 1;
    dest[15] = 1;
}

void mat4_mult(mat4 dest, mat4 left, mat4 right) {
    mat4_zero(dest);
    int base;
    for (int i = 0; i < 16; i++) {
        base = i % 4;
        for (int j = 0; j < 4; j++) {
            dest[i] += left[i - base + j] * right[base + j * 4];
        }
    }
}

void mat4_translate(mat4 m, float x, float y, float z) {
    for (int i=0 ; i<4 ; i++) {
        m[12 + i] += m[i] * x + m[4 + i] * y + m[8 + i] * z;
    }
}

void mat4_perspective(mat4 matrix, float fov, float near, float far) {
    float f = 1.0f / tan(fov * deg_to_rad / 2.0);
    float frustum = near - far;

    mat4_zero(matrix);

    matrix[0]  = f / get_aspect_ratio();
    matrix[5]  = f;
    matrix[10] = (far + near) / frustum;
    matrix[11] = -1.0f;
    matrix[14] = (2.0f * far * near) / frustum;
}

void mat4_look_at(mat4 matrix, vec3 from, vec3 to, vec3 up) {
    mat4_zero(matrix);

    vec3 f = sub(to, from);
    f = normalize(f);

    vec3 s = cross(f, up);

    s = normalize(s);
    vec3 u = cross(s, f);

    matrix[0] = s.x;
    matrix[1] = u.x;
    matrix[2] = -f.x;

    matrix[4] = s.y;
    matrix[5] = u.y;
    matrix[6] = -f.y;

    matrix[8] = s.z;
    matrix[9] = u.z;
    matrix[10] = -f.z;

    matrix[15] = 1.0f;

    mat4_translate(matrix, -from.x, -from.y, -from.z);
}

/*
 * Combination
 */

#define lincomb(a, b, i, j) _Generic((a, b, i, j), \
    default: add(mult(a, i), mult(b, j)))

#define lerp(a, b, i) _Generic((a, b, i), \
    default: lincomb(a, b, i, 1 - i))

/*
 * Other helpers
 */

#define dist(a, b) _Generic((a, b), norm2(add(a, mult(b, -1.0))))

/*
 * Camera helpers
 */

typedef struct {
    float fov, znear, zfar;
    vec3 from;
    vec3 to;
    vec3 up;
    mat4 v;
    mat4 p;
    mat4 vp;
} Camera;

void __cam_update_vp(Camera *camera) {
    mat4_mult(camera->vp, camera->v, camera->p);
}

void cam_update_view(Camera *camera, vec3 *from, vec3 *to, vec3 *up) {
    if (from != NULL) {
        camera->from = *from;
    }
    if (to != NULL) {
        camera->to = *to;
    }
    if (up != NULL) {
        camera->up = *up;
    }

    mat4_look_at(camera->v, camera->from, camera->to, camera->up);
    __cam_update_vp(camera);
}

void cam_update_perspective(Camera *camera, float fov, float znear, float zfar) {
    camera->fov   = fov;
    camera->znear = znear;
    camera->zfar  = zfar;

    mat4_perspective(camera->p, fov, znear, zfar);
    __cam_update_vp(camera);
}

void cam_get_mvp(mat4 mvp, Camera *camera, mat4 model) {
    mat4_mult(mvp, model, camera->vp);
}

#endif
