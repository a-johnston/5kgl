#ifndef __5KGL_COLOR

#define __5kGL_COLOR

#include <math.h>

typedef float color[4];

color white = (color) { 1.0f, 1.0f, 1.0f, 1.0f };
color black = (color) { 0.0f, 0.0f, 0.0f, 1.0f };
color red   = (color) { 1.0f, 0.0f, 0.0f, 1.0f };
color green = (color) { 0.0f, 1.0f, 0.0f, 1.0f };
color blue  = (color) { 0.0f, 0.0f, 1.0f, 1.0f };

color color_interpolate(color a, color b, float i) {
    float j = 1 - i;
    return (color) {
        a[0] * i + b[0] * j,
        a[1] * i + b[1] * j,
        a[2] * i + b[2] * j
    };
}

color color_from_HSV(float hue, float sat, float val) {
    int i;
    float f, p, q, t;

    if (sat == 0) {
        return (color) { val, val, val };
    }

    hue *= 6.0f;
    i    = (int) hue;
    f    = hue - i;
    p    = val * (1 - sat);
    q    = val * (1 - sat * f);
    t    = val * (1 - sat * (1 - f));
}

#endif
