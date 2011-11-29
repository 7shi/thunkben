#include <stdio.h>

int test1(int a, int b) {
    return a + b;
}

_declspec(naked) int test2(int a, int b) {
    _asm {
        mov eax, 4[esp]
        add eax, 8[esp]
        ret
    }
}

int main() {
    printf("test1(2, 3): %d\n", test1(2, 3));
    printf("test2(2, 3): %d\n", test2(2, 3));
}
