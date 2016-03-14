#ifndef __MESH_UTIL

#define __MESH_UTIL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "5kgl.h"
#include "color.c"
#include "util.c"
#include "render.c"

/*
 * GLSL Uniform setters
 */

void _pass_gl_matrix_4fv(GLuint handle, int count, void *matrix) {
    glUniformMatrix4fv(handle, count, GL_FALSE, *((mat4*) matrix));
}

#endif

int _attrib_size[] = {3, 3, 3, 4};

GLuint make_buffer(
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

Shader* make_shader(char *vertex, char *fragment) {
    GLuint v = _make_shader(GL_VERTEX_SHADER, vertex);
    GLuint f = _make_shader(GL_FRAGMENT_SHADER, fragment);
    GLuint p = _make_program(v, f);

    Shader *shader = (Shader*) malloc(sizeof(Shader));
    shader->vert = v;
    shader->frag = f;
    shader->prog = p;

    for (int i = 0; i < NUMBER_ATTRIBUTES; i++) {
        shader->handles[i] = _NO_MAPPING;
    }

    shader->draw_mode = GL_TRIANGLES;
    shader->unif = create_list();

    return shader;
}

void map_shader_attrib(Shader *shader, int attrib, char *handle) {
    if (attrib == TRIS) {
        return;
    }

    int temp = glGetAttribLocation(shader->prog, handle);

    if (temp >= 0) {
        shader->handles[attrib] = temp;
    }
}

uniform_data* map_shader_uniform(Shader *shader, int type, char *handle, int count, void *pointer) {
    uniform_data *data = (uniform_data*) malloc(sizeof(uniform_data));
    data->handle = glGetUniformLocation(shader->prog, handle);
    data->count  = count;
    data->data   = pointer;

    switch (type) {
        case MATRIX_4FV:
            data->func = _pass_gl_matrix_4fv;
            list_add(shader->unif, data);
            break;
    }

    return data;
}

void _bind_program_mesh(Shader *shader, Mesh *mesh) {
    glUseProgram(shader->prog);

    for (int i = 0; i < NUMBER_ATTRIBUTES; i++) {
        if (mesh->vbo[i] != _NO_MAPPING && shader->handles[i] != _NO_MAPPING) {
            glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[i]);
            glVertexAttribPointer(shader->handles[i], _attrib_size[i], GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(shader->handles[i]);
        }
    }

    for (int i = 0; i < shader->unif->length; i++) {
        uniform_data *unif = (uniform_data*) list_get(shader->unif, i);
        (unif->func)(unif->handle, unif->count, unif->data);
    }
}

void _draw_mesh_tris(Shader *shader, Mesh *mesh) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[TRIS]);
    glDrawElements(shader->draw_mode, mesh->attr[TRIS]->length * 3, GL_UNSIGNED_SHORT, 0);
}

