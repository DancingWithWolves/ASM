global _start

section .data

jump_table:
        dq simple
        times 60 dq 0 
        dq print_binary
        dq print_char
        dq print_decimal
        times 10 dq 0
        dq print_octal
        times 3 dq 0
        dq print_string
        times 4 dq 0
        dq print_hex


format:		db "I %s %x %d%%%c%b", 10, 0
string:		db "love", 0
buf: resb 64d

section .text

%define NEXT_PARAM_R8 R8
%define NEXT_FORMAT_SYMBOL_RBP RBP
%define BUFFER_SIZE_RBX RBX

%macro printf 1-*               ;macro with undefined qty of parameters
        %rep %0                 ;loop at all parameters 
                %rotate -1      ;%1 now is the last parameter
                push %1         ;push last param into stack
        %endrep
        
        call handle

        %rep %0                 ;clear stack
                add RSP, 4
        %endrep
%endmacro

_start:
        xor BUFFER_SIZE_RBX, BUFFER_SIZE_RBX
        printf format, string, 3802, 100, '!', 127

handle:
        mov NEXT_PARAM_R8, RSP
        add NEXT_PARAM_R8, 8                       ;[NEXT_PARAM_R8] now is the last pushed arg -- format address
        mov NEXT_FORMAT_SYMBOL_RBP, [NEXT_PARAM_R8]    ;NEXT_FORMAT_SYMBOL_RBP points to current format symbol
        add NEXT_PARAM_R8, 8                       ;NEXT_PARAM_R8 is current arg

        dec NEXT_FORMAT_SYMBOL_RBP
next:  
        inc NEXT_FORMAT_SYMBOL_RBP

        cmp byte [NEXT_FORMAT_SYMBOL_RBP], 0        ;program ends by 0-symbol in format string
        je end_program

        cmp byte [NEXT_FORMAT_SYMBOL_RBP], '%'      ;if not '%' print source format symbol
        jne simple
        
        inc NEXT_FORMAT_SYMBOL_RBP
        
        movzx R12, byte [NEXT_FORMAT_SYMBOL_RBP]
        jmp qword [8 * R12 - 8 * '%' + jump_table]

        ;cmp byte [NEXT_FORMAT_SYMBOL_RBP], '%'      ;case %%
        ;je simple

        ;cmp byte [NEXT_FORMAT_SYMBOL_RBP], 'b'
        ;je print_binary

        ;cmp byte [NEXT_FORMAT_SYMBOL_RBP], 'c'
        ;je print_char

        ;cmp byte [NEXT_FORMAT_SYMBOL_RBP], 'd'
        ;je print_decimal

        ;cmp byte [NEXT_FORMAT_SYMBOL_RBP], 'o'
        ;je print_octal

        ;cmp byte [NEXT_FORMAT_SYMBOL_RBP], 's'
        ;je print_string   

        ;cmp byte [NEXT_FORMAT_SYMBOL_RBP], 'x'
        ;je print_hex
;=============================================\
;adds next format string symbol to buffer
;=============================================|
simple:        
        mov RAX, [NEXT_FORMAT_SYMBOL_RBP]
        call to_buf
        jmp next
;=============================================/



;=============================================\
;adds a char located in next param to buffer
;=============================================|
print_char:
        mov RAX, [NEXT_PARAM_R8]
        add NEXT_PARAM_R8, 8
        call to_buf
        jmp next
;=============================================/

   
;=============================================\
;adds string to buffer
;=============================================|
print_string:
        mov RSI, [NEXT_PARAM_R8]
        add NEXT_PARAM_R8, 8
.str_loop:
        mov AL, [RSI]
        cmp AL, 0
        je next

        call to_buf
        inc RSI
        jmp .str_loop
;=============================================/


;=============================================\
;puts next agr as binary number to buf
;ENTRY: RAX -- value
;       CL -- base (degree of 2)
;DESTR: R13, R14, CH
;=============================================|
print_binary:
        mov RAX, [NEXT_PARAM_R8]
        add NEXT_PARAM_R8, 8
        mov CL, 1
        call print_degree
        jmp next
;=============================================/


;=============================================\
;puts next agr as octal number to buf
;ENTRY: RAX -- value
;       CL -- base (degree of 2)
;DESTR: R13, R14, CH
;=============================================|      
print_octal:
        mov RAX, [NEXT_PARAM_R8]
        add NEXT_PARAM_R8, 8
        mov CL, 3
        call print_degree
        jmp next
