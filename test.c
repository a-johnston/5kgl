#include "util.h"
#include "mesh.h"
#include "space_math.h"
#include "render.h"
#include <stdio.h>

int main() {
    mat4 m = {0};
    quat *q = c_quat(0, 0, 0, 1);

    quat_to_matrix(*q, m);
    
    mat4_perspective(m, 16.0f / 9.0f, 3.141592f / 4.0f, 1.0f, 10.0f);

    for (int i = 0; i < 16; i += 4) {
        printf("[ %f %f %f %f ]\n", m[i], m[i + 1], m[i + 2], m[i + 3]);
    }
}
