int findUserByUserName(const char user_name[]) {

	int index;

	for(index = 0; index <= number_of_users; index++) {
		if(strcmp(users[index].user_name, user_name) == 0) {
			return 1;
		}
	}

	return 0;

}

int checkCredentials(const char user_name[], const char password[]) {

	int index;

	for(index = 0; index <= number_of_users; index++) {

		if(strcmp(users[index].user_name, user_name) == 0) {

			if(strcmp(users[index].password, password) == 0) {

				return 1;
			}

		}

	} 

	return 0;

}

int findUserBySession(const char user_name[]) {

	int index;

	for(index = 0; index <= number_of_sessions; index++) {

		if(strcmp(sessions[index].user.user_name, user_name) == 0) {

			return index;

		}

	}

	return -1;

}

int findClientAddress(struct sockaddr_in clientAddress) {

	int index;

	for(index = 0; index <= number_of_sessions; index++) {
		if((sessions[index].clientAddress.sin_addr.s_addr == clientAddress.sin_addr.s_addr)) {
			if(sessions[index].clientAddress.sin_port == clientAddress.sin_port) {
				return index;
			}
		}
	}

	return -1;

}

int printClientAddress() {

	int index;

	for(index = 0; index <= number_of_sessions; index++) {
		printf("asd: %s:%d\n", inet_ntoa(sessions[index].clientAddress.sin_addr), ntohs(sessions[index].clientAddress.sin_port));
	}

}


const char* processUserRequest(char user_name[], int connecting_socket, struct sockaddr_in clientAddress) {

	int sessionIndex;

	// Not having any existing sessions
	if(number_of_sessions == 0) {

		if(findUserByUserName(user_name)) {
			memcpy(&sessions[number_of_sessions].clientAddress, &clientAddress, sizeof(struct sockaddr_in));
			sessions[number_of_sessions].sessionStatus = NOT_AUTHENTICATED_USER;
			strcpy(sessions[number_of_sessions].user.user_name, user_name);
			number_of_sessions++;
			return USER_FOUND;
		}

		else {
			memcpy(&sessions[number_of_sessions].clientAddress, &clientAddress, sizeof(struct sockaddr_in));
			sessions[number_of_sessions].sessionStatus = NOT_IDENTIFIED_USER;
			number_of_sessions++;
			return USER_NOT_FOUND;
		}

	}

	// Had existing sessions
	else {

		sessionIndex = findUserBySession(user_name);

		if(sessionIndex == -1) {

			if(findUserByUserName(user_name)) {

				memcpy(&sessions[number_of_sessions].clientAddress, &clientAddress, sizeof(struct sockaddr_in));
				sessions[number_of_sessions].sessionStatus = NOT_AUTHENTICATED_USER;
				strcpy(sessions[number_of_sessions].user.user_name, user_name);
				number_of_sessions++;
				return USER_FOUND;

			}

			else {

				memcpy(&sessions[number_of_sessions].clientAddress, &clientAddress, sizeof(struct sockaddr_in));
				sessions[number_of_sessions].sessionStatus = NOT_IDENTIFIED_USER;
				number_of_sessions++;
				return USER_NOT_FOUND;

			}

		}

		else {

			if(sessions[sessionIndex].sessionStatus == NOT_IDENTIFIED_USER) {

				if(findUserByUserName(user_name)) {

					memcpy(&sessions[number_of_sessions].clientAddress, &clientAddress, sizeof(struct sockaddr_in));
					sessions[number_of_sessions].sessionStatus = NOT_AUTHENTICATED_USER;
					strcpy(sessions[number_of_sessions].user.user_name, user_name);
					number_of_sessions++;
					return USER_FOUND;

				}

				else {

					memcpy(&sessions[number_of_sessions].clientAddress, &clientAddress, sizeof(struct sockaddr_in));
					sessions[number_of_sessions].sessionStatus = NOT_IDENTIFIED_USER;
					number_of_sessions++;
					return USER_NOT_FOUND;

				}


			}

			else if(sessions[sessionIndex].sessionStatus == NOT_AUTHENTICATED_USER) {

				if(findUserByUserName(user_name)) {

					memcpy(&sessions[sessionIndex].clientAddress, &clientAddress, sizeof(struct sockaddr_in));
					sessions[sessionIndex].sessionStatus = NOT_AUTHENTICATED_USER;
					strcpy(sessions[sessionIndex].user.user_name, user_name);
					return USER_FOUND;

				}

				else {

					memcpy(&sessions[sessionIndex].clientAddress, &clientAddress, sizeof(struct sockaddr_in));
					sessions[sessionIndex].sessionStatus == NOT_IDENTIFIED_USER;
					strcpy(sessions[sessionIndex].user.user_name, user_name);
					return USER_NOT_FOUND;

				}

			}

			else if(sessions[sessionIndex].sessionStatus == AUTHENTICATED_USER) {

				return USER_ALREADY_LOGINNED;

			}


		}



	}

	return NULL;

}

const char* processPasswordRequest(char password[], int connecting_socket, struct sockaddr_in clientAddress) {

	int sessionIndex;

	sessionIndex = findClientAddress(clientAddress);

	if(sessionIndex == -1) {

		memcpy(&sessions[number_of_sessions].clientAddress, &clientAddress, sizeof(clientAddress));
		sessions[number_of_sessions].sessionStatus = NOT_IDENTIFIED_USER;
		number_of_sessions++;
		return USER_NAME_SHOULD_BE_SENT_FIRST;

	}

	else {

		if(sessions[sessionIndex].sessionStatus == NOT_IDENTIFIED_USER) {

			memcpy(&sessions[number_of_sessions].clientAddress, &clientAddress, sizeof(clientAddress));
			sessions[number_of_sessions].sessionStatus = NOT_IDENTIFIED_USER;
			return USER_NAME_SHOULD_BE_SENT_FIRST;

		}

		else if(sessions[sessionIndex].sessionStatus == NOT_AUTHENTICATED_USER) {

			if(checkCredentials(sessions[sessionIndex].user.user_name, password)) {

				printf("%s\n", sessions[sessionIndex].user.user_name);
				sessions[sessionIndex].sessionStatus = AUTHENTICATED_USER;
				return LOGIN_SUCCESSFUL;

			}

			else {
				return PASSWORD_FALSE;
			}

		}

		else if(sessions[sessionIndex].sessionStatus == AUTHENTICATED_USER) {
			return USER_ALREADY_LOGINNED;
		}

	}

}

void processClientRequest(char buffer[], int connecting_socket, struct sockaddr_in clientAddress) {


	char opcode[100];
	char operand[100];
	char message_reply[100];

	strcpy(opcode, strtok(buffer, " "));
	strcpy(operand, strtok(NULL, "\0"));

	printf("3333333333333333\n");

	if(strcmp(opcode, "USER") == 0) {
		strcpy(message_reply, processUserRequest(operand, connecting_socket, clientAddress));
		printf("%s\n", message_reply);
		sendMessage(connecting_socket, message_reply);
	}

	else if(strcmp(opcode, "PASS") == 0) {

		strcpy(message_reply, processPasswordRequest(operand, connecting_socket, clientAddress));
		printf("%s\n", message_reply);
		sendMessage(connecting_socket, message_reply);

	}


}
