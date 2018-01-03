int checkValidationRoomRequest(char room_name[]) {

	int index;

	for(index = 0; index <= number_of_rooms; index++) {
		if((strcmp(rooms[index].room_name, room_name) == 0) && (rooms[index].roomStatus == ROOM_IDLE)) {
			return 1;
		}
	}	

	return 0;
}

int getRoomIndex(char room_name[]) {

	int index;

	for(index = 0; index < number_of_rooms; index++) {

		if(strcmp(rooms[index].room_name, room_name) == 0) {
			return index;
		}

	}

	return -1;

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
	int roomIndex;
	char room_name[100];
	char user_name[100];

	sessionIndex = findSessionByClientAddress(clientAddress);

	if(sessionIndex == -1) {
		return USER_UNAUTHORIZED_ACTION;
	}

	else {

		if(sessions[sessionIndex].sessionStatus == AUTHENTICATED_USER) {

			sendTestingAcceptedResponse(connecting_socket, clientAddress);
			bzero(room_name, BUFFER_SIZE);
			receiveMessage(connecting_socket, room_name);

			printf("%s\n", room_name);
			
			if(checkValidationRoomRequest(room_name)) {
				printf("nnn\n");
				strcpy(buffer, ROOM_REQUEST_ACCEPTED);
				sendMessage(connecting_socket, buffer);
				sessions[sessionIndex].sessionStatus = CURRENTLY_WAITING_IN_TESTING_MODE;
				
				// Add user to the room
				roomIndex = getRoomIndex(room_name);
				// printf("%d\n", number_of_rooms);

				strcpy(user_name, sessions[sessionIndex].user.user_name);
				printf("%s\n", user_name);
				//strcpy(rooms[roomIndex].sessions.user_name, user_name);
				//memcpy(rooms[roomIndex].sessions.clientAddress, clientAddress);


			}

			else {
				printf("mmm\n");
				strcpy(buffer, ROOM_REQUEST_DENIED);
				sendMessage(connecting_socket, buffer);
			}
			
			
		}

	}

}