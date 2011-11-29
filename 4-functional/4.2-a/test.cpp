#include <stdio.h>

struct Test {
    int n;
    Test(int n) : n(n) {}
    void show() {
        printf("%d\n", n);
    }
};

int main() {
    Test t(1);
    auto f = &Test::show;
    (t.*f)();
}
