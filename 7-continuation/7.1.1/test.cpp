#include <stdio.h>
#include <setjmp.h>

jmp_buf jb;

class Test {
    const char *s;
public:
    Test(const char *s) : s(s) {}
    ~Test() { printf("%s\n", s); }
};

void test() {
    Test t1("t1");
    longjmp(jb, 1);
}

int main() {
    Test t2("t2");
    if (setjmp(jb) == 0) {
        Test t3("t3");
        test();
    }
}
