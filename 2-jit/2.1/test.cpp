// This program can not work.

#include <stdio.h>

unsigned char testf[] = {
    0x8b, 0x44, 0x24, 0x04, // mov eax, [esp+4]
    0x03, 0x44, 0x24, 0x08, // add eax, [esp+8]
    0xc3 // ret
};

int main() {
    auto test = reinterpret_cast<int(*)(int, int)>(&testf);
    printf("test(2, 3): %d\n", test(2, 3));
}
