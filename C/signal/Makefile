CC = gcc
OPTIONS = -O3 -std=c99
LIB = -ldbus-1 -lpthread -lrt
INCLUDE = -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include

all : client server

client:
	$(CC) $(LIB) $(INCLUDE) $(OPTIONS) -o client client.c

server:
	$(CC) $(LIB) $(INCLUDE) $(OPTIONS) -o server server.c

clean:
	rm -f client server
