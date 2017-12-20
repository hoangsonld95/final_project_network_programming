int readAccountsFromFile() {
	
	FILE *filePointer;
	char line[100];

	filePointer = fopen("accounts.txt", "r");

	if(filePointer == NULL) return 0;

	while(fgets(line, 100, filePointer) != NULL) {

		strcpy(users[number_of_users].user_name, strtok(line, "\t"));
		strcpy(users[number_of_users].password, strtok(NULL, "\n"));

		number_of_users++;

	}

	fclose(filePointer);

	return 1;
	
}

int readRoomsFromFile() {

	FILE *filePointer;
	char line[100];

	filePointer = fopen("rooms.txt", "r");

	if(filePointer == NULL) return 0;

	while(fgets(line, 100, filePointer) != NULL) {

		line[strlen(line) - 1] = '\0';

		strcpy(rooms[number_of_rooms].room_name, line);

		rooms[number_of_rooms].roomStatus = ROOM_IDLE;
		number_of_rooms++;

	}

	fclose(filePointer);

	return 1;
}
