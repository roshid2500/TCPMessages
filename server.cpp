#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT	 12551

int main() {
	int sockfd, rdy, b;
	socklen_t len;
	char buffer[1024];
	struct sockaddr_in servaddr, cliaddr1, cliaddr2;
	fd_set readfds;

	// Create a UDP socket
	// Notice the use of SOCK_DGRAM for UDP packets
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr1));
	memset(&cliaddr, 0, sizeof(cliaddr3));

  // Fill server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
  servaddr.sin_port = htons(PORT); // port number

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&v, sizeof(v)) < 0 ){
    perror("setsockopt");
    exit(2);
  }

  // Bind the socket with the server address
	if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		std::cout << "Bind failed" << std::endl;
		exit(1);
	}

	//Connect with the two clients
	if(listen(sockfd, 10) < 0){
		perror("Listen");
		exit(2);
	}

	int maxFd = sockfd + 1;
	FD_ZERO(&readfds)
  while(1){
  	FD_SET(sockfd, &readfds)
		rdy = select(maxFd, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(sockfd, &readfds)){
			len = sizeof(cliaddr1);
			fd_c1 = accept(sockfd, (struct sockaddr*)&cliaddr1, &len);


		}

	}

	return 0;
}
