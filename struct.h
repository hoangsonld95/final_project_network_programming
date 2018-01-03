enum SessionStatus {
	NOT_IDENTIFIED_USER, // chua dang nhap
	NOT_AUTHENTICATED_USER,	// dung user, chua nhap password
	AUTHENTICATED_USER, // dang nhap roi
	CURRENTLY_IN_TRAINING_MODE, // dang trong trang thai training
	CURRENTLY_WAITING_IN_TESTING_MODE, /// dang cho o phong test
	CURRENTLY_IN_TESTING_MODE, // dang trong trang thai test
};

enum RoomStatus {
	ROOM_TESTING, // phong dang thi
	ROOM_IDLE	// phong chua thi
};

struct User {
	char user_name[100]; 
	char password[100];
};

struct Questions_Answers_Mapping {
	int question;
	char answer;
};

struct Session {
	struct User user;
	struct sockaddr_in clientAddress;
	enum SessionStatus sessionStatus;
	struct Questions_Answers_Mapping questions_answers_mapping[100];
};

struct Room {
	char room_name[100]; // ten phong thi
	int number_of_questions; // so cau hoi
	int testing_time; // thoi gian thi, tinh bang giay
	struct Session sessions[100]; // cac phien ket noi voi phong
	int number_of_sessions_in_room; // so session hien tai trong phong
	enum RoomStatus roomStatus; // trang thai phong
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
#define BUFFER_SIZE 3000