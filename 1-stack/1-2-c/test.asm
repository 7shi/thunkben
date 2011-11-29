.386
.MODEL FLAT
.CODE

EXTERN _test:NEAR

PUBLIC _entry
_entry:
	; test(test(1, 2), 3)
	sub esp, 8
	mov dword ptr [esp+4], 2
	mov dword ptr [esp], 1
	call _test
	mov dword ptr [esp+4], 3
	mov [esp], eax
	call _test
	add esp, 8

	ret

END
