#ifndef __GL5K_UTIL

#define __GL5K_UTIL

#include "5kgl.h"

list* split_string(char *str, char *pattern) {
    list *l = create_list();

    char *copy = strdup(str);
    char *substr;
    while ((substr = strsep(&copy, pattern))) {
        list_add(l, substr);
    }

    free(copy);

    return l;
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
    _ensure_capacity(l, l->length, DEFAULT_LIST_CAPACITY);
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
    return l->data[i];
}

int list_find(list *l, void *value) {
    for (int i = 0; i < l->length; i++) {
        if (l->data[i] == value) {
            return i;
        }
    }
    return -1;
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

void list_clear(list *l) {
    for (int i = 0; i < l->length; i++) {
        free(l->data[i]);
    }

    l->length   = 0;
    l->capacity = DEFAULT_LIST_CAPACITY;
    l->data     = (void**) realloc(l->data, sizeof(void*) * l->capacity);
}

void list_free(list *l) {
    list_clear(l);

    if (l->data != NULL) {
        free(l->data);
    }

    free(l);
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

    list *lines = read_lines(filename);
    for (int i = 0; i < lines->length; i++) {
        list *parts = split_string(list_get(lines, i), " ");

        char *tag = (char*) list_get(parts, 0);

        if (strcmp("v", tag) == 0) {

        } else if (strcmp("vt", tag) == 0) {

        } else if (strcmp("vn", tag) == 0) {

        } else if (strcmp("f", tag) == 0) {

        }

        list_free(parts);
    }
    list_free(lines);

    return mesh;
}

#endif
