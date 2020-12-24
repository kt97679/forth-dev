CFLAGS= -O2 # -DBIG_ENDIAN
# Uncomment -DBIG_ENDIAN for big-endian processors (m68k etc)
CC=gcc

all: sod32 forth.img forth.glo

sod32: engine.o special.o term.o main.o 
	$(CC) -o sod32 engine.o special.o term.o main.o

engine.o: engine.c sod32.h

special.o: special.c sod32.h

main.o: main.c sod32.h

asm386: engine.S
	$(CC) -c engine.S;\
	make 

forth.img: extend.4 kernel.img 
	echo 'S" extend.4" INCLUDED '|./sod32 kernel.img 

kernel.img: kernel.4 cross.4
	echo 'S" cross.4" INCLUDED '|./sod32 forth.img

forth.glo: kernel.4 extend.4
	./sod32 forth.img < doglos.4

zip: 
	rm -i sod.zip;\
	zip sod special.c engine.c engine.S main.c kernel.img\
        kernel.4 cross.4 extend.4 Makefile sod32.h sod32.4 README\
        glosgen.4 doglos.4 sod32.doc term.c tetris.4
