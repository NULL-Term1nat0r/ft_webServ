#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
	char buffer[BUFFER_SIZE];
	int bytes_received;

	// Receive data from the client
	bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
	if (bytes_received < 0) {
		perror("Error receiving data");
		return;
	}

	// Process the received data (you can implement your logic here)
	printf("Received from client: %.*s", bytes_received, buffer);

	// Send a simple HTML response to the client
	const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>";
	write(client_socket, response, strlen(response));

	// Write the received data to the web page
	write(client_socket, buffer, bytes_received);

	const char* response_end = "</h1></body></html>";
	write(client_socket, response_end, strlen(response_end));

	// Close the client socket
	close(client_socket);
}

int main() {
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	// Create socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		perror("Error creating socket");
		exit(1);
	}

	int reuse = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		perror("setsockopt(SO_REUSEADDR) failed");
		exit(1);
	}

	// Bind socket
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Error binding socket");
		exit(1);
	}

	// Listen for incoming connections
	if (listen(server_socket, 5) < 0) {
		perror("Error listening");
		exit(1);
	}

	printf("Server listening on port %d\n", PORT);

	while (1) {
		// Accept incoming connection
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
		if (client_socket < 0) {
			perror("Error accepting connection");
			continue;
		}

		// Handle the client request in a separate function
		handle_client(client_socket);
	}

	// Close the server socket (never reached in this example)
	close(server_socket);

	return 0;
}
