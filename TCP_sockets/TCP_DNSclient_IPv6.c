#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT 3333 // the port client will be connecting to 

#define MAXDATASIZE 46 // max number of bytes we can get at once 



int main(int argc, char *argv[])
{
	int my_fd, numbytes;  
	char buf[MAXDATASIZE];
	struct sockaddr_in6 my_sock;
	

	if (argc != 3) {
		fprintf(stderr,"usage: client serverIP hostname_to_resolve\n");
		exit(1);
	}

	my_fd=socket(PF_INET6,SOCK_STREAM,0);
	if(my_fd==-1){
		perror("socket");
		return 1;
	}

	my_sock.sin6_family = PF_INET6;
	if (inet_pton(AF_INET6, argv[1], &(my_sock.sin6_addr)) != 1) {
			fprintf(stderr, "Invalid address\n");
			return 1;
		}
	my_sock.sin6_port = htons(PORT);

	if (connect(my_fd, (struct sockaddr *)&my_sock, sizeof my_sock) == -1) {
		close(my_fd);
		perror("client: connect");
		return 1;
	}

	if (send(my_fd, argv[2], 255, 0) == -1)
	perror("send");

	if ((numbytes = recv(my_fd, buf, MAXDATASIZE-1, 0)) == -1) {
		perror("recv");
		exit(1);
	}

	buf[numbytes] = '\0';

	printf("client: received '%s'\n",buf);

	close(my_fd);

	return 0;
}
