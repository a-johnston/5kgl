#ifndef __MESH_UTIL

#define __MESH_UTIL

#include "space_math.h"
#include "util.h"

typedef struct {
    int vert_count;
    int tri_count;

    list *verts;
    list *tris;
    list *normals;
} mesh;

mesh make_mesh() {
    return (mesh) {
        0,
        0,
        create_list(),
        create_list(),
        create_list()
    };
}

void translate(mesh m, vec3 v) {
    for (int i = 0; i < m.vert_count; i++) {
        *((vec3*) list_get(m.verts, i)) = add(*((vec3*) list_get(m.verts, i)), v);
    }
}

void scale(mesh m, vec3 v) {
    for (int i = 0; i < m.vert_count; i++) {
        *((vec3*) list_get(m.verts, i)) = mult(*((vec3*) list_get(m.verts, i)), v);
    }
}

int add_point(mesh m, vec3 *p) {
    return list_add(m.verts, p);
}

int add_tri(mesh m, ivec3 *tri) {
    return list_add(m.tris, tri);
}

int add_normal(mesh m, vec3 *n) {
    return list_add(m.normals, n);
}

void make_normals(mesh m) {
    list_clear(m.normals);

    //clean slate
    vec3 *temp;
    for (int i = 0; i < m.verts->length; i++) {
        temp  = malloc(sizeof(vec3));
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

        *(vec3*) list_get(m.normals, tri.i) = add(*(vec3*) list_get(m.normals, tri.i), norm);
        *(vec3*) list_get(m.normals, tri.j) = add(*(vec3*) list_get(m.normals, tri.j), norm);
        *(vec3*) list_get(m.normals, tri.k) = add(*(vec3*) list_get(m.normals, tri.k), norm);
    }

    //ensure norm = 1
    for (int i = 0; i < m.normals->length; i++) {
        *(vec3*) list_get(m.normals, i) = normalize(*(vec3*) list_get(m.normals, i));
    }
}

#endif
