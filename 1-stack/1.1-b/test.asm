.386
.MODEL FLAT
.CODE

PUBLIC _entry
_entry:
	; push 1
	sub esp, 4              ; esp -= 4
	mov dword ptr [esp], 1  ; *(dword *)esp = 1

	; push 2
	sub esp, 4              ; esp -= 4
	mov dword ptr [esp], 2  ; *(dword *)esp = 1

	; push 3
	sub esp, 4              ; esp -= 4
	mov dword ptr [esp], 3  ; *(dword *)esp = 3

	; pop eax
	mov eax, [esp]          ; eax = *(dword *)esp
	add esp, 4              ; esp += 4

	; pop ebx
	mov ebx, [esp]          ; ebx = *(dword *)esp
	add esp, 4              ; esp += 4

	; pop ecx
	mov ecx, [esp]          ; ecx = *(dword *)esp
	add esp, 4              ; esp += 4

	ret

END
