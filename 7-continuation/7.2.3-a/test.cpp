#include <stdio.h>

typedef struct {
    unsigned long eip, esp, ebp, ebx, edi, esi;
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
        jmp edx
    }
}

_declspec(naked) void _fastcall mylongjmp(myjmp_buf *jbuf, int value) {
    _asm {
        mov eax, edx
        mov esp, [ecx+ 4]
        mov ebp, [ecx+ 8]
        mov ebx, [ecx+12]
        mov edi, [ecx+16]
        mov esi, [ecx+20]
        jmp dword ptr [ecx]
    }
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
