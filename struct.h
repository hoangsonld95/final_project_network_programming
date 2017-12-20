enum SessionStatus {
	NOT_IDENTIFIED_USER,
	NOT_AUTHENTICATED_USER,
	AUTHENTICATED_USER,
	CURRENTLY_IN_TRAINING_MODE,
	CURRENTLY_IN_TESTING_MODE,
	IDLE
};

enum RoomStatus {
	ROOM_TESTING,
	ROOM_IDLE
};

struct User {
	char user_name[100];
	char password[100];
};

struct Room {
	char room_name[100];
	struct User users[100];
	enum RoomStatus roomStatus;
};

struct Questions_Answers_Mapping {
	int question;
	int answer;
};

struct Session {
	struct User user;
	struct sockaddr_in clientAddress;
	enum SessionStatus sessionStatus;
	struct Questions_Answers_Mapping questions_answers_mapping[100];
};

struct Socket_Tupple {

	int connecting_socket;
	struct sockaddr_in clientAddress;

};

struct User users[100];
struct Session sessions[100];
struct Room rooms[100];

int number_of_users = 0;
int number_of_sessions = 0;
int number_of_rooms = 0;