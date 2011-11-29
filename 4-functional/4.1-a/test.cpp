#include <stdio.h>

void test(int a) {
    printf("%d\n", a);
}

int main() {
    auto f = test;
    f(1);
}
