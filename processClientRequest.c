#include "struct.h"
#include "handleTrainingRequest.c"


int findUserByUserName(const char user_name[]) {

	int index;

	for(index = 0; index <= number_of_users; index++) {
		if(strcmp(users[index].user_name, user_name) == 0) {
			return 1;
		}
	}

	return -1;

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

int findSessionByUsername(const char user_name[]) {

	int index;

	for(index = 0; index < number_of_sessions; index++) {

		if(strcmp(sessions[index].user.user_name, user_name) == 0) {

			return index;

		}

	}

	return -1;
}

int findSessionByClientAddress(struct sockaddr_in clientAddress) {

	int index;

	for(index = 0; index <= number_of_sessions; index++) {

		if(sessions[index].clientAddress.sin_addr.s_addr == clientAddress.sin_addr.s_addr) {

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
	int userSessionIndex;

	printf("-----\n");
	printf("%s\n", user_name);

	sessionIndex = findSessionByClientAddress(clientAddress);

	if(sessionIndex == -1) {


		userSessionIndex = findSessionByUsername(user_name);

		if(userSessionIndex == -1) {


			if(findUserByUserName(user_name)) {

				memcpy(&sessions[number_of_sessions].clientAddress, &clientAddress, sizeof(clientAddress));
				sessions[number_of_sessions].sessionStatus = NOT_AUTHENTICATED_USER;
				strcpy(sessions[number_of_sessions].user.user_name, user_name);
				//printf("%s\n", sessions[number_of_sessions].user.user_name);
				number_of_sessions++;
				return USER_FOUND;

			}

			else {

				return USER_NOT_FOUND;

			}
		}

		else {

			if(sessions[userSessionIndex].sessionStatus == NOT_AUTHENTICATED_USER) {

				memcpy(&sessions[userSessionIndex].clientAddress, &clientAddress, sizeof(clientAddress));
				sessions[userSessionIndex].sessionStatus = NOT_AUTHENTICATED_USER;
				return USER_FOUND;

			}

			else if(sessions[userSessionIndex].sessionStatus == AUTHENTICATED_USER) {

				return USER_ALREADY_LOGINNED;

			}

		}

	}

	else {

		if(strcmp(sessions[sessionIndex].user.user_name, user_name) == 0) {

			printf("%s\n", user_name);
			printf("%s\n", sessions[sessionIndex].user.user_name);

			if(sessions[sessionIndex].sessionStatus == NOT_AUTHENTICATED_USER) {

				memcpy(&sessions[sessionIndex].clientAddress, &clientAddress, sizeof(clientAddress));
				return  USER_FOUND;

			}

			else if(sessions[sessionIndex].sessionStatus == AUTHENTICATED_USER) {

				return USER_ALREADY_LOGINNED;

			}

		}

		else {

			if(sessions[sessionIndex].sessionStatus == NOT_AUTHENTICATED_USER) {

				strcpy(sessions[sessionIndex].user.user_name, user_name);
				return USER_FOUND;

			}

			else if(sessions[sessionIndex].sessionStatus == AUTHENTICATED_USER) {

				return LOGOUT_COMPULSORY;

			}

		}
		
	}

}

const char* processPasswordRequest(char password[], int connecting_socket, struct sockaddr_in clientAddress) {

	int sessionIndex;

	sessionIndex = findSessionByClientAddress(clientAddress);

	if(sessionIndex == -1) {

		return USER_NAME_SHOULD_BE_SENT_FIRST;

	}

	else {

		if(sessions[sessionIndex].sessionStatus == NOT_IDENTIFIED_USER) {

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

const char* processLogoutRequest(char user_name[], int connecting_socket, struct sockaddr_in clientAddress) {

	int sessionIndex;

	sessionIndex = findSessionByClientAddress(clientAddress);

	if(sessionIndex == -1) {

		return LOGOUT_INVALID;

	}

	else {

		if(strcmp(sessions[sessionIndex].user.user_name, user_name) == 0) {

			if(sessions[sessionIndex].sessionStatus == NOT_IDENTIFIED_USER) {

				return LOGOUT_INVALID;

			}

			else if(sessions[sessionIndex].sessionStatus == NOT_AUTHENTICATED_USER) {

				return LOGOUT_INVALID;

			}

			else if(sessions[sessionIndex].sessionStatus == AUTHENTICATED_USER) {

				if(strcmp(sessions[sessionIndex].user.user_name, user_name) == 0) {
					printf("%s\n", sessions[sessionIndex].user.user_name);
					printf("%s\n", user_name);
					memset(sessions[sessionIndex].user.user_name, '\0', BUFFER_SIZE);
					sessions[sessionIndex].sessionStatus = NOT_IDENTIFIED_USER;
					return LOGOUT_ACCEPTED;
				}

				else {

					return LOGOUT_INVALID;

				}

				

			}

		}

		else {

			return LOGOUT_INVALID;

		}

	}

}

const char* processTrainingRequest(int connecting_socket, struct sockaddr_in clientAddress) {

	int sessionIndex;

	sessionIndex = findSessionByClientAddress(clientAddress);

	if(sessionIndex == -1) {
		return USER_UNAUTHORIZED_ACTION;
	}

	else {

		if(sessions[sessionIndex].sessionStatus == AUTHENTICATED_USER) {
			// Send the training questions back to user
			sendTrainingAcceptedResponse(connecting_socket, clientAddress);
			sendTrainingQuestionsToUser(connecting_socket, clientAddress);
			//sendTrainingLogoutRequest(connecting_socket, clientAddress);
			receiveTrainingContinuingOrQuittingRequest(connecting_socket, clientAddress);
			//receiveTrainingAnswersFromUser(connecting_socket, clientAddress);
			//sendResultsBackToUser(connecting_socket, clientAddress);
			return TRAINING_REQUEST_ACCEPTED;


		}

		else {

			return USER_UNAUTHORIZED_ACTION;

		}


	}

	return NULL;

}

const char* processTestingRequest(int connecting_socket, struct sockaddr_in clientAddress) {

	int sessionIndex;

	sessionIndex = findSessionByClientAddress(clientAddress);

	if(sessionIndex == -1) {
		return USER_UNAUTHORIZED_ACTION;
	}

	else {

		if(sessions[sessionIndex].sessionStatus == AUTHENTICATED_USER) {

		}

	}


}


void processClientRequest(char buffer[], int connecting_socket, struct sockaddr_in clientAddress) {

	char opcode[100];
	char operand[100];
	char message_reply[100];
	int messageLength;

	messageLength = strlen(buffer);

	strcpy(opcode, strtok(buffer, " "));
	
	if(strlen(opcode) ==  messageLength) {
		strcpy(operand, "\0");
		printf("<<<<<<<<<\n");
	}

	else {
		strcpy(operand, strtok(NULL, "\0"));
		printf(">>>>\n");
		printf("%s\n", operand);
	}
	




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

	else if(strcmp(opcode, "LOGOUT") == 0) {

		strcpy(message_reply, processLogoutRequest(operand, connecting_socket, clientAddress));
		printf("%s\n", message_reply);
		sendMessage(connecting_socket, message_reply);

	}


	else if((strcmp(opcode, "TRAINING-MODE") == 0)) {


		strcpy(message_reply, processTrainingRequest(connecting_socket, clientAddress));
		//printf("%s\n", message_reply);
		sendMessage(connecting_socket, message_reply);

	}

	else if((strcmp(opcode, "TESTING-MODE")) == 0) {
		strcpy(message_reply, processTestingRequest(connecting_socket, clientAddress));
		printf("%s\n", message_reply);
		sendMessage(connecting_socket, message_reply);
	}


	else {
		strcpy(message_reply, SYNTAX_WRONG);
		printf("%s\n", message_reply);
		sendMessage(connecting_socket, message_reply);
	}


	}
