#ifndef __5kgl

#define __5kgl

#include "util.c"

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

// quaternion math and helper functions

quat quat_from_axis_angle(vec3, double);

#endif
