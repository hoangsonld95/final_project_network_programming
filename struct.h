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

struct Questions_Answers_Mapping_Training {
	int question;
	int answer;
};

struct Questions_Answers_Mapping_Testing {
	int question;
	int answer;
};

struct Session {
	struct User user;
	struct sockaddr_in clientAddress;
	enum SessionStatus sessionStatus;
	struct Questions_Answers_Mapping_Training questions_answers_mapping_training[100];
	struct Questions_Answers_Mapping_Testing questions_answers_mapping_testing[100];
};

struct Socket_Tupple {

	int connecting_socket;
	struct sockaddr_in clientAddress;

};

struct User users[100];
struct Session sessions[100];
struct Room rooms[100];

extern int number_of_users = 0;
extern int number_of_sessions = 0;
extern int number_of_rooms = 0;

#define BACKLOG 5
#define BUFFER_SIZE 1000
#define NUMBER_OF_TRAINING_QUESTIONS;