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
	char room_name[100];

	filePointer = fopen("rooms.txt", "a+");

	if(filePointer == NULL) return 0;

	while(!feof(filePointer)) {
		fscanf(filePointer, "%s\t%d\t%d", rooms[number_of_rooms].room_name, &rooms[number_of_rooms].number_of_questions, &rooms[number_of_rooms].testing_time);
		rooms[number_of_rooms].roomStatus = ROOM_IDLE;
		number_of_rooms++;
	}

	rooms[number_of_rooms].number_of_sessions_in_room = 0;

	printf("done\n");

	fclose(filePointer);

	return 1;
}
