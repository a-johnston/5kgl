#include "util.h"
#include "mesh.h"
#include "space_math.h"
#include "render.h"
#include <stdio.h>

void print_mat4(mat4 arr) {
    for (int i = 0; i < 16; i += 4) {
        printf("[ %f %f %f %f ]\n", arr[i], arr[i+1], arr[i+2], arr[i+3]);
    }
}

void print_quat(quat q) {
    printf("( %f %f %f %f )\n", q.x, q.y, q.z, q.w);
}

int main() {
    mat4 m = {0.0f};

    quat q = quat_from_euler_angles(16.0, 49.0, 112.0);
    quat_to_matrix(q, m);

    print_quat(q);
    print_mat4(m);
}
