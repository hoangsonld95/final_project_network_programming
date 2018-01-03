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

int readAnswersFromFile(char answers[]) {

	int index = 0;
	char character;
	char line[100];

	FILE *filePointer = fopen("training/answers.txt", "r");

	while(fgets(line, sizeof(line), filePointer)!=NULL) {
		//printf("%s\n", line);
		character = line[strlen(line) - 2];
		answers[index] = character;
		printf("%c\n", answers[index]);
		index++;
	}

}

int generateRandomQuestions(int random_number[], char answers[], struct sockaddr_in clientAddress) {

	int sessionIndex;
	int index;

	generateRandomNumbersBetweenRange(random_number, SENDING_TRAINING_QUESTIONS, 1, TOTAL_TRAINING_QUESTIONS);

	sessionIndex = findSessionByClientAddress(clientAddress);
	printf("SESSION INDEX %d\n", sessionIndex);

	for(index = 0; index < SENDING_TRAINING_QUESTIONS; index++) {

		sessions[sessionIndex].questions_answers_mapping[index].question = random_number[index];
		sessions[sessionIndex].questions_answers_mapping[index].answer = answers[random_number[index] - 1];

		//printf("%d\n", random_number[index]);
		printf("%d.%c\n", sessions[sessionIndex].questions_answers_mapping[index].question,
			sessions[sessionIndex].questions_answers_mapping[index].answer);
	}


}

int concenatingToSendingFile(FILE *filePointer, int random_number[]) {

	int i;

	FILE *componentFilePointer;

	char mode[] = "training_";
	char directory[100];
	char extension[] = ".txt";

	char filename[100];
	char character[10];
	char question_number[10];
	char read_character;

	for(i = 0; i < SENDING_TRAINING_QUESTIONS; i++) {

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

}


int sendTrainingQuestionsToUser(int connecting_socket, struct sockaddr_in clientAddress) {

	int random_number[10];
		
	int i, messageLength;
	
	char read_character;
	
	char buffer[BUFFER_SIZE];
	int index;
	int sessionIndex;
	char answers[100];

	int count = 0;

	FILE *filePointer = NULL;
	FILE *componentFilePointer = NULL;

	struct stat fileStatistic;
	int file_size;

	int read_bytes, sent_bytes;

	readAnswersFromFile(answers);

	generateRandomQuestions(random_number, answers, clientAddress);
	
	filePointer = fopen("training/sending_file.txt", "w");	
	
	concenatingToSendingFile(filePointer, random_number);
	
	// Send the file to user

	filePointer = fopen("training/sending_file.txt", "r+");

	read_bytes = 1;

	while(!feof(filePointer)) {

		memset(buffer, '\0', strlen(buffer));
		read_bytes = fread(buffer, BUFFER_SIZE, 1, filePointer);

		printf("%s\n", buffer);
		sendMessage(connecting_socket, buffer);

	}
	
	fclose(filePointer);


	printf("zzz\n");

	sessionIndex = findSessionByClientAddress(clientAddress);

	bzero(buffer, BUFFER_SIZE);
	receiveMessage(connecting_socket, buffer);

	if(strcmp(buffer, TIME_EXCEEDED) == 0) {
		printf("44\n");
		sessions[sessionIndex].sessionStatus = AUTHENTICATED_USER;
	}

	else {

		printf("This is %s\n", buffer);

		// Compute test result

		count = 0;

		messageLength = strlen(buffer);

		printf("BLABLA: %d\n", sessionIndex);

		for(index = 0; index < messageLength; index++) {
			printf("%c\n", sessions[sessionIndex].questions_answers_mapping[index].answer);
			if(buffer[index] == sessions[sessionIndex].questions_answers_mapping[index].answer) {
				count++;
			}
		}

		printf("%d\n", count);

		// Set back the session status 
		sessions[sessionIndex].sessionStatus = AUTHENTICATED_USER;

		send(connecting_socket, &count, sizeof(int), 0);

		

	}

	return 1;

}

int sendTrainingAcceptedResponse(int connecting_socket, struct sockaddr_in clientAddress) {

	char message_reply[100];
	int sessionIndex; 

	sessionIndex = findSessionByClientAddress(clientAddress);

	sessions[sessionIndex].sessionStatus = CURRENTLY_IN_TRAINING_MODE;

	strcpy(message_reply, TRAINING_REQUEST_ACCEPTED);
	sendMessage(connecting_socket, message_reply);

}


const char* processTrainingRequest(int connecting_socket, struct sockaddr_in clientAddress) {

	int sessionIndex;
	char buffer[BUFFER_SIZE];

	sessionIndex = findSessionByClientAddress(clientAddress);

	if(sessionIndex == -1) {
		return USER_UNAUTHORIZED_ACTION;
	}

	else {

		if(sessions[sessionIndex].sessionStatus == AUTHENTICATED_USER) {
			
			// Send the training questions back to user
			sendTrainingAcceptedResponse(connecting_socket, clientAddress);
			sendTrainingQuestionsToUser(connecting_socket, clientAddress);
			
		}

		else {

			strcpy(buffer, USER_UNAUTHORIZED_ACTION);
			sendMessage(connecting_socket, buffer);
					
		}

	}

	return NULL;

}
