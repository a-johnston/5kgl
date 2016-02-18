#ifndef __MESH_UTIL

#define __MESH_UTIL

#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include "space_math.h"
#include "color.h"
#include "util.h"

typedef struct {
    list *verts;
    list *tris;
    list *normals;
    list *colors;
} mesh;

unsigned int make_buffer(
    GLenum target,
    const void *buffer_data,
    GLsizei buffer_size
) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
    return buffer;
}

void floatify_vec3(list l, float *d) {
    for (int i = 0; i < l.length; i++) {
        d[i * 3 + 0] = ((vec3*) l.data[i])->x;
        d[i * 3 + 1] = ((vec3*) l.data[i])->y;
        d[i * 3 + 2] = ((vec3*) l.data[i])->z;
    }   
}

void floatify_ivec3(list l, short *d) {
    for (int i = 0; i < l.length; i++) {
        d[i * 3 + 0] = ((ivec3*) l.data[i])->i;
        d[i * 3 + 1] = ((ivec3*) l.data[i])->j;
        d[i * 3 + 2] = ((ivec3*) l.data[i])->k;
    }   
}

mesh* make_mesh() {
    mesh* m = (mesh*) malloc(sizeof(mesh));
    *m = (mesh) {
        create_list(),
        create_list(),
        create_list(),
        create_list()
    };
    return m;
}

void mesh_translate(mesh m, vec3 v) {
    for (int i = 0; i < m.verts->length; i++) {
        *((vec3*) list_get(m.verts, i)) = add_vec3(*((vec3*) list_get(m.verts, i)), v);
    }
}

void mesh_scale(mesh m, double n) {
    for (int i = 0; i < m.verts->length; i++) {
        vec3 j = *((vec3*) ((void**) m.verts)[i]);
        j.x *= n;
        j.y *= n;
        j.z *= n;
    }
}

int mesh_add_point(mesh m, vec3 *p) {
    return list_add(m.verts, p);
}

int mesh_add_tri(mesh m, ivec3 *tri) {
    return list_add(m.tris, tri);
}

int mesh_add_normal(mesh m, vec3 *n) {
    return list_add(m.normals, n);
}

int mesh_add_color(mesh m, color *c) {
    return list_add(m.colors, c);
}

void mesh_build_triangle(mesh m, vec3 *a, vec3 *b, vec3 *c) {
    ivec3 *tri = (ivec3*) malloc(sizeof(ivec3));
    tri->i = mesh_add_point(m, a);
    tri->j = mesh_add_point(m, b);
    tri->k = mesh_add_point(m, c);
    mesh_add_tri(m, tri);
}

void mesh_build_quad(mesh m, vec3 *a, vec3 *b, vec3 *c, vec3 *d) {
    mesh_build_triangle(m, a, b, c);
    mesh_build_triangle(m, b, c, d);
}

mesh* mesh_build_cube() {
    vec3 p0 = (vec3) { -1, -1, -1 };
    vec3 p1 = (vec3) {  1, -1, -1 };
    vec3 p2 = (vec3) { -1,  1, -1 };
    vec3 p3 = (vec3) {  1,  1, -1 };
    vec3 p4 = (vec3) { -1, -1,  1 };
    vec3 p5 = (vec3) {  1, -1,  1 };
    vec3 p6 = (vec3) { -1,  1,  1 };
    vec3 p7 = (vec3) {  1,  1,  1 };

    mesh *m = make_mesh();

    mesh_build_quad(*m, &p0, &p2, &p1, &p3);
    mesh_build_quad(*m, &p1, &p3, &p5, &p7);
    mesh_build_quad(*m, &p0, &p4, &p2, &p6);
    mesh_build_quad(*m, &p0, &p1, &p4, &p5);
    mesh_build_quad(*m, &p2, &p6, &p3, &p7);
    mesh_build_quad(*m, &p4, &p5, &p6, &p7);

    return m;
}

mesh* mesh_build_test() {
    vec3 *p0 = c_vec3(-0.5, -0.5, 0);
    vec3 *p1 = c_vec3( 0.5, -0.5, 0);
    vec3 *p2 = c_vec3(-0.5,  0.5, 0);
    vec3 *p3 = c_vec3( 0.5,  0.5, 0);

    mesh *m = make_mesh();

    mesh_build_quad(*m, p0, p2, p1, p3);

    return m;
}

void mesh_make_normals(mesh m) {
    list_clear(m.normals);

    //clean slate
    vec3 *temp;
    for (int i = 0; i < m.verts->length; i++) {
        temp  = (vec3*) malloc(sizeof(vec3));
        *temp = (vec3) { 0.0, 0.0, 0.0 };
        list_add(m.normals, temp);
    }

    //sum up contributing normals for "smooth" surfaces
    for (int i = 0; i < m.tris->length; i++) {
        ivec3 tri = *((ivec3*) list_get(m.tris, i));
        vec3 norm = normal_vector(
            (vec3*) list_get(m.verts, tri.i),
            (vec3*) list_get(m.verts, tri.j),
            (vec3*) list_get(m.verts, tri.k));

        *(vec3*) list_get(m.normals, tri.i) = add_vec3(*(vec3*) list_get(m.normals, tri.i), norm);
        *(vec3*) list_get(m.normals, tri.j) = add_vec3(*(vec3*) list_get(m.normals, tri.j), norm);
        *(vec3*) list_get(m.normals, tri.k) = add_vec3(*(vec3*) list_get(m.normals, tri.k), norm);
    }

    //ensure norm = 1
    for (int i = 0; i < m.normals->length; i++) {
        *(vec3*) list_get(m.normals, i) = normalize(*(vec3*) list_get(m.normals, i));
    }
}

#endif
