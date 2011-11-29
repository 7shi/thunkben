.386
.MODEL FLAT
.CODE

EXTERN _printf:NEAR

PUBLIC _entry
_entry:
	; printf("%d\n", test(2, 3))

	mov edx, 3
	mov ecx, 2
	call _test

	push eax
	push offset format
	call _printf
	add esp, 8

	ret

; int _fastcall test(int a, int b) { return a + b; }
_test:
    mov eax, ecx
    add eax, edx
	ret

.DATA
format DB '%d', 10, 0

END
