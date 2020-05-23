locals @@
.model tiny
.code
org 100h

start:
		mov ax, 4c00h
		int 21h

;=======================================================================
; Entry: 	CX - array size, 
;			AH - byte to search, 
;			SI - pointer
; Exit:  	BA - index of first found element or ffffh if was not found
; Destr: 	BX, CX = 0 
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
; Entry: 	CX - size, 
;			BX - pointer to first array, 
;			SI - pointer to second array
; Exit:  	AL: 0 - equal, 
;				1 - first is bigger, 
;				2 - second is bigger
; Destr: 	CX, AH
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
; Entry: 	CX - size, 
;			DI - pointer to memory to set, 
;			AL - char to set
; Destr: 	CX
;=======================================================================

memset	proc
		cld
		rep stosb
		ret
		endp

;=======================================================================
; Entry: 	CX - size, 
;			DI - pointer to, 
;			SI - pointer from
; Destr: 	CX
;=======================================================================

memcpy	proc
		cld
@@next:		
		rep stosb
		ret
		endp

;=======================================================================
; Entry: 	SI - pointer to, 
;			AH - element to search
; Exit:  	BX - index if found or ffffh if not
; Destr: 	AL, AH
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
; Entry: 	BX - first string, 
;			SI - second string
; Exit:  	AL: 0 if equal, 
;				1 if first is above, 
;				2 if second is above
; Destr: 	AH, AL
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
; Entry: 	SI - str to search length of
; Exit:  	CX - length
; Destr: 	AL
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
; Entry: 	SI - from, 
;			DI - to
; Destr: 	AL
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