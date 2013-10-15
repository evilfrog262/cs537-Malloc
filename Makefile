all: 
	gcc -c -fpic mem.c -Wall -Werror
	gcc -shared -o libmem.so mem.o
	gcc -lmem -L. -o testProgram main.c -Wall -Werror	

clean:
	rm -rf *o
