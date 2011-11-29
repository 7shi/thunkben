#include <stdio.h>

int main() {
    int n = 1;
    auto t = [n] { printf("%d\n", n); };
    t();
}
