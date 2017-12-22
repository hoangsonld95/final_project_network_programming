#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

#define BUFFER_SIZE 1000

#define BACKLOG 5

int main(int argc, char const *argv[])
{

	struct sockaddr_in serverAddress, clientAddress;
	int listening_socket, connecting_socket;
	socklen_t addrlen;

	char buffer[BUFFER_SIZE];
	char file_name[256];

	time_t intps;
	struct tm* tmi;

	int count;
	int received_bytes, written_bytes;

	FILE *filePointer;

	char destination[INET_ADDRSTRLEN];

	if(argc != 2) {
		printf("usage: ./a.out <server_port> \n");
	}

	listening_socket = socket(AF_INET, SOCK_STREAM, 0);

	if(listening_socket == -1) {
		perror("socket() failed");
		return EXIT_SUCCESS;
	} 

	addrlen = sizeof(struct sockaddr_in);

	bzero(&serverAddress, addrlen);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(atoi(argv[1]));
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listening_socket, (struct sockaddr*)&serverAddress, addrlen) == -1) {
		perror("bind() failed");
		return EXIT_FAILURE;
	}

	bzero(buffer, BUFFER_SIZE);
	listen(listening_socket, BACKLOG);

	connecting_socket = accept(listening_socket, (struct sockaddr*)&clientAddress, &addrlen);
	if(connecting_socket == -1) {
		perror("accept() failed");
		return EXIT_FAILURE;
	}

	else {

		printf("You've received a connection from %s:%d\n", inet_ntoa(serverAddress.sin_addr), ntohs(clientAddress.sin_port));

		bzero(file_name, 256);
		intps = time(NULL);
		tmi = localtime(&intps);
		sprintf(file_name, "client.%d.%d.%d.%d.%d.%d", tmi->tm_mday, tmi->tm_mon+1, 1900+tmi->tm_year, tmi->tm_hour, tmi->tm_min, tmi->tm_sec);
		printf("Creating the copied output file: %s\n", file_name);

		if((filePointer = fopen(file_name, "a")) == NULL) {
			printf("fopen() failed\n");
			exit(3);
		}

		bzero(buffer, BUFFER_SIZE);

		received_bytes = recv(connecting_socket, buffer, BUFFER_SIZE, 0);

		while(received_bytes) {

			if(received_bytes == -1) {
				perror("recv() failed");
				exit(4);
			}

			written_bytes = fwrite(buffer, received_bytes, 1, filePointer); 

			if(written_bytes == 0) {
				perror("fwrite() failed");
				return EXIT_FAILURE;
			}

			count = count + written_bytes;

			bzero(buffer, BUFFER_SIZE);

			received_bytes = recv(connecting_socket, buffer, BUFFER_SIZE, 0);

		}

		fclose(filePointer);

		close(connecting_socket);

	}

	close(listening_socket);

	printf("Transferred file from %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
	printf("File size: %d\n", count);

	return EXIT_SUCCESS;
}