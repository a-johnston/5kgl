#include "util.h"
#include "mesh.h"
#include "space_math.h"
#include "render.h"
#include <stdio.h>

int main() {
    int len;
    char *content = read_file("junk.txt", &len);

    vec3 v = (vec3) { 1.0, 1.0, 1.0 };
    vec3 w = (vec3) { 2.0, 3.0, 4.0 };

    list *vecList = create_list();

    list_add(vecList, &v);
    printf("%d\n", vecList->length);
    list_add(vecList, &w);

    printf("%d %d\n", vecList->length, list_find(vecList, &v));

    list_remove(vecList, 0);

    printf("%d %d\n", vecList->length, list_find(vecList, &v));

    list_insert(vecList, &v, 0);

    printf("%d %d\n", vecList->length, list_find(vecList, &v));

    v = mult(v, w);

    printf("%f %f %f\n", v.x, v.y, v.z);
}
