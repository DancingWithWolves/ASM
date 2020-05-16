debug: build
	edb --run a.out

run: build
	./a.out

build: printf.o
	ld printf.o
printf.o: printf.asm
	nasm -f elf64 printf.asm
clean:
	rm *.o