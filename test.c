#include <5kgl.h>

int main() {
    list *l = read_lines("test.txt");
    for (int i = 0; i < l->length; i++) {
        printf("[%s]\n", (char*) list_get(l, i));
    }
    return 0;
}
