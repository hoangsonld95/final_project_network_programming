#include <time.h>
#include <stdlib.h>
#include <stdio.h>	
#include <string.h>

// Check whether the generated random number is existing in the array random_number[] or not
int checkDistinctiveNumber(int generated_number[], int number_of_generated_numbers, int random_number) {

	int index;

	for(index = 0; index < number_of_generated_numbers; index++) {

		if(random_number == generated_number[index]) return 0;

	}

	return 1;

}

// Generate respectively number_of_random_numbers random numbers within range [starting_number, ending_number]  

int generateRandomNumbersBetweenRange(int generated_number[], int number_of_random_numbers, int starting_number, int ending_number) {

	int index;
	int number;
	int current_index;

	srand(time(NULL));

	current_index = 0;

	while(current_index < number_of_random_numbers) {

		number = rand()%(ending_number + 1 - starting_number) + starting_number;

		// Check whether the generated random number exists in the array random_number[] or not
		if(checkDistinctiveNumber(generated_number, current_index, number)) {
			generated_number[current_index] = number;
			printf("%d\n", generated_number[current_index]);
			current_index++;
		}

	}

	return 0;

}

// int main(int argc, char const *argv[])
// {
// 	int random_number[20];

// 	int number_of_random_numbers = 8;
// 	int starting_number = 1;
// 	int ending_number = 16;
// 	char character;
// 	char *question_number;

// 	char filename1[100];
// 	char filename2[100];

// 	generateRandomNumbersBetweenRange(random_number, number_of_random_numbers, starting_number, ending_number);

// 	FILE *filePointer;
// 	FILE *firstPointer, *secondPointer;

// 	filePointer = fopen("training/test.txt", "a+");

// 	firstPointer = fopen("training/training_1.txt", "r");
// 	strcpy(filename1, strrchr("training/training_1.txt", '/') + 1);
// 	printf("%s\n", filename1);
// 	strtok(filename1, "_");
// 	question_number = strtok(NULL, "\0");
// 	sprintf(question_number, "%d", atoi(question_number));
// 	printf("%s\n", question_number);


// 	secondPointer = fopen("training/training_2.txt", "r");
// 	strcpy(filename2, strrchr("training/training_2.txt", '/') + 1);
// 	printf("%s\n", filename2);

// 	while((character = fgetc(firstPointer)) != EOF) {
// 		fputc(character, filePointer);
// 	}

// 	while((character = fgetc(secondPointer)) != EOF) {
// 		fputc(character, filePointer);
// 	}

// 	fclose(firstPointer);
// 	fclose(secondPointer);

// 	fclose(filePointer);

// 	//remove("training/test.txt");



// 	return 0;
// }