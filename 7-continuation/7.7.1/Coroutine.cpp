#include "Coroutine.h"

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

std::stack<CoroutineBase *> coroutines;

CoroutineBase::~CoroutineBase() {}