void _unbind_program_mesh(Shader *shader, Mesh *mesh) {
    for (int i = 0; i < NUMBER_ATTRIBUTES; i++) {
        if (mesh->vbo[i] != _NO_MAPPING && shader->handles[i] != _NO_MAPPING) {
            glDisableVertexAttribArray(shader->handles[i]);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void draw_mesh(Shader *shader, Mesh *mesh) {
    _bind_program_mesh(shader, mesh);
    _draw_mesh_tris(shader, mesh);
    _unbind_program_mesh(shader, mesh);
}

void pack_vec3(list *l, float d[]) {
    for (int i = 0; i < l->length; i++) {
        d[i * 3 + 0] = ((vec3*) l->data[i])->x;
        d[i * 3 + 1] = ((vec3*) l->data[i])->y;
        d[i * 3 + 2] = ((vec3*) l->data[i])->z;
    }
}

void pack_ivec3(list *l, short d[]) {
    for (int i = 0; i < l->length; i++) {
        d[i * 3 + 0] = ((ivec3*) l->data[i])->i;
        d[i * 3 + 1] = ((ivec3*) l->data[i])->j;
        d[i * 3 + 2] = ((ivec3*) l->data[i])->k;
    }
}

void pack_color(list *l, float d[]) {
    for (int i = 0; i < l->length; i++) {
        d[i * 3 + 0] = ((color*) l->data[i])->r;
        d[i * 3 + 1] = ((color*) l->data[i])->g;
        d[i * 3 + 2] = ((color*) l->data[i])->b;
        d[i * 3 + 3] = ((color*) l->data[i])->a;
    }
}

// TODO : tear apart the section under this
// ideas: hold onto type for each enumerated attrib?
//        redo vec3, color, etc to be arrays of a known length?

GLuint make_vert_buffer(Mesh *m) {
    float data[m->attr[VERT]->length * 3];
    pack_vec3(m->attr[VERT], data);
    return make_buffer(GL_ARRAY_BUFFER, data, sizeof(data));
}

GLuint make_norm_buffer(Mesh *m) {
    float data[m->attr[NORM]->length * 3];
    pack_vec3(m->attr[NORM], data);
    return make_buffer(GL_ARRAY_BUFFER, data, sizeof(data));
}

GLuint make_tri_buffer(Mesh *m) {
    short data[m->attr[TRIS]->length * 3];
    pack_ivec3(m->attr[TRIS], data);
    return make_buffer(GL_ARRAY_BUFFER, data, sizeof(data));
}

GLuint make_color_buffer(Mesh *m) {
    float data[m->attr[COLOR]->length * 4];
    pack_vec3(m->attr[COLOR], data);
    return make_buffer(GL_ARRAY_BUFFER, data, sizeof(data));
}

Mesh* make_mesh() {
    Mesh *m = (Mesh*) malloc(sizeof(Mesh));
    for (int i = 0; i < NUMBER_ATTRIBUTES; i++) {
        m->attr[i] = create_list();
        m->vbo[i] = _NO_MAPPING;
    }
    return m;
}

_attrib_buffer_maker _buffer_maker[] = {
    make_vert_buffer,
    make_tri_buffer,
    make_norm_buffer,
    make_color_buffer
};

GLuint _mesh_bufferer(Mesh *m, int i) {
    return _buffer_maker[i](m);
}

void mesh_make_vbo(Mesh *m) {
    for (int i = 0; i < NUMBER_ATTRIBUTES; i++) {
        if (m->attr[i]->length > 0) {
            m->vbo[i] = _mesh_bufferer(m, i);
        }
    }
}

void mesh_translate(Mesh *m, vec3 v) {
    for (int i = 0; i < m->attr[VERT]->length; i++) {
        *((vec3*) list_get(m->attr[VERT], i)) = add_vec3(*((vec3*) list_get(m->attr[VERT], i)), v);
    }
}

void mesh_scale(Mesh *m, double n) {
    for (int i = 0; i < m->attr[VERT]->length; i++) {
        vec3 j = *((vec3*) ((void**) m->attr[VERT])[i]);
        j.x *= n;
        j.y *= n;
        j.z *= n;
    }
}

int mesh_add_point(Mesh *m, vec3 *p) {
    return list_add(m->attr[VERT], p);
}

int mesh_add_tri(Mesh *m, ivec3 *tri) {
    return list_add(m->attr[TRIS], tri);
}

int mesh_add_normal(Mesh *m, vec3 *n) {
    return list_add(m->attr[NORM], n);
}

int mesh_add_color(Mesh *m, color *c) {
    return list_add(m->attr[COLOR], c);
}

void mesh_build_triangle(Mesh *m, vec3 *a, vec3 *b, vec3 *c) {
    ivec3 *tri = (ivec3*) malloc(sizeof(ivec3));
    tri->i = mesh_add_point(m, a);
    tri->j = mesh_add_point(m, b);
    tri->k = mesh_add_point(m, c);
    mesh_add_tri(m, tri);

    vec3 *norm = c_vec3(0, 0, 0);
    *norm = normal_vector(a, b, c);
    mesh_add_normal(m, norm);
    mesh_add_normal(m, norm);
    mesh_add_normal(m, norm);
}

void mesh_build_quad(Mesh *m, vec3 *a, vec3 *b, vec3 *c, vec3 *d) {
    mesh_build_triangle(m, a, b, c);
    mesh_build_triangle(m, d, c, b);
}

Mesh* mesh_build_cube() {
    vec3 *p0 = c_vec3(-1, -1, -1);
    vec3 *p1 = c_vec3( 1, -1, -1);
    vec3 *p2 = c_vec3(-1,  1, -1);
    vec3 *p3 = c_vec3( 1,  1, -1);
    vec3 *p4 = c_vec3(-1, -1,  1);
    vec3 *p5 = c_vec3( 1, -1,  1);
    vec3 *p6 = c_vec3(-1,  1,  1);
    vec3 *p7 = c_vec3( 1,  1,  1);

    Mesh *m = make_mesh();

    mesh_build_quad(m, p0, p2, p1, p3);
    mesh_build_quad(m, p1, p3, p5, p7);
    mesh_build_quad(m, p0, p4, p2, p6);
    mesh_build_quad(m, p0, p1, p4, p5);
    mesh_build_quad(m, p2, p6, p3, p7);
    mesh_build_quad(m, p4, p5, p6, p7);

    return m;
}

Mesh* mesh_build_plane() {
    vec3 *p0 = c_vec3(-0.5, -0.5, 0);
    vec3 *p1 = c_vec3( 0.5, -0.5, 0);
    vec3 *p2 = c_vec3(-0.5,  0.5, 0);
    vec3 *p3 = c_vec3( 0.5,  0.5, 0);

    Mesh *m = make_mesh();

    mesh_build_quad(m, p0, p2, p1, p3);

    return m;
}

void mesh_make_normals(Mesh *m) {
    list_clear(m->attr[NORM]);

    //clean slate
    vec3 *temp;
    for (int i = 0; i < m->attr[VERT]->length; i++) {
        temp  = (vec3*) malloc(sizeof(vec3));
        *temp = (vec3) { 0.0, 0.0, 0.0 };
        mesh_add_normal(m, temp);
    }

    //sum up contributing normals for "smooth" surfaces
    for (int i = 0; i < m->attr[TRIS]->length; i++) {
        ivec3 tri = *((ivec3*) list_get(m->attr[TRIS], i));
        vec3 norm = normal_vector(
            (vec3*) list_get(m->attr[VERT], tri.i),
            (vec3*) list_get(m->attr[VERT], tri.j),
            (vec3*) list_get(m->attr[VERT], tri.k));

        add_vec3_mutable((vec3*) list_get(m->attr[NORM], tri.i), &norm);
        add_vec3_mutable((vec3*) list_get(m->attr[NORM], tri.j), &norm);
        add_vec3_mutable((vec3*) list_get(m->attr[NORM], tri.k), &norm);
    }

    //ensure norm = 1
    for (int i = 0; i < m->attr[NORM]->length; i++) {
        normalize_mutable((vec3*) list_get(m->attr[NORM], i));
    }
}
