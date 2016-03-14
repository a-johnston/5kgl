#ifndef __GL5K_UTIL

#define __GL5K_UTIL

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_LIST_CAPACITY 20

typedef struct {
    void **data;
    int capacity;
    int length;
} list;

list* create_list() {
    list* l = (list*) malloc(sizeof(list));

    l->data     = (void**) malloc(sizeof(void*) * DEFAULT_LIST_CAPACITY);
    l->capacity = DEFAULT_LIST_CAPACITY;
    l->length   = 0;

    return l;
}

int list_add(list *l, void *value) {
    if (l->length >= l->capacity) {
        l->capacity += DEFAULT_LIST_CAPACITY;
        l->data = (void**) realloc(l->data, sizeof(void*) * l->capacity);
    }

    l->data[l->length] = value;
    l->length += 1;
    return l->length - 1;
}

void* list_insert(list *l, void *value, int i) {
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
    for (int i = i; i < l->length - 1; i++) {
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

#endif
