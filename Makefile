build: DNASim.o
	gcc -Wall DNASim.o -o DNASim.exe
DNASim.o: DNASim.c
	gcc -Wall -c DNASim.c -o DNASim.o
run: DNASim.exe
	./DNASim.exe
clean:
	rm -rf DNASim.exe *~DNASim.exe DNASim.o
