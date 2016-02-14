#ifndef __5KGL_COLOR

#define __5kGL_COLOR

#include <math.h>

typedef struct {
    float r;
    float g;
    float b;
    float a;
} color;

color COLOR_WHITE = (color) { 1.0f, 1.0f, 1.0f, 1.0f };
color COLOR_BLACK = (color) { 0.0f, 0.0f, 0.0f, 1.0f };
color COLOR_RED   = (color) { 1.0f, 0.0f, 0.0f, 1.0f };
color COLOR_GREEN = (color) { 0.0f, 1.0f, 0.0f, 1.0f };
color COLOR_BLUE  = (color) { 0.0f, 0.0f, 1.0f, 1.0f };

color COLOR_YELLOW = (color) { 1.0f, 1.0f, 0.0f, 1.0f };
color COLOR_PURPLE = (color) { 1.0f, 0.0f, 1.0f, 1.0f };
color COLOR_CYAN   = (color) { 0.0f, 1.0f, 1.0f, 1.0f };
color COLOR_BLANK  = (color) { 0.0f, 0.0f, 0.0f, 0.0f };




color color_interpolate(color a, color b, float i) {
    float j = 1 - i;
    return (color) {
        a.r * i + b.r * j,
        a.g * i + b.g * j,
        a.b * i + b.b * j
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

    switch (i) {
        case 0:
            return (color) { val, t, p };
        case 1:
            return (color) { q, val, p };
        case 2:
            return (color) { p, val, t };
        case 3:
            return (color) { p, q, val };
        case 4:
            return (color) { t, p, val };
    }

    return (color) { val, p, q };
}

#endif
