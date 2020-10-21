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
	//vars declarations
	int sockfd, rdy, conn, fd_c1, fd_c2, choice;
	socklen_t len;
	char buffer[1024];
	char str1[100] = "X: Alice received before Y: Bob";
	char str2[100] = "Y: Bob received before X: Alice";
	struct sockaddr_in servaddr, cliaddr1, cliaddr2;
	fd_set readfds;

	// Create TCP socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//clear the addresses structs
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr1, 0, sizeof(cliaddr1));
	memset(&cliaddr2, 0, sizeof(cliaddr2));

  // Fill server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
  servaddr.sin_port = htons(PORT); // port number


  // Bind the socket with the server address
	if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		std::cout << "Bind failed" << std::endl;
		exit(1);
	}

	//Listen for clients
	if(listen(sockfd, 10) < 0){
		perror("Listen");
		exit(2);
	}

	//set up vars and clear fd_set
	int maxFd = sockfd + 1;
	conn = 0; choice = 0;
	FD_ZERO(&readfds);

  while(conn != 2){
  	FD_SET(sockfd, &readfds);
		rdy = select(maxFd, &readfds, NULL, NULL, NULL);
		memset(buffer, 0, 1024);	//clear buffer

		//first connection
		if(FD_ISSET(sockfd, &readfds) && conn == 0){
			len = sizeof(cliaddr1);

			//accept client connection amd read packet
			fd_c1 = accept(sockfd, (struct sockaddr*)&cliaddr1, &len);
			read(fd_c1, buffer, sizeof(buffer));

			std::cout << "Received: " << buffer << std::endl;
			//Saves info of which client connected first into choice
			if(strcmp(buffer,"Alice") == 0) choice = 0;
			else choice = 1;

			conn++;	//increment so while loop will not enter this if branch again
		}

		memset(buffer, 0, 1024);	//clear buffer

		//second connection, only enters this branch if first client is connected
		if(FD_ISSET(sockfd, &readfds) && conn == 1){
			len = sizeof(cliaddr2);
			fd_c2 = accept(sockfd, (struct sockaddr*)&cliaddr2, &len);
			read(fd_c2, buffer, sizeof(buffer));
			std::cout << "Received: " << buffer << std::endl;

			//chooses which client came first based on first conn
			if(choice == 0) strcpy(buffer, str1);
			else strcpy(buffer,str2);

			//send ACKs to both clients
			sendto(fd_c1, (const char *)buffer, strlen(buffer),
			MSG_CONFIRM, (const struct sockaddr *) &cliaddr1, len);
			sendto(fd_c2, (const char *)buffer, strlen(buffer),
			MSG_CONFIRM, (const struct sockaddr *) &cliaddr2, len);

			conn++;	//conn will now equal 2 and break
			//close fds
			close(fd_c1);
			close(fd_c2);
		}
	}

	std::cout << "Sent acknowledgment to both X and Y" << std::endl;
	close(sockfd);	//close server socket
	return 0;
}
