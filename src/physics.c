#ifndef __PHYS_UTIL

#define __PHYS_UTIL

#include "5kgl.h"

vec3 collide(struct sphere_hitbox player, Hitbox hbox) {
    switch (hbox.type) {
        case RECT:
            return collide_sphere_rect(player, *(struct rect_hitbox*) hbox.hitbox);
        case SPHERE:
            return collide_sphere_sphere(player, *(struct sphere_hitbox*) hbox.hitbox);
    }
    fprintf(stderr, "unimplemented hitbox collision\n");
    return player.pos;
}

vec3 collide_sphere_rect(struct sphere_hitbox player, struct rect_hitbox rect) {
    double dx, dy, dz;

    dx = player.pos.x - (rect.pos.x + rect.dim.x / 2);
    dy = player.pos.y - (rect.pos.y + rect.dim.y / 2);
    dz = player.pos.z - (rect.pos.z + rect.dim.z / 2);

    if (abs(dx) > player.r && abs(dy) > player.r && abs(dz) > player.r) {
        return player.pos;
    }

    dx /= rect.dim.x;
    dy /= rect.dim.y;
    dz /= rect.dim.z;

    if (abs(dx) > abs(dy) && abs(dx) > abs(dz)) { // collide x
        return (vec3) {
            dx > 0 ? rect.pos.x + rect.dim.x + player.r : rect.pos.x - player.r,
            player.pos.y,
            player.pos.z
        };
    } else if (abs(dy) > abs(dz)) { // collide y
        return (vec3) {
            player.pos.x,
            dy > 0 ? rect.pos.y + rect.dim.y + player.r : rect.pos.y - player.r,
            player.pos.z
        };
    } else { // collide z
        return (vec3) {
            player.pos.x,
            player.pos.y,
            dz > 0 ? rect.pos.z + rect.dim.z + player.r : rect.pos.z - player.r
        };
    }
}

vec3 collide_sphere_sphere(struct sphere_hitbox player, struct sphere_hitbox sphere) {
    if (vec3_distance(player.pos, sphere.pos) >= player.r + sphere.r) {
        return player.pos;
    }

    return add_vec3(sphere.pos, mult_vec3(normalize(sub(player.pos, sphere.pos)), player.r + sphere.r));
}

#endif
