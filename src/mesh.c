#ifndef __MESH_UTIL

#define __MESH_UTIL

#include "5kgl.h"

/*
 * GLSL Uniform setters
 */

void _pass_gl_matrix_4fv(GLuint handle, int count, void *matrix) {
    glUniformMatrix4fv(handle, count, GL_FALSE, matrix);
}

void _pass_gl_texture_2d(GLuint handle, int texUnit, void *texId) {
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, *(GLuint*)texId);
    glUniform1i(handle, texUnit);
}

const GLuint _NO_MAPPING = 0xdeadbeef;

int _attrib_size[] = {3, 3, 3, 2, 4};

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

void free_buffer(GLuint buffer) {
    glDeleteBuffers(1, &buffer);
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
    shader->unif = vector_create(sizeof(uniform_data));

    return shader;
}

void free_shader(Shader* shader) {
    glDeleteShader(shader->vert);
    glDeleteShader(shader->frag);
    glDeleteProgram(shader->prog);
    vector_free(shader->unif);
    free(shader);
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

uniform_data* map_shader_uniform(Shader *shader, int type, char *handle, int count) {
    uniform_data data;
    data.handle = glGetUniformLocation(shader->prog, handle);
    data.count  = count;
    data.hints  = 0;
    data.func = NULL;

    switch (type) {
        case MATRIX_4FV:
            data.func = _pass_gl_matrix_4fv;
            break;
        case TEXTURE_2D:
            data.func = _pass_gl_texture_2d;
            break;
    }

    if (data.func) {
        int i = vector_add(shader->unif, &data);
        return vector_get(shader->unif, i);
    } else {
        return NULL;
    }
}

static void bind_program_mesh(Shader *shader, Mesh *mesh, Vector *uniforms) {
    glUseProgram(shader->prog);

    for (int i = 0; i < NUMBER_ATTRIBUTES; i++) {
        if (mesh->vbo[i] != _NO_MAPPING && shader->handles[i] != _NO_MAPPING) {
            glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[i]);
            glVertexAttribPointer(shader->handles[i], _attrib_size[i], GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(shader->handles[i]);
        }
    }

    for (int i = 0; i < shader->unif->length; i++) {
        uniform_data *unif = (uniform_data*) vector_get(shader->unif, i);
        (unif->func)(unif->handle, unif->count, *(void**) vector_get(uniforms, i));
    }
}

static void draw_mesh_tris(Shader *shader, Mesh *mesh) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[TRIS]);
    // magic 3 likely won't work if not disjoint triangles, ie strips
    glDrawElements(shader->draw_mode, mesh->attr[TRIS]->length * 3, GL_UNSIGNED_INT, 0);
}

