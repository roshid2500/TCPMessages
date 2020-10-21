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

using namespace std;

int main() {
	int sockfd, n, send;
	socklen_t len;
	char buffer[1024];
	struct sockaddr_in servaddr;

	memset(buffer, 0, 1024);
	strcpy(buffer, "Alice");
	// Create a UDP socket
	// Notice the use of SOCK_DGRAM for UDP packets
	sockfd = socket(AF_INET, SOCK_STREAM, 0);


	memset(&servaddr, 0, sizeof(servaddr));

	// Fill server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
	servaddr.sin_port = htons(PORT); // port number
	len = sizeof(servaddr);

 if(connect(sockfd, (struct sockaddr*) &servaddr, len) < 0){
	 perror("Connect failed");
 }
	//Send the TCP packet to the server
	send = sendto(sockfd, (const char *)buffer, strlen(buffer),
	MSG_CONFIRM, (const struct sockaddr *) &servaddr, len);

	//receive packet
	while(1){
		n = recvfrom(sockfd, (char *)buffer, sizeof(buffer),
		MSG_WAITALL, ( struct sockaddr *) &servaddr, &len);
		buffer[n] = '\0';

		if(n != -1)
			break;
	}

	//print ACK message
	std::cout << buffer << std::endl;
	memset(buffer, 0, 1024);
	close(sockfd);
	return 0;
}
