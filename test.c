#include "util.h"
#include "mesh.h"
#include "space_math.h"
#include "render.h"
#include <stdio.h>

int main() {
    mat4 m = {0};
    //quat *q = c_quat(0, 0, 0, 1);

    //quat_to_matrix(*q, m);

    vec3 f  = (vec3) { -0.57735026f, -0.57735026f, -0.57735026f };
    vec3 up = (vec3) { 0, 0, 1 };

    vec3 alt;

    alt.x = f.y * up.z - f.z * up.y;
    alt.y = f.z * up.x - f.x * up.z;
    alt.z = f.x * up.y - f.y * up.x;

    vec3 res = cross(f, up);

    printf("%f %f %f\n", res.x, res.y, res.z);
    printf("%f %f %f\n", alt.x, alt.y, alt.z);
    
    //mat4_perspective(m, 16.0f / 9.0f, 3.141592f / 4.0f, 1.0f, 10.0f);

    //for (int i = 0; i < 16; i += 4) {
    //    printf("[ %f %f %f %f ]\n", m[i], m[i + 1], m[i + 2], m[i + 3]);
    //}
}
