
all:
	gcc -o tcp-server tcp-server.c -Wall
	gcc -o tcp-client tcp-client.c -Wall  
	gcc -o tcp-server-multithread tcp-server-multithread.c -Wall -lpthread

clean:
	rm -f tcp-server tcp-client tcp-server-multithread


