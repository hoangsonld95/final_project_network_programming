enum SessionStatus {
	NOT_IDENTIFIED_USER, // nguoi dung chua nhan dien duoc
	NOT_AUTHENTICATED_USER, // nguoi dung chua xac thuc 
	AUTHENTICATED_USER, // nguoi dung da xac thuc
	CURRENTLY_IN_TRAINING_MODE, // dang o che do training
	CURRENTLY_IN_TESTING_MODE, // dang o che do testing
};

enum RoomStatus {
	ROOM_TESTING, // phong dang o che do thi 
	ROOM_IDLE // phong dang o che do khong thi
	ROOM_TESTING_DONE,
};

struct User {
	char user_name[100];
	char password[100];

};

struct Room {
	char room_name[100]; // ten phong
	struct User users[100]; // danh sach nguoi dung
	enum RoomStatus roomStatus; // trang thai phong
	int testing_time; // thoi gian thi
};

struct Questions_Answers_Mapping_Training { // luu question-answer trong session cua server - che do training
	int question; 
	int answer;
};

struct Questions_Answers_Mapping_Testing { // luu question-answer trong session cua server - che do testing
	int question;
	int answer;
};

struct Session { 
	struct User user;
	struct sockaddr_in clientAddress; // luu dia chi client 
	enum SessionStatus sessionStatus; // trang thai session
	struct Questions_Answers_Mapping_Training questions_answers_mapping_training[10]; // luu question-answer training
	struct Questions_Answers_Mapping_Testing questions_answers_mapping_testing[10]; // luu question-answer testing
};

struct Socket_Tupple { // luu dia chi client + socket dang ket noi voi client cua server

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
#define NUMBER_OF_TRAINING_QUESTIONS 10;