
all: myhtpasswd

clean:
	rm -f		\
		*.o 	\
		myhtpasswd

myhtpasswd: myhtpasswd.o
	gcc -o myhtpasswd myhtpasswd.o -lstdc++
	strip myhtpasswd
	chmod 4711 myhtpasswd

myhtpasswd.o: myhtpasswd.cc
	gcc -c myhtpasswd.cc -std=c++11 -O3 -Wall -Wpedantic -Wconversion

