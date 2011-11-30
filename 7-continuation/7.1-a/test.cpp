#include <stdio.h>
#include <setjmp.h>

jmp_buf jb;

int main() {
    if (setjmp(jb) == 0) {
        printf("setjmp\n");
        printf("test\n");
        longjmp(jb, 1);
        printf("never reach here\n");
    }
    printf("longjmp done.\n");
}
