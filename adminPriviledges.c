int checkExistenceInTheRoomDatabase(char room_name[]) {

	int index;

	for(index = 0; index < number_of_rooms; index++) {

		if(strcmp(rooms[index].room_name, room_name) == 0) {

			return 1;

		}

	}

	return 0;

}

int checkExistenceInTheAccountDatabase(char user_name[]) {

	int index;

	for(index = 0; index < number_of_users; index++) {

		if(strcmp(users[index].user_name, user_name) == 0) {
			return 1;
		}

	}

	return 0;

}


int addRoom(FILE *filePointer) {

	char room_name[100];
	int number_of_questions;
	int testing_time;
	int check = 2;

	while(1) {

		printf("Enter the name of room: \n");
		fgets(room_name, 100, stdin);
		room_name[strlen(room_name) - 1] = '\0';

		check = checkExistenceInTheRoomDatabase(room_name);

		if(!check) {
			printf("Enter number of questions: \n");
			scanf("%d", &number_of_questions);

			printf("Enter allowed testing time in room: (unit: minute)\n");
			scanf("%d", &testing_time);

			number_of_rooms++;

			strcpy(rooms[number_of_rooms].room_name, room_name);
			rooms[number_of_rooms].number_of_questions = number_of_questions;
			rooms[number_of_rooms].testing_time = testing_time;

			printf("%s%d%d\n", rooms[number_of_rooms].room_name, rooms[number_of_rooms].number_of_questions, rooms[number_of_rooms].testing_time);

			fprintf(filePointer, "\n%s\t%d\t%d", rooms[number_of_rooms].room_name, rooms[number_of_rooms].number_of_questions, rooms[number_of_rooms].testing_time);

			break;

		}

		else {
			break;
		}

	}

	fclose(filePointer);


	return 1;

}

int addAccount(FILE *filePointer) {

	char user_name[100];
	char password[100];
	int check = 2;

	while(1) {

		printf("Enter user name: \n");
		fgets(user_name, 100, stdin);
		user_name[strlen(user_name) - 1] = '\0';

		check = checkExistenceInTheAccountDatabase(user_name);

		if(check == 1) {
			printf("User name already exists\n");
			continue;
		}

		else {
			printf("Enter password\n");
			fgets(password, 100, stdin);
			password[strlen(password) - 1] = '\0';

			number_of_users++;
			strcpy(users[number_of_users].user_name, user_name);
			strcpy(users[number_of_users].password, password);

			printf("%s\n", users[number_of_users].user_name);
			printf("%s\n", users[number_of_users].password);

		}

		

		fprintf(filePointer, "\n%s\t%s", users[number_of_users].user_name, users[number_of_users].password);
		break;

	}

	fclose(filePointer);

}	


int showRoomResult(FILE *filePointer) {

}