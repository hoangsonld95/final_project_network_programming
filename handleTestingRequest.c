int checkValidationRoomRequest(char room_name[]) {

	int index;

	for(index = 0; index <= number_of_rooms; index++) {
		if(strcmp(rooms[index].room_name, room_name) == 0) {
			return 1;
		}
	}	

	return 0;
}


int sendTestingAcceptedResponse(int connecting_socket, struct sockaddr_in clientAddress) {

	char message_reply[100];
	int sessionIndex;

	sessionIndex = findSessionByClientAddress(clientAddress);

	sessions[sessionIndex].sessionStatus = CURRENTLY_IN_TESTING_MODE;

	strcpy(message_reply, TESTING_REQUEST_ACCEPTED);
	sendMessage(connecting_socket, message_reply);

}

const char* processTestingRequest(int connecting_socket, struct sockaddr_in clientAddress) {

	int sessionIndex;
	char buffer[BUFFER_SIZE];

	sessionIndex = findSessionByClientAddress(clientAddress);

	if(sessionIndex == -1) {
		return USER_UNAUTHORIZED_ACTION;
	}

	else {

		if(sessions[sessionIndex].sessionStatus == AUTHENTICATED_USER) {

			sendTestingAcceptedResponse(connecting_socket, clientAddress);
			bzero(buffer, BUFFER_SIZE);
			receiveMessage(connecting_socket, buffer);

			printf("%s\n", buffer);
			
			if(checkValidationRoomRequest(buffer)) {
				printf("nnn\n");
				strcpy(buffer, ROOM_REQUEST_ACCEPTED);
				sendMessage(connecting_socket, buffer);

				

			}

			else {
				printf("mmm\n");
				strcpy(buffer, ROOM_REQUEST_DENIED);
				sendMessage(connecting_socket, buffer);
			}
			
			
		}

	}

}