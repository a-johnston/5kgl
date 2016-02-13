#ifndef __SPACE_MATH

#define __SPACE_MATH

#include <math.h>

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

#define add(x, y) _Generic((x), \
    vec2 : add_vec2, \
    vec3 : add_vec3, \
    quat : add_quat)(x, y)

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

vec3 mult_vec33(vec3 a, vec3 b) {
    return (vec3) { a.x * b.x, a.y * b.y, a.z * b.z };
}

quat mult_quat(quat a, double n) {
    return (quat) { a.x * n, a.y * n, a.z * n, a.w * n };
}

quat mult_quatquat(quat a, quat b) {
    return (quat) { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

#define mult(x, y) _Generic((y), \
    double : _Generic((x), \
        vec2 : mult_vec2, \
        vec3 : mult_vec3, \
        quat : mult_quat), \
    vec2 : mult_vec22, \
    vec3 : mult_vec33, \
    quat : mult_quatquat) (x, y)

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

#define dot(x, y) _Generic((x), \
    vec2 : dot_vec2 \
    vec3 : dot_vec3 \
    quat : dot_quat)(x, y)

/*
 * Norms and normalization
 */

double norm2_vec2(vec2 a) {
    return a.x * a.x + a.y * a.y;
}

double norm2_vec3(vec3 a) {
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

double norm2_quat(quat a) {
    return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
}

#define norm2(x) _Generic((x), \
    vec2 : norm2_vec2, \
    vec3 : norm2_vec3, \
    quat : norm2_quat)(x)

#define norm(x) _Generic((x), default: sqrt(norm2(x)))

#define normalize(x) _Generic((x), \
    default: ((norm2(x) == 0.0) ? x : mult(x, 1 / norm(x))))

/*
 * 3D vector math
 */

vec3 cross(vec3 a, vec3 b) {
    return (vec3) {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * a.x
    };
}

vec3 normal_vector(vec3 *a, vec3 *b, vec3 *c) {
    return normalize(cross(sub(*b, *a), sub(*c, *a)));
}

/*
 * Quaternion math and helpers
 */

quat quat_from_axis_angle(vec3 axis, double angle) {
    angle /= 2.0;
    double sina = sin(angle);
    return (quat) {
        axis.x * sina,
        axis.y * sina,
        axis.z * sina,
        cos(angle)
    };
}

quat quat_from_euler_angles(double yaw, double pitch, double roll) {
    yaw   /= 2.0;
    pitch /= 2.0;
    roll  /= 2.0;

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

void quat_to_matrix(quat q, mat4 m) {
    m[0]  = (float) (1 - 2 * (q.y * q.y + q.z * q.z));
    m[1]  = (float)     (2 * (q.x * q.y + q.z * q.w));
    m[2]  = (float)     (2 * (q.x * q.z + q.y * q.w));
    m[3]  = 0.0f;

    m[4]  = (float)     (2 * (q.x * q.y + q.z * q.w));
    m[5]  = (float) (1 - 2 * (q.x * q.x + q.z * q.z));
    m[6]  = (float)     (2 * (q.y * q.z + q.x * q.w));
    m[7]  = 0.0f;

    m[8]  = (float)     (2 * (q.x * q.z + q.y * q.w));
    m[9]  = (float)     (2 * (q.y * q.z + q.x * q.w));
    m[10] = (float) (1 - 2 * (q.x * q.x + q.y * q.y));
    m[11] = 0.0f;

    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
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

#endif
