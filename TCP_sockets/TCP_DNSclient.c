//
// a stream socket DNS client
//

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

#define PORT "3333" // the port client will be connecting to 

#define MAXDATASIZE 253 // max number of bytes we can get at once 



int main(int argc, char *argv[])
{
	int my_fd, numbytes;  
	char buf[MAXDATASIZE];
	struct sockaddr_in my_sock;
	

	if (argc != 3) {
		fprintf(stderr,"usage: client serverIP hostname_to_resolve\n");
		exit(1);
	}

	my_fd=socket(PF_INET,SOCK_STREAM,0);
	if(my_fd==-1){
		perror("socket");
		return 1;
	}

	my_sock.sin_family = PF_INET;
	my_sock.sin_addr.s_addr = inet_addr(argv[1]);
	my_sock.sin_port = htons(3333);

	if (connect(my_fd, (struct sockaddr *)&my_sock, sizeof my_sock) == -1) {
		close(my_fd);
		perror("client: connect");
		return 1;
	}



	if (send(my_fd, argv[2], MAXDATASIZE, 0) == -1)
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
