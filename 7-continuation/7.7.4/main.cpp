#include <iostream>
#include "Coroutine.h"

using namespace std;

int main() {
    struct Test {
        int n;
        void test() {
            for (;;) yield(++n);
        }
    } test;
    Coroutine<int> cr = std::bind(&Test::test, &test);
    test.n = 0;
    if (cr()) cout << cr.value << endl;
    if (cr()) cout << cr.value << endl;
    test.n = 5;
    if (cr()) cout << cr.value << endl;
}
