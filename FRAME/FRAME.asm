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
		;mov dx, 6d		even length of the frame
		;mov cx, 6d		even height of the frame
		
		mov si, 6d		;min(length, height)
		mov dx, 2d		;length - si + 1
		mov cx, 2d		;height - si + 1
Draw:
		cmp si, 0h		;animation loop
		je EndProg
		
		call DrawFrame
		inc cx			;new height
		inc dx			;new length
		dec si			;loop counter
Delay:
		push cx	
		push dx			;to save data
		
		mov cx, 1h		;main byte 0.25sec		
		mov dx, 0e848h		;second byte 0.25 sec		
		mov ah, 86h		;delay interruption
		int 15h			
		pop dx 
		pop cx
		
		jmp Draw		;animation loop
EndProg:		
		ret	

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
		
		mov di, SCREEN_CENTRE	
		shr dx, 1		;dx *= 2
		sub di, dx		;centralization
		shl dx, 1

		shl di, 1			
		cld			;will ++di
		
		mov ah, SYMBOL_STYLE		;symbol style
		
		
		mov al, 'É'		;top line symbols
		mov bh, 'Í'		;
		mov bl, '»'		;
		
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

end Start