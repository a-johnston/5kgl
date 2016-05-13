#ifndef __PHYS_UTIL

#define __PHYS_UTIL

#include "5kgl.h"

vec3 collide(struct sphere_hitbox player, Hitbox hbox) {
    switch (hbox.type) {
        case RECT:
            return collide_sphere_rect(player, *(struct rect_hitbox*) hbox.hitbox);
        case SPHERE:
            return collide_sphere_sphere(player, *(struct sphere_hitbox*) hbox.hitbox);
        case PLANE:
            break;
    }
    return player.pos;
}

vec3 collide_sphere_rect(struct sphere_hitbox player, struct rect_hitbox rect) {
    (void) rect;
    return player.pos;
}

vec3 collide_sphere_sphere(struct sphere_hitbox player, struct sphere_hitbox sphere) {
    if (vec3_distance(player.pos, sphere.pos) >= player.r + sphere.r) {
        return player.pos;
    }

    return add_vec3(sphere.pos, mult_vec3(normalize(sub(player.pos, sphere.pos)), player.r + sphere.r));
}

#endif
