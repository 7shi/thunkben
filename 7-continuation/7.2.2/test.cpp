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

myjmp_buf jb;

int main() {
    if (mysetjmp(&jb) == 0) {
        printf("setjmp\n");
        printf("test\n");
        mylongjmp(&jb, 1);
        printf("never reach here\n");
    }
    printf("longjmp done.\n");
}
