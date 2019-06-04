
all: myhtpasswd

clean:
	rm -f		\
		*.o 	\
		myhtpasswd

myhtpasswd: myhtpasswd.o
	gcc -o myhtpasswd myhtpasswd.o
	strip myhtpasswd
	chmod 4711 myhtpasswd

myhtpasswd.o: myhtpasswd.c
	gcc -c myhtpasswd.c -std=c99 -O3

