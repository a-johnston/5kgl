#ifndef __5KGL_COLOR

#define __5kGL_COLOR

#include "5kgl.h"

color color_interpolate(color a, color b, float i) {
    float j = 1 - i;
    return (color) {
        a.r * i + b.r * j,
        a.g * i + b.g * j,
        a.b * i + b.b * j,
        a.a * i + b.a * j
    };
}

color color_from_HSV(float hue, float sat, float val) {
    int i;
    float f, p, q, t;

    if (sat == 0) {
        return (color) { val, val, val, 1 };
    }

    hue *= 6.0f;
    i    = (int) hue;
    f    = hue - i;
    p    = val * (1 - sat);
    q    = val * (1 - sat * f);
    t    = val * (1 - sat * (1 - f));

    switch (i) {
        case 0:
            return (color) { val, t, p, 1 };
        case 1:
            return (color) { q, val, p, 1 };
        case 2:
            return (color) { p, val, t, 1 };
        case 3:
            return (color) { p, q, val, 1 };
        case 4:
            return (color) { t, p, val, 1 };
    }

    return (color) { val, p, q, 1 };
}

#endif
