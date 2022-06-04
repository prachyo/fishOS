toy-os: kasm.o kc.o libc.o
	ld -m elf_i386 -T kernel/link.ld -o toy-os kasm.o kc.o libc.o

libc.o: stdio.o string.o stdlib.o

stdio.o: 
	gcc -m32 libc/stdio/printf.c libc/stdio/putchar.c -o stdio.o

string.o:
	gcc -m32 libc/string/memcmp.c libc/string/memcpy.c libc/string/memmove.c libc/string/memset.c libc/string/strlen.c -o string.o

stdlib.o:
	gcc -m32 libc/stdlib/abort.c -o stdlib.o

kc.o: kernel/kernel.c
	gcc -m32 -c kernel/kernel.c -o kc.o

kasm.o: kernel/kernel.s
	nasm -f elf32 kernel/kernel.s -o kasm.o

.PHONY: clean

clean:
	rm toy-os; rm kasm.o; rm kc.o
	
