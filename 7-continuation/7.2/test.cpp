#include <stdio.h>

typedef struct {
    unsigned long eip, esp, ebp, ebx, edi, esi;
} myjmp_buf;

_declspec(naked) int mysetjmp(myjmp_buf *jbuf) {
    _asm {
        mov edx, [esp+4]
        mov eax, [esp]
        mov [edx   ], eax
        mov [edx+ 4], esp
        mov [edx+ 8], ebp
        mov [edx+12], ebx
        mov [edx+16], edi
        mov [edx+20], esi
        xor eax, eax
        ret
    }
}

_declspec(naked) void mylongjmp(myjmp_buf *jbuf, int value) {
    _asm {
        mov eax, [esp+ 8]
        mov edx, [esp+ 4]
        mov ecx, [edx   ]
        mov esp, [edx+ 4]
        mov ebp, [edx+ 8]
        mov ebx, [edx+12]
        mov edi, [edx+16]
        mov esi, [edx+20]
        mov [esp], ecx
        ret
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
