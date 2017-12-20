CFLAGS = -c -Wall
CC = gcc
LIBS =  -lpthread

all: server
server: server.o 
	${CC} -pthread server.o -o server -std=c99

udp_server.o: server.c
	${CC} ${CFLAGS} server.c


all: client
client: client.o 
	${CC} -pthread client.o -o client -std=c99

udp_client.o: client.c
	${CC} ${CFLAGS} client.c


clean:
	rm -f *.o *~

		

