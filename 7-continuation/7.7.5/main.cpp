#include <iostream>
#include "Coroutine.h"

using namespace std;

int main() {
    struct Test {
        int n;
        Test() : n(0) {}
        void operator()() {
            for (;;) yield(++n);
        }
    } test;
    Coroutine<int> cr1 = test;
    if (cr1()) cout << "cr1: " << cr1.value << endl;
    if (cr1()) cout << "cr1: " << cr1.value << endl;
    Coroutine<int> cr2 = test;
    if (cr2()) cout << "cr2: " << cr2.value << endl;
    if (cr2()) cout << "cr2: " << cr2.value << endl;
    cout << "test.n: " << test.n << endl;
}
