.186
.model tiny

.code
		org 100h
		SHAPE_STYLE equ 30h
		SYMBOL_STYLE equ 4eh
		VIDEO_MEMORY equ 0b800h
		SCREEN_CENTRE equ 760d
		LINE_LENGTH equ 160d

Start:
		xor ax, ax		; mov ax, 0
		mov es, ax

		; Save old code
		cli
		mov bx, 9*4		; es:[bx] --> int 09H in ISR Table

		mov ax, word ptr es:[bx]
		mov Old09, ax
		mov ax, word ptr es:[bx+2]
		mov Old09+2, ax
		sti

		mov word ptr es:[bx], offset New09
		mov ax, cs		;current label
		mov word ptr es:[bx+2], ax
	
	
		; End program but don't unload it from RAM:
		mov ax, 3100H
		mov dx, 400H
		int 21h

;=============================================================
;Remains in RAM and cathes 09Int and prints scancodes with frame  
;Entry: AL - symbol to print
;	AH - symbol style	
;Destr: BX, CX, DX, DI, ES
;=============================================================
New09 		proc
		; Save registers:
		push ax bx cx dx di es

		mov ax, VIDEO_MEMORY
		mov es, ax

		in al, 60H		;scancode of pressed key
		mov ah, SYMBOL_STYLE
		call PrintSymbol

		; Restore registers
		pop es di dx cx bx ax

		; Data here intreprets as code
		db 0eaH		; jmp far
Old09 		dw 0		; jmp arguments
		dw 0
	
		iret
		endp

;=============================================================
;Prints a symbol in the centre of the screen with 1x1 frame around it  
;Entry: AL - symbol to print
;	AH - symbol style	
;Destr: BX, CX, DX, DI, ES
;=============================================================
PrintSymbol	proc
		push ax
		mov dx, 1		;width of a frame
		mov cx, 1		;height of a frame
		call DrawFrame
		
		pop ax
		mov di, SCREEN_CENTRE
		shl di, 1
		add di, LINE_LENGTH

		stosw

		ret
		endp


;=============================================================
;Prints a frame in the centre of the screen of specified size  
;Entry: DX - length of a frame
;	CX - height of a frame	
;Destr: AX
;=============================================================
DrawFrame	proc
		push cx
		push dx

		push VIDEO_MEMORY
		pop es		
		
		mov di, SCREEN_CENTRE	;centralization
		shl dx, 1
		sub di, dx		
		inc di
		shl di, 1
		shr dx, 1
					
		cld			;will ++di			
		
		mov al, 201d		;top line symbols
		mov bh, 205d		;
		mov bl, 187d		;
		
		push dx			;to save the length for next iterations
		push di			;same for pozition
		call DrawLine
		pop di			
		pop dx
		add di, LINE_LENGTH	;transition to the next line

		mov al, 186d		;middle line symbols
		mov bh, ' '
		mov bl, 186d
@@Next:					;loop for 'cx' middle lines
		push dx			;to save the length for next iterations
		push di			;same for pozition
		call DrawLine
		pop di
		pop dx		
		add di, LINE_LENGTH	;tranzition to the next line
		loop @@Next

		mov al, 200d		;bot line symbols
		mov bh, 205d
		mov bl, 188d
		
		push dx			;to save the length for next iterations
		push di			;same for pozition
		call DrawLine
		pop di		
		pop dx
		add di, LINE_LENGTH	;transition to the next line
		
		mov al, 0d
		mov bh, 0d
		mov bl, 0d

		push dx			;to save the length for next iterations
		push di			;same for pozition
		call DrawLine
		pop di
		pop dx
		add di, LINE_LENGTH	;transition to the next line

		pop dx			;saving data
		pop cx			;saving data
		ret
		endp
;===========================================
;Entry: ES - video mem
;	DI - start pozition
;	DX - length of the line
;	AH - style
;	AL - left symbol
;	BH - middle symbol
;	BL - right symbol
;Destr:	CX
;Exit:	DI - exit point 
;===========================================
DrawLine	proc
		push cx

		cmp al, 0h
		je LastIter

		mov cx, dx 	;stosw requires counter cx and it must be length
		stosw
		jmp WithoutFirstSymbol
		

LastIter:	
		mov ah, 0h
		mov cx, dx
		stosw
		mov ah, 30h		
		
WithoutFirstSymbol:
		mov al, bh
		rep stosw
		
		mov al, bl
		stosw
		
		cmp bl, 187d		;if first line
		je FirstIter
		
		mov cx, 2d		;print shadow
		mov al, 0h
		mov ah, SHAPE_STYLE
		stosw
		mov ah, 4eh
FirstIter:
		mov al, bl		;to save data
		pop cx				
		ret
		endp		
		
		iret	
		endp
end Start