;=============================================/

;=============================================\
;puts next agr as hex number to buf
;ENTRY: RAX -- value
;       CL -- base (degree of 2)
;DESTR: R13, R14, CH
;=============================================|
print_hex:
        mov RAX, [NEXT_PARAM_R8]
        add NEXT_PARAM_R8, 8
        mov CL, 4
        call print_degree
        jmp next
;=============================================/

;=============================================\
;ENTRY: RAX -- value
;       CL -- base (degree of 2)
;DESTR: R13, R14, CH
;=============================================|
print_degree:
        mov CH, CL
        xor R13, R13            ;R13 -- corresponding base
.prepare_base:
        shl R13, 1
        inc R13
        dec CH
        cmp CH, 0
        ja .prepare_base

        xor R10, R10            ;R10 -- reversed value
        xor CH, CH              ;counter

.reverse_loop:
        mov R14, RAX
        and R14, R13

        shl R10, cl
        add R10, R14

        shr RAX, CL
        inc CH

        cmp RAX, 0
        jne .reverse_loop

.print_loop:
        mov R14, R10
        and R14, R13

        add R14, '0'

        cmp R14, '9'
        jna .digit
        add R14, 'A' - '0' - 10

.digit:
        mov RAX, R14
        call to_buf

        shr R10, CL
        dec CH
        cmp CH, 0
        jne .print_loop

        ret
;=============================================/


;=============================================\ 
;adds decimal number to buffer as symbols array
;=============================================| 
print_decimal:
        mov RAX, [NEXT_PARAM_R8]

        ;making the mask for last bit
        xor R9, R9      
        inc R9
        shl R9, 31d

        ;looking for the sign of the number
        mov R10, [NEXT_PARAM_R8]   
        and R10, R9
        cmp R9, R10
        jne .positive_decimal

        ;print minus
        push RAX
        mov RAX, '-'    
        call to_buf
        pop RAX

        ;get a positive value from negative
        not RAX
        inc RAX

        
.positive_decimal:
        mov R9, 10d
        call print_number
        add NEXT_PARAM_R8, 8
        jmp next
;=============================================/

;=============================================\
;Prints a positive decimal number
;ENTRY: R9 -- base
;       RAX -- number to print
;DESTR: CH, R10, R11, RAX
;=============================================|
print_number:
        xor R10, R10            ;reversed value
        xor CH, CH              ;number of digits
        mov R11, RAX

.reverse_loop:

        ;reverse digits in number
        mov RAX, R11
        div R9d

        shl R10, 4              ;log 2 16

        mov R11, RAX
        inc CH

        cmp R11, R9
        jae .reverse_loop

        shl R10, 4
        add R10, R11
        inc CH

.print_loop:
        mov RAX, R10
        and RAX, 1111b
        add RAX, '0'
        call to_buf

        shr R10, 4
        dec CH
        cmp CH, 0
        jne .print_loop

        ret
;=============================================/


;=============================================\
;Puts a symbol into buf
;ENTRY: RAX - char to put
;DESTR: RDI
;=============================================|
to_buf:
        cmp BUFFER_SIZE_RBX, 64d
        jae from_buf

        mov RDI, buf
        add RDI, BUFFER_SIZE_RBX
        stosb
        inc BUFFER_SIZE_RBX
        
        ret
;=============================================/

;=============================================\
;Prints a buffer's content on a screen
;=============================================|
from_buf:
        push RDX
        push RSI
        cmp BUFFER_SIZE_RBX, 0
        je .quit
        
        mov RDX, BUFFER_SIZE_RBX
        mov RSI, buf
        call printf_symbols
        xor BUFFER_SIZE_RBX, BUFFER_SIZE_RBX
.quit:
        pop RSI
        pop RDX
        ret
;=============================================/

;=============================================\
;prints RDX symbols from [RSI]
;ENTRY: RSI -- symbol address, RDX -- qty of symbols to print
;=============================================|
printf_symbols:
        push RAX
        push RDI

        xor RAX, RAX
        inc RAX

        xor RDI, RDI
        inc RDI

        syscall
        
        pop RDI
        pop RAX
        ret
;=============================================/

;=============================================\
;ends a program
;=============================================|
end_program:
        call from_buf
        mov RAX, 60
        xor RDI, RDI
        syscall
;=============================================/     
