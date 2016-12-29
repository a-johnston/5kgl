#ifndef __GAME_UTIL

#define __GAME_UTIL

#include "5kgl.h"

static Vector *actors = NULL;

static Camera *main_camera = NULL;

void set_main_camera(Camera *camera) {
    main_camera = camera;
}

Camera* get_main_camera() {
    return main_camera;
}

Actor make_actor(void* (*create) (), void (*step) (void*, double), void (*draw) (void*), void (*destroy) (void*)) {
    return (Actor) { NULL, create, step, draw, destroy };
}

void add_actor(Actor* actor) {
    vector_add(actors, actor);
}

void set_actors(Vector *new_actors) {
    if (actors) vector_free(actors);
    actors = new_actors;
}

Vector *get_actors() {
    return actors;
}

void start_game() {
    if (!actors) {
        return;
    }

    for (int i = 0; i < actors->length; i++) {
        Actor *actor = vector_get(actors, i);

        if (actor->create && !actor->data) {
            actor->data = actor->create();
        }
    }
}

void step_scene(double time) {
    if (!actors) {
        return;
    }

    for (int i = 0; i < actors->length; i++) {
        Actor *actor = vector_get(actors, i);
        if (actor->step) actor->step(actor->data, time);
    }
}

void draw_scene() {
    if (!actors) {
        return;
    }

    for (int i = 0; i < actors->length; i++) {
        Actor *actor = vector_get(actors, i);
        if (actor->draw) actor->draw(actor->data);
    }
}

void end_scene() {
    if (!actors) {
        return;
    }

    for (int i = 0; i < actors->length; i++) {
        Actor *actor = vector_get(actors, i);
        if (actor->destroy) actor->destroy(actor->data);
    }

    vector_clear(actors);
}

void end_game() {
    end_scene();
    vector_free(actors);
}

#endif
