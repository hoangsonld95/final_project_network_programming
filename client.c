#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "errorDefinition.h"
#include "struct.h"

#define BUFFER_SIZE 1000

void showMessage(char buffer[]) {

	if(strcmp(buffer, USER_FOUND) == 0) {
		printf("USER NAME FOUND. CONTINUE TO ENTER PASSWORD\n");
		return;
	}

	else if(strcmp(buffer, USER_NOT_FOUND) == 0) {
		printf("USER NAME NOT FOUND. ENTER PASSWORD AGAIN\n");
		return;
	}

	else if(strcmp(buffer, PASSWORD_FALSE) == 0) {
		printf("PASSWORD IS FALSE. ENTER PASSWORD AGAIN\n");
	}

	else if(strcmp(buffer, PASSWORD_REQUIRED) == 0) {
		printf("PASSWORD IS REQUIRED. ENTER PASSWORD TO LOGIN\n");
		return;
	}

	else if(strcmp(buffer, USER_ALREADY_LOGINNED) == 0) {
		printf("USER HAS ALREADY LOGINNED\n");
		return;
	}

	else if(strcmp(buffer, TRAINING_REQUEST_ACCEPTED) == 0) {
		printf("TRAINING REQUEST ACCEPTED\n");
		return;
	}

	else if(strcmp(buffer, TRAINING_REQUEST_DENIED) == 0) {
		printf("TRAINING REQUEST DENIED\n");
		return;
	}

	else if(strcmp(buffer, TESTING_REQUEST_ACCEPTED) == 0) {
		printf("TESTING REQUEST ACCEPTED\n");
		return;
	}

	else if(strcmp(buffer, TESTING_REQUEST_DENIED) == 0) {
		printf("TESTING REQUEST DENIED\n");
		return;
	}

	else if(strcmp(buffer, USER_UNAUTHORIZED_ACTION) == 0) {
		printf("UNAUTHORIZED ACTION. PLEASE LOGIN\n");
		return;
	}

	else if(strcmp(buffer, LOGIN_SUCCESSFUL) == 0) {
		printf("LOGIN IS SUCCESSFUL\n");
		return;
	}

	else if(strcmp(buffer, LOGOUT_ACCEPTED) == 0) {
		printf("LOGOUT ACCEPTED\n");
		return;
	}

	else if(strcmp(buffer, LOGOUT_DENIED) == 0) {
		printf("LOGOUT DENIED\n");
		return;
	}

	else if(strcmp(buffer, LOGOUT_INVALID) == 0) {
		printf("HAVE NOT LOGINNED. WHY LOG OUT???\n");
		return;
	}

	else if(strcmp(buffer, LOGOUT_COMPULSORY) == 0) {
		printf("NEED TO LOGOUT TO LOGIN TO NEW ACCOUNT\n");
		return;
	}

	else if(strcmp(buffer, SYNTAX_WRONG) == 0) {
		printf("WRONG SYNTAX! PLEASE CHECK THE SYNTAX AGAIN\n");
	}

}

int main(int argc, char const *argv[])
{
	int client_socket, server_port, bytes_sent, bytes_received;
	struct sockaddr_in serverAddress;
	socklen_t addrlen;

	char buffer[BUFFER_SIZE];
	char user_name[100], password[100];
	int messageLength;

	if(argc != 3) {
		printf("Correct format: [./client SERVER_IP SERVER_PORT]\n");
	}

	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	if(client_socket < 0) {
		printf("socket() failed\n");
		return 0;
	}

	server_port = atoi(argv[2]);

	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(server_port);
	serverAddress.sin_addr.s_addr = inet_addr(argv[1]);

	addrlen = sizeof(serverAddress);

	if(connect(client_socket, (struct sockaddr*)&serverAddress, addrlen) < 0) {
		printf("connect() failed\n");
		return 0;
	}

	while(1) {

		printf("Enter command\n");
		memset(buffer, '\0', BUFFER_SIZE);
		fgets(buffer, BUFFER_SIZE, stdin);
		buffer[strlen(buffer)-1] = '\0';

		messageLength = strlen(buffer);
		bytes_sent = send(client_socket, &messageLength, sizeof(int), 0);
		bytes_sent = send(client_socket, buffer, messageLength, 0);

		bytes_received = recv(client_socket, &messageLength, sizeof(int), 0);
		bytes_received = recv(client_socket, buffer, messageLength, 0);

		buffer[bytes_received] = '\0';

		showMessage(buffer);

	}

	close(client_socket);


	return 0;
}