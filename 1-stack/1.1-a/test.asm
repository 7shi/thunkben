.386
.MODEL FLAT
.CODE

PUBLIC _entry
_entry:
	push 1
	push 2
	push 3
	pop eax
	pop ebx
	pop ecx

	ret

END
