/*
socket() is used to create a socket connection
IPv4 --> AF_INET
TCP --> SOCK_STREAM
UDP --> SOCK_DGRAM
socket(add_family,socket_type,0);
*/


// CODE __ server socket 


/*int server_socket;
server_socket = socket(AF_INET, SOCK_STREAM, 0);
if (server_socket == -1)
{
	perror("Socket creation failed");
	exit(EXIT_FAILURE);
}
*/

// Use of bind() function 

/*struct sockaddr_in server_address;
server_address.sin_family = AF_INET;
server_address.sin_port = htons(8080);		 // Port number
server_address.sin_addr.s_addr = INADDR_ANY; // Bind to all available network interfaces

if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
{
	perror("Bind failed");
	exit(EXIT_FAILURE);
}
*/

// 5 is the max no of upcoming req pending in queue;

/*if (listen(server_socket, 5) == -1)
{
	perror("Listen failed");
	exit(EXIT_FAILURE);
}
*/

// accept() function used to accept the upcoming req and it returns new conn with the client;

/*int client_socket;
struct sockaddr_in client_address;
socklen_t client_address_len = sizeof(client_address);

client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
if (client_socket == -1) {
	perror("Accept failed");
	exit(EXIT_FAILURE);
}
*/


// Now data can be recieved or sent

/*char message[] = "Hello, client!"; // send() is basically sending data to the client side 
send(client_socket, message, strlen(message), 0);
*/

/*char buffer[1024];
recv(client_socket, buffer, sizeof(buffer), 0); // recv() is recieving data in client side
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
	
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);		 // Port number
	server_address.sin_addr.s_addr = INADDR_ANY; // Bind to all available network interfaces

	if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_socket, 5) == -1)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	int client_socket;
	struct sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);

	client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
	if (client_socket == -1)
	{
		perror("Accept failed");
		exit(EXIT_FAILURE);
	}

	char message[] = "Hello, client!";
	send(client_socket, message, strlen(message), 0);

	char buffer[1024];
	recv(client_socket, buffer, sizeof(buffer), 0);
	printf("Data received: %s\n", buffer);

	close(client_socket);
	close(server_socket);

	return 0;
}
