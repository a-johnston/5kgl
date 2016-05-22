#ifndef __GAME_UTIL

#define __GAME_UTIL

#include "5kgl.h"

static list *actors = NULL;

static Camera *main_camera = NULL;

static void **asset_store = NULL;

void set_main_camera(Camera *camera) {
    main_camera = camera;
}

Camera* get_main_camera() {
    return main_camera;
}

Actor* make_actor(void* (*create) (), void (*step) (void*, double), void (*draw) (void*), void (*destroy) (void*)) {
    Actor* actor = (Actor*) malloc(sizeof(Actor));
    *actor = (Actor) { NULL, create, step, draw, destroy };
    return actor;
}

void add_actor(Actor* actor) {
    list_add(actors, actor);
    if (actor->create) actor->data = actor->create();
}

list* set_actors(list *new_actors) {
    list* temp = actors;
    actors = new_actors;
    return temp;
}

list* get_actors() {
    return actors;
}

void start_game() {
    if (!actors) {
        return;
    }

    int i = 0;
    Actor *actor;
    while (list_iterate(actors, &i, (void*) &actor)) {
        if (actor->create) actor->create();
    }
}

void step_scene(double time) {
    if (!actors) {
        return;
    }

    int i = 0;
    Actor *actor;
    while (list_iterate(actors, &i, (void*) &actor)) {
        if (actor->step) actor->step(actor->data, time);
    }
}

void draw_scene() {
    if (!actors) {
        return;
    }

    int i = 0;
    Actor *actor;
    while (list_iterate(actors, &i, (void*) &actor)) {
        if (actor->draw) actor->draw(actor->data);
    }
}

void end_scene() {
    if (!actors) {
        return;
    }

    int i =0;
    Actor *actor;
    while (list_iterate(actors, &i, (void*) &actor)) {
        if (actor->destroy) actor->destroy(actor->data);
    }

    list_clear(actors);
}

void end_game() {
    end_scene();
    list_free(actors);
    free(asset_store);
}

void add_to_store(char *key, void *asset) {
    if (!asset_store) {
        asset_store = (void**) malloc(sizeof(void**) * DEFAULT_STORE_SIZE);
    }

    // TODO: make a half decent hashtable for this/ something similar
    asset_store[hash_string(key) % DEFAULT_STORE_SIZE] = asset;
}

void* get_from_store(char *key) {
    if (!asset_store) {
        return NULL;
    }

    return asset_store[hash_string(key) % DEFAULT_STORE_SIZE];
}

#endif
