int generateRandomNumber(int starting_number, int ending_number) {

	int result;

	srand(time(NULL));

	result = (rand()%(ending_number - starting_number + 1)) + starting_number;

	return result;

}