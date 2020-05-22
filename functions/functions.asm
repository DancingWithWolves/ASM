locals @@
.model tiny
.code
org 100h

start:
		mov ax, 4c00h
		int 21h

;=======================================================================
; Entry: cx - array size, ah - byte to search, si - pointer
; Exit:  bx - index of first found element or ffffh if was not found
;=======================================================================
memchr	proc
		cld
		mov bx, cx
@@next:		
		lodsb
		cmp ah, al
		je @@found
		
		loop @@next
		
		mov bx, 0ffffh		;nullptr
		ret
@@found:		
		sub bx, cx
		ret
		endp

;=======================================================================
; Entry: cx - size, bx - pointer to first array, si - pointer to second array
; Exit:  al: 0 - equal, 1 - first is bigger, 2 - second is bigger
;=======================================================================

memcmp	proc
		cld
		xor al, al
@@next:		
		lodsb
		mov ah, byte ptr[bx]
		inc bx
		
		cmp al, ah
		jb @@below
		cmp al, ah
		ja @@above
		
		loop @@next
		ret

@@above:	
		inc al
		ret
@@below:	
		inc al
		inc al
		ret
		endp

;=======================================================================
; Entry: cx - size, di - pointer to memory to set, al - char to set
;=======================================================================

memset	proc
		cld
		rep stosb
		ret
		endp

;=======================================================================
; Entry: cx - size, di - pointer to, si - pointer from
;=======================================================================

memcpy	proc
		cld
@@next:		
		lodsb
		stosb
		loop @@next
		ret
		endp

;=======================================================================
; Entry: si - pointer to, ah - element to search
; Exit:  bx - index if found or ffffh if not
;=======================================================================

strchr	proc
		xor bx, bx
@@next:		
		cld
		lodsb
		cmp al, 0
		je @@not_found
		cmp al, ah
		je @@found
		inc bx
		jmp @@next

@@not_found:	
		mov bx, 0ffffh	;nullptr
@@found:
		ret
		endp

;==========================================================================
; Entry: bx - first string, si - second string
; Exit:  al: 0 if equal, 1 if first is bigger, 2 if second
;==========================================================================

strcmp	proc
@@next:		
		cld
		lodsb
		mov ah, byte ptr[bx]
		inc bx

		cmp al, 0
		je @@end

		sub al, ah
		cmp al, 0
		ja @@firstAbove
		jb @@secondAbove
		jmp @@next
	
@@firstAbove:
		xor al, al
		inc al
		jmp @@end
@@secondAbove:
		xor al, al
		inc al
		inc al
@@end:
		ret
		endp

;=======================================================================
; Entry: si - str to search length of
; Exit:  cx - length
;=======================================================================

strlen	proc
		xor cx, cx
@@next:
		cld
		lodsb
		cmp al, 0
		je @@found
		inc cx
		jmp @@next

@@found:	
		ret
		endp

;=======================================================================
; Entry: si - from, di - to
; Destr: si, di, al
;=======================================================================

strcpy	proc
@@next:		
		cld
		lodsb
		stosb
		cmp al, 0
		je @@endstr
		jmp @@next
@@endstr:	
		ret
		endp

end start