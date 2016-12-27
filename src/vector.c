#ifndef __5KGL_VECTOR

#define __5KGL_VECTOR

#include "5kgl.h"

static void _ensure_capacity(Vector *vector, int capacity) {
    if (capacity > vector->capacity) {
        void *temp = realloc(
            vector->data,
            vector->element_size * vector->capacity * 2
        );

        if (temp) {
            vector->data = temp;
            vector->capacity = capacity;
        }
    }
}

Vector *vector_create(int element_size) {
    Vector *vector = (Vector*) malloc(sizeof(Vector));
    vector->data = calloc(10, element_size);
    vector->element_size = element_size;
    vector->capacity = 10;
    vector->length = 0;

    return vector;
}

void vector_free(Vector *vector) {
    free(vector->data);
    free(vector);
}

void vector_clear(Vector *vector) {
    vector->length = 0;
}

int vector_add(Vector *vector, void *e) {
    _ensure_capacity(vector, vector->length + 1);
    vector_set(vector, e, vector->length);
    return vector->length++;
}

void vector_add_all(Vector *vector, Vector *toAdd) {
    _ensure_capacity(vector, vector->length + toAdd->length);
    memcpy(
        vector->data + vector->length * vector->element_size,
        toAdd->data,
        toAdd->length * vector->element_size
    );
}

void vector_insert(Vector *vector, void *e, int i) {
    _ensure_capacity(vector, vector->length + 1);
    void *p = vector_get(vector, i);
    memmove(
        p + vector->element_size,
        p,
        vector->element_size * (vector->length - i - 1)
    );
    vector_set(vector, e, i);
    vector->length++;
}

void vector_remove(Vector *vector, int i) {
    void *p = vector_get(vector, i);
    memmove(
        p,
        p + vector->element_size,
        vector->element_size * (vector->length - i - 1)
    );
    vector->length--;
}

void vector_set(Vector *vector, void *e, int i) {
    memcpy(
        vector->data + i * vector->element_size,
        e,
        vector->element_size
    );
}

void *vector_get(Vector *vector, int i) {
    return vector->data + vector->element_size * i;
}

#endif
