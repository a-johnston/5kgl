#ifndef __MESH_UTIL

#define __MESH_UTIL

#include "space_math.h"
#include "color.h"
#include "util.h"

typedef struct {
    int vert_count;
    int tri_count;

    list *verts;
    list *tris;
    list *normals;
    list *colors;
} mesh;

mesh make_mesh() {
    return (mesh) {
        0,
        0,
        create_list(),
        create_list(),
        create_list(),
        create_list()
    };
}

void mesh_translate(mesh m, vec3 v) {
    for (int i = 0; i < m.vert_count; i++) {
        *((vec3*) list_get(m.verts, i)) = add_vec3(*((vec3*) list_get(m.verts, i)), v);
    }
}

void mesh_scale(mesh m, vec3 v) {
    for (int i = 0; i < m.vert_count; i++) {
        *((vec3*) list_get(m.verts, i)) = mult_vec33(*((vec3*) list_get(m.verts, i)), v);
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
    vec3 *norm = (vec3*) malloc(sizeof(vec3));
    *norm = normal_vector(a, b, c);
    mesh_add_normal(m, norm);

    ivec3 *tri = (ivec3*) malloc(sizeof(ivec3));
    tri->i = mesh_add_point(m, a);
    tri->j = mesh_add_point(m, b);
    tri->k = mesh_add_point(m, c);
    mesh_add_tri(m, tri);
}

void mesh_build_quad(mesh m, vec3 *a, vec3 *b, vec3 *c, vec3 *d) {
    
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
