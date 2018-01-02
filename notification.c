void showMessage(char buffer[]) {

	if(strcmp(buffer, USER_FOUND) == 0) {
		printf("USER NAME FOUND. CONTINUE TO ENTER PASSWORD\n");
		return;
	}

	else if(strcmp(buffer, USER_NOT_FOUND) == 0) {
		printf("USER NAME NOT FOUND. ENTER USER NAME AGAIN\n");
		return;
	}

	else if(strcmp(buffer, PASSWORD_FALSE) == 0) {
		printf("PASSWORD IS FALSE. ENTER PASSWORD AGAIN\n");
	}

	else if(strcmp(buffer, PASSWORD_REQUIRED) == 0) {
		printf("PASSWORD IS REQUIRED. ENTER PASSWORD TO LOGIN\n");
		return;
	}

	else if(strcmp(buffer, USER_ALREADY_LOGINNED) == 0) {
		printf("USER HAS ALREADY LOGINNED\n");
		return;
	}

	else if(strcmp(buffer, TRAINING_REQUEST_DENIED) == 0) {
		printf("TRAINING REQUEST DENIED\n");
		return;
	}

	else if(strcmp(buffer, TIME_EXCEEDED) == 0) {
		printf("TIME EXCEEDED\n");
		printf("YOU GOT 0 POINTS FOR NOT ATTEMPTING TO ANSWER\n");
		return;
	}

	else if(strcmp(buffer, TESTING_REQUEST_ACCEPTED) == 0) {
		printf("TESTING REQUEST ACCEPTED\n");
		return;
	}

	else if(strcmp(buffer, TESTING_REQUEST_DENIED) == 0) {
		printf("TESTING REQUEST DENIED\n");
		return;
	}

	else if(strcmp(buffer, ROOM_REQUEST_ACCEPTED) == 0) {
		printf("ROOM REQUEST ACCEPTED. WAIT TO TEST FROM SERVER\n");
		return;
	}

	else if(strcmp(buffer, ROOM_REQUEST_DENIED) == 0) {
		printf("ROOM REQUEST DENIED\n");
		return;
	}

	else if(strcmp(buffer, USER_UNAUTHORIZED_ACTION) == 0) {
		printf("UNAUTHORIZED ACTION. PLEASE LOGIN\n");
		return;
	}

	else if(strcmp(buffer, LOGIN_SUCCESSFUL) == 0) {
		printf("LOGIN IS SUCCESSFUL\n");
		printf("*****\n");
		printf("TYPE ONE OF THE FOLLOWING SYNTAXES TO CONTINUE: \n");
		printf("1. [TRAINING-MODE]\n");
		printf("2. [TESTING-MODE]\n");
		printf("*****\n");
		return;
	}

	else if(strcmp(buffer, LOGOUT_ACCEPTED) == 0) {
		printf("LOGOUT ACCEPTED\n");
		return;
	}

	else if(strcmp(buffer, LOGOUT_DENIED) == 0) {
		printf("LOGOUT DENIED\n");
		return;
	}

	else if(strcmp(buffer, LOGOUT_INVALID) == 0) {
		printf("HAVE NOT LOGINNED. WHY LOG OUT???\n");
		return;
	}

	else if(strcmp(buffer, LOGOUT_COMPULSORY) == 0) {
		printf("NEED TO LOGOUT TO LOGIN TO NEW ACCOUNT\n");
		return;
	}

	else if(strcmp(buffer, SYNTAX_WRONG) == 0) {
		printf("WRONG SYNTAX! PLEASE CHECK THE SYNTAX AGAIN\n");
	}

}