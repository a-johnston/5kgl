#include <5kgl.h>

int main() {
    list *l = split_string("test split string", " ");
    for (int i = 0; i < l->length; i++) {
        printf("%s\n", (char*) list_get(l, i));
    }
    return 0;
}
