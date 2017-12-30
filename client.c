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

#define BUFFER_SIZE 200

int receiveFileFromServer(int client_socket) {

	time_t intps;
	struct tm* tmi;
	char file_name[100];
	FILE *filePointer;
	char buffer[BUFFER_SIZE];
	int file_size;

	int received_bytes, written_bytes;
	int count = 0;

	bzero(file_name, 256);
	intps = time(NULL);
	tmi = localtime(&intps);
	//sprintf(file_name, "client.%d.%d.%d.%d.%d.%d.txt", tmi->tm_mday, tmi->tm_mon+1, 1900+tmi->tm_year, tmi->tm_hour, tmi->tm_min, tmi->tm_sec);
	sprintf(file_name, "client.txt");
	printf("Creating the copied output file: %s\n", file_name);

	if((filePointer = fopen(file_name, "w")) == NULL) {
		printf("fopen() failed\n");
		return 0;
	}

	do {

		memset(buffer, '\0', BUFFER_SIZE);
		received_bytes = recv(client_socket, buffer, BUFFER_SIZE, 0);
		if(received_bytes < 0) {
			printf("Error! Cannot receive data from client\n");
			close(client_socket);
			break;
		}

		else {
			buffer[strlen(buffer)] = '\0';
			printf("%s", buffer);
			fputs(buffer, filePointer);
			memset(buffer, '\0', BUFFER_SIZE);

			if(received_bytes == 0) {
				break;
			}
			if(received_bytes > 0 && received_bytes < 100) {
				break;
			}
		}

	} while(received_bytes > 0);

	//fputs(EOF, filePointer);

	fclose(filePointer);

	return 1;

}

int sendResultsToServer(int client_socket) {

	char results[100];
	int bytes_sent;

	printf("Enter your results (separated by space respectively)\n");
	fgets(results, 100, stdin);
	results[strlen(results) - 1] = '\0';

	bytes_sent = send(client_socket, results, strlen(results), 0);
	if(bytes_sent == -1) {
		printf("Error! Cannot send results to server\n");
		return -1;
	}

	printf("Results has been sended\n");


}

void showMessage(char buffer[]) {

	if(strcmp(buffer, USER_FOUND) == 0) {
		printf("USER NAME FOUND. CONTINUE TO ENTER PASSWORD\n");
		return;
	}

	else if(strcmp(buffer, USER_NOT_FOUND) == 0) {
		printf("USER NAME NOT FOUND. ENTER USER NAME AGAIN\n");
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
		printf("*****\n");
		printf("TYPE ONE OF THE FOLLOWING SYNTAXES TO CONTINUE: \n");
		printf("1. [TRAINING-MODE]\n");
		printf("2. [TESTING-MODE]\n");
		printf("*****\n");
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

		if(strcmp(buffer, TRAINING_REQUEST_ACCEPTED) == 0) {
			printf("TRAINING REQUEST ACCEPTED\n");
			// Receive training questions
			receiveFileFromServer(client_socket);
			sendResultsToServer(client_socket);
		}

		else {
			showMessage(buffer);
		}

	}

	close(client_socket);


	return 0;
}