#ifndef __PHYSICS

#define __PHYSICS

#include "space_math.h"

typedef struct {
    vec3 position;
    quat rotation;
} transform;

vec3 raycast(vec3 from, vec3 direction, vec3 a, vec3 b, vec3 c) {
    return from;
}

vec3 collide_rect_prism(vec3 pos, double radius, vec3 min_corner, vec3 max_corner) {
    return pos;
}

#endif
