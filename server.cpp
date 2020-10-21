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
	// boolean v = true;
	int sockfd, rdy, conn, fd_c1, choice;
	socklen_t len;
	char buffer[1024];
	char str1[100] = "X: Alice received before Y: Bob";
	char str2[100] = "Y: Bob received before X: Alice";
	struct sockaddr_in servaddr, cliaddr1, cliaddr2;
	fd_set readfds;

	// Create a UDP socket
	// Notice the use of SOCK_DGRAM for UDP packets
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr1, 0, sizeof(cliaddr1));
	memset(&cliaddr2, 0, sizeof(cliaddr2));

  // Fill server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
  servaddr.sin_port = htons(PORT); // port number

	// if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&v, sizeof(v)) < 0 ){
  //   perror("setsockopt");
  //   exit(2);
  // }

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
	conn = 0;
	choice = 0;
	FD_ZERO(&readfds);
  while(1){
  	FD_SET(sockfd, &readfds);
		rdy = select(maxFd, &readfds, NULL, NULL, NULL);
		memset(buffer, 0, 1024);
		//first connection
		if(FD_ISSET(sockfd, &readfds) && conn == 0){
			len = sizeof(cliaddr1);
			fd_c1 = accept(sockfd, (struct sockaddr*)&cliaddr1, &len);
			read(fd_c1, buffer, sizeof(buffer));
			std::cout << "Received: " << buffer << std::endl;
			if(strcmp(buffer,"Alice") == 0){
				strcpy(buffer, str1);
				choice = 0;
			}
			else{
				strcpy(buffer,str2);
				choice = 1;
			}
			sendto(fd_c1, (const char *)buffer, strlen(buffer),
			MSG_CONFIRM, (const struct sockaddr *) &cliaddr1, len);
			conn++;
			close(fd_c1);
		}

		if(FD_ISSET(sockfd, &readfds) && conn == 1){
			len = sizeof(cliaddr1);
			fd_c1 = accept(sockfd, (struct sockaddr*)&cliaddr1, &len);
			read(fd_c1, buffer, sizeof(buffer));
			std::cout << "Received: " << buffer << std::endl; 
			if(choice == 0){
				strcpy(buffer, str1);
			}
			else{
				strcpy(buffer,str2);
			}
			sendto(fd_c1, (const char *)buffer, strlen(buffer),
			MSG_CONFIRM, (const struct sockaddr *) &cliaddr1, len);
			conn++;
			close(fd_c1);
		}

		if(conn == 2)
			break;
	}
	std::cout << "Sent acknowledgment to both X and Y" << std::endl;
	return 0;
}
