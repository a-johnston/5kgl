#ifndef __GAME_UTIL

#define __GAME_UTIL

#include "5kgl.h"

list *actors;

Actor* make_actor(void (*create) (), void (*step) (double), void (*draw) ()) {
    Actor* actor = (Actor*) malloc(sizeof(Actor));
    *actor = (Actor) { create, step, draw };
    return actor;
}

list* set_actors(list *new_actors) {
    list* temp = actors;
    actors = new_actors;
    return temp;
}

list* get_actors() {
    return actors;
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

#endif