static void unbind_program_mesh(Shader *shader, Mesh *mesh) {
    for (int i = 0; i < NUMBER_ATTRIBUTES; i++) {
        if (mesh->vbo[i] != _NO_MAPPING && shader->handles[i] != _NO_MAPPING) {
            glDisableVertexAttribArray(shader->handles[i]);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void draw_mesh(Shader *shader, Mesh *mesh, Vector *uniforms) {
    bind_program_mesh(shader, mesh, uniforms);
    draw_mesh_tris(shader, mesh);
    unbind_program_mesh(shader, mesh);
}

static int bytesPerAttr[] = {
    sizeof(vec3),
    sizeof(ivec3),
    sizeof(vec3),
    sizeof(vec2),
    sizeof(color)
};

Mesh* make_mesh() {
    Mesh *m = (Mesh*) malloc(sizeof(Mesh));
    for (int i = 0; i < NUMBER_ATTRIBUTES; i++) {
        m->attr[i] = vector_create(bytesPerAttr[i]);
        m->vbo[i]  = _NO_MAPPING;
    }
    return m;
}

void free_mesh(Mesh *mesh) {
    for (int i = 0; i < NUMBER_ATTRIBUTES; i++) {
        vector_free(mesh->attr[i]);
        mesh->attr[i] = NULL;

        if (mesh->vbo[i] != _NO_MAPPING) {
            free_buffer(mesh->vbo[i]);
        }
    }
    free(mesh);
}

void mesh_make_vbo(Mesh *m) {
    for (int i = 0; i < NUMBER_ATTRIBUTES; i++) {
        if (m->attr[i]->length > 0) {
            int size = m->attr[i]->length * m->attr[i]->element_size;
            m->vbo[i] = make_buffer(GL_ARRAY_BUFFER, m->attr[i]->data, size);
        }
    }
}

void mesh_translate(Mesh *m, vec3 v) {
    for (int i = 0; i < m->attr[VERT]->length; i++) {
        *((vec3*) vector_get(m->attr[VERT], i)) = add_vec3(*((vec3*) vector_get(m->attr[VERT], i)), v);
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

void mesh_add_mesh(Mesh *m, Mesh *toAdd) {
    for (int i = 0; i < NUMBER_ATTRIBUTES; i++) {
        vector_add_all(m->attr[i], toAdd->attr[i]);
    }
}

int mesh_add(Mesh *m, int attr, void *p) {
    return vector_add(m->attr[attr], p);
}

void mesh_build_triangle(Mesh *m, vec3 *a, vec3 *b, vec3 *c) {
    ivec3 tri = {
        mesh_add(m, VERT, a),
        mesh_add(m, VERT, b),
        mesh_add(m, VERT, c)
    };
    mesh_add(m, TRIS, &tri);

    vec3 norm = normal_vector(a, b, c);
    mesh_add(m, NORM, &norm);
    mesh_add(m, NORM, &norm);
    mesh_add(m, NORM, &norm);
}

void mesh_build_quad(Mesh *m, vec3 *a, vec3 *b, vec3 *c, vec3 *d) {
    int va = mesh_add(m, VERT, a);
    int vb = mesh_add(m, VERT, b);
    int vc = mesh_add(m, VERT, c);
    int vd = mesh_add(m, VERT, d);

    ivec3 tri1 = { va, vb, vc };
    ivec3 tri2 = { vd, vc, vb };

    mesh_add(m, TRIS, &tri1);
    mesh_add(m, TRIS, &tri2);

    vec3 norm = normal_vector(a, b, c);

    mesh_add(m, NORM, &norm);
    mesh_add(m, NORM, &norm);
    mesh_add(m, NORM, &norm);
    mesh_add(m, NORM, &norm);
}

Mesh* mesh_build_cube() {
    vec3 p0 = (vec3) {-1, -1, -1};
    vec3 p1 = (vec3) { 1, -1, -1};
    vec3 p2 = (vec3) {-1,  1, -1};
    vec3 p3 = (vec3) { 1,  1, -1};
    vec3 p4 = (vec3) {-1, -1,  1};
    vec3 p5 = (vec3) { 1, -1,  1};
    vec3 p6 = (vec3) {-1,  1,  1};
    vec3 p7 = (vec3) { 1,  1,  1};

    Mesh *m = make_mesh();

    mesh_build_quad(m, &p0, &p2, &p1, &p3);
    mesh_build_quad(m, &p1, &p3, &p5, &p7);
    mesh_build_quad(m, &p0, &p4, &p2, &p6);
    mesh_build_quad(m, &p0, &p1, &p4, &p5);
    mesh_build_quad(m, &p2, &p6, &p3, &p7);
    mesh_build_quad(m, &p4, &p5, &p6, &p7);

    return m;
}

Mesh* mesh_build_plane() {
    vec3 p0 = (vec3) { -0.5, -0.5, 0 };
    vec3 p1 = (vec3) {  0.5, -0.5, 0 };
    vec3 p2 = (vec3) { -0.5,  0.5, 0 };
    vec3 p3 = (vec3) {  0.5,  0.5, 0 };

    Mesh *m = make_mesh();

    mesh_build_quad(m, &p0, &p2, &p1, &p3);

    return m;
}

void mesh_make_normals(Mesh *m) {
    vector_clear(m->attr[NORM]);

    //clean slate
    for (int i = 0; i < m->attr[VERT]->length; i++) {
        vec3 temp = (vec3) { 0.0, 0.0, 0.0 };
        mesh_add(m, NORM, &temp);
    }

    //sum up contributing normals for "smooth" surfaces
    for (int i = 0; i < m->attr[TRIS]->length; i++) {
        ivec3 tri = *((ivec3*) vector_get(m->attr[TRIS], i));
        vec3 norm = normal_vector(
            (vec3*) vector_get(m->attr[VERT], tri.i),
            (vec3*) vector_get(m->attr[VERT], tri.j),
            (vec3*) vector_get(m->attr[VERT], tri.k));

        add_vec3_mutable((vec3*) vector_get(m->attr[NORM], tri.i), &norm);
        add_vec3_mutable((vec3*) vector_get(m->attr[NORM], tri.j), &norm);
        add_vec3_mutable((vec3*) vector_get(m->attr[NORM], tri.k), &norm);
    }

    //ensure normalized
    for (int i = 0; i < m->attr[NORM]->length; i++) {
        normalize_mutable((vec3*) vector_get(m->attr[NORM], i));
    }
}

static int _point_in_mesh(vec3 *point, Mesh *mesh) {
    vec3 *temp;
    for (int i = 0; i < mesh->attr[VERT]->length; i++) {
        temp = (vec3*) vector_get(mesh->attr[VERT], i);

        if (point->x == temp->x && point->y == temp->y && point->z == temp->z) {
            return i;
        }
    }

    vec3 new = (vec3) { point->x, point->y, point->z };
    return mesh_add(mesh, VERT, &new);
}

Mesh* simplify_mesh(Mesh *old) {
    Mesh *new = make_mesh();

    ivec3 *tri;
    for (int i = 0; i < old->attr[TRIS]->length; i++) {
        tri = (ivec3*) vector_get(old->attr[TRIS], i);
        ivec3 newTri = (ivec3) {
            _point_in_mesh((vec3*) vector_get(old->attr[VERT], tri->i), new),
            _point_in_mesh((vec3*) vector_get(old->attr[VERT], tri->j), new),
            _point_in_mesh((vec3*) vector_get(old->attr[VERT], tri->k), new)
        };
        mesh_add(new, TRIS, &newTri);
    }

    free_mesh(old);
    mesh_make_normals(new);
    return new;
}

#endif
