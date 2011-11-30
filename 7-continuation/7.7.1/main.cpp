#include <iostream>
#include <string>
#include "Coroutine.h"

using namespace std;

void test1() {
    yield<string>("abc");
    yield<string>("1234");
    yield<string>("finish!");
}

void test2() {
    Coroutine<string> cr = test1;
    while (cr()) {
        auto s = cr.value;
        cout << s << ": ";
        yield<int>(s.size());
    }
}

int main() {
    Coroutine<int> cr = test2;
    while (cr())
        cout << cr.value << endl;
}
