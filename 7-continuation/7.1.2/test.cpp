#include <stdio.h>
#include <setjmp.h>

jmp_buf jb1, jb2;
int i;

void test() {
    for (i = 1; i <= 10; i++)
        if (setjmp(jb2) == 0)
            longjmp(jb1, i);
    longjmp(jb1, -1);
}

int main() {
    int value = setjmp(jb1); // –{“–‚Í‚â‚Á‚Ä‚Í‚¢‚¯‚È‚¢
    if (value == 0)
        test();
    else if (value > 0) {
        printf("%d\n", value);
        longjmp(jb2, 1);
    }
}
