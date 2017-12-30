#include "generateRandomization.c"
// sendTrainingQuestionsToUser(connecting_socket, clientAddress);
// receiveTrainingAnswersFromUser(connecting_socket, clientAddress);
// sendResultsBackToUser(connecting_socket, clientAddress);

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

	FILE *filePointer = NULL;
	FILE *componentFilePointer = NULL;

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

	



}

int main(int argc, char const *argv[])
{
	
	sendTrainingQuestionsToUser();

	return 0;
}

// int receiveTrainingAnswersFromUser(int connecting_socket, struct sockaddr_in clientAddress) {



// }

// int sendResultsBackToUser(int connecting_socket, struct sockaddr_in clientAddress) {


// }