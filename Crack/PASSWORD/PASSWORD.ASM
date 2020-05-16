.model tiny

.data
fail_msg: db 'Access denied', 13, 10, '$'
success_msg: db 'Access allowed', 13, 10, '$'
real_password: db 'let me iiiiin', 13, 10, '$'
buf: db '0000000000000000000000000000000000000000000000000000000000000000'
input: db '0000000000000000000'
not_a_password: db 'password', 13, 10, '$'
password: db 'not a password', 13, 10, '$'
do_not_look: db 'qwerty', 13, 10, '$'
go_away: db '123456qwerty', 13, 10, '$'


.code
org 100h
Locals @@

Start:
		xor dx, dx
		push dx

		mov cx, 20d
		mov di, offset input
		call gets
		
		mov cx, 4d
		mov di, cs 
		call getHash
		

		mov di, offset buf
		stosw
		mov al, '$'
		stosb

troll2:
		pop dx
		cmp dx, 0
		jne troll2
		jmp troll
troll:


		mov ax, cs
		mov ds, ax
		mov ah, 9h
		mov dx, offset buf
		int 21h

		mov dx, offset input
		int 21h

		mov si, offset buf
		mov di, offset input
		call strcmp
		
		mov ax, cs
		mov ds, ax

		mov ah, 9h
		cmp dh, 0h
		je success
		
fault:				
		mov dx, offset fail_msg
		int 21h
		jmp EndProg
success:
		mov dx, offset success_msg
		int 21h
		jmp EndProg

;===========================================================
; compares 2 strings
; ENTRY:ES:[DI] -- first address
;	ES:[SI] -- second address
; EXIT: DH = 0 of equals and 1 if not
;===========================================================
strcmp		proc
		xor dh, dh
		inc dh
@@loop:
		mov al, byte ptr es:[si]
		mov ah, byte ptr es:[di]
		cmp ah, al
		jne @@end
		
		cmp ah, '$'
		je @@positive_end
		inc di
		inc si
		jmp @@loop	
@@positive_end:
		dec dh			
@@end:
		ret
		endp
;===========================================================
; counts hash from es:[dx] address
; ENTRY:DI -- address to count from
;	CX -- qty of symbols to count
; EXIT: AX -- hash
;===========================================================
getHash		proc
		mov dx, offset real_password
		mov si, dx
		xor ax, ax
@@next:

		mov bh, byte ptr es:[di]
		inc dx	

		add ax, bx
		
		add ax, word ptr es:[si]
		inc si  
		loop @@next
		
		ret
		endp
;===========================================================
; puts a string from buffer to es:di until the end symbol
; ENTRY:ES:DI - buffer address,
;	CX -- max length
;	AH -- buffer size
;===========================================================
gets		proc
		xor ah, ah
		inc ah
		shl ah, 3

@@get_symbol:
		int 21h

		cmp al, 0Dh
		je @@end_gets	
		
		cmp al, 0Ah
		je @@end_gets
		
		cmp al, 24h
		je @@end_gets

		cmp al, 0h
		je @@end_gets
		
		stosb
		loop @@get_symbol
@@end_gets:	
		mov al, '$'
		stosb		
		ret
		endp
EndProg:
		mov ax, 4c00h
		int 21h
end Start