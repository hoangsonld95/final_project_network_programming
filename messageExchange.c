int receiveMessage(int connecting_socket, char buffer[]) {

	int messageLength, bytes_received;

	printf("11111111111111\n");

	bytes_received = recv(connecting_socket, &messageLength, sizeof(int), 0);

	if(bytes_received <= 0) {
		printf("Client disconnected\n");
		return 0;
	}
	printf("abc: %d\n", bytes_received);

	memset(buffer, '\0', 100);

	bytes_received = recv(connecting_socket, buffer, messageLength, 0);
	
	if(bytes_received <= 0) {
		printf("Client disconnected\n");
		return 0;
	}
	printf("%s\n", buffer);

	return 1; 

}

int sendMessage(int connecting_socket, char buffer[]) {
	
	int messageLength, bytes_sent;

	messageLength = strlen(buffer);

	bytes_sent = send(connecting_socket, &messageLength, sizeof(int), 0);

	if(bytes_sent <= 0) {
		printf("send() failed\n");
		return 0;
	}

	bytes_sent = send(connecting_socket, buffer, messageLength, 0);

	if(bytes_sent <= 0) {
		printf("send() failed\n");
		return 0;
	}

	return 1;

}
