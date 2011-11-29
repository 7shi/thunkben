.386
.MODEL FLAT
.CODE

EXTERN _test:NEAR

PUBLIC _entry
_entry:
	; test(test(1, 2), test(3, 4))
	push 4
	push 3
	call _test
	add esp, 8
	push eax
	push 2
	push 1
	call _test
	add esp, 8
	push eax
	call _test
	add esp, 8

	ret

END
