#ifndef __GAME_UTIL

#define __GAME_UTIL

#include "5kgl.h"

list *actors;

Actor* make_actor(void (*create) (), void (*step) (double), void (*draw) (), void (*destroy) ()) {
    Actor* actor = (Actor*) malloc(sizeof(Actor));
    *actor = (Actor) { create, step, draw, destroy };
    return actor;
}

void add_actor(Actor* actor) {
    list_add(actors, actor);
    (actor->create)();
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

    for (int i = 0; i < actors->length; i++) {
        (((Actor*) list_get(actors, i))->create)();
    }
}

void step_scene(double time) {
    if (!actors) {
        return;
    }

    for (int i = 0; i < actors->length; i++) {
        (((Actor*) list_get(actors, i))->step)(time);
    }
}

void draw_scene() {
    if (!actors) {
        return;
    }

    for (int i = 0; i < actors->length; i++) {
        (((Actor*) list_get(actors, i))->draw)();
    }
}

void end_scene() {
    if (!actors) {
        return;
    }

    for (int i = 0; i < actors->length; i++) {
        (((Actor*) list_get(actors, i))->destroy)();
    }

    list_clear(actors);
}

void end_game() {
    end_scene();
    list_free(actors);
}

#endif
