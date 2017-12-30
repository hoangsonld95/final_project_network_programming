#include "generateRandomization.c"
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
// sendTrainingQuestionsToUser(connecting_socket, clientAddress);
// receiveTrainingAnswersFromUser(connecting_socket, clientAddress);
// sendResultsBackToUser(connecting_socket, clientAddress);
#define BUFFER_SIZE 100

int sendTrainingQuestionsToUser(int connecting_socket, struct sockaddr_in clientAddress) {

	// Make the sending file
	int starting;
	int ending;
	int random_number[10];
	char question_number[10];
	int i;
	char extension[] = ".txt";
	char mode[] = "training_";
	char filename[100];
	char character[10];
	char read_character;
	char directory[100];
	char buffer[BUFFER_SIZE];

	int count = 0;

	FILE *filePointer = NULL;
	FILE *componentFilePointer = NULL;

	struct stat fileStatistic;
	int file_size;
	struct timeval start, stop, delta;

	int read_bytes, sent_bytes;

	starting = 1;
	ending = 15;

	generateRandomNumbersBetweenRange(random_number, 5, 1, 15);

	filePointer = fopen("training/sending_file.txt", "w+");

	for(i = 0; i < 5; i++) {
		memset(filename, '\0', 100);
		memset(character, '\0', 10);
		memset(directory, '\0', 100);
		strcpy(directory, "training/");
		sprintf(question_number, "%d", random_number[i]);
		strcat(filename, mode);
		strcat(filename, question_number);
		strcat(filename, extension);
		printf("%s\n", filename);

		sprintf(question_number, "%d", i+1);
		// Append question number in the content of file

		strcat(character, question_number);
		strcat(character, ". ");
		fprintf(filePointer, "%s", character);

		strcat(directory, filename);
		componentFilePointer = fopen(directory, "r+");


		read_character = fgetc(componentFilePointer);

		while(read_character != EOF) {
			fputc(read_character, filePointer);
			read_character = fgetc(componentFilePointer);
		}

		fprintf(filePointer, "======================================\n");

		fclose(componentFilePointer);

	}

	fclose(filePointer);

	// Send the file to user

	filePointer = fopen("training/sending_file.txt", "r");

	if(stat("training/sending_file.txt", &fileStatistic) == -1) {
		perror("stat() failed");
		return 0;
	}

	else {
		file_size = fileStatistic.st_size;
	}

	
	while(!feof(filePointer)) {
		memset(buffer, '\0', strlen(buffer));
		read_bytes = fread(buffer, BUFFER_SIZE, 1, filePointer);
		if(read_bytes >= 0) {
			count = count + read_bytes*BUFFER_SIZE;
			printf("%s", buffer);
			sent_bytes = send(connecting_socket, buffer, strlen(buffer), 0);
			if(sent_bytes == -1) {
				printf("Error! Cannot send data to client\n");
				break;
			}
		}
		else {
			break;
		}
	}

	//send(connecting_socket, buffer, 0, 0);
	
	fclose(filePointer);

	return 1;

}

int sendTrainingAcceptedResponse(int connecting_socket, struct sockaddr_in clientAddress) {

	char message_reply[100];
	int bytes_received;

	strcpy(message_reply, TRAINING_REQUEST_ACCEPTED);
	sendMessage(connecting_socket, message_reply);



}

int receiveTrainingAnswersFromUser(int connecting_socket, struct sockaddr_in clientAddress) {



}

// int sendResultsBackToUser(int connecting_socket, struct sockaddr_in clientAddress) {


// }