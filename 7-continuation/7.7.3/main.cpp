#include <iostream>
#include "Coroutine.h"

using namespace std;

void test() {
    int n = 0;
    for (;;) n = yield(++n);
}

int main() {
    Coroutine<int> cr = test;
    if (cr()) cout << cr.value << endl;
    if (cr()) cout << cr.value << endl;
    cr.value = 5;
    if (cr()) cout << cr.value << endl;
}
