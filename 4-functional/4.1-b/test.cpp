#include <stdio.h>

struct Test {
    static void show(int n) {
        printf("%d\n", n);
    }
};

int main() {
    auto f = Test::show;
    f(1);
}
