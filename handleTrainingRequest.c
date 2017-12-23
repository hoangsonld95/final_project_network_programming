void sendTrainingQuestionsToUser(int connecting_socket, struct sockaddr_in clientAddress) {

	// Take randomly 5 respective numbers, assign it to the corresponding question in the question folder

	int question_number[NUMBER_OF_TRAINING_QUESTIONS];
	int index;

	for(index = 0; index < NUMBER_OF_TRAINING_QUESTIONS; index++) {

		question_number[index] = generateRandomNumber()

	}

}