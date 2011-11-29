.386
.MODEL FLAT
.CODE

EXTERN _test:NEAR

PUBLIC _entry
_entry:
	sub esp, 16

	; int a = test(1, 2)
	mov dword ptr [esp+4], 2
	mov dword ptr [esp], 1
	call _test
	mov [esp+12], eax

	; int b = test(3, 4)
	mov dword ptr [esp+4], 4
	mov dword ptr [esp], 3
	call _test
	mov [esp+8], eax

	; test(a, b)
	mov [esp+4], eax
	mov eax, [esp+12]
	mov [esp], eax
	call _test

	add esp, 16

	ret

END
