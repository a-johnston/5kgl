#ifndef __GL5K_UTIL

#define __GL5K_UTIL

#include "5kgl.h"

list* split_string(char *str, char *pattern) {
    list *l = create_list();

    char *copy = strdup(str);
    char *substr;
    while ((substr = strsep(&copy, pattern))) {
        list_add(l, strdup(substr));
    }

    free(copy);

    return l;
}

// src: djb2 http://www.cse.yorku.ca/~oz/hash.html
int hash_string(char *str) {
    int hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

double clamp(double low, double value, double high) {
    return value > high ? high : (value < low ? low : value);
}

list* create_list() {
    list* l = (list*) malloc(sizeof(list));

    l->data     = (void**) malloc(sizeof(void*) * DEFAULT_LIST_CAPACITY);
    l->capacity = DEFAULT_LIST_CAPACITY;
    l->length   = 0;

    return l;
}

void _ensure_capacity(list *l, int length, int buffer) {
    if (l->capacity < length) {
        l->capacity = length + buffer;
        l->data = (void**) realloc(l->data, sizeof(void*) * l->capacity);
    }
}

int list_add(list *l, void *value) {
    _ensure_capacity(l, l->length + 1, DEFAULT_LIST_CAPACITY);
    l->data[l->length] = value;
    l->length += 1;
    return l->length - 1;
}

void list_add_all(list *l, list *other) {
    _ensure_capacity(l, l->length + other->length, 0);

    for (int i = 0; i < other->length; i++) {
        l->data[l->length + i] = other->data[i];
    }
    
    l->length += other->length;
}

void* list_insert(list *l, void *value, int i) {
    _ensure_capacity(l, i, 0);
    void *temp = l->data[i];
    l->data[i] = value;
    return temp;
}

void* list_get(list *l, int i) {
    return l->data[int_mod(i, l->length)];
}

int list_find(list *l, void *value) {
    for (int i = 0; i < l->length; i++) {
        if (l->data[i] == value) {
            return i;
        }
    }
    return -1;
}

void* list_iterate(list *l, int *i, void **value) {
    if (*i < l->length) {
        *value = l->data[*i];
        (*i)++;
        return *value;
    }
    return NULL;
}

void* list_remove(list *l, int i) {
    void *temp = l->data[i];
    for (; i < l->length - 1; i++) {
        l->data[i] = l->data[i + 1];
    }
    l->data[l->length - 1] = NULL;
    l->length -= 1;
    return temp;
}

int list_remove_element(list *l, void *value) {
    int i = list_find(l, value);

    if (i == -1) {
        return 0;
    }

    list_remove(l, i);

    return 1;
}

void _safe_free_element(list *l, void *p) {
    for (int i = 0; i < l->length; i++) {
        if (l->data[i] == p) {
            l->data[i] = NULL;
        }
    }

    free(p);
}

void list_clear(list *l) {
    for (int i = 0; i < l->length; i++) {
        _safe_free_element(l, l->data[i]);
    }

    l->length   = 0;
    l->capacity = DEFAULT_LIST_CAPACITY;
    l->data     = (void**) realloc(l->data, sizeof(void*) * l->capacity);
}

void list_free(list *l) {
    if (!l) {
        return;
    }

    for (int i = 0; i < l->length; i++) {
        _safe_free_element(l, l->data[i]);
    }

    free(l->data);
    l->data = NULL;
    free(l);
}

void list_free_keep_elements(list *l) {
    if (l) {
        free(l->data);
        free(l);
    }
}

ring_buffer* make_ring_buffer(int size) {
    ring_buffer *buffer = (ring_buffer*) malloc(sizeof(ring_buffer));
    *buffer = (ring_buffer) {
        (int*) malloc(size * sizeof(int)),
        size,
        0
    };
    return buffer;
}

void ring_buffer_free(ring_buffer *buffer) {
    free(buffer->data);
    free(buffer);
}

void ring_buffer_add(ring_buffer *ring, int i) {
    ring->data[int_mod(ring->i, ring->size)] = i;
    ring->i++;
}

int ring_buffer_get(ring_buffer *ring, int i) {
    return ring->data[int_mod((ring->i - 1) + i, ring->size)];
}

int int_mod(int i, int m) {
    if (i >= 0) {
        return i % m;
    } else {
        return m + (i % m);
    }
}

void* read_file(const char *filename, int *length) {
    FILE *f = fopen(filename, "r");
    void *buffer;

    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    *length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc(*length+1);
    *length = fread(buffer, 1, *length, f);
    fclose(f);
    ((char*)buffer)[*length] = '\0';

    return buffer;
}

list* read_lines(const char *filename) {
    int length;
    void *buffer = read_file(filename, &length);
    list *l = split_string((char*) buffer, "\n");
    list_remove(l, l->length - 1); // TODO this might be garbage, but files should end with newline
    free(buffer);
    return l;
}

Mesh* read_obj(const char *filename) {
    Mesh *mesh = make_mesh();

    list *verts = create_list();
    list *norms = create_list();
    list *uvs   = create_list();

    list *lines = read_lines(filename);
    for (int i = 0; i < lines->length; i++) {
        char *line = (char*) list_get(lines, i);

        list *parts = split_string(line, " ");

        char *tag = (char*) list_get(parts, 0);

        if (strcmp("v", tag) == 0) {
            // 5kgl does not support homogeneous coordinates in Mesh structs,
            // so this block ignores the 4th parameter if given
            vec3 v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            list_add(verts, &v);
        } else if (strcmp("vt", tag) == 0) {
            vec2 v;
            sscanf(line, "vt %f %f", &v.x, &v.y);
            list_add(uvs, &v);
        } else if (strcmp("vn", tag) == 0) {
            vec3 v;
            sscanf(line, "vn %f %f %f", &v.x, &v.y, &v.z);
            list_add(norms, &v);
        } else if (strcmp("f", tag) == 0) {
            ring_buffer *buffer = make_ring_buffer(2);

            int c = -1;
            int i = 1;
            void *str;

            while (list_iterate(parts, &i, &str)) {
                list *vert = split_string((char*) str, "/");

                // add point and add index to buffer
                ring_buffer_add(buffer, mesh_add(mesh, VERT, list_get(verts, atoi(list_get(vert, 0)) - 1)));

                // add UV information if it exists
                if (vert->length > 1 && strlen((char*) list_get(vert, 1)) > 0) {
                    mesh_add(mesh, UV, list_get(uvs, atoi(list_get(vert, 1)) - 1));
                }

                // add normal information if it exists
                if (vert->length > 2 && strlen((char*) list_get(vert, 2)) > 0) {
                    mesh_add(mesh, NORM, list_get(norms, atoi(list_get(vert, 2)) - 1));
                }

                if (buffer->i == 1) { // holds the first point as a reference for the convex hull
                    c = ring_buffer_get(buffer, 0);
                } else if (buffer->i > 2) { // builds the triangle assuming the ngon is a convex hull
                    ivec3 tri = (ivec3) {
                        c,
                        ring_buffer_get(buffer, 1),
                        ring_buffer_get(buffer, 0)
                    };
                    mesh_add(mesh, TRIS, &tri);
                }
                list_free(vert);
            }
            ring_buffer_free(buffer);
        }
        list_free(parts);
    }
    list_free(lines);

    list_free_keep_elements(verts);
    list_free_keep_elements(norms);
    list_free_keep_elements(uvs);

    return mesh;
}

Mesh* read_raw(const char *filename) {
    Mesh *mesh = make_mesh();

    float x1, y1, z1, x2, y2, z2, x3, y3, z3;

    list *lines = read_lines(filename);
    for (int i = 0; i < lines->length; i++) {
        sscanf((char*) list_get(lines, i), "%f %f %f %f %f %f %f %f %f",
            &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3);

        mesh_build_triangle(mesh,
            &(vec3) { x1, y1, z1 },
            &(vec3) { x2, y2, z2 },
            &(vec3) { x3, y3, z3 });
    }

    return mesh;
}

// src: https://github.com/opengl-tutorials/ogl/blob/master/common/texture.cpp
GLuint load_bmp(const char *filename) {
    FILE *f = fopen(filename, "rb");
    
    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return 0;
    }

    unsigned char  header[54];
    unsigned int   dataPos, width, height, size;
    unsigned char *data;

    if (fread(header, 1, 54, f) != 54
     || *(int*)&header[0x1E] != 0
     || *(int*)&header[0x1C] != 24) {
        fprintf(stderr, "Bad BMP file %s\n", filename);
        return 0;
    }

    dataPos = *(int*)&(header[0x0A]);
    size    = *(int*)&(header[0x22]);
    width   = *(int*)&(header[0x12]);
    height  = *(int*)&(header[0x16]);

    if (size == 0) {
        size = width * height * 3;
    }

    if (dataPos == 0) {
        dataPos = 54;
    }

    data = (unsigned char*) malloc(sizeof(char) * size);
    fread(data, 1, size, f);
    fclose(f);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

#endif
