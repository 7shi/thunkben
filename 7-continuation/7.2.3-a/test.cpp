#include <stdio.h>
#include <string.h>
#include <vector>

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

myjmp_buf jb1, jb2;
int i, *p;

void test() {
    for (i = 1; i <= 10; i++)
        if (mysetjmp(&jb2) == 0) {
            for (p = (int *)jb2.esp; p < (int *)jb1.esp; p++)
                printf("%p: %p\n", p, *p);
            mylongjmp(&jb1, i);
        }
    mylongjmp(&jb1, -1);
}

int main() {
    int value = mysetjmp(&jb1);
    if (value == 0)
        test();
    else if (value > 0) {
        printf("%d\n", value);
        mylongjmp(&jb2, 1);
    }
}
