#include <stdio.h>
#include <setjmp.h>

jmp_buf jb;

void test1(const char *s) {
    setjmp(jb);
    printf("&s: %p, s: %p; %s\n", &s, s, s);
}

void test2(int a) {
    printf("&a: %p, a: %d\n", &a, a);
    longjmp(jb, 0);
}

int main() {
    test1("test");
    test2(0);
    printf("end\n");
}
