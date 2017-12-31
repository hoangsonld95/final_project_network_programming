#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "errorDefinition.h"
#include "processClientRequest.c"
#include "fileHandling.c"
#include "messageExchange.c"


struct sockaddr_in clientAddress;

void initializeSessions() {
	int index;

	for(index = 0; index < 100; index++) {
		sessions[index].sessionStatus = NOT_IDENTIFIED_USER;
	}

	return;
}


void printPredefinedSyntax() {

	printf("------------------------------------\n");
	printf("********ADMINISTRATION PRIVILEDGES*********\n");
	printf("1. ADD ROOM : [ADD NAME_OF_ROOM]\n");
	printf("2. REMOVE ROOM : [REMOVE NAME_OF_ROOM]\n");
	printf("3. ADD USER : [ADD NAME_OF_USER]\n");
	printf("4. REMOVE USER : [REMOVE NAME_OF_USER]\n");
	printf("5. SHOW ROOM RESULTS : [SHOW ROOM_RESULTS]\n");
	printf("6. EXIT THE PROGRAM\n");
	printf("------------------------------------\n");

}


void *connection_handler(void *socketTupple) {

	int connecting_socket;	
	char buffer[100];
	struct sockaddr_in clientAddress;
	int check = 1;

	connecting_socket = ((struct Socket_Tupple*)socketTupple)->connecting_socket;
	memcpy(&clientAddress, &(((struct Socket_Tupple*)socketTupple)->clientAddress), sizeof(clientAddress));

	printf("%d\n", connecting_socket);
	printf("%s\n", inet_ntoa(clientAddress.sin_addr));
	printf("%d\n", ntohs(clientAddress.sin_port));

	while(check) {

		check = receiveMessage(connecting_socket, buffer);
		printf("def: %s\n", buffer);
		processClientRequest(buffer, connecting_socket, clientAddress);

	}

	close(connecting_socket);

	return NULL;

}

int main(int argc, char const *argv[])
{
	pthread_t sniffer_thread;

	int listening_socket, connecting_socket;
	struct sockaddr_in serverAddress;
	struct Socket_Tupple *socketTupple; 

	socklen_t addrlen;
	int server_port;

	printPredefinedSyntax();

	listening_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(listening_socket < 0) {
		printf("socket() failed\n");
		return 0;
	}

	server_port = atoi(argv[1]);

	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(server_port);

	if(bind(listening_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		perror("\n Error: ");
		return 0;
	}

	if(listen(listening_socket, BACKLOG) < 0) {
		perror("\nError: ");
		return 0;
	}

	addrlen = sizeof(struct sockaddr_in);

	initializeSessions();

	while((connecting_socket = accept(listening_socket, (struct sockaddr*)&clientAddress, &addrlen))) {

		printf("You've got a connection from %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

		socketTupple = malloc(sizeof(struct Socket_Tupple));
		memset(socketTupple, '\0', sizeof(struct Socket_Tupple));

		socketTupple->connecting_socket = connecting_socket;
		memcpy(&(socketTupple->clientAddress), &clientAddress, sizeof(clientAddress));

		readAccountsFromFile();
		readRoomsFromFile();

		if(pthread_create(&sniffer_thread, NULL, connection_handler, (void*)socketTupple) < 0) {
			perror("pthread_create() failed");
			break;
		}

		printf("Thread handling is successful\n");

	}

	if(connecting_socket < 0) {
		perror("accept() failed");
		return 1;
	}

	return 0;
}