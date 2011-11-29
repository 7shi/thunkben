.386
.MODEL FLAT
.CODE

EXTERN _test:NEAR

PUBLIC _entry
_entry:
	; test(2, 3)
	push 3
	push 2
	call _test
	add esp, 8

	ret

END
