
// For a server with more than one network cards / IP addresses - enter the IP address to bind the server socket to.
// Example:
// ./TCP_DNSserver.out 192.168.1.2
// If no IP address is entered as a parameter, binds to all IP addresses of the server.

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

struct sockaddr_in my_sock;
int my_fd;

#define MAXBUFLEN 1000
char buf[MAXBUFLEN];

int numbytes;
struct sockaddr_in client_addr;
socklen_t addr_len = sizeof client_addr;
char s[INET_ADDRSTRLEN];

int main(int argc, char *argv[])
{

	my_fd=socket(AF_INET,SOCK_DGRAM,0);
	if(my_fd==-1){
		perror("socket");
		return 1;
	}
	
	memset(&my_sock, 0, sizeof(my_sock));
	if(argc==1){
		my_sock.sin_family = AF_INET;
		my_sock.sin_addr.s_addr = INADDR_ANY;
		my_sock.sin_port = htons(3333); //PORT;
	}
	else{
		if (inet_pton(AF_INET, argv[1], &(my_sock.sin_addr)) != 1) {
			fprintf(stderr, "Invalid address\n");
			return 1;
		}
		my_sock.sin_family = AF_INET;
		my_sock.sin_port = htons(3333); //PORT;
		
		char ipinput[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(my_sock.sin_addr), ipinput, INET_ADDRSTRLEN);
		printf("IP Address = %s\n", ipinput);

	}
	if( bind(my_fd, (struct sockaddr *)&my_sock, sizeof my_sock) == -1){
		perror("bind");
		return 1;
	}
	while(1){
		if ((numbytes = recvfrom(my_fd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
			perror("recvfrom");
			return 1;
		}

		printf("listener: got packet from %s\n",
		inet_ntop(client_addr.sin_family,
		&client_addr.sin_addr, s, sizeof s));
		printf("listener: packet is %d bytes long\n", numbytes);
		buf[numbytes] = '\0';
		printf("listener: packet contains \"%s\"\n", buf);
	}
//	close(my_fd);

	return 0;
}
