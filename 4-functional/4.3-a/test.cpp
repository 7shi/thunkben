#include <stdio.h>

struct Test {
    int n;
    Test(int n) : n(n) {}
    void operator()() {
        printf("%d\n", n);
    }
};

int main() {
    Test t(1);
    t();
}
