#include <time.h>
#include <stdlib.h>
#include <stdio.h>

// Check whether the generated random number is existing in the array random_number[] or not
int checkDistinctiveNumber(int generated_number[], int number_of_generated_numbers, int random_number) {

	int index;

	for(index = 0; index < number_of_generated_numbers; index++) {

		if(random_number == generated_number[index]) return 0;

	}

	return 1;

}

// Generate respectively number_of_random_numbers random numbers within range [starting_number, ending_number]  

int generateRandomNumbersBetweenRange(int random_number[], int number_of_random_numbers, int starting_number, int ending_number) {

	int index;
	int number;
	int generated_number[10];
	int current_index;

	srand(time(NULL));

	current_index = 0;

	while(current_index < number_of_random_numbers) {

		number = rand()%(ending_number + 1 - starting_number) + starting_number;

		// Check whether the generated random number exists in the array random_number[] or not
		if(checkDistinctiveNumber(generated_number, current_index, number)) {
			generated_number[current_index] = number;
			printf("%d\n", number);
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

// 	generateRandomNumbersBetweenRange(random_number, number_of_random_numbers, starting_number, ending_number);


// 	return 0;
// }