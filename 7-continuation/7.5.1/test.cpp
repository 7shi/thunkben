#include <stdio.h>

void test() {
    int a;
    printf("\r&a: %p ", &a);
    fflush(stdout);
    test();
}

int main() {
    int a;
    printf("&a: %p\n", &a);
    test();
    return 0;
}
