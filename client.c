#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "errorDefinition.h"
#include "struct.h"
#include "messageExchange.c"
#include "notification.c"

int receiveFileFromServer(int client_socket) {

	time_t intps;
	struct tm* tmi;
	char file_name[100];
	FILE *filePointer;
	char buffer[BUFFER_SIZE];
	int file_size;
	char results[100];
	int messageLength;

	int received_bytes, written_bytes;
	int count = 0;
	int check;

	bzero(file_name, 256);
	sprintf(file_name, "client.txt");
	printf("Creating the copied output file: %s\n", file_name);

	if((filePointer = fopen(file_name, "w+")) == NULL) {
		printf("fopen() failed\n");
		return 0;
	}

	check = 2;

	while((check = receiveMessage(client_socket, buffer)) <= 0) {

		memset(buffer, '\0', BUFFER_SIZE);

		fputs(buffer, filePointer);
	
	}

	printf("lll\n");

	fclose(filePointer);

}

int submitResult(int client_socket) {

	char results[100];
	int count = 0;
	int messageLength;
	char buffer[BUFFER_SIZE];
	
	struct timeval timeout;
	int input_ready= 0;
	int limitTime = TRAINING_TIME_LIMIT;	
	fd_set input_set;

	printf("You will get 15 sec to finish the training questions!. YOU CAN SUBMIT BEFORE TIME LIMIT.\nIF TIME EXCEEDS AND YOU DON'T SUBMIT, YOU GOT ZERO\n");

	printf("Enter your result \n");
	FD_SET(0, &input_set);
	timeout.tv_sec = limitTime;
	timeout.tv_usec = 0;
	input_ready = select(1, &input_set, NULL, NULL, &timeout);

	if(input_ready == -1) {
		printf("Cannot read your result\n");
		return -1;
	}

	if(input_ready) {
		memset(results, '\0', 100);
		fgets(results, 100, stdin);
		results[strlen(results) - 1] = '\0';
		sendMessage(client_socket, results);
		return 1;
	}

	else {
		strcpy(results, TIME_EXCEEDED);
		showMessage(TIME_EXCEEDED);
		return 0;
	}
	
	return 1;

}

int receiveResult(int client_socket) {

	int count = 0;

	recv(client_socket, &count, sizeof(int), 0);

	printf("You've got %d right answers / 5 questions\n", count);
	printf("You've just finished the training mode. Logging out training mode ...\n");
	printf("*****\n");
	printf("TYPE ONE OF THE FOLLOWING SYNTAXES TO CONTINUE: \n");
	printf("1. [TRAINING-MODE]\n");
	printf("2. [TESTING-MODE]\n");
	printf("*****\n");

}


int main(int argc, char const *argv[])
{
	int client_socket, server_port, bytes_sent, bytes_received;
	struct sockaddr_in serverAddress;
	socklen_t addrlen;

	char buffer[BUFFER_SIZE];
	char temporary[BUFFER_SIZE];
	char room_name[100];
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

		if(strcmp(buffer, TRAINING_REQUEST_ACCEPTED) == 0) {
			printf("TRAINING REQUEST ACCEPTED\n");
			// Receive training questions
			receiveFileFromServer(client_socket);

			printf("ttt\n");
			
			if(submitResult(client_socket)) {
				receiveResult(client_socket);
			}
			
		}

		if(strcmp(buffer, TESTING_REQUEST_ACCEPTED) == 0) {
			printf("TESTING REQUEST ACCEPTED\n");
			printf("Enter the room you want to test in: \n");
			bzero(room_name, 100);
			fgets(room_name, 100, stdin);
			room_name[strlen(room_name) - 1] = '\0';
			sendMessage(client_socket, room_name);
			bzero(temporary, BUFFER_SIZE);
			receiveMessage(client_socket, temporary);
			printf("%s\n", temporary);
			//receiveFileFromServer(client_socket);
		}

		else {
			showMessage(buffer);
		}

	}

	close(client_socket);


	return 0;
}