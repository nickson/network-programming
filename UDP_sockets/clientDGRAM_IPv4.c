#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

struct sockaddr_in server_addr;
socklen_t addr_len = sizeof server_addr;
int my_fd;
int numbytes;

#define SERVERPORT 3333    // the port users will be connecting to

int main(int argc, char *argv[])
{


	if (argc != 3) {
		fprintf(stderr,"usage: talker hostname message\n");
		return 1;
	}

	if (inet_pton(AF_INET, argv[1], &(server_addr.sin_addr)) != 1) {
		fprintf(stderr, "Invalid address\n");
		return 1;
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVERPORT); 


	if ((my_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("talker: socket");
		return 2;
	}


	if ((numbytes = sendto(my_fd, argv[2], strlen(argv[2]), 0, (struct sockaddr*)&server_addr, addr_len)) == -1) {
		perror("talker: sendto");
		return 1;
	}

	printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
	close(my_fd);

	return 0;
}
