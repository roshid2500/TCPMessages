#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <chrono>
#define PORT	 12551


int main() {
	//var declarations
	int sockfd, n, send;
	socklen_t len;
	char buffer[1024];
	struct sockaddr_in servaddr;

	memset(buffer, 0, 1024);
	memset(&servaddr, 0, sizeof(servaddr));
	strcpy(buffer, "Alice");

	//create TCP socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// Fill server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
	servaddr.sin_port = htons(PORT); // port number
	len = sizeof(servaddr);

	//connect to the server
	if(connect(sockfd, (struct sockaddr*) &servaddr, len) < 0){
		perror("Connect failed");
	}

	//Send the TCP packet to the server
	send = sendto(sockfd, (const char *)buffer, strlen(buffer),
	MSG_CONFIRM, (const struct sockaddr *) &servaddr, len);

	//Keep running until receive ACK from server
	while(1){
		n = read(sockfd, buffer, sizeof(buffer));
		if(n > 0) break;
	}

	//print ACK message
	std::cout << buffer << std::endl;

	memset(buffer, 0, 1024);
	close(sockfd);	//close client socket
	return 0;
}
