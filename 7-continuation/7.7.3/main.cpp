#include <iostream>
#include "Coroutine.h"

using namespace std;

int main() {
    int n = 0;
    Coroutine<int> cr = [&] {
        for (;;) yield(++n);
    };
    if (cr()) cout << cr.value << endl;
    if (cr()) cout << cr.value << endl;
    n = 5;
    if (cr()) cout << cr.value << endl;
}
