.386
.MODEL FLAT
.CODE

EXTERN _printf:NEAR

PUBLIC _entry
_entry:
	; Test t = { 2 };
	push 2

	; printf("%d\n", t.test(3));
	mov ecx, esp
	push 3
	call ?test@Test@@QAEHH@Z

	push eax
	push offset format
	call _printf
	add esp, 12

	ret

; struct Test { int a; int test(int b) { return a + b; } };
?test@Test@@QAEHH@Z:
    mov eax, DWORD PTR [ecx]
    add eax, DWORD PTR 4[esp]
    ret 4

.DATA
format DB '%d', 10, 0

END
