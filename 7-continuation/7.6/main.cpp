#include <iostream>
#include "Coroutine.h"

using namespace std;

void test() {
    for (int i = 0; i <= 5; i++)
        yield(i);
}

int main() {
    Coroutine<int> cr = test;
    while (cr())
        cout << cr.value << endl;
}
