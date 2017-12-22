#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

#define BUFFER_SIZE 1

int duration (struct timeval *start, struct timeval *stop, struct timeval *delta) {

	suseconds_t microstart, microstop, microdelta;

	microstart = (suseconds_t) (100000*(start->tv_sec))+ start->tv_usec;
	microstop = (suseconds_t) (100000*(stop->tv_sec))+ stop->tv_usec;
	microdelta = microstop - microstart;

	delta->tv_usec = microdelta%100000;
    delta->tv_sec = (time_t)(microdelta/100000);

    if((*delta).tv_sec < 0 || (*delta).tv_usec < 0)
        return -1;
    else
        return 0;

}

int main(int argc, char const *argv[])
{
	
	int client_socket;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in serverAddress;

	socklen_t addrlen;
	FILE *filePointer;

	struct stat fileStatistic;
	int file_size;

	struct timeval start, stop, delta;

	int read_bytes;
	int sent_bytes;
	int count;

	if(argc != 4) {
		printf("Error usage: %s <ip_server> <port_server> <filename>\n", argv[0]);
		return EXIT_FAILURE;
	}

	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(client_socket == -1) {
		perror("socket() failed");
		return EXIT_FAILURE;
	}

	addrlen = sizeof(struct sockaddr_in);

	bzero(&serverAddress, addrlen);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(atoi(argv[2]));
	if(inet_pton(AF_INET, argv[1], &serverAddress.sin_addr)==0) {
		printf("Invalid IP address\n");
		return EXIT_FAILURE;
	}

	if((filePointer = fopen(argv[3], "rb")) == NULL) {
		perror("fopen() failed");
		return EXIT_FAILURE;
	}

	if(stat(argv[3], &fileStatistic) == -1) {
		perror("stat() failed");
		return EXIT_FAILURE;
	}

	else {
		file_size = fileStatistic.st_size;
	}

	bzero(buffer, BUFFER_SIZE);

	if(connect(client_socket, (struct sockaddr*)&serverAddress, addrlen) == -1) {
		perror("connect() failed");
		exit(3);
	}

	gettimeofday(&start, NULL);

	read_bytes = fread(buffer, 1, 1, filePointer);

	while(read_bytes) {

		if(read_bytes == -1) {
			perror("read() failed");
			return EXIT_FAILURE;
		}

		sent_bytes = sendto(client_socket, buffer, read_bytes*BUFFER_SIZE , 0, (struct sockaddr*)&serverAddress, addrlen);
		if(sent_bytes == -1) {
			perror("send() failed");
			return EXIT_FAILURE;
		}

		count += sent_bytes;

		bzero(buffer, BUFFER_SIZE);

		read_bytes = fread(buffer, 1, 1, filePointer);

	}

	sent_bytes = sendto(client_socket, buffer, 0, 0, (struct sockaddr*)&serverAddress, addrlen);
	gettimeofday(&stop, NULL);
	duration(&start, &stop, &delta);

	printf("Number of bytes transferred: %d\n", count);
	printf("File size: %d\n", file_size);
	printf("Transferring time: %ld.%ld\n", delta.tv_sec, delta.tv_usec);

	close(client_socket);


	return EXIT_SUCCESS;
}