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
	int sockfd, n;
	socklen_t len;
	char buffer[1024];
	struct sockaddr_in servaddr, cliaddr1, cliaddr2;

	// Create a UDP socket
	// Notice the use of SOCK_DGRAM for UDP packets
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

  // Fill server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
  servaddr.sin_port = htons(PORT); // port number

  // Bind the socket with the server address
	if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		std::cout << "Bind failed" << std::endl;
		exit(1);
	}

  while (1) {
		//Receive the first client packet and its address information
    while(1){
      n = recvfrom(sockfd, (char *)buffer, sizeof(buffer),
        MSG_WAITALL, ( struct sockaddr *) &cliaddr1, &len);
      buffer[n] = '\0';

			//std::string name = buffer;
		  //memset(buffer, 0, 1024);
			//buffe

      //Otherwise, the server responds
      sendto(sockfd, (const char *)buffer, strlen(buffer),
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr1, len);
    }

    //Receive the second client packet and its address information
    while(1){
      n = recvfrom(sockfd, (char *)buffer, sizeof(buffer),
        MSG_WAITALL, ( struct sockaddr *) &cliaddr2, &len);
      buffer[n] = '\0';

      //Otherwise, the server responds
      sendto(sockfd, (const char *)buffer, strlen(buffer),
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr2, len);
    }


	}
	return 0;
}
