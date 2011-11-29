.386
.MODEL FLAT
.CODE

EXTERN _printf:NEAR

PUBLIC _entry
_entry:
	; printf("%d\n", test(2, 3))

	push 3
	push 2
	call _test
	add esp, 8

	push eax
	push offset format
	call _printf
	add esp, 8

	ret

_test:
	mov eax, [esp+4]
	add eax, [esp+8]
	; ret
	pop edx
	jmp edx

.DATA
format DB '%d', 10, 0

END
