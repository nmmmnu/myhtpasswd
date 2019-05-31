
all: myhtpasswd

clean:
	rm *.o myhtpasswd

myhtpasswd: myhtpasswd.o
	gcc -o myhtpasswd myhtpasswd.o
	chmod 4711 myhtpasswd

myhtpasswd.o: myhtpasswd.c
	gcc -c myhtpasswd.c -std=c99

