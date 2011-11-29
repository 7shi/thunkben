.386
.MODEL FLAT
.CODE

EXTERN _test:NEAR

PUBLIC _entry
_entry:
	; test(2, 3)
	sub esp, 8                ; esp -= 8
	mov dword ptr [esp+4], 3  ; *(dword *)(esp + 4) = 3
	mov dword ptr [esp], 2    ; *(dword *)(esp) = 2
	call _test                ; test()
	add esp, 8                ; esp += 8

	ret

END
