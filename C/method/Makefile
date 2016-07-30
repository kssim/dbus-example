CC = gcc
OPTIONS = -O3 -std=gnu99
LIB = -ldbus-1
INCLUDE = -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include

all : clean client server

client:
	$(CC) $(LIB) $(INCLUDE) $(OPTIONS) -o client client.c common.c

server:
	$(CC) $(LIB) $(INCLUDE) $(OPTIONS) -o server server.c common.c

clean:
	rm -f client server
