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
	int sockfd, a, b;
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

	//Connect with the two clients
	int connections = 0;
	int newCon1 = -1, newCon2 = -1;
	while(connections != 2){
		listen(sockfd, 3);
		if(connections == 0)
			newCon1 = accept(sockfd, (sockaddr *)&cliaddr1, &len);
		if(connections == 1)
			newCon2 = accept(sockfd, (sockaddr *)&cliaddr2, &len);
		if (newCon2 != -1)
			break;
		if(newCon1 != -1 && connections == 0)
			connections++;
	 }

	int aFirst = 0;

  while(1){
  	switch((a = read(newCon1,buffer,1024) > 0 ) || (b = read(newCon2,buffer,1024) > 0 )){
			case a:
				std::string str = buffer;
				memset(buffer, 0, 1024);
				str = str + ' came first';
				send(newCon1, str, str.length() + 1);
				break;
			case b:
				std::string str = buffer;
				memset(buffer, 0, 1024);
				str = str + ' came first';
				send(newCon2, str, str.length() + 1);
				break
			default:
				continue;
		}

	}

	return 0;
}
