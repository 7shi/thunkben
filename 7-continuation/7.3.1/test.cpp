#include <string.h>
#include <vector>
#include <functional>

typedef struct {
    unsigned long eip, esp, ebp, ebx, edi, esi;
    void *stack;
    int length;
} myjmp_buf;

_declspec(naked) int _fastcall mysetjmp(myjmp_buf *jbuf) {
    _asm {
        pop edx
        mov [ecx   ], edx
        mov [ecx+ 4], esp
        mov [ecx+ 8], ebp
        mov [ecx+12], ebx
        mov [ecx+16], edi
        mov [ecx+20], esi
        xor eax, eax
        mov [ecx+24], eax
        mov [ecx+28], eax
        jmp edx
    }
}

_declspec(naked) void _fastcall mylongjmp(myjmp_buf *jbuf, int value) {
    _asm {
        mov eax, edx
        mov edx, ecx
        mov esp, [edx+ 4]
        mov edi, esp
        mov esi, [edx+24]
        mov ecx, [edx+28]
        cld
        rep movsb
        mov ebp, [edx+ 8]
        mov ebx, [edx+12]
        mov edi, [edx+16]
        mov esi, [edx+20]
        jmp dword ptr [edx]
    }
}

void save_stack(std::vector<char> *dest, unsigned long last, myjmp_buf *callee) {
    callee->length = last - callee->esp;
    dest->resize(callee->length);
    callee->stack = &(*dest)[0];
    memcpy(callee->stack, (void *)callee->esp, callee->length);
}

template <class T> class Coroutine {
    myjmp_buf caller, callee;
    std::vector<char> stack;
    std::function<void()> f;
    int status;

public:
    T value;
    Coroutine() : status(0) {}
    Coroutine(const decltype(f) &f) : f(f), status(0) {}
    void operator=(const decltype(f) &f) { this->f = f; }

    bool operator()() {
        if (mysetjmp(&caller)) return true;
        switch (status) {
        case 0:
            status = 1;
            f();
            status = 3;
            break;
        case 2:
            status = 1;
            mylongjmp(&callee, 1);
        }
        return false;
    }

    T yield(T value) {
        this->value = value;
        if (mysetjmp(&callee) == 0) {
            status = 2;
            save_stack(&stack, caller.esp, &callee);
            mylongjmp(&caller, 1);
        }
        return this->value;
    }
};

#include <stdio.h>

Coroutine<int> cr;

void test() {
    for (int i = 0; i <= 5; i++)
        cr.yield(i);
}

int main() {
    cr = test;
    while (cr())
        printf("%d\n", cr.value);
    printf("end\n");
}
